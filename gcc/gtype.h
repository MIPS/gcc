/* Generic data handling for GNU CC.
   Copyright (C) 2000, 2001 Free Software Foundation, Inc.

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */


/* Various structures that might not be declared at this point.  */
struct rtx_def;
union tree_node;
struct varray_head_tag;

/* A description of the pointer fields in a type.  */
typedef struct type_definition_s const *type_definition_p;

/* These are all the types we know.
   (This list needs to be known at compile time so we can
   easily write initializers for FIELD_DEFINITIONS).
   All types we will want to put in a PCH need to be in this list, so
     we can index into various arrays of dimension num_type_defs.  */
enum type_index_e
{
  tree_type_ndx,
  rtx_type_ndx,
  rtvec_type_ndx,
  string_type_ndx,
  lang_type_type_ndx,
  lang_decl_type_ndx,
  function_type_ndx,
  lang_function_type_ndx,
  emit_status_type_ndx,
  dbx_typevec_type_ndx,
  dbx_file_type_ndx,
  hash_tree_type_ndx,
  varray_tree_type_ndx,
  varray_rtx_type_ndx,
  varray_rtvec_type_ndx,
  varray_string_type_ndx,
  typed_addresses_list_type_ndx,

  lang1_type_ndx,
  lang2_type_ndx,
  lang3_type_ndx,
  lang4_type_ndx,
  lang5_type_ndx,

  num_type_defs,
  no_type_ndx
};
#define type_index_to_type(ti) ((ti) + basic_type_defs)
#define type_to_type_index(td) ((enum type_index_e)((td) - basic_type_defs))
#define tree_type_def	type_index_to_type (tree_type_ndx)
#define rtx_type_def	type_index_to_type (rtx_type_ndx)
#define rtvec_type_def	type_index_to_type (rtvec_type_ndx)
#define string_type_def	type_index_to_type (string_type_ndx)
#define lang_type_type_def	type_index_to_type (lang_type_type_ndx)
#define lang_decl_type_def	type_index_to_type (lang_decl_type_ndx)
#define function_type_def	type_index_to_type (function_type_ndx)
#define lang_function_type_def	type_index_to_type (lang_function_type_ndx)
#define emit_status_type_def	type_index_to_type (emit_status_type_ndx)
#define dbx_typevec_type_def	type_index_to_type (dbx_typevec_type_ndx)
#define dbx_file_type_def	type_index_to_type (dbx_file_type_ndx)
#define hash_tree_type_def	type_index_to_type (hash_tree_type_ndx)
#define varray_tree_type_def	type_index_to_type (varray_tree_type_ndx)
#define varray_rtx_type_def	type_index_to_type (varray_rtx_type_ndx)
#define varray_rtvec_type_def	type_index_to_type (varray_rtvec_type_ndx)
#define varray_string_type_def	type_index_to_type (varray_string_type_ndx)
#define typed_addresses_list_type_def	\
  type_index_to_type (typed_addresses_list_type_ndx)
#define lang1_type_def		type_index_to_type (lang1_type_ndx)
#define lang2_type_def		type_index_to_type (lang2_type_ndx)
#define lang3_type_def		type_index_to_type (lang3_type_ndx)
#define lang4_type_def		type_index_to_type (lang4_type_ndx)
#define lang5_type_def		type_index_to_type (lang5_type_ndx)

/* This is not `const', because we allow TREE_TYPE_DEF to be modified
   by frontends.  */
struct type_definition_s;
extern struct type_definition_s basic_type_defs[num_type_defs];

typedef unsigned code_type;

/* Functions to fetch codes and sizes.  */
typedef code_type (*code_fetcher_fnp) PARAMS ((const void *));
typedef size_t (*size_fetcher_fnp) PARAMS ((const void *, type_definition_p));
typedef void (*prewrite_hook_fnp) PARAMS ((const void *));

