/* Plugin-side interface to global compiler state for GCC plugins.
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

#ifndef FEATURE_H
#define FEATURE_H

/* Access to values of either features or elements of feature sets */
extern const void *get_feature (const char *feature_name);
extern const void *get_subfeature (const char *feature_name,
				   const char *subfeat_name);
extern void *set_subfeature (const char *feature_name,
			     const char *subfeat_name, void *value);

/* Extract the size of features and feature sets */
extern int get_feature_size (const char *feature_name);
extern int get_num_features (int type);
extern int get_num_subfeatures (const char *feat_name, int type);

/* List of all known features, or all known elements of a feature set */
extern const char **list_features (void);
extern const char **list_subfeatures (const char *feat_name, int type);

#endif /* FEATURE_H */
