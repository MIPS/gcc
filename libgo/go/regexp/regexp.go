// Copyright 2009 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

// Package regexp implements regular expression search.
//
// The syntax of the regular expressions accepted is the same
// general syntax used by Perl, Python, and other languages.
// More precisely, it is the syntax accepted by RE2 and described at
// http://code.google.com/p/re2/wiki/Syntax, except for \C.
// For an overview of the syntax, run
//   godoc regexp/syntax
//
// All characters are UTF-8-encoded code points.
//
// There are 16 methods of Regexp that match a regular expression and identify
// the matched text.  Their names are matched by this regular expression:
//
//	Find(All)?(String)?(Submatch)?(Index)?
//
// If 'All' is present, the routine matches successive non-overlapping
// matches of the entire expression.  Empty matches abutting a preceding
// match are ignored.  The return value is a slice containing the successive
// return values of the corresponding non-'All' routine.  These routines take
// an extra integer argument, n; if n >= 0, the function returns at most n
// matches/submatches.
//
// If 'String' is present, the argument is a string; otherwise it is a slice
// of bytes; return values are adjusted as appropriate.
//
// If 'Submatch' is present, the return value is a slice identifying the
// successive submatches of the expression. Submatches are matches of
// parenthesized subexpressions (also known as capturing groups) within the
// regular expression, numbered from left to right in order of opening
// parenthesis. Submatch 0 is the match of the entire expression, submatch 1
// the match of the first parenthesized subexpression, and so on.
//
// If 'Index' is present, matches and submatches are identified by byte index
// pairs within the input string: result[2*n:2*n+1] identifies the indexes of
// the nth submatch.  The pair for n==0 identifies the match of the entire
// expression.  If 'Index' is not present, the match is identified by the
// text of the match/submatch.  If an index is negative, it means that
// subexpression did not match any string in the input.
//
// There is also a subset of the methods that can be applied to text read
// from a RuneReader:
//
//	MatchReader, FindReaderIndex, FindReaderSubmatchIndex
//
// This set may grow.  Note that regular expression matches may need to
// examine text beyond the text returned by a match, so the methods that
// match text from a RuneReader may read arbitrarily far into the input
// before returning.
//
// (There are a few other methods that do not match this pattern.)
//
package regexp

import (
	"bytes"
	"io"
	"regexp/syntax"
	"strconv"
	"strings"
	"sync"
	"unicode"
	"unicode/utf8"
)

var debug = false

// Error is the local type for a parsing error.
type Error string

func (e Error) String() string {
	return string(e)
}

// Error codes returned by failures to parse an expression.
var (
	ErrInternal            = Error("internal error")
	ErrUnmatchedLpar       = Error("unmatched '('")
	ErrUnmatchedRpar       = Error("unmatched ')'")
	ErrUnmatchedLbkt       = Error("unmatched '['")
	ErrUnmatchedRbkt       = Error("unmatched ']'")
	ErrBadRange            = Error("bad range in character class")
	ErrExtraneousBackslash = Error("extraneous backslash")
	ErrBadClosure          = Error("repeated closure (**, ++, etc.)")
	ErrBareClosure         = Error("closure applies to nothing")
	ErrBadBackslash        = Error("illegal backslash escape")
)

const (
	iStart     = iota // beginning of program
	iEnd              // end of program: success
	iBOT              // '^' beginning of text
	iEOT              // '$' end of text
	iChar             // 'a' regular character
	iCharClass        // [a-z] character class
	iAny              // '.' any character including newline
	iNotNL            // [^\n] special case: any character but newline
	iBra              // '(' parenthesized expression: 2*braNum for left, 2*braNum+1 for right
	iAlt              // '|' alternation
	iNop              // do nothing; makes it easy to link without patching
)

// An instruction executed by the NFA
type instr struct {
	kind  int    // the type of this instruction: iChar, iAny, etc.
	index int    // used only in debugging; could be eliminated
	next  *instr // the instruction to execute after this one
	// Special fields valid only for some items.
	char   int        // iChar
	braNum int        // iBra, iEbra
	cclass *charClass // iCharClass
	left   *instr     // iAlt, other branch
}

func (i *instr) print() {
	switch i.kind {
	case iStart:
		print("start")
	case iEnd:
		print("end")
	case iBOT:
		print("bot")
	case iEOT:
		print("eot")
	case iChar:
		print("char ", string(i.char))
	case iCharClass:
		i.cclass.print()
	case iAny:
		print("any")
	case iNotNL:
		print("notnl")
	case iBra:
		if i.braNum&1 == 0 {
			print("bra", i.braNum/2)
		} else {
			print("ebra", i.braNum/2)
		}
	case iAlt:
		print("alt(", i.left.index, ")")
	case iNop:
		print("nop")
	}
}

// Regexp is the representation of a compiled regular expression.
// The public interface is entirely through methods.
// A Regexp is safe for concurrent use by multiple goroutines.
type Regexp struct {
	expr        string // the original expression
	prefix      string // initial plain text string
	prefixBytes []byte // initial plain text bytes
	inst        []*instr
	start       *instr // first instruction of machine
	prefixStart *instr // where to start if there is a prefix
	nbra        int    // number of brackets in expression, for subexpressions
}

type charClass struct {
	negate bool // is character class negated? ([^a-z])
	// slice of int, stored pairwise: [a-z] is (a,z); x is (x,x):
	ranges     []int
	cmin, cmax int
}

