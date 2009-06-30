// {([
//
// Copyright (c) 2008 Google Inc. All rights reserved.
//
// rrh@google.com 08/08/2008
//
// $Header: $
//

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

int string_distance(const char *a, const char *b)
{
  const size_t n = strlen(a);
  const size_t m = strlen(b);
  int *c = new int[(n+1)*(m+1)];
#define access(i, j) ((i)*(m+1)+(j))
  for (size_t i = 0; i <= n; i++) {
    c[access(i,0)] = i;
  }
  for (size_t j = 0; j <= m; j++) {
    c[access(0,j)] = j;
  }
  for (size_t i = 1; i <= n; i++) {
    for (size_t j = 1; j <= m; j++) {
      const int x = c[access(i-1,j-0)] + 1;
      const int y = c[access(i-0,j-1)] + 1;
	    int z = c[access(i-1,j-1)] + 0;
      if (a[i-1] != b[j-1]) {
	z += 1;
      }
      if (x <= y && x <= z) {
	c[access(i,j)] = x;
      } else if (y <= x && y <= z) {
	c[access(i,j)] = y;
      } else {
	c[access(i,j)] = z;
      }
    }
  }
  const int back = c[access(n, m)];
  delete [] c;
  return back;
}

double string_similarity (const char *a, const char *b, int &edit_distance)
{
  const size_t totalLength = strlen(a) + strlen(b);
  edit_distance = string_distance(a, b);
  double m1 = (1.0*totalLength - 2.0*edit_distance)/totalLength;
  if (m1 < 0) {
    m1 = 0.0;
  }
  return m1;
}

//
// Test jig
//
void string_distance_test(const char *a, const char *b)
{
  fprintf(stdout, "string_distance %10s %10s ==> %2d\n",
    a, b, string_distance(a, b)
  );
}

void string_distance_unittest(void)
{
  string_distance_test("abc", "abc");
  string_distance_test("ab", "abc");
  string_distance_test("bc", "abc");
  string_distance_test("Abc", "abc");
  string_distance_test("abC", "abc");
  string_distance_test("AbC", "abc");
}

void append(const char *&dst, const char *s1, const char *s2)
{
  const size_t nlg = 0
    + (dst ? ::strlen(dst) : 0)
    + (s1 ? ::strlen(s1) : 0)
    + (s2 ? ::strlen(s2) : 0)
  ;
  char *back = new char[nlg+1];
  ::snprintf(back, nlg+1, "%s%s%s",
    dst ? dst : "",
    s1 ? s1 : "",
    s2 ? s2 : ""
  );
  delete [] dst; dst = 0;
  dst = back;
}

const char *stringtrim(const char *src)
{
  if (src == 0) {
    return 0;
  }
  const char *first = src;
  while (*first && isspace(*first)) {
    first += 1;
  }
  const char *last = src + strlen(src) - 1;
  while (last >= first && isspace(*last)) {
    last -= 1;
  }
  const size_t lg = last - first + 1;
  char *back = new char[lg+1];
  ::strncpy(back, first, lg);
  back[lg] = 0;
  return back;
}

//
// Read the grammar files enumerated on the argument line.
// Output one file for each block of rules.
// The output files end up in the directory "./tmp"
//

int global_block_number = 0;

class Key {
public:
  const char *ifname;
  int lineno_s;
  int lineno_e;
};

class KeyRecord {
public:
  Key key;
  const char *ofname;		// output file name

  const char *rules;		// rules (grammar)
  const char *trimrules;	// string trimmed rules (grammar)
  size_t lgrules;		// length of rules, bytes

  const char *sems;		// semantic actions (block-labeled C code)
  const char *trimsems;		// string trimmed semantic actions
  size_t lgsems;		// length of semantic actions, bytes

  KeyRecord *best_other;	//              best other KeyRecord
  double best_similarity;	// similarty of best other KeyRecord
  int best_edit_distance;	// edit dist of best other KeyRecord

public:
  KeyRecord(void);
};

KeyRecord::KeyRecord(void) {
  ofname = 0;
  rules = 0;
  trimrules = 0;
  sems = 0;
  trimsems = 0;
  lgrules = 0;
  lgsems = 0;
  best_other = 0;
  best_similarity = 0.0;
  best_edit_distance = -1;
}

void print_singleton(FILE *fp, const KeyRecord *rp)
{
  const Key &k = rp->key;
  fprintf(fp, " {%s %4d %4d}", k.ifname, k.lineno_s, k.lineno_e);
}

