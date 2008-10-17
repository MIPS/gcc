/* Functions related to concepts and models.
   Copyright (C) 2005, 2006, 2007, 2008  Free Software Foundation, Inc.
   Written by Douglas Gregor <dgregor@osl.iu.edu>

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA. */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "cp-tree.h"
#include "flags.h"
#include "toplev.h"
#include "rtl.h"
#include "diagnostic.h"
#include "hashtab.h"
#include "pointer-set.h"
#include "timevar.h"

static void mark_deduced_parms (tree, tree, bool*);
static bool check_refinement (tree, tree);
static bool define_models_for_superiors (tree, bool);
static bool maybe_define_model_for_superior (tree, tree, bool);
static tree forward_parameter (tree);
static tree deduce_associated_type_from_signatures (tree, tree, 
						    struct pointer_set_t *);
static bool add_associated_type_defaults (tree, struct pointer_set_t *);
static tree instantiate_operation_type (tree, tree);
static tree build_operation_decl (tree, tree, tree, tree);
static bool declare_model_operations (tree, struct pointer_set_t *);
static tree build_implicit_copy_assignment_expr (tree,  tree, tree);
static tree build_implicit_method_call_expr (tree, tree, tree);
static tree build_implicit_model_operation_expr (tree, tree *);
static bool define_model_operations_for_superiors (tree, void*);

/* See typeck.c */
extern int archetypes_represent_themselves;

/* Builds a new REQUIREMENT node containing a concept requirement
  (e.g., LessThanComparable<T>) or a negated concept requirement
  (e.g., !LessThanComparable<T>).  CONCEPT_ID is a template-id for the
  requirement.  NOT_REQ is TRUE to indicate that this is a negated
  concept requirement.  

  This routine will ensure the correctness of CONCEPT_ID. It will also
  inspect the requirement itself to determine if the requirement
  should be turned into a same-type requirement or an integral
  constant expression requirement, and build the appropriate
  REQUIREMENT node.  */
tree 
build_concept_requirement (tree concept_id, bool not_req)
{
  /* DPG TBD: CACHE THESE! */
  tree std = namespace_binding (std_identifier, global_namespace);
  tree same_type_con
    = lookup_qualified_name (std, 
                             get_identifier("SameType"),
                             /*is_type_p=*/true,
                             /*complain=*/false);
  tree derived_from_con
    = lookup_qualified_name (std, 
                             get_identifier("DerivedFrom"),
                             /*is_type_p=*/true,
                             /*complain=*/false);
  tree true_con  
    = lookup_qualified_name (std, 
                             get_identifier("True"),
                             /*is_type_p=*/true,
                             /*complain=*/false);
  tree con;

  if (!concept_id || concept_id == error_mark_node)
    return error_mark_node;

  con = most_general_template (CLASSTYPE_TI_TEMPLATE (concept_id));

  if (con == same_type_con && !not_req)
    {
      /* We have a same-type constraint.  */
      tree args = CLASSTYPE_TI_ARGS (concept_id);
      return build_same_type_requirement (TREE_VEC_ELT (args, 0),
                                          TREE_VEC_ELT (args, 1));
    }
  if (con == derived_from_con && !not_req)
    {
      /* We have a same-type constraint.  */
      tree args = CLASSTYPE_TI_ARGS (concept_id);
      return build_derived_from_requirement (TREE_VEC_ELT (args, 0),
					     TREE_VEC_ELT (args, 1));
    }
  else if (con == true_con)
    {
      /* We have an integral constant expression constraint.  */
      tree expr = TREE_VEC_ELT (CLASSTYPE_TI_ARGS (concept_id), 0);

      if (not_req)
        /* Negate the condition.  */
        expr = build_unary_op (TRUTH_NOT_EXPR, expr, 0);

      return build_ice_requirement (expr);
    }
  else if (CLASSTYPE_CONCEPT_P (TREE_TYPE (con)))
    {
      tree t = make_node (REQUIREMENT);
      WHERE_REQ_KIND (t) = not_req? REQ_NOT_CONCEPT : REQ_CONCEPT;
      WHERE_REQ_MODEL (t) = concept_id;
      return t;
    }
  else
    {
      error ("cannot use non-concept %<%T%> in a requirement", 
	     TREE_TYPE (con));
      return NULL_TREE;
    }
}

/* Build a new same-type requirement that equates FIRST and
   SECOND. Returns a new REQUIREMENT node.  */
tree 
build_same_type_requirement (tree first, tree second)
{
  tree t = make_node (REQUIREMENT);
  WHERE_REQ_KIND (t) = REQ_SAME_TYPE;
  WHERE_REQ_FIRST_TYPE (t) = first;
  WHERE_REQ_SECOND_TYPE (t) = second;
  return t;
}

/* Build a new derived-from requirement that states that DERIVED
   derives from BASE. */
tree 
build_derived_from_requirement (tree derived, tree base)
{
  tree t = make_node (REQUIREMENT);
  WHERE_REQ_KIND (t) = REQ_DERIVED_FROM;
  WHERE_REQ_DERIVED (t) = derived;
  WHERE_REQ_BASE (t) = base;
  return t;
}

/* Build a new integral constant expression requirement. EXPR is an
   expression, Returns a new REQUIREMENT node.  */
tree 
build_ice_requirement (tree expr)
{
  tree t = make_node (REQUIREMENT);
  WHERE_REQ_KIND (t) = REQ_ICE;
  WHERE_REQ_CONSTANT_EXPRESSION (t) = expr;
  return t;
}

/* Hash an ASSOCIATED_TYPE.  K is really of type `tree'.  */

static hashval_t
associated_type_hash (const void* k)
{
  hashval_t hash;
  tree t = (tree) k;

  hash = (htab_hash_pointer (TYPE_CONTEXT (t))
	  ^ htab_hash_pointer (DECL_NAME (TYPE_NAME (t)))
	  ^ htab_hash_pointer (TREE_TYPE (t)));

  return hash;
}

typedef struct associated_type_info {
  tree scope;
  tree name;
  tree default_type;
} associated_type_info;

/* Compare two ASSOCIATED_TYPEs.  K1 is really of type `tree', K2 is
   really of type `associated_type_info*'  */

static int
associated_type_compare (const void * k1, const void * k2)
{
  tree t1;
  const associated_type_info *t2;

  t1 = (tree) k1;
  t2 = (const associated_type_info *) k2;

  return (DECL_NAME (TYPE_NAME (t1)) == t2->name
	  && TYPE_CONTEXT (t1) == t2->scope
	  && TREE_TYPE (t1) == t2->default_type);
}

/* Build an associated type named NAME inside the concept CONCEPT with
   (potentially NULL) default type TYPE.  */
static GTY ((param_is (union tree_node))) htab_t associated_type_htab;

tree 
build_associated_type (tree context, tree name, tree type)
{
  tree t;
  associated_type_info ati;
  void **e;
  hashval_t hash;

  if (associated_type_htab == NULL)
    /* Allocate the hash table, if we haven't already done so.  */
    associated_type_htab = htab_create_ggc (61, &associated_type_hash,
					    &associated_type_compare, NULL);

  ati.scope = context;
  ati.name = name;
  ati.default_type = type;

  hash = (htab_hash_pointer (ati.scope) 
	  ^ htab_hash_pointer (ati.name)
	  ^ htab_hash_pointer (ati.default_type));

  e = htab_find_slot_with_hash (associated_type_htab, &ati, hash, INSERT);
  if (*e)
    t = (tree) *e;
  else 
    {
      t = make_node (ASSOCIATED_TYPE);
      gcc_assert (TREE_CODE (name) == TYPE_DECL);
      gcc_assert (context != NULL_TREE);
      TYPE_CONTEXT (t) = context;
      TYPE_NAME (t) = name;
      TREE_TYPE (t) = type;

      /* ASSOCIATED_TYPE nodes require structural equality, because
	 the lookup into the context can change from one context to
	 another.  */
      SET_TYPE_STRUCTURAL_EQUALITY (t);

      /* Place this type in the hash table. */
      *e = t;
    }
  return t;
}

/* Walk through all of the requirements in a requirements CLAUSE,
   calling the user-defined function FN with each requirement (a
   REQUIREMENT node) and the user-supplied DATA.  If FN ever returns a
   value other than NULL_TREE, walk_requirements aborts immediately
   and returns that value.  */
tree
walk_requirements (tree clause, int flags, walk_requirements_fn fn,
                   void *data)
{
  tree req;

  for (req = clause; req; req = WHERE_REQ_CHAIN (req))
    {
      tree result;

      if (flags == REQ_VISIT_NORMAL && WHERE_REQ_ARTIFICIAL (req))
	/* The caller does not want to see any "artificial"
	   requirements.  */
	break;

      if (WHERE_REQ_IMPLICIT (req) && !(flags & REQ_VISIT_IMPLICIT))
        /* Skip implicit requirements.  */
        continue;
     
      if (WHERE_REQ_FROM_REFINEMENT (req) && !(flags & REQ_VISIT_REFINEMENT))
        /* Skip refined requirements.  */
        continue;

      if (WHERE_REQ_FROM_NESTED (req) && !(flags & REQ_VISIT_ASSOCIATED))
        /* Skip associated requirements.  */
        continue;

      /* Visit this requirement.  */
      result = fn (req, data);
      if (result != NULL_TREE)
        return result;
    }

  return NULL_TREE;
}
               
/* Implementation detail of lookup_function_in_where_clause.  */
struct lookup_function_in_requirements_data_t 
{
  /* The name of the entity we are looking for.  */
  tree name;

  /* The binding(s) that we have found so far, if any.  */
  tree binding;
};

/* Determines whether the two functions are distinct. The functions
   are distinct if they have different signatures or if they have the
   same signature but come from different places in the refinement
   lattice.  */
static bool
distinct_functions (tree fn1, tree fn2)
{
  tree refinements;

  if (same_type_p (TREE_TYPE (fn1), TREE_TYPE (fn2)))
    /* Types match exactly; we don't have to look through the
       parameter types.  */
    ;
  else
    {
      tree arg1, arg2;

      /* Check that they have the same number of parameters with the
         same type.  */
      for (arg1 = TYPE_ARG_TYPES (TREE_TYPE (fn1)),
           arg2 = TYPE_ARG_TYPES (TREE_TYPE (fn2));
           arg1 || arg2;
           arg1 = TREE_CHAIN (arg1), arg2 = TREE_CHAIN (arg2))
        {
          if (!arg1 || !arg2)
            return true;
          
          if (!same_type_p (TREE_VALUE (arg1), TREE_VALUE (arg2)))
            return true;
        }
    }

  /* The signatures of the two functions are the same, and the caller
     has ensured that the names are the same.  The functions may be
     the same, if they came from the same concept. This could happen
     when two requirements involve concepts that refine the same base
     concept-id, as in:

       concept Base<typename X> {
         X& operator++(X&);
       }

       concept Refined1<typename X> : Base<X> { }
       concept Refined2<typename X> : Base<X> { }

       template<typename X>
       requires Refined1<X> && Refined2<X> 
       void foo(X& x) {
         ++x; // here, we see both Refined1<X>::operator++ and 
              // Refined2<X>::operator++, but both come from Base<X> and
              // are therefore not distinct.
       }

       FIXME: Ideally, our search would only find Base<X>::operator++,
       so we wouldn't have to remove duplicates. But, our model
       insists on duplicating the operator++'s in the more-refined
       concepts, so we need to do this extra check.  */
  if (same_type_p (DECL_CONTEXT (fn1), DECL_CONTEXT (fn2)))
    return false;

  refinements = common_refinements (DECL_CONTEXT (fn1), DECL_CONTEXT (fn2));
  if (refinements)
    return false;

  return true;
}

/* If NEW_FN is a distinct function from all of the functions in
   CURRENT_FN (which may be a single function or an OVERLOAD), create
   an overload containing CURRENT_FN and NEW_FN. Otherwise, returns
   CURRENT_FN. This routine is used from within
   lookup_function_in_requirements_r to detect when two concept
   operations actually came from the same base concept.  */

static tree
build_overload_if_distinct(tree current_fn, tree new_fn)
{
  if (TREE_CODE (current_fn) == OVERLOAD)
    {
      tree fn;
      for (fn = current_fn; fn; fn = OVL_NEXT (fn))
        if (!distinct_functions (OVL_FUNCTION (fn), new_fn))
          return current_fn;
    }
  else if (!distinct_functions (current_fn, new_fn))
    return current_fn;

  return build_overload (new_fn, current_fn);
}

/* This function is a subroutine of
   lookup_function_in_where_clause. It looks for an operation with a
   specific name in each concept map requirement it is given. VDATA is
   a pointer to a lookup_in_requirements_data_t. */
static tree
lookup_function_in_requirements_r (tree req, void *vdata)
{
  struct lookup_function_in_requirements_data_t *data
    = (struct lookup_function_in_requirements_data_t *)vdata;
  tree new_binding = NULL_TREE;

  if (!WHERE_REQ_MODEL_P (req))
    return NULL_TREE;

  /* Get the binding. */
  new_binding = lookup_member (WHERE_REQ_MODEL (req), data->name,
                               /*protect=*/2, /*want_type=*/false);

  /* Ignore the artificial typedef of a concept name. */
  if (new_binding && DECL_P (new_binding) && DECL_ARTIFICIAL (new_binding))
    new_binding = NULL_TREE;

  /* If we have a function binding */
  if (new_binding
      && (TREE_CODE (new_binding) == OVERLOAD
          || BASELINK_P (new_binding)
          || DECL_FUNCTION_TEMPLATE_P (new_binding)))
    {
      /* Add the binding to the set of function bindings we
         already know about (we may end up with a set of
         overloads). Combine the new and old bindings into an
         overload set. */
      
      if (BASELINK_P (new_binding))
        new_binding = BASELINK_FUNCTIONS (new_binding);
      
      if (!data->binding)
        data->binding = new_binding;
      else 
        {
          if (TREE_CODE (new_binding) == OVERLOAD) 
            {
              tree fn;
              for (fn = new_binding; fn; fn = OVL_NEXT (fn))
                data->binding = build_overload_if_distinct (data->binding,
                                                            OVL_CURRENT (fn));
            }
          else
            data->binding = build_overload_if_distinct (data->binding, 
                                                        new_binding);
          OVL_USED (data->binding) = 1;
        }
    }
  
  return NULL_TREE;
}

/* Lookup NAME in the given requirements CLAUSE. We will only find
   associated function names.  */
tree
lookup_function_in_where_clause (tree where_clause, tree name)
{
  struct lookup_function_in_requirements_data_t data;
  data.name = name;
  data.binding = NULL_TREE;
  walk_requirements (where_clause, 
                     REQ_VISIT_IMPLICIT | REQ_VISIT_ASSOCIATED, 
                     &lookup_function_in_requirements_r, &data);
  return data.binding;
}

/* Implementation detail of lookup_associated_type_in_where_clause.  */
struct lookup_type_in_requirements_data_t 
{
  /* The type in which the nested name NAME was specified.  */
  tree type;

  /* The name of the entity we are looking for.  */
  tree name;

  /* The binding(s) that we have found so far, if any.  */
  tree binding;
};

/* This function is a subroutine of
   lookup_associated_type_in_where_clause. It looks for an associated
   type with a specific name in each concept map requirement it is
   given. VDATA is a pointer to a lookup_in_requirements_data_t. */
static tree
lookup_associated_type_in_requirements_r (tree req, void *vdata)
{
  struct lookup_type_in_requirements_data_t *data
    = (struct lookup_type_in_requirements_data_t *)vdata;
  tree new_binding = NULL_TREE;
  tree model_id;
  tree args;
  int i, n;
  bool found;

  if (!WHERE_REQ_MODEL_P (req))
    return NULL_TREE;

  model_id = WHERE_REQ_MODEL (req);
  args = CLASSTYPE_TI_ARGS (model_id);
  n = NUM_TMPL_ARGS (args);
  found = false;

  /* One of the template arguments of MODEL_ID must be the
     same as DATA->TYPE. If we're only allowing associated types to
     come from the template header, the first template
     argument of MODEL_ID is the only one we'll check. */
  for (i = 0; i < n && !found; ++i)
    {
      if (TYPE_P (TMPL_ARG (args, 1, i))
          && same_type_p (TMPL_ARG (args, 1, i), data->type))
        found = true;
    }
  
  if (!found)
    return NULL_TREE;
  
  /* DPG TBD: We need to deal with ambiguity resolution, here,
     once we eliminate the combining of associated types in
     refinement hierarchies. */
  new_binding = lookup_member (model_id, data->name, 
                               /*protect=*/2, /*want_type=*/1);
  
  /* Ignore the artificial typedef of a concept name. */
  if (new_binding && DECL_P (new_binding) && DECL_ARTIFICIAL (new_binding))
    new_binding = NULL_TREE;
  
  if (new_binding)
    {
      if (!data->binding)
        data->binding = new_binding;
      else if (!same_type_p (TREE_TYPE (data->binding),
                             TREE_TYPE (new_binding)))
        {
          error ("reference to type %<%T::%E%> is ambiguous", 
                 data->type, data->name);
          inform ("could be %qD", data->binding);
          inform ("or %qD", new_binding);
          return error_mark_node;
        }
    }

  return NULL_TREE;
}

