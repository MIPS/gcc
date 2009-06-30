#define _GNU_SOURCE
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


enum tokens {
   COLON_token =          (1 << 0)
  ,COMMA_token =          (1 << 1)
  ,LPAREN_token =         (1 << 2)
  ,RPAREN_token =         (1 << 3)
  ,LBRACK_token =         (1 << 4)
  ,RBRACK_token =         (1 << 5)
  ,LBRACE_token =         (1 << 6)
  ,RBRACE_token =         (1 << 7)
  ,INT_token =            (1 << 8)
  ,ID_token =             (1 << 9)
  ,DONE_token =           (1 << 10)
  ,SEMI_token =           (1 << 11)
  ,PATH_token =           (1 << 12)
  ,EQUAL_token =          (1 << 13)
  ,REDUCE_token =         (1 << 14)
  ,TERM_token =           (1 << 15)
  ,NONTERM_token =        (1 << 16)
  ,START_token =          (1 << 17)
  ,INCLUDE_token =        (1 << 18)
  ,BAR_token =            (1 << 19)
  ,AT_token =             (1 << 20)
  ,TERM_ALIGN_token =     (1 << 21)
  ,TERM_INCR_token =      (1 << 22)
  /*
   * If you add here, be sure to extend tok2str
   */
  ,unused_0 =             (1 << 23)
  ,unused_1 =             (1 << 24)
  ,unused_2 =             (1 << 25)
  ,unused_3 =             (1 << 26)
  ,unused_4 =             (1 << 27)
  ,unused_5 =             (1 << 28)
  ,unused_6 =             (1 << 29)
  ,unused_7 =             (1 << 30)
  ,unused_8 =             (1 << 31)
};


#define HASH_SIZE 2047

typedef struct hash_node {
  struct hash_node *next;
  const char *id;
  int keyword;
  int kind;               /* reduce, term, nonterm */
  FILE *file;             /* for file-name */
  int kids;               /* for term */
  struct hash_node *path; /* for reduce */
} Hash_Node;


void label_file(FILE *fp)
{
  const char *start_comment = "/*";
  const char *end_comment = "*/";
  if (fp == 0) return;
  fprintf(fp, "%s %s\n", start_comment, end_comment);
  fprintf(fp, "%s THIS FILE IS AUTOMATICALLY PRODUCED BY plug %s\n", start_comment, end_comment);
  fprintf(fp, "%s DO NOT EDIT %s\n", start_comment, end_comment);
  fprintf(fp, "%s %s\n", start_comment, end_comment);
}


FILE *f = 0;
FILE *bf = 0;
FILE *opfile = 0;
FILE *opnfile = 0;
FILE *rulefile = 0;
FILE *infofile = 0;

int first_opcode;

const char *file_name;	/* file name of grammar source file */
const char *base_name;	/* file name only part of file_name */
const char *dir_name;	/* directory name part of file_name */


FILE *open_and_update_names(const char *fn) {
  FILE *f;
  char *ndn;
  if (fn[0]=='/') {
    ndn = strdup(fn);
  } else {
    int fnl = strlen(fn);
    int dnl = strlen(dir_name);
    ndn = (char *) malloc((fnl + dnl + 2)*sizeof(char));
    strcpy(ndn, dir_name);
    strcat(ndn, "/");
    strcat(ndn, fn);
  }
  file_name = (char *) canonicalize_file_name(ndn);
  free(ndn); ndn = 0;
  if (file_name) {
    f = fopen(file_name, "r");
    if (f) {
      dir_name = strdup(file_name);
      ndn = rindex(dir_name, '/');
      if (ndn)
        *ndn = 0;
      else
        fprintf(stderr, "\n" "oops, rindex return NULL\n");

      base_name = rindex(file_name, '/');
      if (!base_name)
        base_name = file_name;
      else
        base_name += 1; /* move one right to the '/' */
      return f;
    } else {
      perror(file_name);
    }
  } else {
    base_name = "<stdin>";
  }
  return 0;
}

int line_number = -1;	/* input file line number; first line is synthetic */
int last_was_nl = 1;	/* last character was a new line */
int start_new_file = 1;	/* */

int token;
int number;
int case_number = 0;
Hash_Node *id;

Hash_Node *start_node = 0;

Hash_Node *hash_table[HASH_SIZE] = { 0 };


unsigned hash(const char *s) {
  unsigned v = *s++;
  while (*s)
    v = 3*v + *s++;
  return v;
}


Hash_Node *find(const char *s) {
  unsigned h = hash(s) % 2047;
  Hash_Node *p = hash_table[h];
  while (p) {
    if (strcmp(s, p->id) == 0)
      return p;
    p = p->next;
  }
  p = (Hash_Node *) malloc(sizeof(Hash_Node));
  p->next = hash_table[h];
  hash_table[h] = p;
  p->id = strdup(s);
  p->keyword = 0;
  p->kind = 0;
  p->file = 0;
  p->kids = 0;
  p->path = 0;
  return p;
}


