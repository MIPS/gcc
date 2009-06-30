#{([

#
# Copyright (c) 2008 Google Inc. All rights reserved.
#
# $Header: $
#
# -*- mode: python -*-

import inspect
import os
import parser
import pprint
import re
import string
import symbol
import sys
import token
import types

#
# Plug nonterminals are lexically all in lower case, _ or digits;
# Python fortunately matches this with islower.
#
def is_plug_nonterm(symbol):
  return symbol.islower()

#
# Plug terminals are lexically all in upper case, _ or digits;
# Python fortunately matches this with isupper.
#
def is_plug_term(symbol):
  return symbol.isupper()

do_shards = False
if do_shards:
  dir = "shards"
  if os.path.isdir(dir) == False:
    os.mkdir(dir)

def plugrule3(handle, table, actions, trace=0):
  plug_basicrule(handle, table, actions, trace)

def plugrule2(table, actions, trace=0):
  plug_basicrule(None, table, actions, trace)

def plug_basicrule(handle, table, actions, trace):
  global global_rule_to_locus
  global global_file_has_been_read

  trace = 0
  frames = inspect.getouterframes(inspect.currentframe())
  frame = frames[2]		# frames for: plug_basic_rule, plugrule3

  last_file_name   = frame[1]	# full path
  last_line_number = frame[2]	# last line at call site
  actions_start_line_number = last_line_number - actions.count("\n")

  #
  # Assuming standard formatting of plug rules, there's exactly one line
  # in the table for each plug rule and its attributes.
  # However, comments in python are discarded and do NOT appear
  # in the table (a tuple).
  #
  # We'll compensate for this by reading the entire .py file
  # as a text file, and extract the rules from that,
  # and keep track of a map from rule name to file locus.
  #
  line_number = actions_start_line_number - len(table);
  file_name = os.path.basename(last_file_name);

  #
  # Read the file name as a text file to get an exact source file locus.
  # (The file is read only once.)
  #
  read_file_as_text(last_file_name)

  #
  # The variable handle names the block of rules and semantic actions.
  #
  if handle == None:
    handle = file_name + "-" + str(line_number);

  #
  # The variable errorlocus names the conventional file:linenumber.
  #
  errorlocus = file_name + ":" + str(actions_start_line_number)

  #
  # Print out the actions to a file named by the handle.
  # We can then use tkdiff to compare different actions
  # so that we can manually figure out how to merge together
  # blocks of rules that share similar semantics.
  #
  if do_shards:
    shard_fd = open(dir + "/" + handle, "w");
    print >>shard_fd, "// %s line %d" % (file_name, line_number);
    print >>shard_fd, "%s\n" % actions
    shard_fd.close()
    shard_fd = None

  transtab = string.maketrans('', '');
  var_val_map = {}
  rownumber = -1
  vars = table[0]
  for vals in table[1:]:
    rownumber += 1
    if trace:
      print "rn=%d vals=%s" % (rownumber, vals)

    #
    # new_actions will be rewritten according to the bindings
    # established by reference to nonterminals
    # or by reference to table header variables.
    #
    new_actions = actions
    var_val_map.clear()
    for i in xrange(len(vars)):
      var = vars[i]	# name of variable
      val = vals[i]	# value to substitute
      if trace:
	print "var=%s val=%s" % (var, val)

      if var == "rule":
	current_textual_rule = val.translate(transtab, string.whitespace)
	#
	# Analyze the rule, returning the map ntref_to_pos
	# from NT names or NT aliases to burg rhs NT positions.
	# Here, val is the entire rule body.
	#
        ntref_to_pos, ntref_to_nt = plug_analyze_rule(val,
	  file_name, line_number)

	#
	# Given NT specs of the form xyz.suffix remove .suffix
	# .suffix is an alias for the NT that is used to turn
	# .suffix named nonterminals into positional references on the rhs.
	#
	re_object = re.compile(r'\.\b[a-z_0-9]+\b')
	val = re_object.sub('', val, 0)

      #
      # Set up to use safe_substitute
      #   to substitute occurances of $var for val.
      #
      var_val_map[var] = val

    check_attr_use(file_name, actions_start_line_number,
      ntref_to_nt, actions, trace)

    #
    # The function safe_substitute will change $$ to just $,
    # but we have to preserve the $$ for plug usage.
    # (There may be a way to change this behaviour with subclassing Template)
    # So we rewrite $$ to $lhs,
    # and then use Template.safe_substitute to map $lhs to $$.
    #
    re_object = re.compile(r'\$\$')
    new_actions = re_object.sub('$lhs', new_actions, 0)

    t0 = string.Template(new_actions)
    new_actions = t0.safe_substitute(var_val_map)

    t1 = string.Template(new_actions)
    new_actions = t1.safe_substitute(ntref_to_pos)

    #
    # Print out the rewritten actions, with preprocessor style
    # line coordinates suitable for plug/C++.
    # There's one coordinate for the rule,
    # and another coordinate for the semantic actions.
    #
    global global_rule_to_locus
    if global_rule_to_locus.has_key(current_textual_rule):
      text_file_name   = global_rule_to_locus[current_textual_rule][0]
      text_line_number = global_rule_to_locus[current_textual_rule][1]
      print "#line %d \"%s\"" % (text_line_number, text_file_name)
    else:
      print "#line %d \"%s\"" % (line_number, file_name)

    new_actions = new_actions.strip()
    outnumber = 0
    for line in new_actions.split("\n"):
      outnumber += 1
      if outnumber == 2:
	#
	# Bias up the starting line number for the actions so
	# we can compensate for the $rule $cost line.
	#
	print "#line %d \"%s\"" % (actions_start_line_number+2, file_name)
      print "%s" % (line)

  return


