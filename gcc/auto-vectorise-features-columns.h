/*
 * This file gives the columns definitions for the features.  This just makes it easier to print out the features in the
 * right order.
 *
 * Each COLUMN macro defines one feature column.
 * The first parameter is the column name (the same name is used for the field name in the Features struct).
 * The second is the C type of the column.
 * The third is the printf format string to print the column
 * The last is the SQL type of the column.
 *
 * Each ARRAY_COLUMN macro defines an indexed set of very similar feature columns.
 * The first parameter is the name of the field in the features struct.  This field will be an array whose element type
 * is given by the next parameter.
 * The second is the element type of the array field in the features struct.
 * The third is the printf format string to print each column.
 * The fourth is the SQL type of each column.
 * The fifth is the number of elements or columns.
 * The sixth gives the printf format of each column name.  A parameter will to the printf will be given by the next
 * macro parameter - so if this is "foo_%d" and the other is i, then the columns will be named "foo_0", "foo_1", ...
 * "foo_(n-1)
 * The last is the parameter for the previous printf format.  It should expect the index of the column to be i.
 *
 * To use this file, first define the COLUMN macro as you need it.  Then include the file.  Finally, undefine the COUMN
 * macro.
 */

#if !defined( COLUMN )
#   define COLUMN( name, ctype, format, sqltype )
#endif
#if !defined( ARRAY_COLUMN )
#   define ARRAY_COLUMN( name, ctype, print, sqltype, size, sqlnameformat, sqlnameparam )
#endif

/* Depth of the loop */
COLUMN( depth, int, "%d", "bigint" )
/* Maximum level of nesting of subloops of loop */
COLUMN( loop_level, int, "%d", "bigint" )
/* Number of child loops */
COLUMN( num_child_loops, int, "%d", "bigint" )
/* Whether the loop can be duplicated */
COLUMN( can_duplicate, int, "%d", "bigint" )

/* Number of blocks in the loop */
COLUMN( num_nodes, int, "%d", "bigint" )

/* Expected number of LOOP iterations */
COLUMN( expected_loop_iterations, int, "%d", "bigint" )
/* Number of branches in the loop */
COLUMN( num_loop_branches, int, "%d", "bigint" )
/* Number of exits from the loop */
COLUMN( num_exits, int, "%d", "bigint" )
/* Number of bbs executed just once per iteration */
COLUMN( num_once_bbs, int, "%d", "bigint" )

/* Number of cfg edges inside the loop */
COLUMN( num_edges, int, "%d", "bigint" )
/* Number of fallthru edges inside the loop */
COLUMN( num_edges_fallthru, int, "%d", "bigint" )
/* Number of abnormal edges inside the loop */
COLUMN( num_edges_abnormal, int, "%d", "bigint" )
/* Number of abnormalcall edges inside the loop */
COLUMN( num_edges_abnormalcall, int, "%d", "bigint" )
/* Number of eh edges inside the loop */
COLUMN( num_edges_eh, int, "%d", "bigint" )
/* Number of fake edges inside the loop */
COLUMN( num_edges_fake, int, "%d", "bigint" )
/* Number of dfsback edges inside the loop */
COLUMN( num_edges_dfsback, int, "%d", "bigint" )
/* Number of canfallthru edges inside the loop */
COLUMN( num_edges_canfallthru, int, "%d", "bigint" )
/* Number of irreducibleloop edges inside the loop */
COLUMN( num_edges_irreducibleloop, int, "%d", "bigint" )
/* Number of sbicall edges inside the loop */
COLUMN( num_edges_sibcall, int, "%d", "bigint" )
/* Number of loopexit edges inside the loop */
COLUMN( num_edges_loopexit, int, "%d", "bigint" )
/* Number of truevalue edges inside the loop */
COLUMN( num_edges_truevalue, int, "%d", "bigint" )
/* Number of falsevalue edges inside the loop */
COLUMN( num_edges_falsevalue, int, "%d", "bigint" )
/* Number of executable edges inside the loop */
COLUMN( num_edges_executable, int, "%d", "bigint" )
/* Number of crossing edges inside the loop */
COLUMN( num_edges_crossing, int, "%d", "bigint" )
/* Number of complex edges inside the loop */
COLUMN( num_edges_complex, int, "%d", "bigint" )
/* Number of critical edges inside the loop */
COLUMN( num_edges_critical, int, "%d", "bigint" )

/* Number of unique locals referenced in the loop */
COLUMN( num_locals, int, "%d", "bigint" )
/* Number of unique locals which are pointers referenced in the loop */
COLUMN( num_local_ptrs, int, "%d", "bigint" )
/* Number of unique globals referenced in the loop */
COLUMN( num_global_ptrs, int, "%d", "bigint" )
/* Number of unique globals which are pointers referenced in the loop */
COLUMN( num_globals, int, "%d", "bigint" )

/* Total number of ssa nodes */
COLUMN( tree_code_total, int, "%d", "bigint" )
/* Total number of gimple nodes */
COLUMN( gimple_code_total, int, "%d", "bigint" )
/* Total phi args */
COLUMN( phi_arg_count, int, "%d", "bigint" )

/* A count for each tree code found */
ARRAY_COLUMN( tree_code_counts, int, "%d", "bigint", NUM_TREE_CODES, "tree_count_%s", tree_code_name[ i ] )
/* A count for each gimple code found */
ARRAY_COLUMN( gimple_code_counts, int, "%d", "bigint", LAST_AND_UNUSED_GIMPLE_CODE, "gimple_count_%s", gimple_code_name[ i ] )

/* The vectorization factor obtainable for each target.
   For the column name we could use targetm_array[i]->name,
   but the '-' characters could cause trouble.  */
ARRAY_COLUMN (vectorization_factor, int, "%d", "bigint", NUM_TARGETS, "vectorization_factor_%d", i)

#undef COLUMN
#undef ARRAY_COLUMN