void init_keywords(void) {
  id = find("start");   id->keyword = 1; id->kind = START_token;
  id = find("term");    id->keyword = 1; id->kind = TERM_token;
  id = find("nonterm"); id->keyword = 1; id->kind = NONTERM_token;
  id = find("reduce");  id->keyword = 1; id->kind = REDUCE_token;
  id = find("include"); id->keyword = 1; id->kind = INCLUDE_token;
  id = find("term_align");   id->keyword = 1; id->kind = TERM_ALIGN_token;
  id = find("term_incr");    id->keyword = 1; id->kind = TERM_INCR_token;
}

void get_token(void) {
  char c = start_new_file ? '\n' : fgetc(f);
  start_new_file = 0;
  while (isspace(c)) {
    last_was_nl = (c == '\n');
    if (last_was_nl) {
      line_number++;
    }
    c = fgetc(f);
  }
  if (last_was_nl) {
    if (c == '#') {  /* consume #line directive */
      char buf[BUFSIZ];
      char *cp = buf;
      while (c != '\n') {
        *cp++ = c; *cp = 0;
	c = fgetc(f);
      }
      last_was_nl = 1;
      {
	char *start_fname = 0;
	int new_line_number = 0;
	char *cp = buf;
	char termch;
	if (*cp == '#') {
	  cp += 1;
	}
	while (isspace(*cp)) {
	  cp += 1;
	}
	if (strncmp(cp, "line", 4) == 0) {
	  cp += 4;
	  while (isspace(*cp)) {
	    cp += 1;
	  }
	}
	new_line_number = strtol(cp, &cp, 0);
	while (isspace(*cp)) {
	  cp += 1;
	}
	if (*cp == '"') {
	  termch = '"';
	  cp += 1;
	} else {
	  termch = 0;
	}
	start_fname = cp;
	while (*cp != termch) {
	  cp += 1;
	}
	*cp = 0;	/* smash end of file name */
	base_name = strdup(start_fname);
	line_number = new_line_number;
	if (0) {
	  printf("fname=%s lineno=%d\n", file_name, line_number);
	}

	get_token();
	return;
      }
    }
  }
  last_was_nl = 0;
  if (c == EOF)
    token = DONE_token;
  else if (c == ':')
    token = COLON_token;
  else if (c == ',')
    token = COMMA_token;
  else if (c == ';')
    token = SEMI_token;
  else if (c == '=')
    token = EQUAL_token;
  else if (c == '(')
    token = LPAREN_token;
  else if (c == ')')
    token = RPAREN_token;
  else if (c == '[')
    token = LBRACK_token;
  else if (c == ']')
    token = RBRACK_token;
  else if (c == '{')
    token = LBRACE_token;
  else if (c == '}')
    token = RBRACE_token;
  else if (c == '|')
    token = BAR_token;
  else if (c == '@')
    token = AT_token;
  else if (c == '/') {  /* allow C++ style comments */
    c = fgetc(f);
    if (c == '/') {
      do {
        c = fgetc(f);
      } while (c != '\n');
      line_number++;
      last_was_nl = 1;
      get_token();
    }
    else {
      if (file_name)
        fprintf(stderr, "%s, ", file_name);
      fprintf(stderr, "line %d, unexpected character '/'>\n", line_number);
      exit(1);
    }
  }
  else if (c == '-') { /* allow Ada style comments */
    c = fgetc(f);
    if (c == '-') {
      do {
        c = fgetc(f);
      } while (c != '\n');
      line_number++;
      last_was_nl = 1;
      get_token();
    }
    else {
      if (file_name)
        fprintf(stderr, "%s, ", file_name);
      fprintf(stderr, "line %d, unexpected character '-'\n", line_number);
      exit(1);
    }
  }
  else if (c == '%') {
    c = fgetc(f);
    if (c == '%') {
      token = DONE_token;
    }
  }
  else if (isalpha(c)) {
    char s[100];
    int i = 0;
    do {
      s[i++] = c;
      c = fgetc(f);
    } while (isalnum(c) || c == '_');
    s[i] = 0;
    ungetc(c, f);
    id = find(s);
    if (id->keyword)
      token = id->kind;
    else
      token = ID_token;
  }
  else if (isdigit(c)) {
    number = c - '0';
    c = fgetc(f);
    while (isdigit(c)) {
      number = 10*number + c - '0';
      c = fgetc(f);
    }
    ungetc(c, f);
    token = INT_token;
  }
  else if (c == '"') {
    char s[100];
    int i = 0;
    c = fgetc(f);
    while (c != '"') {
      s[i++] = c;
      c = fgetc(f);
    }
    s[i] = 0;
    id = find(s);
    token = PATH_token;
  }
  else {
    if (file_name)
      fprintf(stderr, "%s, ", file_name);
    fprintf(stderr, "line %d, unexpected character '", line_number);
    if (isgraph(c))
      fprintf(stderr, "%c'\n", c);
    else
      fprintf(stderr, "0x%02x'\n", c);
    exit(1);
  }
  if (0) {
    printf("line_number=%4d token=0x%08x\n", line_number, token);
  }
}