func (cclass *charClass) print() {
	print("charclass")
	if cclass.negate {
		print(" (negated)")
	}
	regexp := &Regexp{
		expr:        expr,
		prog:        prog,
		numSubexp:   maxCap,
		subexpNames: capNames,
		cond:        prog.StartCond(),
		longest:     longest,
	}
}

func (cclass *charClass) addRange(a, b int) {
	// range is a through b inclusive
	cclass.ranges = append(cclass.ranges, a, b)
	if a < cclass.cmin {
		cclass.cmin = a
	}
	return regexp, nil
}

func (cclass *charClass) matches(c int) bool {
	if c < cclass.cmin || c > cclass.cmax {
		return cclass.negate
	}
	re.mu.Unlock()
	z := progMachine(re.prog)
	z.re = re
	return z
}

func newCharClass() *instr {
	i := &instr{kind: iCharClass}
	i.cclass = new(charClass)
	i.cclass.ranges = make([]int, 0, 4)
	i.cclass.cmin = 0x10FFFF + 1 // MaxRune + 1
	i.cclass.cmax = -1
	return i
}

func (re *Regexp) add(i *instr) *instr {
	i.index = len(re.inst)
	re.inst = append(re.inst, i)
	return i
}

func (i *inputString) step(pos int) (rune, int) {
	if pos < len(i.str) {
		c := i.str[pos]
		if c < utf8.RuneSelf {
			return rune(c), 1
		}
		return utf8.DecodeRuneInString(i.str[pos:])
	}
	return endOfText, 0
}

func (i *inputString) canCheckPrefix() bool {
	return true
}

func (i *inputString) hasPrefix(re *Regexp) bool {
	return strings.HasPrefix(i.str, re.prefix)
}

func (p *parser) charClass() *instr {
	i := newCharClass()
	cc := i.cclass
	if p.c() == '^' {
		cc.negate = true
		p.nextc()
	}
	left := -1
	for {
		switch c := p.c(); c {
		case ']', endOfFile:
			if left >= 0 {
				p.error(ErrBadRange)
			}
			// Is it [^\n]?
			if cc.negate && len(cc.ranges) == 2 &&
				cc.ranges[0] == '\n' && cc.ranges[1] == '\n' {
				nl := &instr{kind: iNotNL}
				p.re.add(nl)
				return nl
			}
			// Special common case: "[a]" -> "a"
			if !cc.negate && len(cc.ranges) == 2 && cc.ranges[0] == cc.ranges[1] {
				c := &instr{kind: iChar, char: cc.ranges[0]}
				p.re.add(c)
				return c
			}
			p.re.add(i)
			return i
		case '-': // do this before backslash processing
			p.error(ErrBadRange)
		case '\\':
			c = p.nextc()
			switch {
			case c == endOfFile:
				p.error(ErrExtraneousBackslash)
			case ispunct(c):
				// c is as delivered
			case escape(c) >= 0:
				c = int(escaped[escape(c)])
			default:
				p.error(ErrBadBackslash)
			}
			fallthrough
		default:
			p.nextc()
			switch {
			case left < 0: // first of pair
				if p.c() == '-' { // range
					p.nextc()
					left = c
				} else { // single char
					cc.addRange(c, c)
				}
			case left <= c: // second of pair
				cc.addRange(left, c)
				left = -1
			default:
				p.error(ErrBadRange)
			}
		}
	}
	return nil
}

func (p *parser) term() (start, end *instr) {
	switch c := p.c(); c {
	case '|', endOfFile:
		return nil, nil
	case '*', '+':
		p.error(ErrBareClosure)
	case ')':
		if p.nlpar == 0 {
			p.error(ErrUnmatchedRpar)
		}
		return nil, nil
	case ']':
		p.error(ErrUnmatchedRbkt)
	case '^':
		p.nextc()
		start = p.re.add(&instr{kind: iBOT})
		return start, start
	case '$':
		p.nextc()
		start = p.re.add(&instr{kind: iEOT})
		return start, start
	case '.':
		p.nextc()
		start = p.re.add(&instr{kind: iAny})
		return start, start
	case '[':
		p.nextc()
		start = p.charClass()
		if p.c() != ']' {
			p.error(ErrUnmatchedLbkt)
		}
		p.nextc()
		return start, start
	case '(':
		p.nextc()
		p.nlpar++
		p.re.nbra++ // increment first so first subexpr is \1
		nbra := p.re.nbra
		start, end = p.regexp()
		if p.c() != ')' {
			p.error(ErrUnmatchedLpar)
		}
		p.nlpar--
		p.nextc()
		bra := &instr{kind: iBra, braNum: 2 * nbra}
		p.re.add(bra)
		ebra := &instr{kind: iBra, braNum: 2*nbra + 1}
		p.re.add(ebra)
		if start == nil {
			if end == nil {
				p.error(ErrInternal)
				return
			}
			start = ebra
		} else {
			end.next = ebra
		}
		bra.next = start
		return bra, ebra
	case '\\':
		c = p.nextc()
		switch {
		case c == endOfFile:
			p.error(ErrExtraneousBackslash)
		case ispunct(c):
			// c is as delivered
		case escape(c) >= 0:
			c = int(escaped[escape(c)])
		default:
			p.error(ErrBadBackslash)
		}
		fallthrough
	default:
		p.nextc()
		start = &instr{kind: iChar, char: c}
		p.re.add(start)
		return start, start
	}
	panic("unreachable")
}