/* Lookup the associated type NAME (which must be an IDENTIFIER_NODE)
   for the given opaque type TYPE. This is the lookup that will be
   performed for the syntax `TYPE::NAME', which considers associated
   types that come from concept constraints in the where clause.  */
tree 
lookup_associated_type_in_where_clause (tree type, tree name)
{
  struct lookup_type_in_requirements_data_t data;
  data.type = type;
  data.name = name;
  data.binding = NULL_TREE;
  walk_requirements (TREE_VALUE (current_where_clause), 
                     REQ_VISIT_IMPLICIT, 
                     &lookup_associated_type_in_requirements_r, &data);
  return data.binding;
}

/* Declares or defines the name of a concept within the current
   scope. */
tree
xref_concept (tree name, bool structural_p)
{
  tree concept = xref_tag (concept_type, name, ts_current, 
                           /*template_header_p=*/true);

  if (concept != error_mark_node)
    CLASSTYPE_USE_CONCEPT (concept) 
      = structural_p? ck_auto_concept : ck_concept;

  return concept;
}

/* Begin the definition of a new concept. */
tree
begin_concept_definition (tree concept)
{
  return begin_class_definition (concept, /*attributes=*/NULL_TREE);
}

/* Finish the definition of a new concept. */
tree
finish_concept_definition (tree concept)
{
  /* As with associated types in xref_superiors, we need to "pull
     down" operations from superior concepts. We need to be careful
     that we only pull up operations that have not already been
     provided in this concept. */
  tree binfo, base_binfo;
  int i;

  for (binfo = TYPE_BINFO (concept), i = 0;
       BINFO_BASE_ITERATE (binfo, i, base_binfo); i++)
    {
      tree model_id = BINFO_TYPE (base_binfo);
      tree superior_concept = CLASSTYPE_PRIMARY_TEMPLATE_TYPE (model_id);
      tree superior_args = CLASSTYPE_TI_ARGS (model_id);
      tree superior_func;

      for (superior_func = TYPE_METHODS (superior_concept);
           superior_func; superior_func = TREE_CHAIN (superior_func))
        if (TREE_CODE (superior_func) == FUNCTION_DECL
	    && TREE_TYPE (TREE_TYPE (superior_func)) != axiom_type_node)
          {
            tree type = instantiate_operation_type (superior_func, model_id);
            tree func;

            /* Search for an operation with the same name and type in
               the concept we're finishing. */
            for (func = TYPE_METHODS (concept); func; func = TREE_CHAIN (func))
              if (TREE_CODE (func) == FUNCTION_DECL
                  && DECL_NAME (func) == DECL_NAME (superior_func)
                  && compparms (TYPE_ARG_TYPES (TREE_TYPE (func)), 
                                TYPE_ARG_TYPES (TREE_TYPE (superior_func)))
                  && (!DECL_CONV_FN_P (func)
                      || same_type_p (TREE_TYPE (TREE_TYPE (func)),
                                      TREE_TYPE (TREE_TYPE (superior_func))))
                  && (DECL_DESTRUCTOR_P (func) 
                      == DECL_DESTRUCTOR_P (superior_func)))
                break;

            /* We did not find a function with identical type, so
               introduce a new operation. */
            if (!func) 
              {
                tree decl;

                decl = build_operation_decl (superior_func, type, concept,
                                             superior_args);
                gcc_assert (decl != error_mark_node);

                if (DECL_CONSTRUCTOR_P (superior_func))
                  DECL_NAME (decl) = tsubst (DECL_NAME (superior_func), 
                                             superior_args, tf_error,
                                             concept);

                /* The new function decl should NOT be a template at
                 this time, because we do not want it to be considered
                 either a specialization or an instantiation of the
                 requirement in the concept. Thus, we clear out all
                 template information. Appropriate template
                 information will be restored in
                 push_template_decl. */
                DECL_TEMPLATE_INFO (decl) = NULL_TREE;
                DECL_USE_TEMPLATE (decl) = 0; 
                
                /* Mark the function as "inline". */
                DECL_DECLARED_INLINE_P (decl) = 1;
                DECL_INLINE (decl) = 1;
                DECL_ATTRIBUTES (decl) = 
                  tree_cons (get_identifier("always_inline"),
                             NULL_TREE,
                             DECL_ATTRIBUTES (decl));

                /* Declare the function in the concept. */ 
                finish_member_declaration (push_template_decl (decl));
              }
          }
    }

  /* Finish off the requirements clause of the concept.  */
  CLASSTYPE_WHERE_CLAUSE (concept) 
    = finish_where_clause (CLASSTYPE_WHERE_CLAUSE (concept));

  return finish_struct (concept, NULL_TREE);
}

/* Mark each of the template parameters deducible from the template
 * argument T in the TREE_VEC named MARKED. When a deducible template
 * parameter is found, the position in MARKED corresponding to the
 * index of that template parameter will be set to BOOLEAN_TRUE_NODE.
 * If HAS_ASSOC_TYPE is non-NULL and an associated type is encountered,
 * *HAS_ASSOC_TYPE will be set TRUE.
 */
void mark_deduced_parms (tree t, tree marked, bool *has_assoc_type)
{
  /* 14.8.2.4 Deducing template arguments from a type [temp.deduct.type]
   *
   * 9   A template type argument T, a template template argument TT or
   *     a template nontype argument i can be deduced if P and A have one
   *     of the following forms:
   */
  switch (TREE_CODE (t)) {
    /*       T
     *       cvlist T    */
  case CONST_DECL:
    if (DECL_TEMPLATE_PARM_P (t))
      {
        int idx = TEMPLATE_PARM_IDX (DECL_INITIAL (t));
        TREE_VEC_ELT (marked, idx) = boolean_true_node;
      }
    break;

  case TEMPLATE_TYPE_PARM:
  case TEMPLATE_TEMPLATE_PARM:
    TREE_VEC_ELT (marked, TEMPLATE_TYPE_IDX (t)) = boolean_true_node;
    break;

  case TEMPLATE_PARM_INDEX:
    TREE_VEC_ELT (marked, TEMPLATE_PARM_IDX (t)) = boolean_true_node;
    break;

    /*     T*   
     *     T&     */
  case POINTER_TYPE:
  case REFERENCE_TYPE:
    mark_deduced_parms (TREE_TYPE (t), marked, has_assoc_type);
    break;

    /*     T[integer-constant] 
     *     type[i]     */
  case ARRAY_TYPE:
    mark_deduced_parms (TREE_TYPE (t), marked, has_assoc_type);
    mark_deduced_parms (TYPE_DOMAIN (t), marked, has_assoc_type);
    break;

    /*     template-name<T> (where template-name refers to a class template) 
     *     template-name<i> (where template-name refers to a class template) */
  case RECORD_TYPE:
    if (TYPE_PTRMEMFUNC_P (t))
      {
        /* T (type::*)()
         * type (T::*)()
         * type (type::*)(T)
         * type (T::*)(T)
         * T (type::*)(T)
         * T (T::*)()
         * T (T::*)(T)        */
        mark_deduced_parms (TYPE_PTRMEMFUNC_FN_TYPE (t), marked, 
                            has_assoc_type);
        mark_deduced_parms (TYPE_PTRMEMFUNC_OBJECT_TYPE (t), marked, 
                            has_assoc_type);
        break;
      }
    /* Fall through */
  case UNION_TYPE:
    if (CLASSTYPE_TEMPLATE_INFO (t))
      mark_deduced_parms (CLASSTYPE_TI_ARGS (t), marked, has_assoc_type);
    break;

    /*     type(*)(T)
     *     T(*)()
     *     T(*)(T) 
     *
     * Note: We handle these cases as function and method types, not
     * function pointer types. */
  case FUNCTION_TYPE:
  case METHOD_TYPE:
    mark_deduced_parms (TREE_TYPE (t), marked, has_assoc_type);
    {
      tree arg = TYPE_ARG_TYPES (t);
      while (arg)
        {
          mark_deduced_parms (arg, marked, has_assoc_type);
          arg = TREE_CHAIN (arg);
        }
    }
    break;

    /*     T type::*
     *     type T::*
     *     T T::*      */
  case OFFSET_TYPE:
    mark_deduced_parms (TREE_TYPE (t), marked, has_assoc_type);
    mark_deduced_parms (TYPE_OFFSET_BASETYPE (t), marked, has_assoc_type);
    break;

    /*     TT<T>
     *     TT<i>
     *     TT<>     */
  case BOUND_TEMPLATE_TEMPLATE_PARM:
    {
      tree tinfo = TEMPLATE_TEMPLATE_PARM_TEMPLATE_INFO (t);
      mark_deduced_parms (TI_TEMPLATE (tinfo), marked, has_assoc_type);
      mark_deduced_parms (TI_ARGS (tinfo), marked, has_assoc_type);
      break;
    }

    /* Used only for template arguments. */
  case TREE_VEC:
    {
      int len = TREE_VEC_LENGTH (t);
      int idx;
      for (idx = 0; idx < len; ++idx)
        mark_deduced_parms (TREE_VEC_ELT (t, idx), marked, has_assoc_type);
      break;
    }

  case TYPENAME_TYPE:
  case ASSOCIATED_TYPE:
    /* We have found an associated type. */
    if (has_assoc_type)
      *has_assoc_type = true;
    break;

  default:
    /* Nothing is deducible. */
    break;
  }
}

/* Verify that the model-id SUPERIOR is a valid refinement for the
 * given CONCEPT currently being defined. Returns TRUE when the
 * refinement is valid, FALSE otherwise. A refinement is valid when
 * all of the template parameters of the concept can be deduced from
 * the superior model-id. */
bool 
check_refinement (tree concept, tree superior)
{
  int nparms = DECL_NTPARMS (concept);
  int idx;
  tree deduced_parms;
  bool has_assoc_type = false;

  /* Refinements must refer to at least one template parameter. */
  if (!uses_template_parms (superior))
    {
      error ("refinement %<%T%> does not refer to any template parameters", 
             superior);
      return false;
    }

  /* Keep track of whether the template parameters for CONCEPT are
     deduced in the SUPERIOR model-id. */
  deduced_parms = make_tree_vec (nparms);
  for (idx = 0; idx < nparms; ++idx)
    TREE_VEC_ELT (deduced_parms, idx) = NULL_TREE;

  /* Mark all of the deduced parameters. */
  mark_deduced_parms (CLASSTYPE_TI_ARGS (superior), deduced_parms, 
                      &has_assoc_type);

  /* Refinements shall not refer to associated types. */
  if (has_assoc_type)
    {
      error ("refinement %<%T%> is may not refer to an associated type", 
             superior);
      return false;
    }
  return true;
}


/* Attach the list of superiors (i.e., refined concepts) to the
   concept. superiors is a TREE_LIST whose TREE_VALUE fields contain
   model IDs. Additionally, declares the associated types from the
   superiors into the concept. This latter operation is somewhat of a
   hack, because I am not ready to munge name lookup so that it will
   find names in (otherwise) dependent bases. Once we have the notion
   of dependent/non-dependent sorted out for concepts, this can be
   eliminated. Note that we DO NOT do the same for required
   operations at this time. 

   This routine returns an updated version of SUPERIORS that should be
   attached to the concept.
*/
tree
xref_superiors (tree concept, tree superiors)
{
  /* Refinements are stored as virtual, public inheritance. */
  tree sup = superiors;
  tree prev_sup = NULL_TREE;
  tree field;
  tree concept_tmpl = CLASSTYPE_TI_TEMPLATE (concept);

  while (sup) 
    {
      tree superior_type = TREE_VALUE (sup);
      tree args = CLASSTYPE_TI_ARGS (superior_type);
      tree primary = CLASSTYPE_PRIMARY_TEMPLATE_TYPE (superior_type);

      /* Verify that this refinement is well-formed. */
      if (!check_refinement (concept_tmpl, superior_type))
        {
          /* The refinement is broken, so remove it from consideration. */
          if (prev_sup)
            {
              TREE_CHAIN (prev_sup) = TREE_CHAIN (sup);
              sup = TREE_CHAIN (prev_sup);
            }
          else
            {
              superiors = TREE_CHAIN (superiors);
              sup = superiors;
            }

          continue;
        }

      TREE_PURPOSE (sup) = access_public_node; /* public */
      TREE_TYPE (sup) = void_type_node;        /* virtual */
      prev_sup = sup;
      sup = TREE_CHAIN (sup);

      /* Introduce the associated types from the superior concept.
         TBD: This can all go away once we sort out how to do lookups
         in base classes (superior concepts) that use template
         parameters but are not dependent. However, for now we've only
         hacked lookup_member to deal with one level of lookups in
         model-ids, so we need to "pull down" the associated types
         from each superior concept. */
      for (field = TYPE_FIELDS (primary); field; field = TREE_CHAIN (field))
        {
          if (TREE_CODE (field) == TYPE_DECL
              && TREE_CODE (TREE_TYPE (field)) == ASSOCIATED_TYPE)
            {
              tree associated_type = TREE_TYPE (field);
              tree type = TREE_TYPE (associated_type);

              /* See if we already have an associated type with this
                 name. */
              tree has_field = TYPE_FIELDS (concept);
              while (has_field && DECL_NAME (has_field) != DECL_NAME (field))
                has_field = TREE_CHAIN (has_field);

              if (has_field)
                {
                  tree has_assoc = TREE_TYPE (has_field);

                  /* ERROR_MARK_NODE signals an ambiguity in default
                     values. */
                  if (TREE_TYPE (has_assoc) == error_mark_node)
                    /* Do nothing. */;
                  else if (!TREE_TYPE (has_assoc) && type)
                    /* Record a default value for this associated
                       type, which previously had none. */
                    TREE_TYPE (has_assoc) = tsubst (type, args, tf_error,
                                                    concept);
                  else if (TREE_TYPE (has_assoc) && type)
                    {
                      /* If the default values that come from two
                         superior concepts are different, we have an
                         ambiguity.  */
                      ++unchecked_section_of_constrained_template;
                      type = tsubst (type, args, tf_error, concept);
                      --unchecked_section_of_constrained_template;

                      if (!same_type_p (TREE_TYPE (has_assoc), type))
                        TREE_TYPE (has_assoc) = error_mark_node;
                    }
                }
              else
                {
                  /* Introduce a new associated type into the concept
                     being defined. */ 
                  tree context = tsubst (TYPE_CONTEXT (associated_type),
                                         args, tf_error, concept);
                  tree default_type;
                  tree decl;
                  ++unchecked_section_of_constrained_template;
                  default_type = tsubst (type, args, tf_error, concept);
                  --unchecked_section_of_constrained_template;
                  decl = build_lang_decl (TYPE_DECL, DECL_NAME (field),
                                          /*type=*/NULL_TREE);
                  TREE_TYPE (decl) = build_associated_type (context, decl,
                                                            default_type);
                  DECL_CONTEXT (decl) = tsubst (DECL_CONTEXT (decl), args,
                                                tf_error, concept);

                  TYPE_FIELDS (concept) = chainon (TYPE_FIELDS (concept), 
                                                   decl);
                }
            }
        }
    }

  /* Replace ERROR_MARK_NODEs with NULL_TREEs: if the defaults for an
     associated type from different superiors clash, then we have no
     default. */
  for (field = TYPE_FIELDS (concept); field; field = TREE_CHAIN (field))
    if (TREE_CODE (field) == TYPE_DECL 
        && TREE_CODE (TREE_TYPE (field)) == ASSOCIATED_TYPE
        && TREE_TYPE (TREE_TYPE (field)) == error_mark_node)
      TREE_TYPE (TREE_TYPE (field)) = NULL_TREE;

  xref_basetypes (concept, superiors);

  return superiors;
}

/* Begins processing a definition for the given MODEL_ID, which is a
   RECORD_TYPE. */
tree
begin_model_definition (tree model_id)
{
  /* A model definition is represented as a (full or partial)
     specialization. */
  if (uses_template_parms (model_id))
    maybe_process_partial_specialization (model_id);
  else
    begin_specialization ();
 
  /* A model definition has no base types. */
  TYPE_BINFO (model_id) = 0;
  if (!CLASSTYPE_MODEL_P (model_id)) 
    CLASSTYPE_USE_CONCEPT (model_id) = ck_concept_map;
  xref_basetypes (model_id, NULL_TREE);

  TYPE_BEING_DEFINED (model_id) = 0;
  model_id = begin_class_definition (model_id, /*attributes=*/NULL_TREE);
  if (model_id != error_mark_node)
    TYPE_BEING_DEFINED (model_id) = 1;

  return model_id;
}