const char *tok2str(int t) {
  if (t == COLON_token)
    return ":";
  if (t == SEMI_token)
    return ";";
  if (t == EQUAL_token)
    return "=";
  if (t == COMMA_token)
    return ",";
  if (t == LPAREN_token)
    return "(";
  if (t == RPAREN_token)
    return ")";
  if (t == LBRACK_token)
    return "[";
  if (t == RBRACK_token)
    return "]";
  if (t == LBRACE_token)
    return "{";
  if (t == RBRACE_token)
    return "}";
  if (t == INT_token)
    return "INT";
  if (t == ID_token)
    return "ID";
  if (t == PATH_token)
    return "PATH";
  if (t == REDUCE_token)
    return "REDUCE";
  if (t == TERM_token)
    return "TERM";
  if (t == NONTERM_token)
    return "NONTERM";
  if (t == START_token)
    return "START";
  if (t == DONE_token)
    return "%%";
  if (t == INCLUDE_token)
    return "INCLUDE";
  if (t == TERM_ALIGN_token)
    return "term_align";
  if (t == TERM_INCR_token)
    return "term_incr";
  else {
    fprintf(stderr, "\n"
      "internal plug error: can not map token number 0x%08x\n", t);
    exit(1);
  }
  return 0;
}


void check(int set) {
  if (!(token & set)) {
    if (file_name)
      fprintf(stderr, "%s, ", file_name);
    fprintf(stderr, "line %d, unexpected token <%s>\n",
            line_number, tok2str(token));
    exit(1);
  }
}


void expect(int expected) {
  if (token != expected) {
    if (file_name)
      fprintf(stderr, "%s, ", file_name);
    fprintf(stderr, "line %d, expected <%s>, got <%s>\n",
            line_number, tok2str(expected), tok2str(token));
    exit(1);
  }
}



typedef struct pattern {
  Hash_Node *id;
  struct pattern *kid[9];
  int llink[10];
  int rlink[9];
  int a[10];
  int kids;
  int permute;	// 0 ==> no permute; 1 ==> permute; 2 ==> swap
  int leaf;
} Pattern;

typedef struct locus {
  const char *file_name;
  int line_number;
} Locus;

/*
 * a reduction is a named piece of C code containing a semantic action
 * we keep track of the source locus so we can produce #line directives
 * so that error messages from compiling the semantic action will
 * be meaningful with respect to the grammar files
 */
typedef struct reduction {
  struct reduction *next;
  FILE *file;
  const char *code;	/* the C++ code with the semantic action */
  Locus locus;
} Reduction;

/*
 * a Spec corresponds to a production(rule) in the tree grammar
 * we also have a file name and line number pair so that
 * at tree match time (reduction time) we can cross reference to the grammar
 */
typedef struct spec {
  struct spec *next;
  const char *id;
  int cost;
  Pattern *pattern;
  int case_number;	/* aka iburg/burm rule number */
  Locus locus;		/* file locus information */
} Spec;


int global_leaf;

Pattern *parse_pattern(int set) {
  Pattern *p = (Pattern *) calloc(1, sizeof(Pattern));
  expect(ID_token);
  p->id = id;
  if (id->kind == NONTERM_token) {
    get_token();
    p->leaf = global_leaf++;
  }
  else if (id->kind == TERM_token) {
    Hash_Node *termid = id;
    int kids = 0;
    get_token();
    check(LPAREN_token|set);
    if (token == LPAREN_token) {
      get_token();
      p->kid[kids] = parse_pattern(COMMA_token|RPAREN_token|BAR_token|AT_token);
      kids++;
      check(COMMA_token|RPAREN_token|BAR_token|AT_token);
      if (token == COMMA_token) {
        p->permute = 0;
	get_token();
	p->kid[kids++] = parse_pattern(COMMA_token|RPAREN_token);
      }
      else if (token == BAR_token) {
        p->permute = 1;
        do {
          get_token();
          p->kid[kids++] = parse_pattern(BAR_token|RPAREN_token);
          check(BAR_token|RPAREN_token);
        } while (token == BAR_token);
      }
      else if (token == AT_token) {
        p->permute = 2;
	get_token();
	p->kid[kids++] = parse_pattern(AT_token|RPAREN_token);
      }
      expect(RPAREN_token);
      get_token();
    }
    p->kids = kids;
    if (p->permute == 1)
      kids = 2;
    if (termid->kids == -1)
      termid->kids = kids;
    else if (termid->kids != kids) {
      if (file_name)
        fprintf(stderr, "%s, ", file_name);
      fprintf(stderr,
              "line %d, terminal <%s> used inconsistently\n",
              line_number, termid->id);
      exit(1);
    }
  }
  else {
    if (file_name)
      fprintf(stderr, "%s, ", file_name);
    fprintf(stderr,
            "line %d, <%s> should have been declared as either a terminal or a non-terminal\n",
            line_number, id->id);
    exit(1);
  }
  return p;
}


int parse_cost(int set) {
  int cost = 0;
  check(set|LBRACK_token);
  if (token == LBRACK_token) {
    get_token();
    expect(INT_token);
    cost =number << 11;
    get_token();
    expect(COMMA_token);
    get_token();
    expect(INT_token);
    cost += number;
    get_token();
    expect(RBRACK_token);
    get_token();
  }
  return cost;
}