func (p *parser) closure() (start, end *instr) {
	start, end = p.term()
	if start == nil {
		return
	}
	switch p.c() {
	case '*':
		// (start,end)*:
		alt := &instr{kind: iAlt}
		p.re.add(alt)
		end.next = alt   // after end, do alt
		alt.left = start // alternate brach: return to start
		start = alt      // alt becomes new (start, end)
		end = alt
	case '+':
		// (start,end)+:
		alt := &instr{kind: iAlt}
		p.re.add(alt)
		end.next = alt   // after end, do alt
		alt.left = start // alternate brach: return to start
		end = alt        // start is unchanged; end is alt
	case '?':
		// (start,end)?:
		alt := &instr{kind: iAlt}
		p.re.add(alt)
		nop := &instr{kind: iNop}
		p.re.add(nop)
		alt.left = start // alternate branch is start
		alt.next = nop   // follow on to nop
		end.next = nop   // after end, go to nop
		start = alt      // start is now alt
		end = nop        // end is nop pointed to by both branches
	default:
		return
	}
	switch p.nextc() {
	case '*', '+', '?':
		p.error(ErrBadClosure)
	}
	return
}

func (p *parser) concatenation() (start, end *instr) {
	for {
		nstart, nend := p.closure()
		switch {
		case nstart == nil: // end of this concatenation
			if start == nil { // this is the empty string
				nop := p.re.add(&instr{kind: iNop})
				return nop, nop
			}
			return
		case start == nil: // this is first element of concatenation
			start, end = nstart, nend
		default:
			end.next = nstart
			end = nend
		}
	}
	panic("unreachable")
}

func (p *parser) regexp() (start, end *instr) {
	start, end = p.concatenation()
	for {
		switch p.c() {
		default:
			return
		case '|':
			p.nextc()
			nstart, nend := p.concatenation()
			alt := &instr{kind: iAlt}
			p.re.add(alt)
			alt.left = start
			alt.next = nstart
			nop := &instr{kind: iNop}
			p.re.add(nop)
			end.next = nop
			nend.next = nop
			start, end = alt, nop
		}
		return utf8.DecodeRune(i.str[pos:])
	}
	return endOfText, 0
}

func unNop(i *instr) *instr {
	for i.kind == iNop {
		i = i.next
	}
	return i
}

func (re *Regexp) eliminateNops() {
	for _, inst := range re.inst {
		if inst.kind == iEnd {
			continue
		}
		inst.next = unNop(inst.next)
		if inst.kind == iAlt {
			inst.left = unNop(inst.left)
		}
	}
}

func (re *Regexp) dump() {
	print("prefix <", re.prefix, ">\n")
	for _, inst := range re.inst {
		print(inst.index, ": ")
		inst.print()
		if inst.kind != iEnd {
			print(" -> ", inst.next.index)
		}
		print("\n")
	}
}

func (re *Regexp) doParse() {
	p := newParser(re)
	start := &instr{kind: iStart}
	re.add(start)
	s, e := p.regexp()
	start.next = s
	re.start = start
	e.next = re.add(&instr{kind: iEnd})

	if debug {
		re.dump()
		println()
	}
	if pos < len(i.str) {
		r2, _ = utf8.DecodeRune(i.str[pos:])
	}
	return syntax.EmptyOpContext(r1, r2)
}

// Extract regular text from the beginning of the pattern.
// That text can be used by doExecute to speed up matching.
func (re *Regexp) setPrefix() {
	var b []byte
	var utf = make([]byte, utf8.UTFMax)
	var inst *instr
	// First instruction is start; skip that.
Loop:
	for inst = re.inst[0].next; inst.kind != iEnd; inst = inst.next {
		// stop if this is not a char
		if inst.kind != iChar {
			break
		}
		// stop if this char can be followed by a match for an empty string,
		// which includes closures, ^, and $.
		switch inst.next.kind {
		case iBOT, iEOT, iAlt:
			break Loop
		}
		n := utf8.EncodeRune(inst.char, utf)
		b = append(b, utf[0:n]...)
	}
	// point prefixStart instruction to first non-CHAR after prefix
	re.prefixStart = inst
	re.prefixBytes = b
	re.prefix = string(b)
}

// Compile parses a regular expression and returns, if successful, a Regexp
// object that can be used to match against text.
func Compile(str string) (regexp *Regexp, error os.Error) {
	regexp = new(Regexp)
	// doParse will panic if there is a parse error.
	defer func() {
		if e := recover(); e != nil {
			regexp = nil
			error = e.(Error) // Will re-panic if error was not an Error, e.g. nil-pointer exception
		}
	}()
	regexp.expr = str
	regexp.inst = make([]*instr, 0, 10)
	regexp.doParse()
	return
}

	}
	r, w, err := i.r.ReadRune()
	if err != nil {
		i.atEOT = true
		return endOfText, 0
	}
	i.pos += w
	return r, w
}

func (i *inputReader) canCheckPrefix() bool {
	return false
}

func (i *inputReader) hasPrefix(re *Regexp) bool {
	return false
}

func (i *inputReader) index(re *Regexp, pos int) int {
	return -1
}

func (i *inputReader) context(pos int) syntax.EmptyOp {
	return 0
}

// LiteralPrefix returns a literal string that must begin any match
// of the regular expression re.  It returns the boolean true if the
// literal string comprises the entire regular expression.
func (re *Regexp) LiteralPrefix() (prefix string, complete bool) {
	return re.prefix, re.prefixComplete
}