/* Finish processing a model (template) definition. This "closes out"
   the model definition by introducing defaults for associated types
   and operations, so that every requirement of the concept is
   satisfied by the model. It is also responsible for defining model
   (templates) for each of the concepts that are refined by the
   concept being modeled. For instance, consider the following code:

     template<typeid T> concept A { };
     template<typeid T> concept B : A<T> { };

     template<typename T> model B<vector<T> > { }; 

   When we finish the model definition for B<vector<T> >, we will also
   define a model for A<vector<T> >, with the same template
   parameters, arguments, where clause, associated types, operations,
   etc.

   MODEL_ID is a model-id, i.e., a template-id stored as a RECORD_TYPE. 

   IMPLICIT_MODEL is true when this model is implicitly being built,
   i.e., because we're matching a structural concept.

   Returns the completed model_id if successful; error_mark_node if
   something goes wrong.
*/
tree 
finish_model_definition (tree model_id, bool implicit_model)
{
  bool template_p = uses_template_parms (model_id);
  tree template;
  tree concept;
  bool failed;
  struct pointer_set_t *reqs_checked;

  /* If we allow finish_struct to do this, we end up with horrible
     error messages. */
  if (COMPLETE_TYPE_P (model_id))
    {
      error ("redefinition of %q#T", model_id);
      popclass ();
      return error_mark_node;
    }

  /* Keep track of which signature requirements were checked as part
     of associated type deduction.  */
  reqs_checked = pointer_set_create ();

  /* Complete the set of necessary associated type declarations. */
  failed = !add_associated_type_defaults (model_id, reqs_checked);

  /* Declare all of the operations in this model. */
  if (!failed) failed = !declare_model_operations (model_id, reqs_checked);

  /* We're done with the set of requirements that has already been
     checked.  */
  pointer_set_destroy (reqs_checked);

  model_id = finish_struct (model_id, NULL_TREE);
  if (!template_p)
    end_specialization ();
  TYPE_BEING_DEFINED (model_id) = 0;

  /* Define models for each of the superior concepts.  */
  if (!failed) 
    failed = !define_models_for_superiors (model_id, implicit_model);

  /* Define model operations for all of the superiors. But first, we
     need to clear out the all-refinements cache, since we now have
     definitions for all of the superiors. */
  CLASSTYPE_ALL_REFINEMENTS (model_id) = NULL_TREE;
  dfs_walk_superiors (model_id, &define_model_operations_for_superiors,
                      model_id);

  /* Dig out the concept */
  template = most_general_template (CLASSTYPE_TI_TEMPLATE (model_id));
  gcc_assert (TREE_CODE (template) == TEMPLATE_DECL);
  concept = TREE_TYPE (template);

  /* Verify that the nested requirements are met. DPG TBD: We only do
     this for non-template models right now. */
  if (!failed
      && !template_p
      && !where_clause_satisfied_p (CLASSTYPE_WHERE_CLAUSE (concept),
                                    CLASSTYPE_TI_ARGS (model_id),
                                    CLASSTYPE_TI_ARGS (model_id),
                                    /*complain=*/false))
    {
      error ("%qD does not meet the nested requirements of its concept",
             model_id);
      where_clause_satisfied_p (CLASSTYPE_WHERE_CLAUSE (concept),
                                CLASSTYPE_TI_ARGS (model_id),
                                CLASSTYPE_TI_ARGS (model_id),
                                /*complain=*/!implicit_model);
    }

  return failed? error_mark_node : model_id;
}

/* Takes in a parameter to a function and builds an expression that
   forwards this parameter as an argument to another function. */
tree 
forward_parameter (tree parm)
{
  tree arg = convert_from_reference (parm);

  if (TREE_CODE (TREE_TYPE (parm)) == REFERENCE_TYPE
      && TYPE_REF_IS_RVALUE (TREE_TYPE (parm)))
    /* If the parameter's type is an rvalue reference, we need to
       forward it to pick the right underlying operation.  */
    arg = build_static_cast (cp_build_reference_type (TREE_TYPE (arg),
                                                      /*rvalue_ref=*/true),
                             arg);

  return arg;
}

/* Attempts to deduce the definition of associated type ASSOC from the
   signatures required of the corresponding MODEL_ID. ASSOC is a
   TYPE_DECL. 

   REQS_CHECKED is a pointer set that marks which requirements were
   implicitly checked while attempting to deduce associated
   types. These requirements do not need to be checked again.

   Returns NULL_TREE if the associated type could not be deduced,
   ERROR_MARK_NODE if multiple, distinct types were deduced, or a type
   if a single type was deduced. */
static tree 
deduce_associated_type_from_signatures (tree assoc, tree model_id,
					struct pointer_set_t *reqs_checked)
{
  tree name = DECL_NAME (assoc);
  tree args;
  tree template;
  tree concept;
  tree sig;
  tree candidates = NULL_TREE;
  bool failed = false;

  args = CLASSTYPE_TI_ARGS (model_id);

  /* Determine the concept we are trying to match. */
  template = most_general_template (CLASSTYPE_TI_TEMPLATE (model_id));
  gcc_assert (TREE_CODE (template) == TEMPLATE_DECL);
  concept = TREE_TYPE (template);

  for (sig = TYPE_METHODS (concept); sig; sig = TREE_CHAIN (sig))
    {
      /* Only look at signature requirements that return an associated
         type. */
      if (TREE_CODE (sig) == FUNCTION_DECL
          && TREE_CODE (TREE_TYPE (TREE_TYPE (sig))) == ASSOCIATED_TYPE)
        {
          tree rettype;
          tree model_op = NULL_TREE;
          tree sig_parmtypes = NULL_TREE;

          /* Ensure that this signature returns the appropriate
             associated type. */
          rettype = TREE_TYPE (TREE_TYPE (sig));
          if (ASSOCIATED_TYPE_NAME (rettype) != name
              || (!same_type_p (concept, TYPE_CONTEXT (rettype))
                  && !refinement_of_p (concept, TYPE_CONTEXT (rettype))))
            continue;

          /* TBD: Make sure that this signature does not depend on any
             undefined associated types. */

          if (TREE_CODE (TREE_TYPE (sig)) != METHOD_TYPE)
            {
	      sig_parmtypes = tsubst (FUNCTION_FIRST_USER_PARMTYPE (sig),
				      args, tf_error, /*in_decl=*/model_id);

	      if (sig_parmtypes == error_mark_node)
		continue;

              /* Determine if there is an operation defined in the model
                 that matches this signature. If so, add it and its return
                 type to the set of candidates. */
              for (model_op = TYPE_METHODS (model_id); model_op; 
                   model_op = TREE_CHAIN (model_op))
                {
                  if (DECL_NAME (sig) == DECL_NAME (model_op)
                      && (list_length (TYPE_ARG_TYPES (TREE_TYPE (sig)))
                          == list_length (TYPE_ARG_TYPES (TREE_TYPE (model_op)))))
                    {
                      tree rettype;
                      tree cand;
                      
                      /* If the types of the parameter lists are not the
                         same, then we don't have a match. */
                      if (!compparms (sig_parmtypes, 
                                      FUNCTION_FIRST_USER_PARMTYPE (model_op)))
                        continue;
                      
                      /* We have matched the signature. If this return
                         type is unique among the list of candidates, add
                         it to the list of candidates. */
                      rettype = TREE_TYPE (TREE_TYPE (model_op));
                      for (cand = candidates; cand; cand = TREE_CHAIN (cand))
                        if (same_type_p (rettype, TREE_PURPOSE (cand)))
                          break;
                      
                      if (!cand)
                        candidates = tree_cons (rettype, model_op, candidates);
                      
                      break;
                    }
                }
            }

          if (!model_op
              && !DECL_CONSTRUCTOR_P (sig)
              && !DECL_DESTRUCTOR_P (sig))
            {
              /* We did not find a function in the model that matches
                 this signature. So, we want to synthesize the body of
                 the implicitly-generated function for this signature
                 and determine what type it will have. */
              tree expr = NULL_TREE;
	      tree func = NULL_TREE;

              if (DECL_OVERLOADED_OPERATOR_P (sig) == NOP_EXPR)
		{
		  expr = build_implicit_copy_assignment_expr (sig, model_id,
							      NULL_TREE);

		  /* We don't need to check this requirement
		     again.  */
		  pointer_set_insert (reqs_checked, sig);
		}
	      else if (TREE_CODE (TREE_TYPE (sig)) == METHOD_TYPE)
		{
		  expr = build_implicit_method_call_expr (sig, model_id, 
							  NULL_TREE);

		  /* We don't need to check this requirement
		     again.  */
		  pointer_set_insert (reqs_checked, sig);
		}
              else if (TREE_CODE (TREE_TYPE (sig)) == FUNCTION_TYPE)
		{
		  tree func_type;

		  func_type = build_function_type (void_type_node,
						   sig_parmtypes);

                  /* Build a declaration for the model operation. */
                  func = build_operation_decl (sig, func_type, model_id, args);

                  if (func && func != error_mark_node)
                    /* Build the expression that will be used in the
                       body of the model operation. */
                    expr = build_implicit_model_operation_expr (func, 0);
                }
              
              /* Ignore any errors for now. If there is a problem
                 with the body of the implicitly-generated model
                 operation, it will be dealt with later. */
              if (expr && expr != error_mark_node && TREE_TYPE (expr) 
                  && TREE_TYPE (expr) != error_mark_node)
                {
                  tree cand;

                  /* The type of this expression can be used to
                     deduce the associated type.  If this type is
                     unique among the list of candidates, add it
                     to the list of candidates. */
                  rettype = TREE_TYPE (expr);

                  /* If the resulting expression is an lvalue (but
                     not a temporary of class type), then we
                     actually want to deduce a reference type. */
                  if (TREE_CODE (rettype) != REFERENCE_TYPE
                      && TREE_CODE (expr) != TARGET_EXPR
                      && lvalue_p (expr))
                    rettype = cp_build_reference_type (rettype,
                                                       /*rvalue_ref=*/false);
                      
                  for (cand = candidates; cand; cand = TREE_CHAIN (cand))
                    if (same_type_p (rettype, TREE_PURPOSE (cand)))
                      break;
                      
                  if (!cand)
                    candidates = tree_cons (rettype, func, candidates);
                }
	      else if (expr == error_mark_node)
		{
		  /* We don't want to check this requirement again,
		     because doing so would only produce duplicate
		     error messages.  */
		  pointer_set_insert (reqs_checked, sig);
		  failed = true;
		}
            }
        }
    }

  if (failed)
    /* We already spit out some error messages; just return an
       error.  */
    return error_mark_node;
  else if (!candidates)
    /* We cannot deduce this associated type from signatures. */
    return NULL_TREE;
  else if (!TREE_CHAIN (candidates))
    /* We have deduced exactly one type. This is our answer. */
    return TREE_PURPOSE (candidates);
  else
    {
      /* We have deduced multiple, different types. Print an error. */
      candidates = nreverse (candidates);

      error ("%Jambiguous deduction of associated type `%s'", 
             TYPE_NAME (model_id), IDENTIFIER_POINTER (name));
      for (; candidates; candidates = TREE_CHAIN (candidates))
	{
	  if (TREE_VALUE (candidates))
	    inform("%J  deduced %<%T%> from %<%D%>", TREE_VALUE (candidates), 
		   TREE_PURPOSE (candidates), TREE_VALUE (candidates));
	  else
	    inform("  deduced %<%T%>", TREE_PURPOSE (candidates));
	}
      
      return error_mark_node;
    }
}

/* Adds default values for any associated types that are required by
   the concept but not provided in the model (template). Returns true
   unless something went wrong. 

   REQS_CHECKED is as in deduce_associated_type_from_signatures. */
static bool 
add_associated_type_defaults (tree model_id, 
			      struct pointer_set_t *reqs_checked)
{
  tree template;
  tree concept;
  tree req_field, field;

  template = most_general_template (CLASSTYPE_TI_TEMPLATE (model_id));
  gcc_assert (TREE_CODE (template) == TEMPLATE_DECL);
  concept = TREE_TYPE (template);

  /* Check each required type in the concept. */
  for (req_field = TYPE_FIELDS (concept); req_field; 
       req_field = TREE_CHAIN (req_field))
    {
      if (TREE_CODE (req_field) == TYPE_DECL
          && TREE_CODE (TREE_TYPE (req_field)) == ASSOCIATED_TYPE)
        {
          /* Try to find a TYPE_DECL with the same name in the model. */
          for (field = TYPE_FIELDS (model_id); field; 
               field = TREE_CHAIN (field))
            {
              if (TREE_CODE (field) == TYPE_DECL
                  && DECL_NAME (field) == DECL_NAME (req_field))
                break;
            }

          /* If no field was found, try to synthesize one */
          if (!field)
            {
              if (TREE_TYPE (TREE_TYPE (req_field)))
                {
                  /* There is a default type for this associated type,
                     so substitute in our template arguments. */
                  tree new_type, new_field;
                  new_type = tsubst (TREE_TYPE (TREE_TYPE (req_field)),
                                     CLASSTYPE_TI_ARGS (model_id),
                                     tf_error | tf_warning,
                                     NULL_TREE);
                  if (new_type != error_mark_node)
                    {
                      new_field = build_lang_decl (TYPE_DECL, 
                                                   DECL_NAME (req_field),
                                                   new_type);
                      if (processing_template_decl)
                        new_field = push_template_decl (new_field);
                      
                      finish_member_declaration (new_field);
                    }
                  else
                    return false;
                }
              else
                {
                  tree type 
		    = deduce_associated_type_from_signatures (req_field,
							      model_id,
							      reqs_checked);
                  if (type == error_mark_node)
                    return false;
                  else if (type)
                    {
                      tree new_field = build_lang_decl (TYPE_DECL, 
                                                        DECL_NAME (req_field),
                                                        type);
                      if (processing_template_decl)
                        new_field = push_template_decl (new_field);

                      finish_member_declaration (new_field);
                    }
                  else
                    {
                      error("%D does not define associated type %<%s%>", 
                            model_id,
                            IDENTIFIER_POINTER (DECL_NAME (req_field)));
                      return false;
                    }
                }
            }
        }
    }
  return true;
} 

/* (Partially) instantiate an operation type for the given
   model_id. This operation differs from a true instantiation of the
   type because we may not have concrete types and the concrete types
   that we do have might otherwise form invalid declarations, such as
   a constructor for an integer.

   Returns the instantiated type, or error_mark_node on failure. */
static tree 
instantiate_operation_type (tree decl, tree model_id)
{
  tree new_func_type;

  if (DECL_CONSTRUCTOR_P (decl) 
      || DECL_DESTRUCTOR_P (decl)
      || (DECL_OVERLOADED_OPERATOR_P (decl)
          && (NEW_DELETE_OPNAME_P (DECL_NAME (decl))
              || (TREE_CODE (TREE_TYPE (decl)) == METHOD_TYPE
                  && DECL_ASSIGNMENT_OPERATOR_P (decl)))))
    {
      /* Constructors and overloaded new/delete operators are
         particularly tricky, because we need to be able to
         build a signature even when we could not
         normally do so, e.g., for built-in types. Tip-toe
         around this by creating an entirely new
         declaration. Eventually, we might have to do this for
         everything, but we'll avoid it while we can. */
      tree func_type;
      tree this_type;
      tree req_type = TREE_TYPE (decl);
              
      /* Build a function type on which we can perform the
         substitutions. */
      func_type = 
        build_function_type (TREE_TYPE (req_type),
                             TREE_CHAIN (TYPE_ARG_TYPES (req_type)));
      func_type = tsubst (func_type,
                          CLASSTYPE_TI_ARGS (model_id),
                          tf_error | tf_warning,
                          model_id);

      if (func_type == error_mark_node)
        return error_mark_node;

      /* Substitute to get the "this" type. */
      this_type = tsubst (TYPE_METHOD_BASETYPE (req_type),
                          CLASSTYPE_TI_ARGS (model_id),
                          tf_error | tf_warning,
                          model_id);
              
      /* Build the method type. */
      new_func_type = build_method_type (this_type, func_type);
    }
  else
    new_func_type = tsubst (TREE_TYPE (decl), CLASSTYPE_TI_ARGS (model_id),
                            tf_error | tf_warning, model_id);

  return new_func_type? new_func_type : error_mark_node;
}