Spec *parse_spec(void) {
  Spec *s = (Spec *) malloc(sizeof(Spec));
  s->next = NULL;
  if (id->kind != NONTERM_token) {
    if (file_name)
      fprintf(stderr, "%s, ", file_name);
    fprintf(stderr,
            "line %d, <%s> should have been declared as a non-terminal\n",
            line_number, id->id);
    exit(1);
  }
  s->id = id->id;
  get_token();
  expect(COLON_token);
  get_token();
  global_leaf = 0;
  s->pattern = parse_pattern(LBRACK_token|ID_token|SEMI_token|COMMA_token);
  if (global_leaf > 9) {
    if (file_name)
      fprintf(stderr, "%s, ", file_name);
    fprintf(stderr, "line %d, too many leaves in pattern\n",
            line_number);
    exit(1);
  }
  s->locus.line_number = line_number;
  s->locus.file_name = strdup(base_name ? base_name : "<stdin>");
  s->cost = parse_cost(ID_token|SEMI_token|COMMA_token);
  return s;

}


Spec *parse_specs(void) {
  Spec *specs = parse_spec();
  check(COMMA_token|SEMI_token|ID_token);
  while (token == COMMA_token) {
    Spec *nspec;
    get_token();
    nspec = parse_spec();
    nspec->next = specs;
    specs = nspec;
    check(COMMA_token|SEMI_token|ID_token);
  }
  return specs;
}


int current;
int limit;
char *buffer;

void saveC(char c) {
  if (current == limit) {
    limit *= 2;
    buffer = (char *) realloc(buffer, limit*sizeof(char));
  }
  buffer[current++] = c;
}


void save_code(void) {
  int c = fgetc(f);
  while (c != '}') {
    switch (c) {
      case EOF:
        if (file_name)
          fprintf(stderr, "%s, ", file_name);
        fprintf(stderr, "line %d, unexpected EOF\n",
                line_number);
        exit(1);
      case '{':
        saveC(c);
        save_code();
        saveC('}');
        break;
      case '"':
        saveC(c);
        c = fgetc(f);
        while (c != '"') {
          if (c == '\\') {
            saveC(c);
            c = fgetc(f);
          }
          else if (c == '\n') {
            if (file_name)
              fprintf(stderr, "%s, ", file_name);
            fprintf(stderr, "line %d, bad string constant\n", line_number);
            exit(1);
          }
          saveC(c);
          c = fgetc(f);
        }
        saveC(c);
        break;
      case '\'':
        saveC(c);
        c = fgetc(f);
        if (c == '\\') {
          saveC(c);
          c = fgetc(f);
        }
        saveC(c);
        c = fgetc(f);
        if (c == '\'')
          saveC(c);
        else {
          if (file_name)
            fprintf(stderr, "%s, ", file_name);
          fprintf(stderr, "line %d, bad character constant\n", line_number);
          exit(1);
        }
        break;
      case '/':
        c = fgetc(f);
        if (c == '/') {
          char line[1000];
          const char *p = line;
          fgets(line, 1000, f);
          saveC('/');
          saveC('/');
          while (*p)
            saveC(*p++);
        }
        else {
          saveC('/');
          ungetc(c, f);
        }
        break;
      case '\n':
        line_number++;
      default:
        saveC(c);
        break;
    }
    c = fgetc(f);
  }
}


Reduction *parse_reduction(void) {
  Reduction *r = (Reduction *) malloc(sizeof(Reduction));
  r->next = NULL;
  /*
   * add one to current line_number because the directive #line
   * is put on next line in generated file
   */
  r->locus.line_number = line_number + 1;
  r->locus.file_name = strdup(base_name ? base_name : "<stdin>");

  expect(ID_token);
  if (id->kind != REDUCE_token) {
    if (file_name)
      fprintf(stderr, "%s, ", file_name);
    fprintf(stderr,
            "line %d, <%s> wasn't declared as a reduction\n",
            line_number, id->id);
    exit(1);
  }
  r->file = id->path->file;
  get_token();
  expect(LBRACE_token);
  current = 0;
  limit = 10;
  buffer = (char *) malloc(limit*sizeof(char));
  save_code();
  saveC(0);
  r->code = buffer;
  get_token();
  return r;
}

Reduction *parse_reductions(void) {
  Reduction *r = NULL;
  check(ID_token|SEMI_token);
  if (token == ID_token) {
    r = parse_reduction();
    check(COMMA_token|SEMI_token);
    while (token == COMMA_token) {
      Reduction *rr;
      get_token();
      rr = parse_reduction();
      rr->next = r;
      r = rr;
      check(COMMA_token|SEMI_token);
    }
  }
  expect(SEMI_token);
  get_token();
  return r;
}

void free_pattern(Pattern *p) {
  int i;
  for (i = 0; i < p->kids; i++) {
    free_pattern(p->kid[i]);
  }
  free(p); p = 0;
}


void free_spec(Spec *spec) {
  free_pattern(spec->pattern);
  free(spec); spec = 0;
}

void free_reduction(Reduction *reduction) {
  free((char *)reduction->code); reduction->code = 0;
  free(reduction); reduction = 0;
}


int perm_vector[9];