/* A description of one pointer field in a type.  */
typedef struct field_definition_s
{
  /* This description applies if (CODE & MASK) == COND, where
     CODE is returned by the type's CODE_FETCHER.  */
  code_type cond, mask;
  /* The field is OFFSET bytes from the start of the object.  */
  size_t offset;
  /* The field points to an object of type TYPE.  */
  type_definition_p type;
} const *field_definition_p;

/* In a list of fields, a field with TYPE == NULL marks the end of the 
   list.  */
#define NO_MORE_FIELDS { 0, 0, 0, 0 }

/* This routine is called with values of N starting with 0.  
   If there is a Nth extra field in object V of type TD, it should
   set FP to describe that field, and return 1;
   otherwise, it returns 0.

   It may change *FP even if it does return 0.  The COND and MASK
   fields should not be changed ever.  If TYPE is set to NULL,
   this value of N is skipped.
*/
typedef int (*more_fields_fnp) PARAMS ((struct field_definition_s * fp,
	 const void *v, unsigned n, code_type c, type_definition_p td));

/* A description of a subobject.  A subobject is an object whose
   size depends on the object that points to it (the `parent object').  */

typedef struct subobject_definition_s
{
  /* This description applies if (CODE & MASK) == COND, where
  CODE is returned by the type's CODE_FETCHER.  */
  code_type cond, mask;

  /* A pointer to the subobject is OFFSET bytes from the start of the
  parent object.  */
  size_t offset;

  /* SIZE_FETCHER is called with the parent object and returns the size
  of the subobject.  */
  size_fetcher_fnp size_fetcher;

  /* The fields in the subobject are returned by FIELDS.  */
  more_fields_fnp fields;
} const *subobject_definition_p;

/* In a list of subobjects, a subobject with SIZE_FETCHER == NULL marks
   the end of the list.  */
#define NO_MORE_SUBOBJECTS { 0, 0, 0, 0, 0 }

/* A description of the pointer fields in a type.  */
struct type_definition_s
{
  /* Returns the object's CODE, used to determine what fields and subobjects
  apply to this particular object.  */
  code_fetcher_fnp code_fetcher;

  /* Returns the object's size.  */
  size_fetcher_fnp size_fetcher;

  /* Does any pre-write tweaking of the object.  */
  prewrite_hook_fnp prewrite_hook;

  /* Minimum size of the type.  
  Generally, SIZE_FETCHER should be able to compute the size given
  only SIZE bytes of the object.  */
  size_t size;

  /* A list of fields in the type.  */
  field_definition_p field_definitions;

  /* A routine to return additional fields.  */
  more_fields_fnp more_fields;

  /* A list of subobjects in the type.  */
  subobject_definition_p subobject_definitions;

  /* >0 if objects of this type are garbage collected;
    <0 if not, but objects of this type contain gc'd objects;
    0 otherwise.
  There must never be a cycle from a ggc_p<0 object back to itself.  */

  int ggc_p;
};

/* This is how languages handle adding their favourite stuff
at the end of a tree... */
extern void add_tree_fields PARAMS ((const struct field_definition_s * fields));

/* Do some operation on V with context CTX.  *V has type TD.  */
typedef void (*pointerfn_fnp) PARAMS ((void **v, type_definition_p td, void *ctx));

/* Apply FN with context CTX to every pointer in V.  V has type TD.  */
extern void apply_pointer PARAMS ((void *v, type_definition_p td, pointerfn_fnp fn, void *ctx));

/* Do some operation on V with context CTX.  *V has type TD.
Return nonzero if this routine is to be called again on each
pointer contained in *V.  */
typedef int (*r_pointerfn_fnp) PARAMS ((void **v, type_definition_p td, void *ctx));

/* Do some operation on a subobject V of OBJECT with context CTX.
OBJECT has type TD, and V is the subobject described by SD.  Return
nonzero if this routine is to be called again on each pointer
contained in *V.  */
typedef int (*s_pointerfn_fnp) PARAMS ((void *object, type_definition_p td,
	 void **v, subobject_definition_p sd, void *ctx));