/* Builds a new decl for an operation in a concept or model (template)
   CONTEXT given the old DECL, the TYPE of the new decl--which
   probably came from instantiate_operation_type--and the template
   arguments to substitute (typically CLASSTYPE_TI_ARGS(CONTEXT) when
   CONTEXT is a model template).  Returns the new decl. */
static tree 
build_operation_decl (tree decl, tree type, tree context, tree targs)
{
  tree func;
  /* Build the declaration. This code is adapted from
     tsubst_decl. */
  func = copy_decl (decl);
  TREE_TYPE (func) = type;
  /* Clear out the mangled name and RTL for the instantiation.  */
  SET_DECL_ASSEMBLER_NAME (func, NULL_TREE);
  SET_DECL_RTL (func, NULL_RTX);
  DECL_INITIAL (func) = NULL_TREE;
  DECL_CONTEXT (func) = context;
  DECL_ARGUMENTS (func) = tsubst (DECL_ARGUMENTS (decl), targs,
                                  tf_warning_or_error, decl);
  DECL_RESULT (func) = NULL_TREE;
          
  TREE_STATIC (func) = 0;
  TREE_PUBLIC (func) = 1;
  DECL_EXTERNAL (func) = 1;
  DECL_INLINE (func) = 1;
  DECL_ATTRIBUTES (func) = 
    tree_cons (get_identifier("always_inline"),
               NULL_TREE,
               DECL_ATTRIBUTES (func));
  
  /* If this is an instantiation of a function with internal
     linkage, we already know what object file linkage will be
     assigned to the instantiation.  */
  DECL_INTERFACE_KNOWN (func) = !TREE_PUBLIC (func);
  DECL_DEFER_OUTPUT (func) = 0;
  TREE_CHAIN (func) = NULL_TREE;
  DECL_PENDING_INLINE_INFO (func) = 0;
  DECL_PENDING_INLINE_P (func) = 0;
  DECL_SAVED_TREE (func) = NULL_TREE;
  TREE_USED (func) = 0;
  DECL_ARTIFICIAL (func) = 1;

  if (DECL_CONSTRUCTOR_P (func))
    grok_ctor_properties (context, func);
  else if (IDENTIFIER_OPNAME_P (DECL_NAME (func)))
    grok_op_properties (func, /*complain=*/true);
          
  return func;
}

/* Given a DECL and a set of OVERLOADS (that may be a NULL_TREE, a
DECL, or an OVERLOAD node), determine if the DECL duplicates something
in OVERLOADS. */
static bool 
duplicate_decl_for_model (tree decl, tree overloads) 
{ 
  if (overloads == NULL_TREE)
    return false;
  else if (TREE_CODE (overloads) == FUNCTION_DECL)
    {
      if (DECL_NAME (decl) != DECL_NAME (overloads))
        return false;
      if (!same_type_p (TREE_TYPE (decl), TREE_TYPE (overloads)))
        return false;

      return true;
    }
  else if (TREE_CODE (overloads) == OVERLOAD)
    {
      for (; overloads; overloads = OVL_NEXT (overloads))
        if (duplicate_decl_for_model (decl, OVL_CURRENT (overloads)))
          return true;
    }

  return false;
}

/* Given a DECL that was found in the given CONCEPT, return a
   declaration that specializes DECL for the given set of
   MODEL_ARGS. */
tree
build_decl_for_model(tree concept, tree model_args, tree decl)
{
  tree new_decl = decl;

  if (TREE_CODE (decl) == TYPE_DECL && DECL_ARTIFICIAL (decl))
    return decl;
  else if (TREE_CODE (decl) == TYPE_DECL
           && TREE_CODE (TREE_TYPE (decl)) == ASSOCIATED_TYPE)
    {
      tree context 
        = tsubst (TYPE_CONTEXT (TREE_TYPE (decl)), model_args,
                  tf_error, /*in_decl=*/NULL_TREE);

      /* The tsubst call may return a synthesized concept
	 map. However, we really want the dependent type for that
	 synthesized concept map.  */
      context = type_representative (context);

      new_decl = build_lang_decl (TYPE_DECL, DECL_NAME (decl), 
                                  /*type=*/NULL_TREE);
      
      TREE_TYPE (new_decl) 
        = build_associated_type (context, new_decl, /*type=*/NULL_TREE);
    }
  else if (TREE_CODE (decl) == FUNCTION_DECL)
    {
      tree model_id = lookup_template_class (concept, model_args, 
                                             /*in_decl=*/NULL_TREE,
                                             /*context=*/NULL_TREE,
                                             /*entering_scope=*/0,
                                             /*complain=*/tf_none);
      tree type = instantiate_operation_type (decl, model_id);
      new_decl = build_operation_decl (decl, type, model_id,
                                       CLASSTYPE_TI_ARGS (model_id));
      DECL_STATIC_FUNCTION_P (new_decl) = 0;
      DECL_TI_ARGS (new_decl) = model_args;
    }
  else if (TREE_CODE (decl) == OVERLOAD) 
    {
      tree overloads = NULL_TREE;

      for (; decl; decl = OVL_NEXT (decl))
        {
          new_decl = build_decl_for_model (concept, model_args, 
                                           OVL_CURRENT (decl));

          if (!duplicate_decl_for_model (new_decl, overloads))
            overloads = build_overload (new_decl, overloads);
        }
      
      new_decl = overloads;
    }
  else
    warning (0, "unable to build a concept_map decl for %qD", decl);
  return new_decl;
}

/* Produce an error that states that the associated function
   requirement SIG is not satisfied in the given MODEL_ID.  Note that
   SIG is a member of the concept associated with MODEL_ID, so the
   arguments of MODEL_ID have not yet been substituted into SIG.  */
static void
report_unsatisfied_requirement (tree sig, tree model_id)
{
  tree func;

  /* Copy the declaration SIG to FUNC, replacing and instantiating all
     of the bits we need to print the error message. */
  func = copy_decl (sig);
  TREE_TYPE (func) = instantiate_operation_type (sig, model_id);
  DECL_STATIC_FUNCTION_P (func) = 0;

  /* Tweak func to clean up the error message slightly. */
  if (TREE_CODE (TREE_TYPE (func)) == METHOD_TYPE)
    DECL_CONTEXT (func) = TYPE_METHOD_BASETYPE (TREE_TYPE (func));
  else
    DECL_CONTEXT (func) = NULL_TREE;

  error ("%Junsatisfied requirement %<%D%>", func, func);
  inform ("in %<%T%>", model_id);
}

/* Declare the operations required by the concept into the model
   (template), unless the user has specifically provided a definition.
   This must be done before the class type representing the model has
   been closed. Returns true unless something went wrong.

   REQS_CHECKED is a pointer set that marks which requirements were
   already checked while attempting to deduce associated
   types. These requirements do not need to be checked again.

   Note: Does not support pseudo-signature templates at this time,
   because we do not have well-defined rules for matching
   pseudo-signature templates.
 */
static bool 
declare_model_operations (tree model_id, struct pointer_set_t *reqs_checked)
{
  tree template;
  tree concept;
  tree req_func;
  tree args;
  tree func;
  bool okay = true;
  tree* slot = NULL;
  htab_t matched_operations;

  template = most_general_template (CLASSTYPE_TI_TEMPLATE (model_id));
  gcc_assert (TREE_CODE (template) == TEMPLATE_DECL);
  concept = TREE_TYPE (template);

  args = CLASSTYPE_TI_ARGS (model_id);

  /* The set of operations in the model that have been matched to a
     requirement. Any unmatched operations in the model result in an
     error. */
  matched_operations = htab_create (11, htab_hash_pointer, htab_eq_pointer,
                                    NULL);

  /* Check each required operation in the concept. */
  for (req_func = TYPE_METHODS (concept); req_func; 
       req_func = TREE_CHAIN (req_func))
    {
      if (pointer_set_contains (reqs_checked, req_func))
	/* Do nothing; we've already dealt with this requirement. */
	;
      else if (TREE_CODE (req_func) == FUNCTION_DECL
	       && DECL_OVERLOADED_OPERATOR_P (req_func) == NOP_EXPR)
        {
	  tree result_type
	    = tsubst (TREE_TYPE (TREE_TYPE (req_func)),
		      CLASSTYPE_TI_ARGS (model_id),
		      tf_error, model_id);

	  if (result_type == error_mark_node)
	    okay = false;
          /* Requirements for a copy-assignment operator are handled
             as a special case. Since copy-assignment operators cannot
             be remapped in the concept maps, we just check that the
             requirement is fulfilled. Eventually, all member
             functions will be handled in this way, per N2193.  */
          else if (build_implicit_copy_assignment_expr (req_func, model_id,
							result_type)
		   == error_mark_node)
	    {
	      /* DPG TBD: check return value. */
	      report_unsatisfied_requirement (req_func, model_id);
	      okay = false;
	    }
        }
      else if (TREE_CODE (req_func) == FUNCTION_DECL
	       && TREE_CODE (TREE_TYPE (req_func)) == METHOD_TYPE)
	{
	  tree result_type
	    = tsubst (TREE_TYPE (TREE_TYPE (req_func)),
		      CLASSTYPE_TI_ARGS (model_id),
		      tf_error, model_id);

	  if (result_type == error_mark_node)
	    okay = false;
	  /* Member function requirements do not result in
	     declarations within the concept map. Rather, we just
	     check the expression here and then ignore the
	     signature.  */
	  else if (build_implicit_method_call_expr (req_func, model_id,
						    result_type)
		   == error_mark_node)
	    {
	      /* DPG TBD: check return value. */
	      report_unsatisfied_requirement (req_func, model_id);
	      okay = false;
	    }
	}

      else if (TREE_CODE (req_func) == FUNCTION_DECL
               /* Ignore axioms */
               && !(TREE_TYPE (TREE_TYPE (req_func)) == axiom_type_node))
        {
          tree new_func_type = instantiate_operation_type (req_func, model_id);

          if (new_func_type == error_mark_node)
            return false;

          /* Check if an operation has already been provided. */
          for (func = TYPE_METHODS (model_id); func; func = TREE_CHAIN (func))
            {
              if (TREE_CODE (func) == FUNCTION_DECL
                  && DECL_NAME (func) == DECL_NAME (req_func)
                  && same_type_p (new_func_type, TREE_TYPE (func)))
                {
                  slot = (tree*)htab_find_slot (matched_operations, func, 
                                                INSERT);
                  *slot = func;
                  break;
                }
            }

          /* The operation already exists; don't declare it again.  */          
          if (func != NULL_TREE)
            continue;

          func = build_operation_decl (req_func, new_func_type, model_id,
                                       CLASSTYPE_TI_ARGS (model_id));

          if (func != error_mark_node)
            {
              /* The new function decl should NOT be a template at this
                 time, because we do not want it to be considered either a
                 specialization or an instantiation of the requirement in
                 the concept. Thus, we clear out all template
                 information. Appropriate template information will be
                 restored in push_template_decl. */
              DECL_TEMPLATE_INFO (func) = NULL_TREE;
              DECL_USE_TEMPLATE (func) = 0; 

              /* Mark the function as "inline". */
              DECL_DECLARED_INLINE_P (func) = 1;
              DECL_INLINE (func) = 1;
              DECL_ATTRIBUTES (func) = 
                tree_cons (get_identifier("always_inline"),
                           NULL_TREE,
                           DECL_ATTRIBUTES (func));

              /* Mark the context as "being defined". */
              /* TBD: useful? */
              TYPE_BEING_DEFINED (DECL_CONTEXT (func)) = 1;
              
	      /* If there was a default implementation, bring it along
		 without instantiating it.  */
	      if (DECL_SAVED_TREE (req_func))
		DECL_SAVED_TREE (func) = req_func;

              /* Declare the function in the model. */ 
              if (processing_template_decl)
                func = push_template_decl (func);
              finish_member_declaration (func);
              slot = (tree*)htab_find_slot (matched_operations, func, INSERT);
              *slot = func;
            }
          else
            return false;
        }
    }

  /* Verify that all of the model operations declared by the user
     have been used to satisfy requirements. */
  for (func = TYPE_METHODS (model_id); func; func = TREE_CHAIN (func))
    {
      if (TREE_CODE (func) == FUNCTION_DECL
	  && TREE_CODE (TREE_TYPE (func)) == METHOD_TYPE)
	error ("%Jconcept maps cannot override member function requirements",
	       func);
      else if (TREE_CODE (func) == FUNCTION_DECL 
          && !DECL_ARTIFICIAL (func)
          && !htab_find (matched_operations, func))
        {
          tree near_match = NULL_TREE;
          tree old_context = DECL_CONTEXT (func);
          int old_static = DECL_STATIC_FUNCTION_P (func);

          DECL_STATIC_FUNCTION_P (func) = 0;
	  DECL_CONTEXT (func) = NULL_TREE;
          error ("%Jconcept_map operation %<%D%> does not match any concept "
                 "requirement", func, func);
          DECL_STATIC_FUNCTION_P (func) = old_static;
          DECL_CONTEXT (func) = old_context;
          okay = false;
          
          /* Look for a near-match. */
          for (near_match = TYPE_METHODS (concept); near_match; 
               near_match = TREE_CHAIN (near_match))
            {
              if (TREE_CODE (near_match) == FUNCTION_DECL
                  && DECL_NAME (near_match) == DECL_NAME (func))
                {
                  tree near_match_type = 
                    instantiate_operation_type (near_match, model_id);
                  tree have_argtypes, need_argtypes;

                  /* Perform a fuzzy comparison of argument types. */
                  have_argtypes = TYPE_ARG_TYPES (TREE_TYPE (func));
                  need_argtypes = TYPE_ARG_TYPES (near_match_type);
                  while (have_argtypes && need_argtypes)
                    {
                      tree have_arg = TREE_VALUE (have_argtypes);
                      tree need_arg = TREE_VALUE (need_argtypes);

                      /* Perform a near-match, without references. */
                      have_arg = non_reference (have_arg);
                      need_arg = non_reference (need_arg);

                      if (!same_type_p (TYPE_MAIN_VARIANT (have_arg),
                                        TYPE_MAIN_VARIANT (need_arg)))
                        break;

                      have_argtypes = TREE_CHAIN (have_argtypes);
                      need_argtypes = TREE_CHAIN (need_argtypes);
                    }

                  if (have_argtypes == need_argtypes)
                    {
                      /* We have a near match; notify the user. */
                      tree near_match_decl = 
                        build_operation_decl (near_match, near_match_type, 
                                              model_id,
                                              CLASSTYPE_TI_ARGS (model_id));
                      
                      DECL_STATIC_FUNCTION_P (near_match_decl) = 0;
		      DECL_CONTEXT (near_match_decl) = NULL_TREE;
                      inform ("%Jnear match: %<%D%>", near_match, 
                              near_match_decl);
                    }
                }
            }
        }
    }

  return okay;
}

/* This subroutine builds the expression that corresponds to the given
   copy-assignment operator requirement FUNC, as needed by the concept
   map MODEL_ID. 

   RESULT_TYPE, if non-NULL, is the type that the result of the
   expression should be convertible to.

   Returns the underlying expression that will be
   generated, which may be ERROR_MARK_NODE, if there was an error.  */
tree
build_implicit_copy_assignment_expr (tree func, tree model_id, 
				     tree result_type)
{
  tree req_type = TREE_TYPE (func);
  tree arg1_type = TREE_VALUE (TYPE_ARG_TYPES (req_type));
  tree arg2_type = TREE_VALUE (TREE_CHAIN (TYPE_ARG_TYPES (req_type)));
  tree req_args = DECL_ARGUMENTS (func);
  tree arg1, arg2;
  tree expr;
  int flags = LOOKUP_NORMAL;

  if (flag_limit_concept_conversions)
    flags |= LOOKUP_NO_CONVERSION;

  /* DPG TBD: Working around the fact that the copy-assignment
     operator can be expressed either as a free function or a
     member (for now).  */
  if (TREE_CODE (req_type) == METHOD_TYPE)
    /* See through the "this" pointer.  */
    arg1_type = TREE_TYPE (arg1_type);
          
  /* Instantiate the argument types.  */
  arg1_type = tsubst (arg1_type, CLASSTYPE_TI_ARGS (model_id),
                      tf_none, NULL_TREE);
  if (arg1_type == error_mark_node)
    return false;
  arg2_type = tsubst (arg2_type, CLASSTYPE_TI_ARGS (model_id),
                      tf_none, NULL_TREE);
  if (arg2_type == error_mark_node)
    return false;

  /* Build dummy arguments.  */
  arg1 = cp_build_parm_decl (DECL_NAME (req_args), arg1_type);
  arg2 = cp_build_parm_decl (DECL_NAME (TREE_CHAIN (req_args)), 
                             arg2_type);
          
  /* Prepare the arguments to be passed along to
     copy-assignment operation. */
  arg1 = forward_parameter (arg1);
  arg2 = forward_parameter (arg2);

  /* Try to build the expression arg1 = arg2 */
  expr = build_x_modify_expr (arg1, NOP_EXPR, arg2, flags);

  if (expr != error_mark_node && result_type)
    {
      /* Check that EXPR is convertible to RESULT_TYPE.  */
      tree retval;
      /* The type the function is declared to return.  */
      int flags = LOOKUP_NORMAL | LOOKUP_ONLYCONVERTING;

      /* If the result_type is void, there is nothing else we need to
	 do.  */
      if (VOID_TYPE_P (result_type))
	return expr;

      if (type_dependent_expression_p (expr)
          || dependent_type_p (result_type))
        return expr;
      else
        {
          /* Try to convert the expression to the type of the result.  */
          retval = convert_for_initialization
            (NULL_TREE, result_type, expr, flags, "return", NULL_TREE, 0);
        }
      
      if (retval == error_mark_node)
	return error_mark_node;
    }

  return expr;
}