void dump_reduction(Reduction *r) {
  FILE *f = r->file;
  const char *p = r->code;
  fprintf(f, "case %d: {\n", case_number);
  fprintf(f, "#line %d \"%s\"",
    r->locus.line_number,
    r->locus.file_name	/* this is really the base_name, not the full path */
  );
  while (*p) {
    char c = *p++;
    switch (c) {
      case '"':
        fputc(c, f);
        c = *p++;
        while (c != '"') {
          if (c == '\\') {
            fputc(c, f);
            c = *p++;
          }
          fputc(c, f);
          c = *p++;
        }
        fputc(c, f);
        break;
      case '\'':
        fputc(c, f);
        c = *p++;
        if (c == '\\') {
          fputc(c, f);
          c = *p++;
        }
        fputc(c, f);
        c = *p++;
        fputc(c, f);
        break;
      case '/':
        c = *p++;
        if (c == '/') {
          fputc('/', f);
          fputc('/', f);
          while (*p != '\n')
            fputc(*p++, f);
        }
        else {
          fputc('/', f);
          p--;
        }
        break;
      case '$':
        c = *p++;
        if (c == '$')
          fputc('p', f);
        else
          fprintf(f, "kid[%d]", perm_vector[c - '1']);
        break;
      default:
        fputc(c, f);
        break;
    }
  }
  fprintf(f, "}\n  break;\n");
}


void dump_reductions(Reduction *r) {
  while (r) {
    dump_reduction(r);
    r = r->next;
  }
}




/* see Knuth, Volume 4, Section 7.2.1.2, Algorithm L */

int next_perm(int n, int *a) {
  int j, k, l, t;

  j = n - 1;
  while (a[j] >= a[j + 1])
    j--;
  if (j == 0) return 0;

  l = n;
  while (a[j] >= a[l])
    l--;
  t = a[j]; a[j] = a[l]; a[l] = t;

  k = j + 1;
  l = n;
  while (k < l) {
    t = a[k]; a[k] = a[l]; a[l] = t;
    k++;
    l--;
  }
  return 1;
}



/* see Knuth, Volume 4, Section 7.2.1.6, Algorithm B */

int next_tree(int *l, int *r, int ops) {
  int y, k;
  int j = 1;
  while (l[j] == 0) {
    r[j] = 0;
    l[j] = j + 1;
    j++;
  }
  if (j > ops) return 0;

  y = l[j];
  k = 0;
  while (r[y] > 0) {
    k = y;
    y = r[y];
  }

  if (k > 0)
    r[k] = 0;
  else
    l[j] = 0;
  r[y] = r[j];
  r[j] = y;
  return 1;
}


void init_pattern(Pattern *p) {
  int i;
  int kids = p->kids;
  if (p->permute == 0) {
    for (i = 0; i < kids; i++)
      init_pattern(p->kid[i]);
  }
  else if (p->permute == 1) {
    int ops = kids - 1;
    for (i = 1; i < ops; i++) {
      p->llink[i] = i + 1;
      p->rlink[i] = 0;
    }
    p->llink[ops] = 0;
    p->rlink[ops] = 0;
    p->llink[kids] = 1;
    for (i = 0; i <= p->kids; i++)
      p->a[i] = i - 1;
    for (i = 0; i < kids; i++)
      init_pattern(p->kid[i]);
  }
  else if (p->permute == 2) {
    init_pattern(p->kid[1]);
    init_pattern(p->kid[0]);
  }
  else {
    fprintf(stderr, "\n" "poorly structured pattern (init_pattern)\n");
    exit(1);
  }
}



int next_pattern(Pattern *p) {
  int kids = p->kids;
  if (kids == 0)
    return 0;
  else if (kids == 1)
    return next_pattern(p->kid[0]);
  else if (kids == 2 && p->permute == 0) {
    if (next_pattern(p->kid[0]))
      return 1;
    if (next_pattern(p->kid[1])) {
      init_pattern(p->kid[0]);
      return 1;
    }
    return 0;
  }
  else if (kids == 2 && p->permute == 2) {
    if (next_pattern(p->kid[1]))
      return 1;
    if (next_pattern(p->kid[0])) {
      init_pattern(p->kid[1]);
      return 1;
    }
    return 0;
  }
  else if (kids >= 2 && p->permute == 1) {
    int i, j;
    for (i = 0; i < kids; i++) {
      if (next_pattern(p->kid[i]))
        break;
    }
    if (i < kids) {
      for (j = 0; j < i; j++)
        init_pattern(p->kid[j]);
      return 1;
    }
    if (next_perm(kids, p->a)) {
      for (j = 0; j < kids; j++)
        init_pattern(p->kid[j]);
      return 1;
    }
    if (next_tree(p->llink, p->rlink, kids - 1)) {
      for (j = 0; j < kids; j++)
        init_pattern(p->kid[j]);
      for (i = 0; i <= p->kids; i++)
        p->a[i] = i - 1;
      return 1;
    }
    return 0;
  }
  else {
    fprintf(stderr, "\n" "poorly structured pattern (next_pattern)\n");
    exit(1);
  }
}


void dump_pattern(Pattern *p);

int dump_tree(Pattern *p, int root, int leaf) {
  if (root == 0) {
    dump_pattern(p->kid[p->a[leaf]]);
    return leaf + 1;
  }
  else {
    fprintf(bf, "%s(", p->id->id);
    leaf = dump_tree(p, p->llink[root], leaf);
    fputc(',', bf);
    leaf = dump_tree(p, p->rlink[root], leaf);
    fputc(')', bf);
    return leaf;
  }
}


int global_position;

