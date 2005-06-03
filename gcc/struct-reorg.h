/* This file describes the data structure and interface for the data
   reorganization optimization. Currently there is the API for stage 2
   of the optimization - the profile based clustering decisions.  */

/* Represents a linked list of trees, usually of RECORD_TYPE.  These would
   be groups that we want to keep together, but for which it would be 
   inappropriate to use the normal TREE_CHAIN method of connecting them.  */

struct struct_tree_list {
  tree data;
  struct struct_tree_list *next;
};


/* Represents access site of a data field.  */
struct access_site {
  /* The statement in which the access site occurs.  */
  tree stmt;
  /* The tree expression used to access the field.  */
  tree field_access;
  /* True if the access is through an array or could be replaced
     to an array access.  */
  bool direct_access;
  /* Basic block containing the access statement.  Necessary for
     inserting additional statments to reference sub-structure components.  */
  basic_block bb;
  /* Function containing the access statement.  Necessary for 
     insertion additional statements to reference sub-structure components.  */
  struct function *context;
  /* Next in the list of access sites.  */
  struct access_site *next;
};


/* Represents the allocation of an object of the data structure.  */
struct allocation_site {
  /* The statement of the allocation.  */
  tree stmt;
  /* Information about the allocation ??? another way of representation
     could be used here.  */
  tree allocation_expr;
  /* True if the allocation sites allocates an array of objects at once.  */
  bool array_alloc;
  /* Next allocation site for this structure.  */
  struct allocation_site *next;
};

/* Represents new mapping information for fields, once struct has been
   reorganized.  */
struct field_map {
  tree decl;                           /* Either a field decl or a type decl */
  struct field_map *containing_type;   /* Pointer to type containing "decl".  */
  struct field_map *contains;          /* Pointer to contained type/field decl. */
};

/* Represents a field within a data structure and its accesses.  */
struct data_field_entry {
  /* Field index; unique within the data structure, better start with
     zero each time, because we are going to use it as an index for the
     array of fields of the data structure.  */
  int index;
  /* The number of times the field is accessed (according to profiling).  */
  gcov_type count;
  tree decl;
  struct field_map *new_mapping;
  struct access_site *acc_sites;
};

/* This data structures is used for holding the list of accessed fields
   and distances in each basic block (intra-block).
   Two general cases are possible:
   1. At least one access to a field of our interest.
      In this case the linked list will contain at least two elements,
      the first one is a dummy element that holds only the distance from
      the beginning of the basic block to the first accessed field in the
      block (in this case the f_indx field should be -1 and the next must
      not equal zero).
   2. No access to a field of our interest.
      In this case there will be one dummy element in the linked list with
      distance that tells the amount of memory accessed in this basic
      block (in disntance_to_next filed).
  When we want to collect the inter-block information about the accesses
  to the fields we will usually add the distance in the artificial node
  in one block to the distance of the last element (of the linked list)
  of the predecessor block to form the distance from accesses in the
  predecessor blocks.  */
struct bb_field_access {
  int f_index;
  int bb_index;
  int distance_to_next;
  gcov_type count;
  struct bb_field_access *next, *prev;
};

/* This structure describes the data structure clustering and field
   reordering results. The desired field reordering in each cluster is
   described in the FIELD_ORDER array; this array represents an ordered
   list of field IDs (index zero is the first in the definition and so on),
   -1 indicates the end of the list.
   Clustering is described as a hierarchy; the root is the main structure
   though which all the other clusters are accessed.  When generating
   the structure definition a pointer will be added for each CHILD
   unless the child has the DIRECT_ACCESS set.  */
