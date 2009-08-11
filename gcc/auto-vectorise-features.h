/**
 * Print features suitable for machine learning on vectorising loops.
 *
 * Several different output formats are supported.
 */

#ifndef GCC_AUTOVECTORISEFEATURES_H_
#define GCC_AUTOVECTORISEFEATURES_H_

struct loop;

/**
 * This is the name of the environment variable in which holds the file name to print features to.
 */
#define AUTOVECTORISE_FEATURES_ENVNAME "AUTOVECTORISE_FEATURES"
/**
 * A tag for what to print along with the features to identify the application
 */
#define AUTOVECTORISE_FEATURES_BENCHMARKTAG_ENVNAME "AUTOVECTORISE_FEATURES_BENCHMARKTAG"

/**
 * Setup
 */
void AutoVectorise_Features_setup( void );

struct features_d;
extern struct features_d *alloc_features (void);

#define UPDATE_FEATURE_VECTORIZATION_FACTOR 0

extern void update_features (struct features_d *, int, ...);

/**
 * Print the features for a particular loop.
 *
 * @param loop the loop to print features for; may not be NULL
 */
void AutoVectorise_Features_print (struct loop *loop, struct features_d *);
/**
 * Shutdown
 */
void AutoVectorise_Features_shutdown( void );

#endif /*GCC_AUTOVECTORISEFEATURES_H_*/