/* This subroutine builds the implicitly-generated expression that
   corresponds to a given member function requirement FUNC, as needed
   by the concept map MODEL_ID. 

   RESULT_TYPE, if non-NULL, is the type that the result of the
   expression should be convertible to.

   Returns the underlying expression that
   will be generated, or ERROR_MARK_NODE if there was an error.  */
tree
build_implicit_method_call_expr (tree func, tree model_id, tree result_type)
{
  tree func_arguments = DECL_ARGUMENTS (func);
  tree arg, args = NULL_TREE;
  tree expr = NULL_TREE;
  int flags = LOOKUP_NORMAL;

  if (flag_limit_concept_conversions)
    flags |= LOOKUP_NO_CONVERSION;

  /* Build the list of arguments to the function, excluding the
     implicit "this".  */
  func_arguments = tsubst (TREE_CHAIN (func_arguments),
			   CLASSTYPE_TI_ARGS (model_id),
			   tf_warning_or_error, model_id);
  if (func_arguments == error_mark_node)
    return error_mark_node;

  /* Forward each of the arguments on to whatever function we
     call.  */
  for (arg = func_arguments; arg; arg = TREE_CHAIN (arg))
    args = tree_cons (NULL_TREE, forward_parameter (arg), args);

  args = nreverse (args);

  /* New and array new operators. */
  if (DECL_OVERLOADED_OPERATOR_P (func)
      && (GET_OVERLOADED_OPERATOR_CODE (func) == NEW_EXPR
	  || GET_OVERLOADED_OPERATOR_CODE (func) == VEC_NEW_EXPR))
    {
      /* Much here is borrowed from build_new_1 in init.c */
      bool array_p = (GET_OVERLOADED_OPERATOR_CODE (func) == VEC_NEW_EXPR);
      tree fnname;
      tree elt_type;
          
      /* Determine the element type */
      elt_type = tsubst (TYPE_METHOD_BASETYPE (TREE_TYPE (func)),
			 CLASSTYPE_TI_ARGS (model_id),
			 tf_error, model_id);
      elt_type = strip_array_types (elt_type);
          
      fnname = ansi_opname (GET_OVERLOADED_OPERATOR_CODE (func));
          
      if (CLASS_TYPE_P (elt_type)
	  && (array_p
	      ? TYPE_HAS_ARRAY_NEW_OPERATOR (elt_type)
	      : TYPE_HAS_NEW_OPERATOR (elt_type)))
	{
	  /* Use a class-specific operator new.  */
	  tree fns;
	  /* Do name-lookup to find the appropriate operator.  */
	  fns = lookup_fnfields (elt_type, fnname, /*protect=*/2);
	  if (TREE_CODE (fns) == TREE_LIST)
	    {
	      error ("request for member %qD is ambiguous", fnname);
	      print_candidates (fns);
	      return error_mark_node;
	    }
	  else
	    {
	      tree dummy;
	      dummy = build_dummy_object (elt_type);
	      expr = build_new_method_call (dummy, fns, args,
					    /*conversion_path=*/NULL_TREE,
					    LOOKUP_NORMAL,
					    /*fn_p=*/NULL);
	    }
	}
      else
	{
	  tree size; 
	  tree cookie_size = NULL_TREE;
	  tree placement = args;
	  size = TREE_VALUE (placement);
	  placement = TREE_CHAIN (placement);
	  expr = build_operator_new_call (fnname, placement, 
					  &size, &cookie_size,
					  /*fn=*/NULL);
	}
    }
  else if (DECL_OVERLOADED_OPERATOR_P (func)
	   && (GET_OVERLOADED_OPERATOR_CODE (func) == DELETE_EXPR
	       || GET_OVERLOADED_OPERATOR_CODE (func) == VEC_DELETE_EXPR))
    {
      tree size = NULL_TREE; 
      tree placement = args;
      tree addr = TREE_VALUE (placement);
      tree elt_type;
      tree alloc_fn = NULL_TREE;

      /* Determine the element type */
      elt_type = tsubst (TYPE_METHOD_BASETYPE (TREE_TYPE (func)),
			 CLASSTYPE_TI_ARGS (model_id),
			 tf_error, model_id);
      elt_type = strip_array_types (elt_type);

      /* Cast the address back to actual type being
	 deleted. */
      addr = build_static_cast (build_pointer_type (elt_type), addr);

      placement = TREE_CHAIN (placement);

      if (placement)
        {
        alloc_fn = func;
        }

      expr = build_op_delete_call (GET_OVERLOADED_OPERATOR_CODE (func), 
				   addr, size, /*global_p=*/false,
				   placement, alloc_fn);
    }
  /* Constructors and destructors.  */
  else if (DECL_CONSTRUCTOR_P (func) || DECL_DESTRUCTOR_P (func))
    {
      tree instance;
      tree ctype;
      tree instance_type;
      tree this_arg_chain = TREE_CHAIN (DECL_ARGUMENTS (func));
      tree func_context = DECL_CONTEXT (func);

      /* Determine the type that the "this" argument points to.  */
      instance_type = tsubst (TREE_TYPE (TREE_TYPE (DECL_ARGUMENTS (func))),
			      CLASSTYPE_TI_ARGS (model_id),
			      tf_warning_or_error, model_id);

      /* If "this" will point to a reference type or void type, and
	 the function is either a copy constructor or a destructor,
	 just silently accept the copy-constructor or destruction of
	 the reference.  */
      DECL_CONTEXT (func) = TYPE_METHOD_BASETYPE (TREE_TYPE (func));
      if ((TREE_CODE (instance_type) == REFERENCE_TYPE
	   || VOID_TYPE_P (instance_type))
	  && (DECL_DESTRUCTOR_P (func) || copy_fn_p (func) > 0))
	{
	  DECL_CONTEXT (func) = func_context;
	  return args? TREE_VALUE (args) : void_zero_node;
	}

      /* Restore FUNC's context, which we munged to make copy_fn_p
	 work.  */
      DECL_CONTEXT (func) = func_context;

      /* Forward the "this" argument.  */
      TREE_CHAIN (DECL_ARGUMENTS (func)) = NULL_TREE;
      instance = tsubst (DECL_ARGUMENTS (func),
			 CLASSTYPE_TI_ARGS (model_id),
			 tf_warning_or_error, model_id);
      TREE_CHAIN (DECL_ARGUMENTS (func)) = this_arg_chain;
      instance = build_indirect_ref (instance, "unary *");

      ctype = tsubst (TYPE_METHOD_BASETYPE (TREE_TYPE (func)),
		      CLASSTYPE_TI_ARGS (model_id),
		      tf_error, model_id);
      ctype = complete_type (ctype);

      if (!IS_AGGR_TYPE (ctype))
        {
          if (DECL_DESTRUCTOR_P (func))
            {
              gcc_assert (TYPE_HAS_TRIVIAL_DESTRUCTOR (ctype));
              expr = void_zero_node;
            }
          else if (!args) 
            {
              /* Default constructor */
              tree init = build_default_init (ctype, NULL_TREE);

              if (TYPE_NEEDS_CONSTRUCTING (ctype))
                expr = build_special_member_call (instance, 
                                                  complete_ctor_identifier,
                                                  init, TYPE_BINFO (ctype),
                                                  LOOKUP_NORMAL);
              else
                expr = build_x_modify_expr (instance, INIT_EXPR, init, 
                                            flags);
            } 
          else 
            {
              if (TYPE_NEEDS_CONSTRUCTING (ctype))
                expr = build_special_member_call (instance,
                                                  complete_ctor_identifier,
                                                  args, TYPE_BINFO (ctype),
                                                  LOOKUP_NORMAL);
              else 
                {
                  tree init = args;
                  if (TREE_CODE (init) == TREE_LIST)
                    init = 
                      build_x_compound_expr_from_list (init,
                                                       "constructor initializer");
                  
                  expr = build_modify_expr (instance, INIT_EXPR, init);
                }
            }
        }
      else if (DECL_DESTRUCTOR_P (func))
        expr = build_delete(ctype, instance, sfk_complete_destructor,
                            LOOKUP_NORMAL, /*use_global_delete=*/0);
      else {
        if (processing_template_decl)
          {
            /* Cannot build constructor calls for dependent
               types, so build a CAST_EXPR. */
            expr = build_min (CAST_EXPR, ctype, args);
            /* We don't know if it will or will not have side
               effects.  */
            TREE_SIDE_EFFECTS (expr) = 1;
          }
        else
          expr = build_special_member_call (instance, 
                                            complete_ctor_identifier,
                                            args,
                                            TYPE_BINFO (ctype),
                                            LOOKUP_NORMAL);
      }
    }
  else if (TREE_CODE (TREE_TYPE (func)) == METHOD_TYPE)
    {
      tree fns;
      tree instance;
      tree this_arg_chain = TREE_CHAIN (DECL_ARGUMENTS (func));

      /* Forward the "this" argument.  */
      TREE_CHAIN (DECL_ARGUMENTS (func)) = NULL_TREE;
      instance = tsubst (DECL_ARGUMENTS (func),
			 CLASSTYPE_TI_ARGS (model_id),
			 tf_warning_or_error, model_id);
      TREE_CHAIN (DECL_ARGUMENTS (func)) = this_arg_chain;

      instance = build_indirect_ref (instance, "unary *");

      if (type_dependent_expression_p (instance))
        {
          /* Cannot lookup the member within a dependent type,
             so build a call expression directly. */
          tree member;

          member = finish_class_member_access_expr (instance,
                                                    DECL_NAME (func),
                                                    /*template_p=*/false);

          expr = build_nt_call_list (member, args);
          if (!expr || expr == error_mark_node)
            return error_mark_node;
        }
      else
        {
          fns = lookup_member (TREE_TYPE (instance),
                               DECL_NAME (func),
                               /*protect=*/1,
                               /*want_type=*/false);

          if (!fns || fns == error_mark_node)
            return error_mark_node;
          else
            {
              expr = build_new_method_call (instance, fns, args,
                                            NULL_TREE, LOOKUP_NORMAL, 
                                            /*fn_p=*/NULL);
              if (!expr || expr == error_mark_node)
                return error_mark_node;
            }
        }
    }
  else
    gcc_unreachable ();

  if (expr != error_mark_node 
      && expr
      && result_type
      && !type_dependent_expression_p (expr))
    {
      /* Check that EXPR is convertible to RESULT_TYPE.  */
      tree retval;
      /* The type the function is declared to return.  */
      int flags = LOOKUP_NORMAL | LOOKUP_ONLYCONVERTING;

      /* If the result_type is void, there is nothing else we need to
	 do.  */
      if (VOID_TYPE_P (result_type))
	return expr;

      /* First convert the value to the function's return type, then
	 to the type of return value's location to handle the
	 case that functype is smaller than the valtype.  */
      retval = convert_for_initialization
	(NULL_TREE, result_type, expr, flags, "return", NULL_TREE, 0);
      retval = convert (TREE_TYPE (expr), retval);
      
      if (retval == error_mark_node)
	return error_mark_node;
    }

  return expr;
}

/* This subroutine builds the expression that goes into the body of an
   implicitly-generated model operation. For instance, consider the
   following concept and model:

     concept Addable<typename T> {
       T operator+(const T& x, const T& y);
     }

     concept_map Addable<int> {
       // operator+ below is implicitly generated by declare_model_operations
       int operator+(const int& x, const int& y); 
     }

     // operator+ definition implicitly generated by define_model_operations
     int Addable<int>::operator+(const int& x, const int& y) {
       return x + y; // x + y generated by build_implicit_model_operation_expr
     }

   FUNC is the declaration of the model operation for which we are
   building the expression. Returns the expression to be returned from
   the body of the FUNC, or ERROR_MARK_NODE if there was a problem. 

   CANDIDATE_SET is a pointer to a tree that will receive a
   FUNCTION_DECL or OVERLOAD node containing the set of functions that
   should be considered when bypassing the forwarding function (under
   -fabstract-signatures mode). If NULL, no candidate set will be
   computed.  */
static tree
build_implicit_model_operation_expr (tree func, tree *candidate_set)
{
  tree arg, args = NULL_TREE;
  tree expr = NULL_TREE;
  int flags = 0;

  if (flag_limit_concept_conversions)
    flags |= LOOKUP_NO_CONVERSION;
  if (candidate_set)
    flags |= LOOKUP_RETURN_CANDIDATE_SET;

  /* Build the list of arguments to the function. */
  for (arg = DECL_ARGUMENTS (func); arg; arg = TREE_CHAIN (arg))
    args = tree_cons (NULL_TREE, forward_parameter (arg), args);

  args = nreverse (args);

  if (DECL_OVERLOADED_OPERATOR_P (func))
    /* Call overloaded operators "as if" the user had written
       the operator expression, e.g., "a + b" or "new
       int[3]". */
    {
      int nargs = list_length (DECL_ARGUMENTS (func));
      enum tree_code code = GET_OVERLOADED_OPERATOR_CODE (func) ;
      
      if (code == CALL_EXPR)
        {
          expr = finish_call_expr (TREE_VALUE (args), TREE_CHAIN (args),
                                   /*disallow_virtual=*/false,
                                   /*koenig_p=*/true, flags);
        }
      else if (nargs == 1) 
        {
          if (DECL_CONV_FN_P (func))
            {
              /* The return statement will perform the
                 necessary implicit conversion. */
              expr = TREE_VALUE (args);
            }
          else
            switch (code) {
            case COMPONENT_REF:
              expr = build_x_arrow (TREE_VALUE (args));
              if (expr 
                  && (TREE_CODE (expr) == ARROW_EXPR
                      || TREE_CODE (expr) == INDIRECT_REF))
                expr = TREE_OPERAND (expr, 0);
              break;
            default:
              expr = finish_unary_op_expr (code, TREE_VALUE (args));
            }
        }
      else if (nargs == 2)
        {
          bool overloaded_p;
          tree arg1 = TREE_VALUE (args);
          tree arg2 = TREE_VALUE (TREE_CHAIN (args));
          bool assign_p = DECL_ASSIGNMENT_OPERATOR_P (func);

          /* If this operator is expressed as a method, dereference
             the implicit "this" argument.  */
          if (TREE_CODE (TREE_TYPE (func)) == METHOD_TYPE)
            arg1 = build_indirect_ref (arg1, "unary *");

          switch (code) {
          case COMPOUND_EXPR:
            expr = build_x_compound_expr (arg1, arg2);
            break;
                    
          case ARRAY_REF:
            expr = grok_array_decl (arg1, arg2);
            break;

          default:
            expr = assign_p? build_x_modify_expr (arg1, code, arg2, flags)
              : build_x_binary_op (code, arg1, TREE_CODE (arg1),
                                   arg2, TREE_CODE (arg2), &overloaded_p,
                                   flags);
          }
        }
      else
        gcc_unreachable ();

      if (!expr || expr == error_mark_node)
        return error_mark_node;
    }
  else 
    {
      tree fn;
      /* Find the functions that might be called. */
      fn = lookup_function_nonclass(DECL_NAME (func), args,
                                    /*block_p=*/1);
      if (fn && fn != error_mark_node)
        {
          /* Build the function body, which just performs an
             unqualified call to a routine of the same name. */
          expr = finish_call_expr (fn, args,
                                   /*disallow_virtual=*/false,
                                   /*koenig_p=*/true,
                                   flags);
        }
      else
        return error_mark_node;
    }

  if (expr && TREE_CODE (expr) == TREE_LIST)
    {
      /* We have a candidate set, so extract it for the caller.  */
      *candidate_set = TREE_PURPOSE (expr);
      expr = TREE_VALUE (expr);
    }

  if (!expr)
    return error_mark_node;
  else
    return expr;
}