struct field_cluster {
  /* An array of field indexes, the order of the fields in the array
     is the result of field reordering - this should be the definition
     order.  */
  int *fields_order;
  /* Bitmap of field indexes, a bit is set when the field with that index
     is in the cluster.  */
  sbitmap fields_in_cluster;
  /* True means that this cluster is accessed directly with no need for
     indirection from the parent.  This is true for the root and the
     pure array accessed fields.  */
  bool direct_access;
  /* The cluster that points to this cluster; access to the cluster
     are made through the cluster father.  */
  struct field_cluster *parent;
  /* Next cluster in the same hierarchy level pointed to by the same
     parent.  */
  struct field_cluster *sibling;
  /* The first cluster in the next hierarchy level of the same parent.  */
  struct field_cluster *children;
};

struct cpg;
/* Represents a data structure that is candidate for
   clustering/reordering.  */
struct data_structure {
  tree decl;
  /* This is set to indicate that we cannot determind some the field
     accesses of this structure, meaning that we cannot perform
     data reorganization on them.  */
  bool unresolved_field_access;
  /* True if an external function declaration has this struct as 
     type of one of its arguments.  In whole program analysis this 
     this true only for type of arguments in library function calls.  */
  bool in_extern_definition;
  /* Number of fields in the structure.  */
  int num_fields;
  /* According to profiling information number of times a field of the
     structure was accessed.  */
  gcov_type count;
  /* Array of field entries one for each field of the structure,
     indexed by the field ID.  */
  struct data_field_entry *fields;

  struct allocation_site *alloc_sites;

  /* The CPG for this data structure.  */
  struct cpg *cpg;
  /* This is the a tree data structure representing the hierarchy of
     of the clustering of this data structure.  */
  struct field_cluster *struct_clustering;
  /* An array of structure that holds for each basic block
     the list of accessed fields, the array is index by the bb index.  */
  struct bb_field_access **bbs_f_acc_lists;

  /* A linked list of the newly created type(s),  corresponding to the
     reorganization of the original structure.  */
  struct struct_tree_list *new_types;
  bool reorder_only;
};

/*
 * Field access profiling for structure reorganization
    The structure reorganization optimization is profile directed.
    Following is the part responsible for extracting the field access
    profiling information out of the basic block profiling that is
    currently available in GCC.  And the part responsible for representing
    that information in a graph called the Close Proximity Graph or CPG
    in short.
 * Close proximity graph
    The CPG is a relation graph for fields of a structure that we are
    interested in reorganizing its definition.  A node in the graph
    represents a struct field.  An edge between two nodes represents the
    close proximity relation between two fields.  The close proximity
    between two fields is the couple (count, average distance).  The count
    is the number of times one field access is followed immediately (wrt
    other fields of same struct) by an access to the other field
    (symmetrically).  The distance is the number of memory access
    instructions (not to other fields) that comes in between the two
    consecutive accesses.  Function calls should also be taken into
    account, we should distinguish between function calls that have many
    accesses to memory and others that have only small number of accesses
    to memory.  Inter procedural analysis could be used to make accurate
    decisions.  The distances are normalized by the following equation to
    produce the average:
        ____
        \
        /    distance(i) x count(i)
        ----
         n
        ----------------------------
           ____
           \
           /    count(i)
           ----
            n
*/

/* An edge in the CPG (a cell in the matrix).  */
typedef struct cpg_cell {
  gcov_type count;
  int distance;
} cpg_cell_t;

typedef struct cpg {
  /* Information about the data structure of our interest.  */
  struct data_structure *ds;
  /* A matrix of edges ds->NUM_FIELDS X ds->NUM_FIELDS nodes.  */
  struct cpg_cell **matrix;
} cpg_t;

void verify_data_structure (struct data_structure *ds);

/*extern struct bb_field_access * get_last_field_access (struct data_structure *ds,
						       basic_block bb);*/
struct bb_field_access *
get_last_field_access (struct data_structure *ds, basic_block bb);


void dump_cpg (FILE *, cpg_t *);
void free_cpg (cpg_t *cpg);
void update_cpg_for_structure (FILE *, struct data_structure *, struct function *);
 
/* Stage 2 (profile based clustering) API.  Detailed comment in
   struct-reorg-cpg.c  */
bool
cache_aware_data_reorganization (struct data_structure *str,
				 bool reordering_only);