void dump_pattern(Pattern *p) {
  int kids = p->kids;
  if (kids == 0) {
    if (p->id->kind == NONTERM_token)
      perm_vector[p->leaf] = global_position++;
    fputs(p->id->id, bf);
  }
  else if (kids == 1) {
    fprintf(bf, "%s(", p->id->id);
    dump_pattern(p->kid[0]);
    fputc(')', bf);
  }
  else if (kids == 2 && p->permute == 0) {
    fprintf(bf, "%s(", p->id->id);
    dump_pattern(p->kid[0]);
    fputc(',', bf);
    dump_pattern(p->kid[1]);
    fputc(')', bf);
  }
  else if (kids == 2 && p->permute == 2) {
    fprintf(bf, "%s(", p->id->id);
    dump_pattern(p->kid[1]);
    fputc(',', bf);
    dump_pattern(p->kid[0]);
    fputc(')', bf);
  }
  else if (kids >= 2 && p->permute == 1)
    (void) dump_tree(p, 1, 1);
  else {
    fprintf(stderr, "\n" "poorly structured pattern (dump_pattern)\n");
    exit(1);
  }
}

void dump_pattern_info(const Spec *s) {
  fprintf(rulefile, "{\"%s\", %4d}, \t/* %4d */\n",
    s->locus.file_name, s->locus.line_number,
    case_number);
}

void emit_rule(Spec *s, Reduction *r) {
  case_number++;
  s->case_number = case_number;
  fprintf(bf, "%s : ", s->id);
  global_position = 0;
  dump_pattern(s->pattern);
  dump_pattern_info(s);
  fprintf(bf, " = %d (%d);\n", case_number, s->cost);
  dump_reductions(r);
}

void parse_rule(void) {
  Spec *specs = parse_specs();
  Reduction *reductions = parse_reductions();
  while (specs) {
    Spec *spec = specs;
    specs = specs->next;
    init_pattern(spec->pattern);
    do {
      emit_rule(spec, reductions);
    } while (next_pattern(spec->pattern));
    free_spec(spec);
  }
  while (reductions) {
    Reduction *reduction = reductions;
    reductions = reductions->next;
    free_reduction(reduction);
  }
}

void parse_rules(void);

void parse_include(void) {
  FILE *saved_file;
  int saved_line_number;
  Hash_Node *path_id;
  const char *saved_file_name;
  const char *saved_dir_name;
  const char *saved_base_name;

  get_token();
  expect(PATH_token);
  path_id = id;
  get_token();
  expect(SEMI_token);
  saved_file = f;
  saved_dir_name = dir_name;
  saved_file_name = file_name;
  saved_base_name = base_name;
  f = open_and_update_names(path_id->id);
  if (f == 0) {
    if (saved_file_name)
      fprintf(stderr, "%s, ", saved_file_name);
    fprintf(stderr, "line %d, couldn't open include file <%s>\n",
            line_number, path_id->id);
    exit(1);
  }
  saved_line_number= line_number;
  line_number = 0;
  start_new_file = 1;
  get_token();
  parse_rules();
  expect(DONE_token);
  fclose(f); f = 0;
  free((char *)file_name); file_name = 0;
  free((char *)dir_name); dir_name = 0;
  f = saved_file;
  line_number = saved_line_number;
  base_name = saved_base_name;
  file_name = saved_file_name;
  dir_name = saved_dir_name;
  get_token();
}


void parse_rules(void) {
  check(ID_token|INCLUDE_token|DONE_token);
  while (token == ID_token || token == INCLUDE_token) {
    if (token == ID_token)
      parse_rule();
    else if (token == INCLUDE_token)
      parse_include();
    else {
      fprintf(stderr, "plug: inconsistent parser for parse_rules\n");
      exit(1);
    }
    check(ID_token|INCLUDE_token|DONE_token);
  }
}


typedef struct int_node {
  struct int_node *next;
  int val;
} Int_Node;


Int_Node *terminals[HASH_SIZE] = { 0 };


/*
 * Register the terminal number n in the terminals table.
 * Return 1 if the terminal number n exists in the terminals table.
 * Return 0 if this is a new entry.
 */
int register_terminal(int n) {
  unsigned nn = (unsigned) n;
  unsigned h = nn % HASH_SIZE;
  Int_Node *p = terminals[h];
  while (p) {
    if (p->val == n)
      return 1;
    p = p->next;
  }
  p = (Int_Node *) malloc(sizeof(Int_Node));
  p->val = n;
  p->next = terminals[h];
  terminals[h] = p;
  return 0;
}