// MatchReader reports whether the Regexp matches the text read by the
// RuneReader.
func (re *Regexp) MatchReader(r io.RuneReader) bool {
	return re.doExecute(r, nil, "", 0, 0) != nil
}

type state struct {
	inst     *instr // next instruction to execute
	prefixed bool   // this match began with a fixed prefix
	match    *matchVec
}

// Append new state to to-do list.  Leftmost-longest wins so avoid
// adding a state that's already active.  The matchVec will be inc-ref'ed
// if it is assigned to a state.
func (a *matchArena) addState(s []state, inst *instr, prefixed bool, match *matchVec, pos, end int) []state {
	switch inst.kind {
	case iBOT:
		if pos == 0 {
			s = a.addState(s, inst.next, prefixed, match, pos, end)
		}
		return s
	case iEOT:
		if pos == end {
			s = a.addState(s, inst.next, prefixed, match, pos, end)
		}
		return s
	case iBra:
		match.m[inst.braNum] = pos
		s = a.addState(s, inst.next, prefixed, match, pos, end)
		return s
	}
	l := len(s)
	// States are inserted in order so it's sufficient to see if we have the same
	// instruction; no need to see if existing match is earlier (it is).
	for i := 0; i < l; i++ {
		if s[i].inst == inst {
			return s
		}
	}
	s = append(s, state{inst, prefixed, match})
	match.ref++
	if inst.kind == iAlt {
		s = a.addState(s, inst.left, prefixed, a.copy(match), pos, end)
		// give other branch a copy of this match vector
		s = a.addState(s, inst.next, prefixed, a.copy(match), pos, end)
	}
	return s
}

// MatchReader checks whether a textual regular expression matches the text
// read by the RuneReader.  More complicated queries need to use Compile and
// the full Regexp interface.
func MatchReader(pattern string, r io.RuneReader) (matched bool, err error) {
	re, err := Compile(pattern)
	if err != nil {
		return false, err
	}
	// fast check for initial plain substring
	prefixed := false // has this iteration begun by skipping a prefix?
	if re.prefix != "" {
		var advance int
		if bytestr == nil {
			advance = strings.Index(str[pos:], re.prefix)
		} else {
			advance = bytes.Index(bytestr[pos:], re.prefixBytes)
		}
		if advance == -1 {
			return nil
		}
		pos += advance + len(re.prefix)
		prefixed = true
	}
	arena := &matchArena{nil, 2 * (re.nbra + 1)}
	for pos <= end {
		if !found {
			// prime the pump if we haven't seen a match yet
			match := arena.noMatch()
			match.m[0] = pos
			if prefixed {
				s[out] = arena.addState(s[out], re.prefixStart, true, match, pos, end)
				prefixed = false // next iteration should start at beginning of machine.
			} else {
				s[out] = arena.addState(s[out], re.start.next, false, match, pos, end)
			}
			arena.free(match) // if addState saved it, ref was incremented
		}
		in, out = out, in // old out state is new in state
		// clear out old state
		old := s[out]
		for _, state := range old {
			arena.free(state.match)
		}
		s[out] = old[0:0] // truncate state vector
		if found && len(s[in]) == 0 {
			// machine has completed
			break
		}
		charwidth := 1
		c := endOfFile
		if pos < end {
			if bytestr == nil {
				c, charwidth = utf8.DecodeRuneInString(str[pos:end])
			} else {
				c, charwidth = utf8.DecodeRune(bytestr[pos:end])
			}
		}
		pos += charwidth
		for _, st := range s[in] {
			switch st.inst.kind {
			case iBOT:
			case iEOT:
			case iChar:
				if c == st.inst.char {
					s[out] = arena.addState(s[out], st.inst.next, st.prefixed, st.match, pos, end)
				}
			case iCharClass:
				if st.inst.cclass.matches(c) {
					s[out] = arena.addState(s[out], st.inst.next, st.prefixed, st.match, pos, end)
				}
			case iAny:
				if c != endOfFile {
					s[out] = arena.addState(s[out], st.inst.next, st.prefixed, st.match, pos, end)
				}
			case iNotNL:
				if c != endOfFile && c != '\n' {
					s[out] = arena.addState(s[out], st.inst.next, st.prefixed, st.match, pos, end)
				}
			case iBra:
			case iAlt:
			case iEnd:
				// choose leftmost longest
				if !found || // first
					st.match.m[0] < final.match.m[0] || // leftmost
					(st.match.m[0] == final.match.m[0] && pos-charwidth > final.match.m[1]) { // longest
					if final.match != nil {
						arena.free(final.match)
					}
					final = st
					final.match.ref++
					final.match.m[1] = pos - charwidth
				}
				found = true
			default:
				st.inst.print()
				panic("unknown instruction in execute")
			}
		}
	}
	if final.match == nil {
		return nil
	}
	// if match found, back up start of match by width of prefix.
	if final.prefixed && len(final.match.m) > 0 {
		final.match.m[0] -= len(re.prefix)
	}
	return final.match.m
}

// MatchString checks whether a textual regular expression
// matches a string.  More complicated queries need
// to use Compile and the full Regexp interface.
func MatchString(pattern string, s string) (matched bool, err error) {
	re, err := Compile(pattern)
	if err != nil {
		return false, err
	}
	return re.MatchString(s), nil
}

// Match checks whether a textual regular expression
// matches a byte slice.  More complicated queries need
// to use Compile and the full Regexp interface.
func Match(pattern string, b []byte) (matched bool, err error) {
	re, err := Compile(pattern)
	if err != nil {
		return false, err
	}
	return re.Match(b), nil
}

