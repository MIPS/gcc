/* Features: a high-level mechanism for accessing global compiler state in GCC.
   Copyright (C) 2009 Free Software Foundation, Inc.
   
   Contributed by Inria.

   Authors: Grigori Fursin <grigori.fursin@inria.fr>, Cupertino Miranda
   <cupertinomiranda@gmail.com>, Zbigniew Chamski <zbigniew.chamski@gmail.com>.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "hashtab.h"
#include "toplev.h"

#include "feature-internal.h"

htab_t features_hash = NULL; 


/* Hash dependent functions.  */

static hashval_t
features_htab_hash (const void *x)
{
  const struct feature *p = (const struct feature *) x;
  
  if (!p)
    internal_error ("features_htab_hash: called with NULL feature pointer!");
  
  if (p->name != NULL)
    return htab_hash_string(p->name);
  else
    internal_error ("features_htab_hash: NULL-named feature!\n");
}


static int
features_htab_eq (const void *x, const void *y)
{
  const struct feature *p1 = (const struct feature *) x;
  const struct feature *p2 = (const struct feature *) y;

  return !strcmp (p1->name, p2->name);
}


/* Add feature to hash table.  */
void register_feature (const struct feature *feature)
{
  void const **slot;

  if (!feature)
    internal_error ("register_feature() called with NULL feature!\n");

  if (feature->name == NULL)
    internal_error ("register_feature() called with NULL-named feature!\n");

  if(features_hash == NULL)
    features_hash = htab_create_alloc (150, features_htab_hash, 
				       features_htab_eq, NULL, xcalloc, free);

  if(features_hash != NULL)
    {
      slot = (const void **) htab_find_slot (features_hash, feature, INSERT);
      *slot = (const void *) feature;
    }
}


/* FICI0: ici_find_feature: find the feature with a given name.
 * Return values:
 *  - pointer to 'struct feature' if the named feature was found;
 *  - NULL if the feature could not be found.
 */
struct feature *find_feature (const char *feature_name)
{
  struct feature tmp_feature;

  tmp_feature.name = feature_name;

  return (struct feature *) htab_find (features_hash, &tmp_feature);
}


/* Get pointer to feature with <feature_name>.  */
const void *get_feature (const char *feature_name)
{
  const struct feature *feature;

  feature = find_feature (feature_name);

  if(feature == NULL)
    return NULL;

  if(feature->data != NULL)
    return (const void *) feature->data;
  else if (feature->callback != NULL)
    /* Call only non-NULL callbacks */
    return feature->callback ();
  else
    return NULL;
}


/* Return the size of the data associated with feature */
int get_feature_size (const char *feature_name)
{
  const struct feature *feature;
  
  feature = find_feature (feature_name);
  
  if(feature == NULL)
    return -1;

  return feature->data_size;
}


/* FICI0: get specific sub-feature of a main feature.
 * Prerequisites: both feature and subfeature must exist (no diagnostics).
 * Return values:
 *  - string subfeatures: pointer-to-char corresponding to the string;
 *  - integer/boolean/bit subfeatures: integer converted to pointer-to-void.
 */
const void *
get_subfeature (const char *feature_name, const char *subfeat_name)
{
  const struct feature *feature;
  
  feature = find_feature (feature_name);

  /* no feature or no subfeature callback: bail out. */
  if (!feature || !feature->get_subfeature)
    return NULL;

  return (feature->get_subfeature (subfeat_name));
}


/* FICI0: set sub-feature */
void *set_subfeature (const char *feature_name,
			  const char *subfeat_name,
			  void *subfeat_value)
{
  struct feature *feature = find_feature (feature_name);

  /* no feature or no subfeature callback: bail out. */
  if (!feature || !feature->set_subfeature) return NULL;

  return feature->set_subfeature (subfeat_name, subfeat_value); 
}


static int
add_feature_name (void **slot, void *data)
{
  const char ***crnt_name_ptr = (const char ***) data;
  struct feature *feature = *(struct feature **) slot;

  /* store the name of the current fearute at the corresponding
     location */
  **crnt_name_ptr = feature->name;
  
  /* advance the current location */
  (*crnt_name_ptr)++;

  /* return "OK" */
  return 1;
}


/* return the array of all feature names, terminated by NULL */
const char **
list_features (void)
{
  size_t num_features = htab_elements (features_hash);

  /* allocate space for all names + the terminating NULL */
  const char **all_features = (const char **)
    xmalloc (sizeof (const char *) * (num_features+1));
  
  const char ***data = &all_features; /* data ptr for mapped function */
  const char **first_feature = all_features; /* keep track of actual start */

  /* mark end-of-array */
  all_features[num_features] = NULL;

  /* collect all feature names */
  htab_traverse_noresize (features_hash, add_feature_name, data);

  /* use the stored start-of-array - all_features has changed during
     htab_traverse_noresize */
  return first_feature;  
}
