/* This file describes the data structure and interface for the data
   reorganization optimization. Currently there is the API for stage 2
   of the optimization - the profile based clustering decisions.  */

/* Represents access site of a data field.  */
struct access_site {
  /* The statement in which the access site occurs.  */
  tree stmt;
  /* The tree expression used to access the field.  */
  tree field_access;
  /* True if the access is through an array or could be replaced
     to an array access.  */
  bool direct_access;
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

/* Represents a field within a data structure and its accesses.  */
struct data_field_entry {
  /* Field index; unique within the data structure, better start with
     zero each time, because we are going to use it as an index for the
     array of fields of the data structure.  */
  int index;
  /* The number of times the field is accessed (according to profiling).  */
  gcov_type count;
  tree decl;
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
  int f_indx;
  int distance_to_next;
  struct bb_field_access *next;
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

/* Represents a data structure that is candidate for
   clustering/reordering.  */
struct data_structure {
  tree decl;
  /* Number of fields in the structure.  */
  int num_fields;
  /* According to profiling information number of times a field of the
     structure was accessed.  */
  gcov_type count;
  /* Array of field entries one for each field of the structure,
     indexed by the field ID.  */
  struct data_field_entry *fields;

  struct allocation_site *alloc_sites;

  /* This is the a tree data structure representing the hierarchy of
     of the clustering of this data structure.  */
  struct field_cluster *struct_clustering;
  /* An array of structure that holds for each basic block
     the list of accessed fields, the array is index by the bb index.  */
  struct bb_field_access **bbs_f_acc_lists;
};

/*extern struct bb_field_access * get_last_field_access (struct data_structure *ds, 
						       basic_block bb);*/
struct bb_field_access *
get_last_field_access (struct data_structure *ds, basic_block bb);
/* Stage 2 (profile based clustering) API:
   Given a DATA_STRUCTURE with the following data initialized properly:
   DECL, NUM_FIELDS, FIELDS, ALLOC_SITES. The FIELDS array contains entry
   for each one of the fields of the structure, a field could be an
   atomic type or a complex type. In any case it must contain all the
   fields of the data structure. When stage 1 has a complex field it can
   choose to refer to it as a complete one field or a separated fields
   and allocate entry for each one of its fields. The later case means
   that stage 2 can separate fields of that sub-structure from each other.
   An example when this is not possible is when the complex field address
   is taken - In such a case stage 1 must add one entry for the complex
   field.
   This function performs the algorithm for profile based cache aware
   data reorganization and represents its results in STRUCT_CLUSTERING
   field of the given DATA_STRUCTURE STR.  This function should
   be called for each one of the structure that stage 1 has found relevant
   for clustering or field reordering. If only field reordering is
   applicable then the parameter REORDERING_ONLY should be true, other
   wise we assume that any clustering of the structure is legal.
   If the parameter REORDERING_ONLY is true no clustering will be
   performed and only a cache aware field reordering will be made.
   return false in unexpected failure, true otherwise.

   EXAMPLE:
        struct s1 {
          struct s11
          { int f11, f12, f13} f1;
          int f2, f3,f4;
          struct s2 *f5;
        };
        struct s2 {
          int f1, f2, f3, f2;
        }

   Stage 1 can construct one of two possible arrays for FIELDS:
   {f1,f2,f3,f4,f5} or {f11,f12,f13,f2,f3,f4,f5}
   In the first case fields f11, f12, f13 will always be adjacent.
   In the later case the preferred clustering may separate them.  */
bool
cache_aware_data_reorganization (struct data_structure *str, bool reordering_only);