// ReplaceAllString returns a copy of src, replacing matches of the Regexp
// with the replacement string repl.  Inside repl, $ signs are interpreted as
// in Expand, so for instance $1 represents the text of the first submatch.
func (re *Regexp) ReplaceAllString(src, repl string) string {
	n := 2
	if strings.Index(repl, "$") >= 0 {
		n = 2 * (re.numSubexp + 1)
	}
	b := re.replaceAll(nil, src, n, func(dst []byte, match []int) []byte {
		return re.expand(dst, repl, nil, src, match)
	})
	return string(b)
}

// ReplaceAllStringLiteral returns a copy of src, replacing matches of the Regexp
// with the replacement string repl.  The replacement repl is substituted directly,
// without using Expand.
func (re *Regexp) ReplaceAllLiteralString(src, repl string) string {
	return string(re.replaceAll(nil, src, 2, func(dst []byte, match []int) []byte {
		return append(dst, repl...)
	}))
}

// ReplaceAllStringFunc returns a copy of src in which all matches of the
// Regexp have been replaced by the return value of function repl applied
// to the matched substring.  The replacement returned by repl is substituted
// directly, without using Expand.
func (re *Regexp) ReplaceAllStringFunc(src string, repl func(string) string) string {
	b := re.replaceAll(nil, src, 2, func(dst []byte, match []int) []byte {
		return append(dst, repl(src[match[0]:match[1]])...)
	})
	return string(b)
}

func (re *Regexp) replaceAll(bsrc []byte, src string, nmatch int, repl func(dst []byte, m []int) []byte) []byte {
	lastMatchEnd := 0 // end position of the most recent match
	searchPos := 0    // position where we next look for a match
	var buf []byte
	var endPos int
	if bsrc != nil {
		endPos = len(bsrc)
	} else {
		endPos = len(src)
	}
	for searchPos <= endPos {
		a := re.doExecute(nil, bsrc, src, searchPos, nmatch)
		if len(a) == 0 {
			break // no more matches
		}

		// Copy the unmatched characters before this match.
		if bsrc != nil {
			buf = append(buf, bsrc[lastMatchEnd:a[0]]...)
		} else {
			buf = append(buf, src[lastMatchEnd:a[0]]...)
		}

		// Now insert a copy of the replacement string, but not for a
		// match of the empty string immediately after another match.
		// (Otherwise, we get double replacement for patterns that
		// match both empty and nonempty strings.)
		if a[1] > lastMatchEnd || a[0] == 0 {
			buf = repl(buf, a)
		}
		lastMatchEnd = a[1]

		// Advance past this match; always advance at least one character.
		var width int
		if bsrc != nil {
			_, width = utf8.DecodeRune(bsrc[searchPos:])
		} else {
			_, width = utf8.DecodeRuneInString(src[searchPos:])
		}
		if searchPos+width > a[1] {
			searchPos += width
		} else if searchPos+1 > a[1] {
			// This clause is only needed at the end of the input
			// string.  In that case, DecodeRuneInString returns width=0.
			searchPos++
		} else {
			searchPos = a[1]
		}
	}

	// Copy the unmatched characters after the last match.
	if bsrc != nil {
		buf = append(buf, bsrc[lastMatchEnd:]...)
	} else {
		buf = append(buf, src[lastMatchEnd:]...)
	}

	return buf
}

// ReplaceAll returns a copy of src, replacing matches of the Regexp
// with the replacement text repl.  Inside repl, $ signs are interpreted as
// in Expand, so for instance $1 represents the text of the first submatch.
func (re *Regexp) ReplaceAll(src, repl []byte) []byte {
	n := 2
	if bytes.IndexByte(repl, '$') >= 0 {
		n = 2 * (re.numSubexp + 1)
	}
	srepl := ""
	b := re.replaceAll(src, "", n, func(dst []byte, match []int) []byte {
		if len(srepl) != len(repl) {
			srepl = string(repl)
		}
		return re.expand(dst, srepl, src, "", match)
	})
	return b
}

// ReplaceAllLiteral returns a copy of src, replacing matches of the Regexp
// with the replacement bytes repl.  The replacement repl is substituted directly,
// without using Expand.
func (re *Regexp) ReplaceAllLiteral(src, repl []byte) []byte {
	return re.replaceAll(src, "", 2, func(dst []byte, match []int) []byte {
		return append(dst, repl...)
	})
}

// ReplaceAllFunc returns a copy of src in which all matches of the
// Regexp have been replaced by the return value of function repl applied
// to the matched byte slice.  The replacement returned by repl is substituted
// directly, without using Expand.
func (re *Regexp) ReplaceAllFunc(src []byte, repl func([]byte) []byte) []byte {
	return re.replaceAll(src, "", 2, func(dst []byte, match []int) []byte {
		return append(dst, repl(src[match[0]:match[1]])...)
	})
}

var specialBytes = []byte(`\.+*?()|[]{}^$`)

func special(b byte) bool {
	return bytes.IndexByte(specialBytes, b) >= 0
}

// QuoteMeta returns a string that quotes all regular expression metacharacters
// inside the argument text; the returned string is a regular expression matching
// the literal text.  For example, QuoteMeta(`[foo]`) returns `\[foo\]`.
func QuoteMeta(s string) string {
	b := make([]byte, 2*len(s))

	// A byte loop is correct because all metacharacters are ASCII.
	j := 0
	for i := 0; i < len(s); i++ {
		if special(s[i]) {
			b[j] = '\\'
			j++
		}
		b[j] = s[i]
		j++
	}
	return string(b[0:j])
}