void print_pairs(FILE *fp, const KeyRecord *ri, const KeyRecord *rj)
{
  if (ri && rj) {
    fprintf(fp, " tkdiff %4s %4s ", ri->ofname, rj->ofname);
  }
  if (ri) {
    print_singleton(fp, ri);
  }
  fprintf(fp, " vs");
  if (rj) {
    print_singleton(fp, rj);
  }
}

//
// Obsolete: this was pre-pythonization of the plug spec files
//
int read_grammar_file(const char *ifname)
{
  int lineno = 0;
  extern int global_block_number;
  global_block_number += 1;
  FILE *ifd = ::fopen(ifname, "r");
  if (ifd == NULL) {
    return -1;
  }

  enum State {
    State_none,
    State_rules
  };
  State state = State_none;

  int lineno_s = -1;
  int lineno_e = -1;

  const char *rules = 0;	// all syntactic rules in this block
  const char *trimrules = 0;	// all syntactic rules in this block, trimmed
  int lgrules = 0;		// number of lines in rules

  const char *sems = 0;		// all semantic actions in this block
  const char *trimsems = 0;	// all semantic actions in this block, trimmed
  int lgsems  = 0;

  enum {
    NRULES=600
  };
  KeyRecord *ruleinfo[NRULES];

  for (int i = 0; i < NRULES; i++) {
    ruleinfo[i] = 0;
  }

  int nruleinfo = 0;
  int in_shard_region = 0;

  char line[8*BUFSIZ];
  while (::fgets(line, sizeof(line), ifd) == line) {
    lineno += 1;
    if (!in_shard_region) {
      if (::strncmp(line, "<SHARD>", 7) == 0) {
        in_shard_region = 1;
      }
      continue;
    }
    //
    // look for some kind of syntactic rule
    //
    if (isalpha(line[0]) && ::index(line, ':') > line+0) {
      if (state != State_rules) {
        lineno_s = lineno;
	state = State_rules;
	rules = 0;
	trimrules = 0;
	sems = 0;
	trimsems = 0;
      }
      append(trimrules, stringtrim(line), "\n");
      append(rules, line, 0);
      lgrules += 1;
      continue;
    }

    //
    // some intermediate line, between rule definitions
    // and the closing syntax of the rule block
    //
    if (state == State_rules) {
      append(trimsems, stringtrim(line), "\n");
      append(sems, line, 0);
      lgsems +=1;
    }

    //
    // end of block
    //
    if (::strstr(line, "\175;")) {
      lineno_e = lineno;
      char ofname[BUFSIZ];
      ::snprintf(ofname, sizeof(ofname), "tmp/%03d", global_block_number);
      FILE *ofd = ::fopen(ofname, "w");
      if (ofd == NULL) {
        continue;
      }
      fprintf(ofd, \
	"// file %4s contains %3d+%3d lines from %5d .. %5d of file %s\n",
	ofname,
	lgrules,
	lgsems,
	lineno_s, lineno_e,
	ifname
      );
      fprintf(ofd, "%s", rules);
      fprintf(ofd, "%s", sems);
      ::fclose(ofd);

      KeyRecord *p = new KeyRecord();
      ruleinfo[nruleinfo] = p;
      nruleinfo += 1;

      if (nruleinfo >= NRULES) {
        fprintf(stderr, "too many rules, max of %d\n", NRULES);
	exit(1);
      }
      p->ofname = strdup(ofname);

      p->key.ifname = ifname;
      p->key.lineno_s = lineno_s;
      p->key.lineno_e = lineno_e;

      p->trimsems = trimsems;
      p->sems = sems;
      p->lgsems = lgsems;

      p->trimrules = trimrules;
      p->rules = rules;
      p->lgrules = lgrules;

      rules = 0;
      trimrules = 0;
      lgrules = 0;

      sems = 0;
      trimsems = 0;
      lgsems = 0;

      global_block_number += 1;
      state = State_none;
    }
  }
  ::fclose(ifd); ifd = 0;

  const int nkey = nruleinfo;

  //
  // Compare 2 blocks of semantic actions that
  // are within $offset line numbers of each other.
  // use the Levenshtein distance (minimal number of edits)
  //
  for (int i = 0; i < nkey; i++) {
    KeyRecord *ri = ruleinfo[i];
    if (ri->best_other) continue;
    fprintf(stdout, "\n");		// block spacing
    for (int j = 0; j < nkey; j++) {
      KeyRecord *rj = ruleinfo[j];
      if (rj->best_other) continue;

      int edit_distance = 0;
      const double similarity =
	string_similarity(ri->trimsems, rj->trimsems, edit_distance);
      if (edit_distance < 30) {
        rj->best_other = ri;
        rj->best_edit_distance = edit_distance;
        rj->best_similarity = similarity;
	if (ri != rj) {
	  fprintf(stdout, "edit= %4d sim= %8.4f",
	    edit_distance, similarity
	  );
	  print_pairs(stdout, ri, rj);
	  fprintf(stdout, "\n");
	}
      }

    }
  }

  return 0;
}

