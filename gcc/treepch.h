#include "splay-tree.h"
#ifdef HAVE_MDBM_H
#define NDBM_COMPAT
#include "mdbm.h"
#elif HAVE_SDBM_H
#include "sdbm.h"
#elif HAVE_GDBM_H
#include "gdbm.h"
#endif
#include "varray.h"
extern int write_rtl PARAMS ((rtx *));
extern int write_tree PARAMS ((tree *));
extern int write_tree_varray PARAMS ((varray_type));
extern rtx read_rtl PARAMS ((rtx));
extern tree read_tree PARAMS ((tree));
extern varray_type read_tree_varray PARAMS ((varray_type));
extern void lang_pickle_tree PARAMS ((tree, tree));
extern void lang_unpickle_tree PARAMS ((tree));
extern const char * unpickle_string PARAMS ((int));
extern int pickle_string PARAMS ((const char *));
extern splay_tree read_trees;
extern splay_tree written_trees;
extern splay_tree written_pointers;
extern splay_tree written_rtl;
extern splay_tree read_rtls;
extern size_t current_id;
extern const char *datafilename;
extern DBM *datafile;
extern void store_to_db PARAMS ((void *, size_t, void *, size_t));