// The number of capture values in the program may correspond
// to fewer capturing expressions than are in the regexp.
// For example, "(a){0}" turns into an empty program, so the
// maximum capture in the program is 0 but we need to return
// an expression for \1.  Pad appends -1s to the slice a as needed.
func (re *Regexp) pad(a []int) []int {
	if a == nil {
		// No match.
		return nil
	}
	n := (1 + re.numSubexp) * 2
	for len(a) < n {
		a = append(a, -1)
	}
	return a
}

// Find matches in slice b if b is non-nil, otherwise find matches in string s.
func (re *Regexp) allMatches(s string, b []byte, n int, deliver func([]int)) {
	var end int
	if b == nil {
		end = len(s)
	} else {
		end = len(b)
	}

	for pos, i, prevMatchEnd := 0, 0, -1; i < n && pos <= end; {
		matches := re.doExecute(nil, b, s, pos, re.prog.NumCap)
		if len(matches) == 0 {
			break
		}

		accept := true
		if matches[1] == pos {
			// We've found an empty match.
			if matches[0] == prevMatchEnd {
				// We don't allow an empty match right
				// after a previous match, so ignore it.
				accept = false
			}
			var width int
			// TODO: use step()
			if b == nil {
				_, width = utf8.DecodeRuneInString(s[pos:end])
			} else {
				_, width = utf8.DecodeRune(b[pos:end])
			}
			if width > 0 {
				pos += width
			} else {
				pos = end + 1
			}
		} else {
			pos = matches[1]
		}
		prevMatchEnd = matches[1]

		if accept {
			deliver(re.pad(matches))
			i++
		}
	}
}

// Find returns a slice holding the text of the leftmost match in b of the regular expression.
// A return value of nil indicates no match.
func (re *Regexp) Find(b []byte) []byte {
	a := re.doExecute(nil, b, "", 0, 2)
	if a == nil {
		return nil
	}
	return b[a[0]:a[1]]
}

// FindIndex returns a two-element slice of integers defining the location of
// the leftmost match in b of the regular expression.  The match itself is at
// b[loc[0]:loc[1]].
// A return value of nil indicates no match.
func (re *Regexp) FindIndex(b []byte) (loc []int) {
	a := re.doExecute(nil, b, "", 0, 2)
	if a == nil {
		return nil
	}
	return a[0:2]
}

// FindString returns a string holding the text of the leftmost match in s of the regular
// expression.  If there is no match, the return value is an empty string,
// but it will also be empty if the regular expression successfully matches
// an empty string.  Use FindStringIndex or FindStringSubmatch if it is
// necessary to distinguish these cases.
func (re *Regexp) FindString(s string) string {
	a := re.doExecute(nil, nil, s, 0, 2)
	if a == nil {
		return ""
	}
	return s[a[0]:a[1]]
}

// FindStringIndex returns a two-element slice of integers defining the
// location of the leftmost match in s of the regular expression.  The match
// itself is at s[loc[0]:loc[1]].
// A return value of nil indicates no match.
func (re *Regexp) FindStringIndex(s string) (loc []int) {
	a := re.doExecute(nil, nil, s, 0, 2)
	if a == nil {
		return nil
	}
	return a[0:2]
}

// FindReaderIndex returns a two-element slice of integers defining the
// location of the leftmost match of the regular expression in text read from
// the RuneReader.  The match text was found in the input stream at
// byte offset loc[0] through loc[1]-1.
// A return value of nil indicates no match.
func (re *Regexp) FindReaderIndex(r io.RuneReader) (loc []int) {
	a := re.doExecute(r, nil, "", 0, 2)
	if a == nil {
		return nil
	}
	return a[0:2]
}

// FindSubmatch returns a slice of slices holding the text of the leftmost
// match of the regular expression in b and the matches, if any, of its
// subexpressions, as defined by the 'Submatch' descriptions in the package
// comment.
// A return value of nil indicates no match.
func (re *Regexp) FindSubmatch(b []byte) [][]byte {
	a := re.doExecute(nil, b, "", 0, re.prog.NumCap)
	if a == nil {
		return nil
	}
	ret := make([][]byte, 1+re.numSubexp)
	for i := range ret {
		if 2*i < len(a) && a[2*i] >= 0 {
			ret[i] = b[a[2*i]:a[2*i+1]]
		}
	}
	return ret
}

// Expand appends template to dst and returns the result; during the
// append, Expand replaces variables in the template with corresponding
// matches drawn from src.  The match slice should have been returned by
// FindSubmatchIndex.
//
// In the template, a variable is denoted by a substring of the form
// $name or ${name}, where name is a non-empty sequence of letters,
// digits, and underscores.  A purely numeric name like $1 refers to
// the submatch with the corresponding index; other names refer to
// capturing parentheses named with the (?P<name>...) syntax.  A
// reference to an out of range or unmatched index or a name that is not
// present in the regular expression is replaced with an empty slice.
//
// In the $name form, name is taken to be as long as possible: $1x is
// equivalent to ${1x}, not ${1}x, and, $10 is equivalent to ${10}, not ${1}0.
//
// To insert a literal $ in the output, use $$ in the template.
func (re *Regexp) Expand(dst []byte, template []byte, src []byte, match []int) []byte {
	return re.expand(dst, string(template), src, "", match)
}