struct simcarrier {
  double similarity;
  int i;
  int j;
};

int simcarriersorter(const void *vp1, const void *vp2)
{
  const simcarrier *p1 = (const simcarrier *)vp1;
  const simcarrier *p2 = (const simcarrier *)vp2;
  const int reverse = -1;
  if (p1->similarity < p2->similarity) {
    return reverse * -1;
  }
  if (p1->similarity > p2->similarity) {
    return  reverse * 1;
  }
  return 0;
}

int abs(const int x)
{
  return x < 0 ? -x : x;
}
void print_info(FILE *fp, const simcarrier *info, int index,
  size_t *lengths, const char **names)
{
  const int i = info[index].i;
  const int j = info[index].j;
  fprintf(stdout, "%5.3f  %5d %5d    tkdiff -w %30s %30s\n",
    info[index].similarity,
    lengths[i], lengths[j],
    names[i], names[j]
  );
}

int main(int argc, const char *argv[])
{
  if (0) {
    string_distance_unittest();
  }
  if (0) {
    ::mkdir("./tmp", 0777);
    for (int i = 1; i < argc; i++) {
      read_grammar_file(argv[i]);
    }
  }
  if (1) {
    const char **contents = new const char *[argc];
    size_t *lengths = new size_t[argc];
    for (int i = 1; i < argc; i++) {
      contents[i] = 0;
      lengths[i] = 0;
    }
    for (int i = 1; i < argc; i++) {
      FILE *ifd = ::fopen(argv[i], "r");
      if (ifd) {
	struct stat sbuf;
	::stat(argv[i], &sbuf);
	lengths[i] = sbuf.st_size;
	contents[i] = new char[lengths[i]];
	char *last = (char *)contents[i];
        char line[8*BUFSIZ];
	while (::fgets(line, sizeof(line), ifd) == line) {
	  const char *trimmed = stringtrim(line);
	  strcat(last, trimmed);
	  last += strlen(last);
	}
      }
      ::fclose(ifd);
    }

    simcarrier *info = new simcarrier[argc*argc];
    for (int i = 1; i < argc; i++) {
      for (int j = 1; j < argc; j++) {
	const int index = i*argc+j;
        info[index].i = i;
        info[index].j = j;
        info[index].similarity = 0.0;
      }
    }
    for (int i = 1; i < argc; i++) {
      // fprintf(stdout, "progress: %4d/%-4d\n", i, argc);
      if (contents[i] == 0) continue;
      const bool isdivi = strstr(contents[i], "MOD") || strstr(contents[i], "DIV");
      int nhit = 0;
      for (int j = i+1; j < argc; j++) {
	if (contents[j] == 0) continue;
	const bool isdivj = strstr(contents[j], "MOD") || strstr(contents[j], "DIV");
	if (abs(lengths[i] - lengths[j]) > 0.20*lengths[j]) {
	  continue;
	}
	const int index = i*argc+j;
	int edit_distance;
	info[index].similarity =
	  string_similarity(contents[i], contents[j], edit_distance);
	if (0
	|| (isdivi && isdivj)
	|| info[index].similarity > 0.80) {
	  print_info(stdout, info, index, lengths, argv);
	  nhit += 1;
	}
      }
      if (nhit) {
        fprintf(stdout, "\n");
	fflush(stdout);
      }
    }

    if (0) {
      fprintf(stdout, "# --- Sorted Similarity Summary ---\n");
      ::qsort(info, argc*argc, sizeof(info[0]), simcarriersorter);
      int nhit = 0;
      for (int index = 0; index < argc*argc; index++) {
	if (info[index].similarity > 0) {
	  if (info[index].similarity < 1.00) {
	    print_info(stdout, info, index, lengths, argv);
	    nhit += 1;
	    if (nhit > 20) {
	      break;
	    }
	  }
	}
      }
    }

  }

  exit(0);
}

// })]