/* Defines bodies for the operations declared by
   declare_model_operations. However, unlike declare_model_operations,
   this routine may only be invoked *after* the class type
   corresponding to the model has been closed. This corresponds with
   the handling of class members written inline, which are parsed
   after the definition is closed. Returns true unless something goes
   wrong. 

   DECLARED_MODEL is the model that was actually declared by the
   user. When different from MODEL_ID, MODEL_ID is being defined while
   all of the superiors of DECLARED_MODEL are being defined. Thus, it should 
   define all of its operations in terms of DECLARED_MODEL.

   Note: Eventually, this routine will need to handle default
   implementations of operations (provided in the concept). However,
   doing so now is pointless, because models aren't actually used to
   perform name lookup.
  */
bool 
define_model_operations (tree model_id, tree declared_model)
{
  tree func;
  bool found_error = false;
  tree candidate_set;

  /* Check each required operation in the concept. */
  for (func = TYPE_METHODS (model_id); func; func = TREE_CHAIN (func))
    {
      if (TREE_CODE (func) == FUNCTION_DECL 
          /* Don't synthesize operations for the implicitly declared
             constructors and destructors in the concept map
             itself.  */
          && DECL_NAME (func) != complete_ctor_identifier
          && DECL_NAME (func) != base_ctor_identifier
          && DECL_NAME (func) != complete_dtor_identifier
          && DECL_NAME (func) != base_dtor_identifier
          && DECL_NAME (func) != deleting_dtor_identifier
          && DECL_ARTIFICIAL (func))
        {
          tree expr = NULL_TREE;
          tree args = NULL_TREE, arg;
          tree stmt, compount_stmt;
	  tree default_impl = DECL_SAVED_TREE (func);
          int destructor_p;

	  /* Clear out the candidate set.  */
	  candidate_set = NULL_TREE;

	  /* Clear out the function body; we'll be building a new one.  */
	  DECL_SAVED_TREE (func) = NULL_TREE;

          /* If we've run into any functions that are actually
             operations on the concept itself (constructors,
             assignment operators, etc.), ignore them. */
          if (TREE_CODE (TREE_TYPE (func)) == METHOD_TYPE)
	    continue;

          DECL_ARTIFICIAL (func) = 0;
          DECL_INLINE (func) = 1;
          DECL_ATTRIBUTES (func) = 
            tree_cons (get_identifier("always_inline"),
                       NULL_TREE,
                       DECL_ATTRIBUTES (func));
          
          push_deferring_access_checks (dk_no_deferred);
          start_preparsed_function (func, NULL_TREE, 
                                    SF_PRE_PARSED | SF_INCLASS_INLINE);
          
          /* begin_function_body does dangerous things with
             destructors, so make this look like a normal function. */
          destructor_p = DECL_DESTRUCTOR_P (func);
          if (destructor_p)
            DECL_DESTRUCTOR_P (func) = 0;

          stmt = begin_function_body ();

          compount_stmt = begin_compound_stmt (0);

	  /* We want to suppress diagnostics when trying to match an
	     operation that has a default implementation, because we
	     can then fall back to the default implementation.  */
	  if (default_impl)
	    diagnostic_push_suppress (global_dc);

          /* Restore the destructor flag, if needed. */
          if (destructor_p)
            DECL_DESTRUCTOR_P (func) = 1;
         
          if (model_id != declared_model
	      /* We don't handle remapping of constructors */
              && !DECL_CONSTRUCTOR_P (func)
	      /* Or destructors */
              && !DECL_DESTRUCTOR_P (func)
	      /* Or conversion functions */
              && !DECL_CONV_FN_P (func)
	      /* Or methods of any kind! */
	      && TREE_CODE (TREE_TYPE (func)) != METHOD_TYPE)
            {
              /* We are actually building an implementation for an
                 operation within a model that was implicitly declared
                 by the user as the superior of a different
                 model. Just build a call to the same function in that
                 model. */
              tree target;

              /* Build the list of arguments to the function. */
              for (arg = DECL_ARGUMENTS (func); arg; arg = TREE_CHAIN (arg))
                {
                  args = tree_cons(NULL_TREE, 
                                   convert_from_reference (arg), 
                                   args);
                }
              args = nreverse (args);

              if (processing_template_decl
                  && !template_processing_nondependent_p ())
                target = build_min_nt (SCOPE_REF, declared_model,
                                       DECL_NAME (func), NULL_TREE);
              else
                target = lookup_member (declared_model, DECL_NAME (func),
                                        /*protect=*/0, /*want_type=*/0);
              gcc_assert (target != NULL_TREE);
              expr = finish_call_expr (target, args, 
                                       /*disallow_virtual=*/false, 
                                       /*koenig_p=*/false,
                                       /*extra_flags=*/0);
            }
          else
            expr = build_implicit_model_operation_expr (func, &candidate_set);

          if (expr && expr != error_mark_node)
            {
              /* Save the candidate set for this implictly-generated
                 forwarding function, if any.  */
              FUNCTION_SAVED_CANDIDATE_SET (func) = candidate_set;

              if (TREE_TYPE (TREE_TYPE (func)) != void_type_node)
                finish_return_stmt (expr);
              else
                finish_expr_stmt (expr);
            }
          else
            {
	      if (!default_impl)
		{
		  /* Tweak func to clean up the error message slightly. */
		  if (DECL_CONSTRUCTOR_P (func) || DECL_DESTRUCTOR_P (func))
		    DECL_CONTEXT (func) = TREE_TYPE (TREE_VALUE (TYPE_ARG_TYPES (TREE_TYPE (func))));
		  else
		    DECL_CONTEXT (func) = NULL_TREE;
		  DECL_STATIC_FUNCTION_P (func) = 0;

		  error ("unsatisfied requirement %<%D%>", func);
		  inform ("in %<%T%>", model_id);
		}

	      found_error = true;
            }

	  /* If there is a default implementation and there were
	     errors when trying to match the expression itself,
	     instantiate the default implementation.  */          
	  if (default_impl 
	      && (diagnostic_pop_suppress (global_dc) || found_error))
	    {
	      extern htab_t local_specializations;
	      extern hashval_t hash_local_specialization (const void*);
	      extern int eq_local_specializations (const void *, const void *);
	      htab_t saved_local_specializations;
	      tree default_body = DECL_SAVED_TREE (default_impl);
	      tree tmpl_parm;
	      tree spec_parm;

	      /* Save away the current list, in case we are
		 instantiating one template from within the body of
		 another.  */
	      saved_local_specializations = local_specializations;

	      /* Set up the list of local specializations.  */
	      local_specializations = htab_create (37,
						   hash_local_specialization,
						   eq_local_specializations,
						   NULL);
	      
	      /* Dig out the actual statement list or block inside the
		 default implementation.  */
	      if (TREE_CODE (default_body) == BIND_EXPR)
		default_body = BIND_EXPR_BODY (default_body);
	      if (TREE_CODE (default_body) == BIND_EXPR)
		default_body = BIND_EXPR_BODY (default_body);

	      /* Create substitution entries for the parameters.  Note
		 that this duplicates some work from instantiate_decl,
		 which we will need to factor out.  */
	      tmpl_parm = DECL_ARGUMENTS (default_impl);
	      spec_parm = DECL_ARGUMENTS (func);
	      while (tmpl_parm)
		{
		  register_local_specialization (spec_parm, tmpl_parm);
		  tmpl_parm = TREE_CHAIN (tmpl_parm);
		  spec_parm = TREE_CHAIN (spec_parm);
		}

	      /* Instantiate the default implementation.  */
	      tsubst_expr (default_body, CLASSTYPE_TI_ARGS (model_id), 
			   tf_warning_or_error, NULL_TREE,
			   /*integral_constant_expression_p=*/false);

	      /* We don't need the local specializations any more.  */
	      htab_delete (local_specializations);
	      local_specializations = saved_local_specializations;

	      /* Suppress the "found_error" flag.  */
	      found_error = false;
	    }

	  /* Finish the function body.  */
          finish_compound_stmt (compount_stmt);
          finish_function_body (stmt);

	  /* Generate code for the function, if necessary.  */
	  expand_or_defer_fn (finish_function (/*inclass_inline*/2));

          pop_deferring_access_checks ();
        }
    }

  return !found_error;
}
      
/* Determine if there is a model for the given model_id. If complain
   is true, we will complain if the model is ambiguous. For a
   structural model or when complain is true, this routine may attempt
   to instantiate a model template or a concept. 

   STRUCTURAL can have several values:
     0 means that structural checks should NOT be performed; we're
     looking for an explicit model (template) or an existing
     instantiation.

     1 means that we should perform structural checks for structural
     concepts, if no other models are found. This is the normal
     checking mode.

     2 means that we should perform structural checks for structural
     or explicit concepts, if no other models are found. This is
     typically only used for error reporting, where we want to tell
     the user that they have a structural match to an explicit
     concept, but no model declaration.
*/
bool
has_model (tree model_id, bool complain, int structural)
{
  tree template, concept, args, t;

  gcc_assert (TREE_CODE (model_id) == RECORD_TYPE);

  /* We've already marked this as not being a model. */
  if (CLASSTYPE_NOT_MODEL_P (model_id) && structural != 2)
    return false;

  /* We've already marked this as a model. */
  if (CLASSTYPE_MODEL_P (model_id)) 
    return true;

  if (CLASSTYPE_MODEL_P (type_archetype (model_id)))
    return true;

  timevar_push (TV_CONCEPT_MAP);
  
  /* If the model-id uses template parameters, then our lookup of
     models actually checks the (expanded) where clause. */
  if (processing_template_decl && uses_template_parms (model_id))
    {
      tree where_clause 
	= current_where_clause? TREE_VALUE (current_where_clause)
	: NULL_TREE;

      tree req = build_concept_requirement (model_id, /*not_req=*/false);

      /* Add to the list of deductions to reverse after this template
         has been parsed. */
      note_deduced_model (model_id);
          
      if (requirement_in_where_clause_p (req, where_clause))
        {
	  /* DPG TBD: I'm not really happy about using ck_concept_map
	     here, because this is very much like a synthetic concept
	     map.  */
          CLASSTYPE_USE_CONCEPT (model_id) = ck_concept_map;
          POP_TIMEVAR_AND_RETURN (TV_CONCEPT_MAP, true);
        }
    }

  /* Make sure any recursive checks return false. */
  CLASSTYPE_USE_CONCEPT (model_id) = ck_no_concept_map;

  /* Figure out which concept is being instantiated.  */
  template = most_general_template (CLASSTYPE_TI_TEMPLATE (model_id));
  gcc_assert (TREE_CODE (template) == TEMPLATE_DECL);

  /* Figure out which arguments are being used to do the
     instantiation.  */
  args = CLASSTYPE_TI_ARGS (model_id);

  /* Determine what specialization of the original template to
     instantiate.  */
  t = most_specialized_class (model_id, template);
  if (t == error_mark_node)
    {
      /* The result is ambiguous, so let instantiate_class_template
         dump error messages for us. */
      if (complain)
        instantiate_class_template (model_id);
      POP_TIMEVAR_AND_RETURN (TV_CONCEPT_MAP, false);
    }

  /* If t is non-null, then we found a model template from which this
     model-id would be generated. Pick up the USE_CONCEPT from that
     model, which may state that this model-id is or is not a
     model. Then, return whether it is a model or not. */
  if (t != NULL_TREE)
    {
      while (t && TREE_CODE (t) == TREE_LIST)
        t = TREE_TYPE (t);

      /* DPG TBD: This is a hack to work around a problem where
         CLASSTYPE_USE_CONCEPT gets overwritten with ck_concept.
         Moving to purely synthesized concept maps would fix this
         issue.  */
      if (CLASSTYPE_USE_CONCEPT (t) == ck_concept)
        CLASSTYPE_USE_CONCEPT (model_id) = ck_concept_map;
      else
        CLASSTYPE_USE_CONCEPT (model_id) = CLASSTYPE_USE_CONCEPT (t);
      POP_TIMEVAR_AND_RETURN (TV_CONCEPT_MAP, CLASSTYPE_MODEL_P (model_id));
    }

  /* If the concept is structural, perform a structural check. */
  concept = TREE_TYPE (template);
  if ((structural == 2
       || (CLASSTYPE_STRUCTURAL_CONCEPT_P (concept) && structural != 0))
      && !uses_template_parms (model_id))
    {
      /* Simulate the definition of a model and see if it
         type-checks. */
      tree complete_model;

      /* We may be in the middle of deferred access check.  Disable it
         now.  */
      push_deferring_access_checks (dk_no_deferred);     
      push_to_top_level ();
      if (TYPE_CONTEXT (concept))
        push_nested_namespace (TYPE_CONTEXT (concept));

      /* Suppress diagnostics. */
      diagnostic_push_suppress (global_dc);

      complete_model = begin_model_definition (model_id);
      complete_model = finish_model_definition (complete_model,
                                                /*implicit_model=*/true);

      if (diagnostic_kind_count (global_dc, DK_ERROR) > 0)
        complete_model = error_mark_node;

      if (complete_model != error_mark_node
          && !define_model_operations (complete_model, complete_model))
        complete_model = error_mark_node;

      /* Allow diagnostics again and see if any errors were emitted. */
      if (diagnostic_pop_suppress (global_dc))
        complete_model = error_mark_node;

      if (TYPE_CONTEXT (concept))
        pop_nested_namespace (TYPE_CONTEXT (concept));
      pop_from_top_level ();
      pop_deferring_access_checks ();

      /* If we were checking for structural conformance with an
         explicit concept, don't save any results either way. */
      if (structural == 2 && !CLASSTYPE_STRUCTURAL_CONCEPT_P (concept))
        {
          CLASSTYPE_USE_CONCEPT (model_id) = 
            CLASSTYPE_USE_CONCEPT (CLASSTYPE_PRIMARY_TEMPLATE_TYPE (model_id));
          POP_TIMEVAR_AND_RETURN (TV_CONCEPT_MAP, 
				  complete_model != error_mark_node);
        }

      if (complete_model != error_mark_node)
	/* DPG TBD: Perhaps this should be ck_implicit_concept_map,
	   since it was implicitly defined?  */
        CLASSTYPE_USE_CONCEPT (complete_model) = ck_concept_map;
      else {
        CLASSTYPE_USE_CONCEPT (model_id) = ck_no_concept_map;
        complete_model = model_id;
      }
      POP_TIMEVAR_AND_RETURN (TV_CONCEPT_MAP, 
			      CLASSTYPE_MODEL_P (complete_model));
    }

  POP_TIMEVAR_AND_RETURN (TV_CONCEPT_MAP, false);
}

/* Define the model (templates) for each of the superiors of the
   concept. Each of the model templates will have the same template
   parameters, template arguments, and where clause as
   MODEL_ID. 

   IMPLICIT_MODEL is true if this model is being generated
   implicitly. In this case, we will only generate models for
   structural concepts, and will require that models exist for nominal
   concepts.

   Returns true unless something goes wrong. */
static bool 
define_models_for_superiors (tree model_id, bool implicit_model)
{
  tree template, concept;
  bool failed = false;
  bool template_p = uses_template_parms (model_id);

  /* Declare models for each of the superiors of the concept. The
     models will be parameterized in the same way as this model, and
     will have the same associated type and (eventually) function
     requirement definitions. This must be done before the model
     operations are processed, because model operations may refer to
     refined concepts. */
  template = most_general_template (CLASSTYPE_TI_TEMPLATE (model_id));
  gcc_assert (TREE_CODE (template) == TEMPLATE_DECL);
      
  concept = TREE_TYPE (template);

  if (TYPE_BINFO (concept))
    {
      tree binfo, base_binfo;
      int i;
      
      /* If this is not a template, then we've been called before the
         definition of model_id is complete. So, act like this is an
         instantiation by pushing into the appropriate namespace context
         of the concept. */
      if (!template_p)
        {
          push_deferring_access_checks (dk_no_deferred);     
          push_to_top_level ();
          if (TYPE_CONTEXT (concept))
            push_nested_namespace (TYPE_CONTEXT (concept));
        }
      
      /* Process superiors.  */
      for (binfo = TYPE_BINFO (concept), i = 0;
           BINFO_BASE_ITERATE (binfo, i, base_binfo) && !failed; i++)
        failed = !maybe_define_model_for_superior (model_id, 
                                                   BINFO_TYPE (base_binfo),
                                                   implicit_model);

      /* Back out of the namespace context of the concept, if we
         entered it. */
      if (!template_p)
        {
          if (TYPE_CONTEXT (concept))
            pop_nested_namespace (TYPE_CONTEXT (concept));
          pop_from_top_level ();
          pop_deferring_access_checks ();
        }
    }

  return !failed;
}

/* Might define the model for the superior to a concept given the
   MODEL_ID of that concept (or a concept that refines it), along with
   its superiors (recursively). 

   IMPLICIT_MODEL is the same as in define_models_for_superiors.

   Returns true unless something goes wrong. */