// ExpandString is like Expand but the template and source are strings.
// It appends to and returns a byte slice in order to give the calling
// code control over allocation.
func (re *Regexp) ExpandString(dst []byte, template string, src string, match []int) []byte {
	return re.expand(dst, template, nil, src, match)
}

func (re *Regexp) expand(dst []byte, template string, bsrc []byte, src string, match []int) []byte {
	for len(template) > 0 {
		i := strings.Index(template, "$")
		if i < 0 {
			break
		}
		dst = append(dst, template[:i]...)
		template = template[i:]
		if len(template) > 1 && template[1] == '$' {
			// Treat $$ as $.
			dst = append(dst, '$')
			template = template[2:]
			continue
		}
		name, num, rest, ok := extract(template)
		if !ok {
			// Malformed; treat $ as raw text.
			dst = append(dst, '$')
			template = template[1:]
			continue
		}
		template = rest
		if num >= 0 {
			if 2*num+1 < len(match) && match[2*num] >= 0 {
				if bsrc != nil {
					dst = append(dst, bsrc[match[2*num]:match[2*num+1]]...)
				} else {
					dst = append(dst, src[match[2*num]:match[2*num+1]]...)
				}
			}
		} else {
			for i, namei := range re.subexpNames {
				if name == namei && 2*i+1 < len(match) && match[2*i] >= 0 {
					if bsrc != nil {
						dst = append(dst, bsrc[match[2*i]:match[2*i+1]]...)
					} else {
						dst = append(dst, src[match[2*i]:match[2*i+1]]...)
					}
					break
				}
			}
		}
	}
	dst = append(dst, template...)
	return dst
}

// extract returns the name from a leading "$name" or "${name}" in str.
// If it is a number, extract returns num set to that number; otherwise num = -1.
func extract(str string) (name string, num int, rest string, ok bool) {
	if len(str) < 2 || str[0] != '$' {
		return
	}
	brace := false
	if str[1] == '{' {
		brace = true
		str = str[2:]
	} else {
		str = str[1:]
	}
	i := 0
	for i < len(str) {
		rune, size := utf8.DecodeRuneInString(str[i:])
		if !unicode.IsLetter(rune) && !unicode.IsDigit(rune) && rune != '_' {
			break
		}
		i += size
	}
	if i == 0 {
		// empty name is not okay
		return
	}
	name = str[:i]
	if brace {
		if i >= len(str) || str[i] != '}' {
			// missing closing brace
			return
		}
		i++
	}

	// Parse number.
	num = 0
	for i := 0; i < len(name); i++ {
		if name[i] < '0' || '9' < name[i] || num >= 1e8 {
			num = -1
			break
		}
		num = num*10 + int(name[i]) - '0'
	}
	// Disallow leading zeros.
	if name[0] == '0' && len(name) > 1 {
		num = -1
	}

	rest = str[i:]
	ok = true
	return
}

// FindSubmatchIndex returns a slice holding the index pairs identifying the
// leftmost match of the regular expression in b and the matches, if any, of
// its subexpressions, as defined by the 'Submatch' and 'Index' descriptions
// in the package comment.
// A return value of nil indicates no match.
func (re *Regexp) FindSubmatchIndex(b []byte) []int {
	return re.pad(re.doExecute(nil, b, "", 0, re.prog.NumCap))
}

// FindStringSubmatch returns a slice of strings holding the text of the
// leftmost match of the regular expression in s and the matches, if any, of
// its subexpressions, as defined by the 'Submatch' description in the
// package comment.
// A return value of nil indicates no match.
func (re *Regexp) FindStringSubmatch(s string) []string {
	a := re.doExecute(nil, nil, s, 0, re.prog.NumCap)
	if a == nil {
		return nil
	}
	ret := make([]string, 1+re.numSubexp)
	for i := range ret {
		if 2*i < len(a) && a[2*i] >= 0 {
			ret[i] = s[a[2*i]:a[2*i+1]]
		}
	}
	return ret
}

// FindStringSubmatchIndex returns a slice holding the index pairs
// identifying the leftmost match of the regular expression in s and the
// matches, if any, of its subexpressions, as defined by the 'Submatch' and
// 'Index' descriptions in the package comment.
// A return value of nil indicates no match.
func (re *Regexp) FindStringSubmatchIndex(s string) []int {
	return re.pad(re.doExecute(nil, nil, s, 0, re.prog.NumCap))
}

// FindReaderSubmatchIndex returns a slice holding the index pairs
// identifying the leftmost match of the regular expression of text read by
// the RuneReader, and the matches, if any, of its subexpressions, as defined
// by the 'Submatch' and 'Index' descriptions in the package comment.  A
// return value of nil indicates no match.
func (re *Regexp) FindReaderSubmatchIndex(r io.RuneReader) []int {
	return re.pad(re.doExecute(r, nil, "", 0, re.prog.NumCap))
}

const startSize = 10 // The size at which to start a slice in the 'All' routines.

// FindAll is the 'All' version of Find; it returns a slice of all successive
// matches of the expression, as defined by the 'All' description in the
// package comment.
// A return value of nil indicates no match.
func (re *Regexp) FindAll(b []byte, n int) [][]byte {
	if n < 0 {
		n = len(b) + 1
	}
	result := make([][]byte, 0, startSize)
	re.allMatches("", b, n, func(match []int) {
		result = append(result, b[match[0]:match[1]])
	})
	if len(result) == 0 {
		return nil
	}
	return result
}