#
# Read the entire file as text, and construct a map from rules to line numbers
# this map will be more accurate than counting sublists in the first argument.
# Store the information in the dictionary global_rule_to_locus
#
global_rule_to_locus = {}
global_file_has_been_read = {}
def read_file_as_text(file_name):
  global global_rule_to_locus
  global global_file_has_been_read

  if global_file_has_been_read.has_key(file_name):
    return
  global_file_has_been_read[file_name] = 1

  line_number = 0
  rexpr_rule = r"\[\"([A-Za-z0-9._ ]+:[^\"]*)\""
  re_object_rule = re.compile(rexpr_rule);
  transtab = string.maketrans('', '');

  for line in open(file_name, "r").readlines():
    line_number += 1
    line = line.strip()
    m = re_object_rule.match(line)
    if m != None:
      textual_rule = m.group(1)
      textual_rule = textual_rule.translate(transtab, string.whitespace);
      if global_rule_to_locus.has_key(textual_rule):
        print >> sys.stderr, "rule occurs at %s:%-5d and %s:%-5d %s" % (
	  global_rule_to_locus[textual_rule][0],
	  global_rule_to_locus[textual_rule][1],
	  os.path.basename(file_name),
	  line_number,
	  textual_rule
	)
      global_rule_to_locus[textual_rule] = \
	[os.path.basename(file_name), line_number]

  return

global_fault_to_issued = {}
def check_attr_use(filename, actions_line_number,
    ntref_to_nt, actions, trace = 0):
  global global_fault_to_issued

  #
  # Look in the unmodified semantic actions for
  #  $ntref->field
  # and map ntref to the corresponding true nt.
  # Construct a map from true nt to the fields that are used.
  #
  if trace > 1:
    print "ntref_to_nt=%s" % (ntref_to_nt)

  nt_to_attrref = {}
  for ntref in ntref_to_nt:
    nt = ntref_to_nt[ntref]
    rexpr = r"\$(" + ntref + r")[ ]*" + r"->"  + r"[ ]*" + r"([a-zA-Z0-9_]+)";
    re_object = re.compile(rexpr);
    deltaline = 0
    for line in actions.split("\n"):
      for attr_ref in re_object.findall(line):
	#
	# attr_ref is a list of pairs (ntref, field_name)
	#
	field_name = attr_ref[1]
	nt_to_attrref.setdefault(nt, {})
	nt_to_attrref[nt].setdefault(field_name, actions_line_number+deltaline)
      deltaline += 1

  if trace > 0:
    print "nt_to_attrref = %s" % (nt_to_attrref)

  #
  # For each NT, do an asymmetric difference between the set of fields used
  # and the set of legal fields; if we use a field that isn't legal to use
  # issue an error message
  #
  for nt in nt_to_attrref:
    for attrref in nt_to_attrref[nt]:
      first_line_number = nt_to_attrref[nt][attrref]
      if global_nt_to_attrs.has_key(nt) and len(global_nt_to_attrs[nt]) > 0:
	try:
	  global_nt_to_attrs[nt].index(attrref)
	except:
	  msg = "%s:%d: illegal reference to NT %s attribute %s" % \
	    (filename, first_line_number, nt, attrref)
	  if not global_fault_to_issued.has_key(msg):
	    print >>sys.stderr, "%s" % (msg)
	  global_fault_to_issued[msg] = 1

  return