/* Apply FN with context CTX to every pointer reachable from V, and
apply SF to every subobject.  V has type TD.  SF may be NULL in which
case it is treated as returning 1 if and only if the pointer to the subobject
is not NULL.  */
extern void apply_pointer_recursively PARAMS ((void *v, type_definition_p td,
	 r_pointerfn_fnp fn, s_pointerfn_fnp sf, void *ctx));

/* A pointer and its type.  */
typedef struct typed_address_s
{
  void *p;
  type_definition_p td;
  const char *name;
} typed_address;

/* The address of an array of pointers and the type pointed to.  */
struct typed_addresses_s
{
  void **p;
  /* If TD is NULL, it means that P is really only a 'void *' and
  it points to a block of memory of size COUNT that doesn't contain
  any pointers.  */
  type_definition_p td;
  size_t count;
  const char *name;
};

/* A list of typed addreses.  */
enum { TYPED_ADDRESSES_LIST_CHUNK_SIZE = 22 };

typedef struct typed_addresses_list_s
{
  struct typed_addresses_s ta[TYPED_ADDRESSES_LIST_CHUNK_SIZE];
  struct typed_addresses_list_s *next;
} typed_addresses_list, *typed_addresses_list_p;


/* Add a block of typed addresses to a list.  */

extern void add_typed_addresses PARAMS ((typed_addresses_list_p list,
	 void **p, type_definition_p td, size_t count, const char *));

/* Add addresses of various types to a list.  */
extern void add_tree_addresses PARAMS ((typed_addresses_list_p list, union tree_node ** p, size_t count, const char *));
extern void add_rtx_addresses PARAMS ((typed_addresses_list_p list, struct rtx_def ** p, size_t count, const char *));
extern void add_varray_tree_addresses PARAMS ((typed_addresses_list_p list,
	 struct varray_head_tag ** p, size_t count, const char *));

/* Remove an address (typed or untyped) from a list.  */
extern void remove_list_address PARAMS ((typed_addresses_list_p list, void *p));

/* Do something to each pointer in a list.  */
extern void apply_list_pointers PARAMS ((typed_addresses_list_p list, pointerfn_fnp fn, void *ctx));

/* Do something to each pointer referenced by a list.  */
extern void apply_list_pointers_recursively PARAMS ((typed_addresses_list_p
	 list, r_pointerfn_fnp fn, s_pointerfn_fnp sf, void *ctx));

/* Do some operation on V with context CTX.  *V is of size SZ.  */
typedef void (*untypedfn_fnp) PARAMS ((void *v, size_t sz, void *ctx));

/* Add a block of untyped memory to a list.  */
extern void add_untyped_address PARAMS ((typed_addresses_list_p list, void *p, size_t size, const char *));

/* Do something to each untyped memory block in a list.  */
extern void apply_list_untyped PARAMS ((typed_addresses_list_p list, untypedfn_fnp fn, void *ctx));

/* Count items in a list.  */
extern void count_list_pointers PARAMS ((typed_addresses_list_p list, size_t * typed, size_t * untyped));

enum { CHECKSUM_SIZE = 8 };

/* Update a checksum of the addresses in LIST.  If LIST refers to the
same list of pointers, the checksum shouldn't vary even between
different runs of the program.  Initially, set CHKSUM to contain
all zeros.  */

extern void sum_type_addresses_list PARAMS ((unsigned char chksum[CHECKSUM_SIZE], typed_addresses_list_p list));

/* Write out a list.  
KNOWN is a list of pointers that need not be written.  */
extern int write_type_addresses_list PARAMS ((FILE * f, typed_addresses_list_p known, typed_addresses_list_p list));

/* Restore a list.  KNOWN and LIST must point to the same list of pointers
as was originally written.  */
extern int read_type_addresses_list PARAMS ((FILE * f,
	 typed_addresses_list_p known, typed_addresses_list_p list)); 

/* Various global variables used by the precompiled header machinery.
known_pointers are nodes that will always be initialized to the same value,
so we don't need to save them in the PCH, but we need to know about them
so that stuff we do save can refer to them.
data_to_save are just that.  */

extern typed_addresses_list data_to_save;
extern typed_addresses_list known_pointers;