// FindAllIndex is the 'All' version of FindIndex; it returns a slice of all
// successive matches of the expression, as defined by the 'All' description
// in the package comment.
// A return value of nil indicates no match.
func (re *Regexp) FindAllIndex(b []byte, n int) [][]int {
	if n < 0 {
		n = len(b) + 1
	}
	result := make([][]int, 0, startSize)
	re.allMatches("", b, n, func(match []int) {
		result = append(result, match[0:2])
	})
	if len(result) == 0 {
		return nil
	}
	return result
}

// FindAllString is the 'All' version of FindString; it returns a slice of all
// successive matches of the expression, as defined by the 'All' description
// in the package comment.
// A return value of nil indicates no match.
func (re *Regexp) FindAllString(s string, n int) []string {
	if n < 0 {
		n = len(s) + 1
	}
	result := make([]string, 0, startSize)
	re.allMatches(s, nil, n, func(match []int) {
		result = append(result, s[match[0]:match[1]])
	})
	if len(result) == 0 {
		return nil
	}
	return result
}

// FindAllStringIndex is the 'All' version of FindStringIndex; it returns a
// slice of all successive matches of the expression, as defined by the 'All'
// description in the package comment.
// A return value of nil indicates no match.
func (re *Regexp) FindAllStringIndex(s string, n int) [][]int {
	if n < 0 {
		n = len(s) + 1
	}
	result := make([][]int, 0, startSize)
	re.allMatches(s, nil, n, func(match []int) {
		result = append(result, match[0:2])
	})
	if len(result) == 0 {
		return nil
	}
	return result
}

// FindAllSubmatch is the 'All' version of FindSubmatch; it returns a slice
// of all successive matches of the expression, as defined by the 'All'
// description in the package comment.
// A return value of nil indicates no match.
func (re *Regexp) FindAllSubmatch(b []byte, n int) [][][]byte {
	if n < 0 {
		n = len(b) + 1
	}
	result := make([][][]byte, 0, startSize)
	re.allMatches("", b, n, func(match []int) {
		slice := make([][]byte, len(match)/2)
		for j := range slice {
			if match[2*j] >= 0 {
				slice[j] = b[match[2*j]:match[2*j+1]]
			}
		}
		result = append(result, slice)
	})
	if len(result) == 0 {
		return nil
	}
	return result
}

// FindAllSubmatchIndex is the 'All' version of FindSubmatchIndex; it returns
// a slice of all successive matches of the expression, as defined by the
// 'All' description in the package comment.
// A return value of nil indicates no match.
func (re *Regexp) FindAllSubmatchIndex(b []byte, n int) [][]int {
	if n < 0 {
		n = len(b) + 1
	}
	result := make([][]int, 0, startSize)
	re.allMatches("", b, n, func(match []int) {
		result = append(result, match)
	})
	if len(result) == 0 {
		return nil
	}
	return result
}

// FindAllStringSubmatch is the 'All' version of FindStringSubmatch; it
// returns a slice of all successive matches of the expression, as defined by
// the 'All' description in the package comment.
// A return value of nil indicates no match.
func (re *Regexp) FindAllStringSubmatch(s string, n int) [][]string {
	if n < 0 {
		n = len(s) + 1
	}
	result := make([][]string, 0, startSize)
	re.allMatches(s, nil, n, func(match []int) {
		slice := make([]string, len(match)/2)
		for j := range slice {
			if match[2*j] >= 0 {
				slice[j] = s[match[2*j]:match[2*j+1]]
			}
		}
		result = append(result, slice)
	})
	if len(result) == 0 {
		return nil
	}
	return result
}

// FindAllStringSubmatchIndex is the 'All' version of
// FindStringSubmatchIndex; it returns a slice of all successive matches of
// the expression, as defined by the 'All' description in the package
// comment.
// A return value of nil indicates no match.
func (re *Regexp) FindAllStringSubmatchIndex(s string, n int) [][]int {
	if n < 0 {
		n = len(s) + 1
	}
	result := make([][]int, 0, startSize)
	re.allMatches(s, nil, n, func(match []int) {
		result = append(result, match)
	})
	if len(result) == 0 {
		return nil
	}
	return result
}

// Split slices s into substrings separated by the expression and returns a slice of
// the substrings between those expression matches.
//
// The slice returned by this method consists of all the substrings of s
// not contained in the slice returned by FindAllString. When called on an expression
// that contains no metacharacters, it is equivalent to strings.SplitN.
//
// Example:
//   s := regexp.MustCompile("a*").Split("abaabaccadaaae", 5)
//   // s: ["", "b", "b", "c", "cadaaae"]
//
// The count determines the number of substrings to return:
//   n > 0: at most n substrings; the last substring will be the unsplit remainder.
//   n == 0: the result is nil (zero substrings)
//   n < 0: all substrings
func (re *Regexp) Split(s string, n int) []string {

	if n == 0 {
		return nil
	}

	if len(re.expr) > 0 && len(s) == 0 {
		return []string{""}
	}

	matches := re.FindAllStringIndex(s, n)
	strings := make([]string, 0, len(matches))

	beg := 0
	end := 0
	for _, match := range matches {
		if n > 0 && len(strings) >= n-1 {
			break
		}

		end = match[0]
		if match[1] != 0 {
			strings = append(strings, s[beg:end])
		}
		beg = match[1]
	}

	if end != len(s) {
		strings = append(strings, s[beg:])
	}

	return strings
}