static bool 
maybe_define_model_for_superior (tree model_id, tree superior, 
                                 bool implicit_model)
{
  /* Name the model-id for the superior we are defining. */
  tree field;
  tree args = CLASSTYPE_TI_ARGS (model_id);
  tree template;
  tree superior_concept;
  tree superior_model;

  args = tsubst (TYPE_TI_ARGS (superior), args, tf_error, NULL_TREE);

  template = CLASSTYPE_TI_TEMPLATE (superior);
  template = most_general_template (template);
  superior_concept = TREE_TYPE (template);

  superior_model = 
    lookup_template_class_real (superior_concept,
                                args,
                                CLASSTYPE_WHERE_CLAUSE (model_id),
                                /*in_decl=*/NULL_TREE,
                                /*context=*/NULL_TREE,
                                /*entering_scope=*/0,
                                tf_error);

  if (superior_model == error_mark_node)
    return false;

  /* If we already have a model declaration for the superior model-id,
     we're done. */
  if (CLASSTYPE_MODEL_P (superior_model))
    return true;

  if (implicit_model && !CLASSTYPE_STRUCTURAL_CONCEPT_P (superior))
    /* If we have a nominal (non-structural) superior concept but our
       model was implicitly generated by a request for a model of a
       structural concept, then we do not generate models for the
       superior because that model must already exist. */
    return (uses_template_parms (superior_model)
            || has_model (superior_model, /*complain=*/true,
                          /*structural=*/0));

  /* Pick up defaults. */
  args = CLASSTYPE_TI_ARGS (superior_model);

  /* Determine if all of the template parameters can be deduced from
     the arguments for the superior model. If not, the user needs to
     declare the superior model explicitly. */
  if (uses_template_parms (args))
    {
      tree parms = INNERMOST_TEMPLATE_PARMS (current_template_parms);
      int nparms = TREE_VEC_LENGTH (parms);
      int idx;
      bool complained = false;
      tree deduced_parms;
      bool has_assoc_type = false;

      /* Keep track of whether the template parameters for MODEL_ID are
         deduced in the SUPERIOR_MODEL model-id. */
      deduced_parms = make_tree_vec (nparms);
      for (idx = 0; idx < nparms; ++idx)
        TREE_VEC_ELT (deduced_parms, idx) = NULL_TREE;
    
      /* Mark all of the deduced parameters. */
      mark_deduced_parms (args, deduced_parms, &has_assoc_type);

      /* Complain about any parameters that cannot be deduced. */
      for (idx = 0; idx < nparms; ++idx)
        {
          if (TREE_VEC_ELT (deduced_parms, idx) != boolean_true_node)
            {
              tree parm = TREE_VALUE (TREE_VEC_ELT (parms, idx));
            
              /* If we haven't complained that this is an illegal
                 refinement, do so now. */
              if (!complained)
                {
                  /* Check if there is any valid model for the
                     superior model-id. If so, there is nothing we can
                     (or want to) do. */
                  if (has_model (superior_model, 
                                 /*complain=*/false, 
                                 /*structural=*/0))
                    {
                      CLASSTYPE_USE_CONCEPT (superior_model) = ck_concept_map;
                      return true;
                    }

                  error ("concept map %<%T%> cannot be generated from concept map %<%T%>",  
                         superior_model, model_id);
                  inform ("the following template parameters are not deducible:");
                  complained = true;
                }
            
              inform("  `%D`", parm);
            }
        }
    
      if (complained)
        {
          inform("you will need to explicitly define a concept map %<%T%>",
                 superior_model);
          return false;
        }
    }

  /* Build the model definition and recurse. */
  CLASSTYPE_USE_CONCEPT (superior_model) = ck_implicit_concept_map;
  superior_model = begin_model_definition (superior_model);

  /* Introduce all of the associated types defined in the model into
     the model for the superior. No substitutions are necessary here,
     because all of the associated types will be in terms of template
     parameters, templates, and concrete types. */
  for (field = TYPE_FIELDS (model_id); field; field = TREE_CHAIN (field))
    {
      if (TREE_CODE (field) == TYPE_DECL
          && !DECL_ARTIFICIAL (field))
        {
          tree decl = build_lang_decl (TYPE_DECL, DECL_NAME (field), 
                                       TREE_TYPE (field));
          if (processing_template_decl)
            decl = push_template_decl (decl);
          finish_member_declaration (decl);
        }
    }

  /* Finish the superior model and define its operations. */
  superior_model = finish_model_definition (superior_model, implicit_model);
  return superior_model != error_mark_node;
}

/* Defines model operations for superiors of a concept. Used with
   dfs_walk_superiors. */
static bool 
define_model_operations_for_superiors(tree model_id, void *data)
{
  tree declared_model = (tree)data;
  define_model_operations (model_id, declared_model);
  return true;
}


/* Actual recursive implementation of dfs_walk_superiors. */
static bool
dfs_walk_superiors_r (tree model_id, bool (*fn)(tree, void*), void* data,
                      bool skip)
{
  tree superior_model;
  if (!skip && !fn(model_id, data))
    return false;

  if (!CLASSTYPE_ALL_REFINEMENTS (model_id))
    {
      tree template, concept;

      template = most_general_template (CLASSTYPE_TI_TEMPLATE (model_id));
      gcc_assert (TREE_CODE (template) == TEMPLATE_DECL);
      
      concept = TREE_TYPE (template);
      if (TYPE_BINFO (concept))
        {
          /* Visit superiors.  */
          tree binfo, base_binfo;
          int i;
          
          for (binfo = TYPE_BINFO (concept), i = 0;
               BINFO_BASE_ITERATE (binfo, i, base_binfo); i++)
            {
              tree args = CLASSTYPE_TI_ARGS (model_id);
              tree superior_model;
              tree superior_concept;
              
              args = tsubst (TYPE_TI_ARGS (BINFO_TYPE (base_binfo)),
                             args, tf_error, NULL_TREE);
              
              template = CLASSTYPE_TI_TEMPLATE (BINFO_TYPE (base_binfo));
              template = most_general_template (template);
              superior_concept = TREE_TYPE (template);
              
              superior_model = 
                lookup_template_class_real (superior_concept,
                                            args,
                                            CLASSTYPE_WHERE_CLAUSE (model_id),
                                            /*in_decl=*/NULL_TREE,
                                            /*context=*/NULL_TREE,
                                            /*entering_scope=*/0,
                                            tf_error);
              
              /* Pick up defaults. */
              args = CLASSTYPE_TI_ARGS (superior_model);
              gcc_assert (TREE_VEC_LENGTH (args) == DECL_NTPARMS (TYPE_TI_TEMPLATE (superior_model)));

              CLASSTYPE_ALL_REFINEMENTS (model_id) = 
                tree_cons (NULL_TREE, superior_model, 
                           CLASSTYPE_ALL_REFINEMENTS (model_id));
            }
        }
    }

  superior_model = CLASSTYPE_ALL_REFINEMENTS (model_id);
  for (; superior_model; superior_model = TREE_CHAIN (superior_model))
    {
      if (!dfs_walk_superiors_r (TREE_VALUE (superior_model), fn, data, 
                                 /*skip=*/false))
        return false;
    }

  return true;
}


/* Calls fn(superior, data) for all of the superiors of the given
   model-id using a depth-first traversal until FN returns false or
   all superiors have been exhausted. data contains user-defined
   data. */
bool
dfs_walk_superiors (tree model_id, bool (*fn)(tree, void*), void* data)
{
  return dfs_walk_superiors_r (model_id, fn, data, /*skip=*/true);
}

/* Aborts a depth-first traversal of superiors when the MODEL_ID
   matches the SUPERIOR_MODEL_ID stored in DATA. */
static bool 
same_model_p (tree model_id, void* data)
{
  tree superior_model_id = *((tree*)(data));
  return !same_type_p(model_id, superior_model_id);
}

/* True if SUPERIOR_MODEL_ID occurs within the refinement hierarchy of
   MODEL_ID. */
bool 
refinement_of_p (tree model_id, tree superior_model_id)
{
  return !dfs_walk_superiors (model_id, &same_model_p, &superior_model_id);
}

/* Hash a concept instance.  */
static hashval_t
hash_concept_instance (const void* data)
{
  tree inst = (tree)data;
  tree tmpl = most_general_template (CLASSTYPE_TI_TEMPLATE (inst));
  return htab_hash_pointer (tmpl);
}

/* Determine if two concept instances are equivalent.  */
static int
equal_concept_instance (const void* data1, const void* data2)
{
  tree inst1 = (tree)data1;
  tree inst2 = (tree)data2;
  return same_type_p (inst1, inst2);
}

/* Structure used to store data used while traversing the refinements
   of the concept instances provided to common_refinements().  */
struct common_refinements_data_t 
{
  htab_t my_refinements;
  htab_t other_refinements;
  tree common_refinements;
};

static bool 
common_refinements_r (tree instance, void* ptr)
{
  struct common_refinements_data_t* data 
    = (struct common_refinements_data_t*)ptr;

  void **my_slot = htab_find_slot (data->my_refinements, instance, INSERT);
  if (*my_slot)
    /* We've already seen this refinement; skip it.  */
    return true;

  /* Record this refinement.  */
  *my_slot = instance;

  /* If we saw this refinement in the other concept instance, it is a
     common refinement.  */
  if (data->other_refinements && htab_find(data->other_refinements, instance))
    data->common_refinements = tree_cons (NULL_TREE, instance,
                                          data->common_refinements);

  return true;
}

/* Determine the common refinements of the given concept instances, e.g.,
   the set of concept instances that are refined by both INST1 and
   INST2. Returns a TREE_LIST containing those refinements.  */
tree 
common_refinements (tree inst1, tree inst2)
{
  struct common_refinements_data_t data;

  /* Walk INST1's refinements and populate the first hash table.  */
  data.my_refinements = htab_create(11, 
                                    hash_concept_instance,
                                    equal_concept_instance,
                                    NULL);
  data.other_refinements = 0;
  data.common_refinements = NULL_TREE;
  dfs_walk_superiors (inst1, common_refinements_r, &data);

  /* Walk INST2's refinements and populate the list of common
     refinements.  */
  data.other_refinements = data.my_refinements;
  data.my_refinements = htab_create(11, 
                                    hash_concept_instance,
                                    equal_concept_instance,
                                    NULL);  
  dfs_walk_superiors (inst2, common_refinements_r, &data);

  return data.common_refinements;
}

/* Builds a converting constructor for the archetype ARCHETYPE from
   the conversion operator requirement REQ, using concept arguments
   TARGS. Returns a FUNCTION_DECL for the converting constructor.  */
static tree
build_converting_constructor_from_operator_requirement (tree req,
                                                        tree targs,
                                                        tree archetype)
{
  tree parmtypes;
  tree type;
  tree parms;
  tree decl;

  /* Parameter types for the constructor.  */
  parmtypes = tsubst (TYPE_ARG_TYPES (TREE_TYPE (req)),
                      targs, tf_warning_or_error, NULL_TREE);

  /* The type of the constructor.  */
  type = build_function_type (void_type_node, parmtypes);
  type = build_method_type (archetype, type);

  /* Parameters for the constructor.  */
  parms = cp_build_parm_decl (this_identifier, 
                              build_pointer_type (archetype));
  TREE_CHAIN (parms) = cp_build_parm_decl (get_identifier ("from"),
                                           TREE_VALUE (TREE_CHAIN (parmtypes)));

  /* Build the declaration of the constructor.  */
  decl = build_lang_decl (FUNCTION_DECL, DECL_NAME (TYPE_NAME (archetype)),
                          type);
  DECL_ARGUMENTS (decl) = parms;
  DECL_CONSTRUCTOR_P (decl) = 1;
  DECL_CONTEXT (decl) = archetype;
  TREE_PUBLIC (decl) = 1;
  DECL_EXTERNAL (decl) = 1;
  grok_ctor_properties (archetype, decl);

  return decl;
}

/* Builds a copy assignment operator for the archetype ARCHETYPE from
   the copy asssignment operator requirement REQ, using concept arguments
   TARGS. Returns a FUNCTION_DECL for the copy assignment operator.  */
static tree
build_copy_assignment_operator_from_requirement (tree req,
                                                 tree targs,
                                                 tree archetype)
{
  tree parmtypes;
  tree type;
  tree parms;
  tree decl;
  tree result_type;

  /* Parameter types for the copy assignment operator. */
  parmtypes = tsubst (TREE_CHAIN (TYPE_ARG_TYPES (TREE_TYPE (req))),
                      targs, tf_warning_or_error, NULL_TREE);
  /* The type of the copy assignment operator.  */
  result_type = tsubst (TREE_TYPE (TREE_TYPE (req)), 
                        targs, tf_warning_or_error, NULL_TREE);
  type = build_function_type (result_type, parmtypes);
  type = build_method_type (archetype, type);

  /* Parameters for the copy assignment operator.  */
  parms = cp_build_parm_decl (this_identifier, 
                              TREE_VALUE (TYPE_ARG_TYPES (type)));
  TREE_CHAIN (parms) 
    = cp_build_parm_decl 
        (DECL_NAME (TREE_CHAIN (DECL_ARGUMENTS (req))),
         TREE_VALUE (TREE_CHAIN (TYPE_ARG_TYPES (type))));

  /* Build the declaration of the copy assignment operator.  */
  decl = build_lang_decl (FUNCTION_DECL, ansi_assopname (NOP_EXPR), type);
  DECL_ARGUMENTS (decl) = parms;
  SET_OVERLOADED_OPERATOR_CODE (decl, NOP_EXPR);
  DECL_CONTEXT (decl) = archetype;
  TREE_PUBLIC (decl) = 1;
  DECL_EXTERNAL (decl) = 1;
  grok_op_properties (decl, /*complain=*/true);

  return decl;
}

/* Add the member requirements introduced by a model requirement for
   MODEL_ID to the given ARCHETYPE, ignoring requirements for other
   archetypes. */
static void
add_member_requirements_to_archetype (tree model_id, tree archetype)
{
  tree concept, template;
  tree field;
  
  /* Dig out the concept */
  template = most_general_template (CLASSTYPE_TI_TEMPLATE (model_id));
  gcc_assert (TREE_CODE (template) == TEMPLATE_DECL);
  concept = TREE_TYPE (template);

  /* Note that we're processing a template declaration so that
     dependent types are properly recognized. */
  ++processing_template_decl;
  for (field = TYPE_METHODS (concept); field; field = TREE_CHAIN (field))
    {
      tree new_decl = NULL_TREE;

      /* We're only interested in member requirements. */
      if (TREE_CODE (field) == FUNCTION_DECL
	  && TREE_CODE (TREE_TYPE (field)) == METHOD_TYPE)
        {
          tree func_type = instantiate_operation_type (field, model_id);
          tree base_type = TYPE_METHOD_BASETYPE (func_type);

          /* DPG TBD: We use comptypes directly here, because the
             canonical type system is *not* currently working
             properly. */
          if (comptypes (TYPE_MAIN_VARIANT (base_type), archetype, 
                         COMPARE_STRUCTURAL))
            {
              /* Insert the new operation declaration into the archetype. */
              new_decl = build_operation_decl (field, func_type, concept,
					       CLASSTYPE_TI_ARGS (model_id));

              if (DECL_CONSTRUCTOR_P (field))
                DECL_NAME (new_decl) = DECL_NAME (TYPE_NAME (archetype));
            }
        }
      /* Copy assignment operator requirements are handled as a
	 special case, to work around the fact they they might be
	 expressed as members or non-members, e.g., 

	   result_type operator=(T&, const U&);

	 or
	 
	   result_type T::operator=(const U&);

         Only the latter is correct according to N2193, but ConceptGCC
         has only supported the former (up to now).  We'll treat both
         the same way.  */
      else if (TREE_CODE (field) == FUNCTION_DECL
	       && DECL_OVERLOADED_OPERATOR_P (field) == NOP_EXPR)
        {
          tree field_type = TREE_TYPE (field);
          tree base_type;

          /* Determine the base type of the method, if it's a
             method. Otherwise, determine the type of the first
             parameter.  */
          if (TREE_CODE (field_type) == METHOD_TYPE)
            base_type = TYPE_METHOD_BASETYPE (field_type);
          else
            base_type 
              = non_reference (TREE_VALUE (TYPE_ARG_TYPES (field_type)));

          if (comptypes (TYPE_MAIN_VARIANT (base_type), archetype, 
                         COMPARE_STRUCTURAL))
	    /* Build a copy assignment operator.  */
	    new_decl = build_copy_assignment_operator_from_requirement
		         (field, CLASSTYPE_TI_ARGS (model_id), archetype);
        }
      /* Conversion requirements are modeled as user-defined
         conversion functions (which are members) or constructors.  */
      else if (TREE_CODE (field) == FUNCTION_DECL
               && DECL_CONV_FN_P (field))
        {
          tree to_type = tsubst (DECL_CONV_FN_TYPE (field),
                                 CLASSTYPE_TI_ARGS (model_id),
                                 tf_error | tf_warning,
                                 model_id);
          tree from_type_old = 
            non_reference (TREE_VALUE (TYPE_ARG_TYPES (TREE_TYPE (field))));
          tree from_type = tsubst (from_type_old,
                                   CLASSTYPE_TI_ARGS (model_id),
                                   tf_error | tf_warning,
                                   model_id);
          if (comptypes (to_type, from_type, COMPARE_STRUCTURAL))
            /* Just ignore conversions from a type to itself.  */
            ;
          else if (comptypes (TYPE_MAIN_VARIANT (from_type), archetype, 
                              COMPARE_STRUCTURAL))
            {
              /* Build a user-defined conversion operator.  */
              tree func_type = build_function_type (to_type, void_list_node);
              tree method_type = build_method_type (from_type, func_type);
              new_decl = build_operation_decl (field, method_type, model_id,
					       CLASSTYPE_TI_ARGS (model_id));
              DECL_ARGUMENTS (new_decl) = NULL_TREE;
              DECL_NAME (new_decl) = mangle_conv_op_name_for_type (to_type);
              TYPE_HAS_CONVERSION (archetype) = 1;
            }
          else if (!IS_AGGR_TYPE (from_type)
                   && comptypes (TYPE_MAIN_VARIANT (to_type), archetype, 
                                 COMPARE_STRUCTURAL))
	    /* Build a converting constructor.  */
	    new_decl = build_converting_constructor_from_operator_requirement
                         (field, CLASSTYPE_TI_ARGS (model_id), archetype);
        }


      if (new_decl)
	{
	  /* NEW_DECL is the new declaration we want to inject into
	     the archetype. We must first determine if such a
	     declaration already exists, because it came from another
	     concept. */
	  tree t;

	  for (t = TYPE_METHODS (archetype); t; t = TREE_CHAIN (t))
	    {
	      if (DECL_NAME (t) == DECL_NAME (new_decl)
		  && comptypes (TREE_TYPE (t), TREE_TYPE (new_decl),
				COMPARE_STRUCTURAL))
		break;
	    }
	  
	  if (!t)
	    /* NEW_DECL is truly new; put it into the archetype.  */
	    finish_member_declaration (new_decl);
	}

    }

  --processing_template_decl;
}