int next_term_id = 0;
void parse_term_equate(void) {
  int this_term_id = -1;
  if (token == TERM_INCR_token) {
    get_token();
    expect(LPAREN_token);
    get_token();
    expect(INT_token);
    next_term_id += number;
    get_token();
    expect(RPAREN_token);
    get_token();
  } else if (token == TERM_ALIGN_token) {
    get_token();
    expect(LPAREN_token);
    get_token();
    expect(INT_token);
    next_term_id = ((next_term_id+(number-1))/number)*number;
    get_token();
    expect(RPAREN_token);
    get_token();
  } else {
    expect(ID_token);
    if (id->kind) {
      if (file_name) {
	fprintf(stderr, "%s, ", file_name);
      }
      fprintf(stderr, "line %d, %s already declared\n", line_number, id->id);
      exit(1);
    }
    id->kind = TERM_token;
    id->kids = -1;
    fprintf(bf, " %s=", id->id);
    if (first_opcode) {
      fprintf(opfile, "\n  %s=", id->id);
      first_opcode = 0;
    } else {
      fprintf(opfile, ",\n  %s=", id->id);
    }
    get_token();
    if (token == EQUAL_token) {
      expect(EQUAL_token);
      get_token();
      expect(INT_token);
      this_term_id = number;
      get_token();
    } else {
      this_term_id = next_term_id++;
    }
    if (register_terminal(this_term_id)) {
      if (file_name) {
	fprintf(stderr, "%s, ", file_name);
      }
      fprintf(stderr, "line %d, terminal %d already appears\n",
	line_number, this_term_id);
      exit(1);
    }
    fprintf(bf, "%d", this_term_id);
    fprintf(opfile, "%d", this_term_id);
    fprintf(opnfile, "   { %d, \"%s\" },\n", this_term_id, id->id );
  }
}


void parse_reduce_equate(void) {
  Hash_Node *reduce_id;
  expect(ID_token);
  if (id->kind) {
    if (file_name)
      fprintf(stderr, "%s, ", file_name);
    fprintf(stderr, "line %d, %s already declared\n", line_number, id->id);
    exit(1);
  }
  id->kind = REDUCE_token;
  reduce_id = id;
  get_token();
  expect(EQUAL_token);
  get_token();
  expect(PATH_token);
  if (id->file) {
    if (file_name)
      fprintf(stderr, "%s, ", file_name);
    fprintf(stderr, "line %d, file <%s>already opened\n", line_number, id->id);
    exit(1);
  }
  id->file = fopen(id->id, "w");
  if (id->file == 0) {
    if (file_name)
      fprintf(stderr, "%s, ", file_name);
    fprintf(stderr, "line %d, couldn't open file <%s>\n", line_number, id->id);
    exit(1);
  }
  label_file(id->file);
  reduce_id->path = id;
  get_token();
}


void parse_decls(void) {
  opnfile = fopen("icg-opcode-names.c", "w");
  if (opnfile == 0) {
    perror("icg-opcode-names.c");
    exit(1);
  }
  label_file(opnfile);

  opfile = fopen("icg-opcode.h", "w");
  if (opfile == 0) {
    perror("icg-opcode.h");
    exit(1);
  }
  fprintf(opfile, "#ifndef _icg_opcode_h\n");
  fprintf(opfile, "#define _icg_opcode_h\n");
  label_file(opfile);

  fprintf(opnfile, "#include \"icg-opcode.h\"\n"
                   "#include <stdio.h>\n"
                   "static const struct opnames_t {\n"
                   "   const icg_opcodes  op;\n"
                   "   const char        *str;\n"
                   "} opnames [] = {\n");

  fprintf(opfile, "typedef enum {");

  line_number = 0;
  start_new_file = 1;
  first_opcode = 1;
  get_token();
  check(START_token|NONTERM_token|TERM_token|REDUCE_token|ID_token|DONE_token|INCLUDE_token);
  while (token & (START_token|NONTERM_token|TERM_token|REDUCE_token)) {
    if (token == START_token) {
      if (start_node != 0) {
        if (file_name)
          fprintf(stderr, "%s, ", file_name);
        fprintf(stderr, "line %d, goal nonterminal already declared\n", line_number);
        exit(1);
      }
      get_token();
      expect(ID_token);
      start_node = id;
      get_token();
    }
    else if (token == NONTERM_token) {
      get_token();
      expect(ID_token);
      if (id->kind) {
        if (file_name)
          fprintf(stderr, "%s, ", file_name);
        fprintf(stderr, "line %d, %s already declared\n", line_number, id->id);
        exit(1);
      }
      id->kind = NONTERM_token;
      get_token();
      check(COMMA_token|SEMI_token);
      while (token == COMMA_token) {
        get_token();
        expect(ID_token);
        if (id->kind) {
          if (file_name)
            fprintf(stderr, "%s, ", file_name);
          fprintf(stderr, "line %d, %s already declared\n", line_number, id->id);
          exit(1);
        }
        id->kind = NONTERM_token;
        get_token();
        check(COMMA_token|SEMI_token);
      }
    }
    else if (token == TERM_token) {
      fprintf(bf, "%%term");
      get_token();
      parse_term_equate();
      check(COMMA_token|SEMI_token);
      while (token == COMMA_token) {
        get_token();
        parse_term_equate();
        check(COMMA_token|SEMI_token);
      }
      fprintf(bf, "\n");
    }
    else { /* token == REDUCE_token */
      get_token();
      parse_reduce_equate();
      check(COMMA_token|SEMI_token);
      while (token == COMMA_token) {
        get_token();
        parse_reduce_equate();
        check(COMMA_token|SEMI_token);
      }
    }
    expect(SEMI_token);
    get_token();
    check(START_token|NONTERM_token|TERM_token|REDUCE_token|ID_token|DONE_token|INCLUDE_token);
  }
  if (start_node == 0) {
    if (file_name)
      fprintf(stderr, "%s, ", file_name);
    fprintf(stderr, "line %d, missing declaration for goal nonterminal\n", line_number);
    exit(1);
  }
  else if (start_node->kind != NONTERM_token) {
    if (file_name)
      fprintf(stderr, "%s, ", file_name);
    fprintf(stderr,
            "line %d, goal symbol <%s> should be declared as nonterminal\n",
            line_number, start_node->id);
    exit(1);
  }
  fprintf(bf, "\n" "%%start %s\n\n%%%%\n\n", start_node->id);

  fprintf(opfile, "\n" "} icg_opcodes;\n\n");
  fprintf(opfile, "const char *get_icg_opcode_name( icg_opcodes op );\n");
  fprintf(opfile, "#endif /*_icg_opcode_h*/\n");

  fclose(opfile); opfile = 0;

  fprintf(opnfile, "   { 0, 0 } };\n\n");
  fprintf(opnfile,
          "const char *get_icg_opcode_name( icg_opcodes op ) {\n"
          "   static char buffer[128];\n"
          "   int i = 0;\n"
          "   for (;;) {\n"
          "      if (opnames[i].op == 0) {\n"
	  "        snprintf(buffer, sizeof(buffer), \"error: opcode %%d not found\", op);\n"
	  "        return buffer;\n"
	  "      }\n"
          "      if (opnames[i].op == op) {\n"
	  "        return opnames[i].str;\n"
	  "      }\n"
          "      ++i;\n"
          "   }\n"
	  "}\n"
	  );

  fclose(opnfile); opnfile = 0;
}