#
# Given a plug rule as a string of the form "lhs : rhs"
# return two dictionaries.
# The 1st dictionary maps plug nonterminal references to 1-based
# nonterminal positions, with 1 being the leftmost nonterminal on the rhs.
# Thus, for rule x.y we'll map $y to 2(say).
#
# Ths 2nd dictionary maps plug nonterminal references
# to grammatical non terminal names.
# Thus, for rule x.y we'll map $y to $x.
#
def plug_analyze_rule(rule, file_name, line_number, trace=0):
  nt_pos = 0
  ntref_to_pos = {}
  ntref_to_nt  = {}

  #
  # Remove occurances of plug terminal names,
  # using simple lexical naming conventions.
  # The machine spec must adhere to these lexical conventions.
  #
  subject = rule
  re_object = re.compile(r'\b' + r'[A-Z_0-9]+' + r'\b')
  subject = re_object.sub(' ', subject, 0)

  #
  # Remove occurances of plug rule building operators.
  #
  re_object = re.compile(r'[(),|:@]')
  subject = re_object.sub(' ', subject, 0)

  nt_pos = 0
  for plug_nt_handle in subject.split():
    #
    # The nonterminal is lexically of the form x.y.
    # Construct a map $y to $N if y exists, or $x to $N otherwise;
    # we preferentially use the ".y" disambiguator.
    #
    crack_list = plug_nt_handle.split('.')
    if len(crack_list) > 1:
      ntref_to_nt[crack_list[-1]] = crack_list[0]
    else:
      ntref_to_nt[crack_list[0]]  = crack_list[0]

    alias = crack_list[-1]	# the last item on the list
    if nt_pos == 0:
      ntref_to_pos[alias] = "$" + "$"
    else:
      ntref_to_pos[alias] = "$" + str(nt_pos)
    nt_pos += 1
  #
  # Because Template::safe_substitute rewrites $$ to $
  # and $$ is needed for plug, we arrange to use rexprs to
  # rewrite $$ to $lhs, and then use this map to rewrite $lhs back to $$.
  #
  ntref_to_pos['lhs'] = '$$'	# $$ ==> $lhs ==> $$

  if trace:
    print "%s" % (str(ntref_to_nt))

  return ntref_to_pos, ntref_to_nt


#
# This is demo code.
# Walk the python parse tree and print it out.
#
def print_parse_tree(tup, depth):
  if len(tup) == 0:
    return
  x = tup[0]
  if token.ISTERMINAL(x):
    print "%s      TERM ==> %3d %3d %s" % \
      (' '*2*depth, depth, x, token.tok_name[x])
  else:
    print "%s   NONTERM ==> %3d %3d %s" % \
      (' '*2*depth, depth, x, symbol.sym_name[x])
    for kid in tup[1:]:
      print_parse_tree(kid, depth+1)
  return

#
# This is demo code.
# Walk the parse tree tup and construct the list back
# from all python terminal symbols.  Python terminals are not the
# same as plug terminals.
#
def parse_tree_to_terms(tup, back, trace=0):
  if len(tup) > 0:
    x = tup[0]
    if token.ISTERMINAL(x):
      sym = token.tok_name[x]
      if trace:
	print "sym=%s" % (sym)
      if sym == 'NAME':
        ret = [sym, tup[1]]
      elif sym == 'NUMBER':
        ret = [sym, tup[1]]
      else:
        ret = sym
      if trace:
	print "return %s " % (ret)
      back.append(ret)
    else:
      for kid in tup[1:]:
	parse_tree_to_terms(kid, back, trace)
  return back


global_plug_errors = 0
def plug_fault(msg):
  global global_plug_errors

  print >>sys.stderr, "%s" % (msg)
  sys.exit(1)
  return

#
# terminal declarations map terminal names to the legal attribute fields.
#
global_term_number = 0
global_nt_to_attrs = {}
global_is_term = {}
global_is_nonterm = {}
global_print_plug_decls = 1

def nonterm(symbol, fields = []):
  global global_nt_to_attrs
  global global_is_nonterm
  global global_is_term
  global global_print_plug_decls

  if global_is_nonterm.has_key(symbol):
    plug_fault("symbol ``%s'' already declared as a non-terminal" % (symbol))
  if global_is_term.has_key(symbol):
    plug_fault("symbol ``%s'' already declared as a terminal" % (symbol))
  if not is_plug_nonterm(symbol):
    plug_fault("non-terminal symbol ``%s'' does not follow lexical conventions" % (symbol))

  global_is_nonterm[symbol] = 1
  global_nt_to_attrs[symbol] = fields

  if global_print_plug_decls:
    print "nonterm %s;" % (symbol)

  return

def term_incr(incr):
  global global_term_number
  global_term_number += incr
  return

def term_align(align):
  global global_term_number
  global_term_number = ((global_term_number + align-1)/align) * align
  return

def term(symbol):
  global global_is_term
  global global_is_nonterm
  global global_term_number
  global global_print_plug_decls

  if global_is_term.has_key(symbol):
    plug_fault("symbol ``%s'' already declared as a terminal" % (symbol))
  if global_is_nonterm.has_key(symbol):
    plug_fault("symbol ``%s'' already declared as a non-terminal" % (symbol))
  if not is_plug_term(symbol):
    plug_fault("terminal symbol ``%s'' does not follow lexical conventions" % (symbol))

  global_is_term[symbol] = global_term_number

  if global_print_plug_decls:
    print "term %s = %d;" % (symbol, global_term_number)

  global_term_number += 1
  return

def term_cross(name, suffixes):
  for suffix in suffixes:
    term(name + "_" + suffix)
  return

def reduce(block_name, file_name):
  global global_print_plug_decls

  if global_print_plug_decls:
    print "reduce %s = \"%s\";" % (block_name, file_name)
  return

def start(nt):
  global global_is_term
  global global_print_plug_decls

  if not global_is_nonterm.has_key(nt):
    plug_fault("start symbol ``%s'' is not declared as a nonterminal" % (nt))
  if global_print_plug_decls:
    print "start %s;" % (nt)
  return

#})]