/* Builds an archetype for template parameter or associated type TYPE.
   Returns the archetype if an archetype is needed, NULL_TREE
   otherwise.  This routine only declares the archetype;
   define_archetype defines the contents of the archetype.  */
tree
maybe_build_archetype (tree type)
{
  tree archetype;
  tree decl;
  int quals;
  tree name;
  char* name_str;

  timevar_push (TV_ARCHETYPES);

  /* Strip qualifiers. */
  quals = cp_type_quals (type);
  type = TYPE_MAIN_VARIANT (type);

  /* Can only build archetypes for non-dependent template type
     parameters and typename types. */  
  if ((TREE_CODE (type) != TEMPLATE_TYPE_PARM
       && TREE_CODE (type) != TYPENAME_TYPE
       && TREE_CODE (type) != ASSOCIATED_TYPE)
      || dependent_type_p (type)
      || !current_where_clause)
    POP_TIMEVAR_AND_RETURN (TV_ARCHETYPES, NULL_TREE);

  /* Append a "'" to the name of archetype, so that we can recognize
     them in the output. */
  name = DECL_NAME (TYPE_NAME (type));
  name_str = (char *)xmalloc (IDENTIFIER_LENGTH (name) + 2);
  sprintf(name_str, "%s'", IDENTIFIER_POINTER (name));
  name = get_identifier(name_str);
  free(name_str);

  /* Declare the archetype */
  archetype = make_aggr_type (RECORD_TYPE);
  decl = build_decl (TYPE_DECL, name, archetype);
  TYPE_NAME (archetype) = decl;
  TYPE_STUB_DECL (archetype) = decl;
  CLASSTYPE_WHERE_CLAUSE (archetype) = TREE_VALUE (current_where_clause);
  CLASSTYPE_IS_ARCHETYPE (archetype) = 1;
  set_type_archetype (type, archetype);

  timevar_pop (TV_ARCHETYPES);
  return build_qualified_type (archetype, quals);
}

/* Define the members of an archetype based on the requirements clause
   that constraints it. */
tree 
define_archetype (tree archetype)
{
  tree requirements = CLASSTYPE_WHERE_CLAUSE (archetype);
  tree req;
  int old_archetypes_represent_themselves = archetypes_represent_themselves;
  tree bases = NULL_TREE;

  if (COMPLETE_TYPE_P (archetype) || TYPE_BEING_DEFINED (archetype))
    return archetype;

  timevar_push (TV_ARCHETYPES);

  /* Compute the set of base classes of this archetype.  */
  for (req = requirements; req; req = TREE_CHAIN (req))
    {
      if (WHERE_REQ_DERIVED_FROM_P (req)
	  && same_type_p (archetype, WHERE_REQ_DERIVED (req)))
	bases = tree_cons (access_public_node, WHERE_REQ_BASE (req), bases);
    }

  /* Archetypes cannot represent themselves for this routine to work
     properly. */
  archetypes_represent_themselves = 0;

  xref_basetypes (archetype, bases);

  push_to_top_level ();
  
  /* Define the archetype based on the current where clause. */
  archetype = begin_class_definition (archetype, /*attributes=*/NULL_TREE);
  TYPE_HAS_CONSTRUCTOR (archetype) = 1;
  TYPE_HAS_DEFAULT_CONSTRUCTOR (archetype) = 0;
  TYPE_NEEDS_CONSTRUCTING (archetype) = 1;
  for (req = requirements; req; req = TREE_CHAIN (req))
    {
      if (WHERE_REQ_MODEL_P (req) && !WHERE_REQ_FROM_REFINEMENT (req))
        {
          tree model_id = WHERE_REQ_MODEL (req);          
          add_member_requirements_to_archetype (model_id, archetype);
        }
    }
  archetype = finish_struct (archetype, NULL_TREE);

  pop_from_top_level ();

  archetypes_represent_themselves = old_archetypes_represent_themselves;

  timevar_pop (TV_ARCHETYPES);

  return archetype;
}

/* Declares a synthesized concept map for the given CONCEPT_ID.  The
   CONCEPT_ID will be a RECORD_TYPE node that refers to a concept with
   a given set of template arguments, e.g., those specified within a
   requirements clause.

   Returns the new, synthesized concept map. This concept map will be
   the archetype of CONCEPT_ID.  */
tree
synthesize_concept_map (tree concept_id)
{
  tree synthesized = NULL_TREE;
  tree args = NULL_TREE;
  tree template = NULL_TREE;
  tree concept = NULL_TREE;
   
  /* If we already have an archetype, use it.  */
  synthesized = type_archetype (concept_id);

  if (synthesized == concept_id && false)
    {
      args = template_args_as_archetypes (CLASSTYPE_TI_ARGS (concept_id));
      if (args == error_mark_node)
	return error_mark_node;

      template= most_general_template (CLASSTYPE_TI_TEMPLATE (concept_id));
      concept = TREE_TYPE (template);

      /* Find the RECORD_TYPE for the synthesized concept map.  */
      archetypes_represent_themselves++;
      synthesized = lookup_template_class (template, args,
					   /*in_decl=*/NULL_TREE,
					   /*context=*/NULL_TREE,
					   /*entering_scope=*/0, 
					   tf_warning|tf_error);
      archetypes_represent_themselves--;

      /* Mark this as a synthesized concept map.  */
      CLASSTYPE_USE_CONCEPT (synthesized) = ck_synthesized_concept_map;

      /* SYNTHESIZED is the archetype of CONCEPT_ID.  */
      /*      set_type_archetype (concept_id, synthesized); */
      CLASSTYPE_IS_ARCHETYPE (synthesized) = true;

      /* Keep track of this synthesized concept map; we'll need to
	 remove it later.  */
      note_deduced_model (synthesized);
    }

  return synthesized;
}

/* Defines a synthesized concept map, containing all of the
   requirements that can be found within the body of the concept
   map.  */
tree
define_synthesized_concept_map (tree synthesized)
{
  tree template;
  tree concept;
  tree field;
  tree concept_id;
  
  if (COMPLETE_TYPE_P (synthesized) || TYPE_BEING_DEFINED (synthesized))
    return synthesized;

  /* Dig out the concept and representative. */
  template = most_general_template (CLASSTYPE_TI_TEMPLATE (synthesized));
  concept = TREE_TYPE (template);
  concept_id = type_representative (synthesized);

  /* We may be in the middle of deferred access check.  Disable it
     now.  */
  push_deferring_access_checks (dk_no_deferred);     
  push_to_top_level ();
  if (TYPE_CONTEXT (synthesized))
    push_nested_namespace (TYPE_CONTEXT (synthesized));

  /* We're building a new synthesized concept map.  */
  begin_specialization ();
  xref_basetypes (synthesized, NULL_TREE);
  synthesized = begin_class_definition (synthesized, /*attributes=*/NULL_TREE);
  TYPE_BEING_DEFINED (synthesized) = 1;
  
  /* Loop over all of the fields in the concept, sythesizing
     declarations for the associated types.  */
  for (field = TYPE_FIELDS (concept); field; field = TREE_CHAIN (field))
    {
      if (TREE_CODE (field) == TYPE_DECL && !DECL_ARTIFICIAL (field))
	{
	  /* For an associated type requirement, build a TYPE_DECL in
	     the synthesized concept map that references the archetype
	     of the associated type.  */
	  tree assoc_type;
	  tree decl;
	  tree context ;

	  /* Build and store the TYPE_DECL for this associated
	     type.  */
	  decl = build_lang_decl (TYPE_DECL, DECL_NAME (field),
				  NULL_TREE);

	  /* Keep the previous context.  */
	  ++unchecked_section_of_constrained_template;
	  context = tsubst (TYPE_CONTEXT (TREE_TYPE (field)), 
			    CLASSTYPE_TI_ARGS (concept_id),
			    tf_warning_or_error, NULL_TREE);
	  --unchecked_section_of_constrained_template;
	  
	  /* The associated type for the given CONCEPT_ID.  */
	  assoc_type = build_associated_type (context, decl, NULL_TREE);
	  TREE_TYPE (decl) = type_archetype (assoc_type);

	  /* Finish the TYPE_DECL we just built.  */
	  finish_member_declaration (decl);
	}
    }

  /* Loop over all of the methods in the concept, synthesizing
     declarations for the synthetic concept map.  */
  for (field = TYPE_METHODS (concept); field; field = TREE_CHAIN (field))
    {
      if (TREE_CODE (field) == FUNCTION_DECL
	  && !DECL_CONSTRUCTOR_P (field)
	  && !DECL_DESTRUCTOR_P (field)
	  && TREE_CODE (TREE_TYPE (field)) == FUNCTION_TYPE)
	{
	  /* For an associated function requirement, build a
	     FUNCTION_DECL in the synthesized concept map that
	     declares the same operation but using the concept
	     arguments from CONCEPT_ID.  These operation declarations
	     will be undefined, because they are only used for
	     type-checking constrained templates.  */
	  tree type = instantiate_operation_type (field, synthesized);
	  tree other;

	  /* Determine if there is already a requirement with the same
	     name and type. */
	  for (other = TYPE_METHODS (synthesized); other; 
	       other = TREE_CHAIN (other))
	    {
	      if (TREE_CODE (other) == FUNCTION_DECL
		  && DECL_NAME (other) == DECL_NAME (field)
		  && same_type_p (TREE_TYPE (other), type))
		/* We have found a matching function.  */
		break;
	    }

	  if (!other)
	    {
	      /* We did not find another method with the same
		 signature, so add this member.  */
	      tree decl 
		= build_operation_decl (field, type, synthesized, 
					CLASSTYPE_TI_ARGS (synthesized));
	      finish_member_declaration (decl);
	    }
	}
    }

  /* We're done with the concept map.  */
  synthesized = finish_struct (synthesized, NULL_TREE);
  end_specialization ();
  TYPE_BEING_DEFINED (synthesized) = 0;

  /* Return to the previous context.  */
  if (TYPE_CONTEXT (synthesized))
    pop_nested_namespace (TYPE_CONTEXT (synthesized));
  pop_from_top_level ();
  pop_deferring_access_checks ();

  return synthesized;
}

/* Perform constraint propagation for the entity currently being
   declared, which already has some constraints in the where clause. T
   is the declaration of the entity, or a part of its type. When a
   constrained template is used within the declaration, those
   constraints will be added to the current where clause. 

   Returns true if any constraints have been propagated. */
bool
propagate_constraints (tree t)
{
  bool propagated = false;

  if (!t)
    return false;

  if (DECL_P (t) && TREE_TYPE (t))
    return propagate_constraints (TREE_TYPE (t));

  switch (TREE_CODE (t))
    {
    case TREE_LIST:
      while (t) {
        propagated |= propagate_constraints (TREE_PURPOSE (t));
        propagated |= propagate_constraints (TREE_VALUE (t));
        propagated |= propagate_constraints (TREE_TYPE (t));
        t = TREE_CHAIN (t);
      }
      break;

    case TREE_VEC:
      {
        int i;
        for (i = 0; i < TREE_VEC_LENGTH (t); i++)
          propagated |= propagate_constraints (TREE_VEC_ELT (t, i));
      }
      break;

    case OFFSET_TYPE:
      propagated |= propagate_constraints (TYPE_OFFSET_BASETYPE (t));
      propagated |= propagate_constraints (TREE_TYPE (t));
      break;

    case ENUMERAL_TYPE:
    case BOOLEAN_TYPE:
    case INTEGER_TYPE:
    case REAL_TYPE:
    case VOID_TYPE:
      break;

    case POINTER_TYPE:
    case REFERENCE_TYPE:
    case COMPLEX_TYPE:
    case VECTOR_TYPE:
    case ARRAY_TYPE:
      propagated |= propagate_constraints (TREE_TYPE (t));
      break;

    case RECORD_TYPE:
      if (TYPE_PTRMEMFUNC_P (t))
        {
          propagated |= propagate_constraints (TYPE_PTRMEMFUNC_FN_TYPE (t));
          propagated |= propagate_constraints (TYPE_PTRMEMFUNC_OBJECT_TYPE (t));
          break;
        }
      /* Fall through */

    case UNION_TYPE:
    case QUAL_UNION_TYPE:
      /* Never propagate constraints if the class in question is still
         open. */
      if (currently_open_class (t))
        break;

      if (CLASSTYPE_USE_TEMPLATE (t))
        propagated |= propagate_constraints (CLASSTYPE_TI_ARGS (t));

      /* Don't propagate constraints for a concept... that's already
         taken care of by the where clause handling. */
      if (CLASSTYPE_USE_CONCEPT (t))
        break;

      if (CLASSTYPE_USE_TEMPLATE (t)) {
        tree template = most_general_template (CLASSTYPE_TI_TEMPLATE (t));
        tree type = TREE_TYPE (template);
        tree req;

        /* Propagate each constraint from the primary template. */
        for (req = CLASSTYPE_WHERE_CLAUSE (type); req; 
             req = WHERE_REQ_CHAIN (req))
          {
            tree real_req = tsubst (req, 
                                    CLASSTYPE_TI_ARGS (t),
                                    tf_error|tf_warning, 
                                    /*in_decl=*/NULL_TREE);
            if (real_req && real_req != error_mark_node)
              {
                process_requirement (real_req, /*expand=*/false);
                propagated = true;
              }
          }
      }
      break;

    case METHOD_TYPE:
      propagated |= propagate_constraints (TYPE_METHOD_BASETYPE (t));
      /* Fall through */

    case FUNCTION_TYPE:
      propagated |= propagate_constraints (TYPE_ARG_TYPES (t));
      propagated |= propagate_constraints (TREE_TYPE (t));
      break;

    case TEMPLATE_TEMPLATE_PARM:
    case TEMPLATE_TYPE_PARM:
      break;

    case TYPENAME_TYPE:
    case ASSOCIATED_TYPE:
      propagated |= propagate_constraints (TYPE_CONTEXT (t));
      break;

    case BOUND_TEMPLATE_TEMPLATE_PARM:
      propagated |= propagate_constraints (TYPE_TI_ARGS (t));
      break;

    case UNBOUND_CLASS_TEMPLATE:
      break;

    case TYPEOF_TYPE:
      break;

    default:
      break;
    }

  return propagated;
}

#include "gt-cp-concept.h"