#define BIG_LINE 1000

void prologue(void) {
  char line[BIG_LINE];
  fprintf(bf, "%%{\n");
  line_number = 1;
  while (fgets(line, BIG_LINE, f)) {
    line_number++;
    if (line[0] == '%' && line[1] == '%')
      break;
    fputs(line, bf);
  }
  fprintf(bf, "%%}\n\n");
}


void epilogue(void) {
  char line[BIG_LINE];
  fputs("\n" "%%\n", bf);
  while (fgets(line, BIG_LINE, f))
    fputs(line, bf);
}


int main(int argc, char *argv[]) {
  int arg = 1;
  bf = stdout;
  f = stdin;
  dir_name = (char *) get_current_dir_name();
  file_name = 0;
  while (arg < argc) {
    if (argv[arg][0] == '-') {
      if (strcmp(argv[arg], "-o") == 0) {
        if (bf == stdout) {
          arg++;
          if (arg < argc) {
            bf = fopen(argv[arg], "w");
            if (!bf) {
	      perror(argv[arg]);
              fprintf(stderr, "couldn't open %s\n", argv[arg]);
              exit(1);
            }
	    label_file(bf);
          }
          else {
            fprintf(stderr, "expected a pathname after -o\n");
            exit(1);
          }
        }
        else {
          fprintf(stderr, "expected no more than one -o flag\n");
          exit(1);
        }
      }
      else {
        fprintf(stderr, "unexpected command-line option: %s\n", argv[arg]);
        exit(1);
      }
    }
    else if (f == stdin) {
      f = open_and_update_names(argv[arg]);
      if (!f) {
        fprintf(stderr, "couldn't open %s\n", argv[arg]);
        exit(1);
      }
    }
    else {
      fprintf(stderr, "expected no more than one filename as input\n");
      exit(1);
    }
    arg++;
  }

  infofile = fopen("icg-ruleinfo.h", "w");
  if (infofile == 0) {
    perror("icg-ruleinfo.h");
    exit(1);
  }
  label_file(infofile);
  fprintf(infofile, "#if !defined(_icg_ruleinfo_h)\n");
  fprintf(infofile, "#define _icg_ruleinfo_h\n");

  rulefile = fopen("icg-rulemaps.c", "w");
  if (rulefile == 0) {
    perror("icg-rulemaps.h");
    exit(1);
  }
  label_file(rulefile);
  fprintf(rulefile, "#include \"icg-ruleinfo.h\"\n");
  fprintf(rulefile, "typedef struct {\n");
  fprintf(rulefile, "  const char *filename;\n");
  fprintf(rulefile, "  int line_number;\n");
  fprintf(rulefile, "} icg_locus;\n");
  fprintf(rulefile, "icg_locus const icg_rule_to_locus[PLUG_NRULES] = {\n");
  fprintf(rulefile, "{\"?\",0},\n");	/* rule 0 is not used */

  init_keywords();
  prologue();
  parse_decls();
  parse_rules();
  epilogue();

  fprintf(rulefile, "};\n");
  fclose(rulefile); rulefile = 0;

  fprintf(infofile, "\n");
  fprintf(infofile, "#define PLUG_NRULES %4d\n", case_number+1);
  fprintf(infofile, "\n");
  fprintf(infofile, "extern const char *const icg_burm_opname[];\n");
  fprintf(infofile, "extern char const icg_burm_arity[];\n");
  fprintf(infofile, "extern const char *const icg_burm_ntname[];\n");
  fprintf(infofile, "extern const short *const icg_burm_nts[PLUG_NRULES];\n");
  fprintf(infofile, "extern float const icg_burm_cost[PLUG_NRULES][4];\n");
  fprintf(infofile, "extern const char *const icg_burm_string[PLUG_NRULES];\n");
  fprintf(infofile, "\n");
  fprintf(infofile, "#endif\n");
  fclose(infofile); infofile = 0;

  return 0;	/* assume success */
}
