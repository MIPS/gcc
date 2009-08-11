/**
 * Compute machine learning features suitable for predicting the right auto vectorisation heuristic.
 *
 * To add a new feature, put a new COLUMN entry in auto-vectorise-features-columns.h.  That will allocate space for it
 * in the Features structure and will also ensure it is printed in whichever format the user chooses (as well as
 * printing the field in the SQL create table statement, if that's what the user is using).
 * Then add a function to compute the feature call it from Features_compute.
 */

#include <assert.h>

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "obstack.h"
#include "hashtab.h"
#include "cfgloop.h"
#include "tree-flow.h"
#include "tree-flow-inline.h"

#include "auto-vectorise-features.h"

/**
 * This typedef describes which type of output we should have.  The code is able to do both XML and SQL, as well as a
 * more verbose version of the SQL.
 */
enum AutoVectorise_Features_OutputMethod {
    /**
     * Print features in SQL format
     */
    AUTOVECTORISE_FEATURES_SQL,
    /**
     * Print features in SQL format, together with detailed SQL comments which make the values easier for a human to
     * read
     */
    AUTOVECTORISE_FEATURES_SQL_VERBOSECOMMENTS,
    /**
     * Print features in XML format
     */
    AUTOVECTORISE_FEATURES_XML
};
typedef enum AutoVectorise_Features_OutputMethod AutoVectorise_Features_OutputMethod;

/**
 * This struct holds all the features for each loop.
 * Most of the fields are created automatically from "auto-vectorise-features-columns.h"
 */
typedef struct Features Features;
struct Features {
#   define COLUMN( name, ctype, print, sqltype ) ctype name;
#   define ARRAY_COLUMN( name, ctype, print, sqltype, size, sqlnameformat, sqlnameparam ) ctype name[ size ];
#   include "auto-vectorise-features-columns.h"

    /**
     * A hash table to find out how many unique variables are used
     */
    htab_t varTable;
};
/**
 * Set all values in a features struct to zero
 * @param features the features to initialise; may not be NULL
 * @return TRUE on success; FALSE otherwise
 */
static bool Features_init( Features* features ) {
    assert( features );
    memset( features, 0, sizeof( Features ));
    features->varTable = htab_create( 32, htab_hash_pointer, htab_eq_pointer, NULL );
    return features->varTable != NULL;
}
/**
 * Cleanup the features struct
 * @param features the features to cleanup; may not be NULL
 */
static void Features_destroy( Features* features ) {
    assert( features );
    if( features->varTable ) htab_delete( features->varTable );
}
/*----------------------------------------------------------------------------------------------------------------------
 * Printing code
 */

/**
 * Print the header if in SQL mode.
 *
 * This prints a create table sql statement which creates all the fields to be of the right type.
 *
 * @param file the file to print to; may not be NULL
 */
static void Header_printSQL( FILE* file ) {
    int i;
    assert( file );
    fprintf(
        file,
        "create table if not exists MilepostFeatures ("
        " id bigint identity,"
        " benchmark bigint not null,"
        " filename varchar not null,"
        " function varchar not null,"
#           define COLUMN( name, ctype, print, sqltype ) #name " " sqltype " not null,"
#           include "auto-vectorise-features-columns.h"
    );
#   define ARRAY_COLUMN( name, ctype, print, sqltype, size, sqlnameformat, sqlnameparam ) \
        for( i = 0; i < size; ++i ) { \
            fprintf( file, sqlnameformat " %s not null", ( sqlnameparam ), ( sqltype )); \
        }
#   include "auto-vectorise-features-columns.h"
    fprintf( file,
        " foreign key( benchmark ) references Benchmark( id )"
        ");\n"
    );
}
/**
 * Print the header if in XML mode.
 *
 * This prints a <compilation> start element.
 *
 * @param file the file to print to; may not be NULL
 */
static void Header_printXML( FILE* file ) {
    assert( file );
    fprintf( file, "<compilation>\n" );
}
/**
 * Print the footer if in SQL mode.
 *
 * Currently a no-op.
 *
 * @param file the file to print to; may not be NULL
 */
static void Footer_printSQL( FILE* file __attribute__(( unused ))) {
    /* Nothing to do */
}
/**
 * Print the footer if in XML mode.
 *
 * This prints a </compilation> end element.
 *
 * @param file the file to print to; may not be NULL
 */
static void Footer_printXML( FILE* file ) {
    assert( file );
    fprintf( file, "</compilation>\n" );
}

/**
 * Print the features for a loop in SQL mode.
 *
 * @param features the features to print
 * @param file the file to print to
 * @param loop the loop the features come from
 * @param benchmarkId a tag for the current application
 * @param verboseComments whether to print more human readable (but large) info
 */
static void Features_printSQL( Features* features, FILE* file, struct loop* loop, const char* benchmarkId, bool verboseComments ) {
    int i;
    assert( features ); assert( file ); assert( benchmarkId );
    assert( features != NULL );
    if( verboseComments ) {
        fprintf( file, "#benchmark=%s\n", benchmarkId );
        fprintf( file, "#filename=%s\n", main_input_filename );
        fprintf( file, "#function=%s\n", current_function_name( ));
        fprintf( file, "#loop=%d\n", loop->num );
#       define COLUMN( name, ctype, print, sqltype ) fprintf( file, "#" #name "=" print "\n", features->name );
#       define ARRAY_COLUMN( name, ctype, print, sqltype, size, sqlnameformat, sqlnameparam ) \
            for( i = 0; i < size; ++i ) { \
                fprintf( file, "#" sqlnameformat "=" print "\n", ( sqlnameparam ), features->name[ i ] ); \
            }
#       include "auto-vectorise-features-columns.h"
    }

    fprintf(
        file,
        "insert into MilepostFeatures"
        " VALUES("
        "  null,"
        "  %s,'%s','%s',%d"
#       define COLUMN( name, ctype, print, sqltype ) "," print
#       include "auto-vectorise-features-columns.h"
        ,
        benchmarkId, main_input_filename, current_function_name(), loop->num
#       define COLUMN( name, ctype, print, sqltype ) , features->name
#       include "auto-vectorise-features-columns.h"
    );
#   define ARRAY_COLUMN( name, ctype, print, sqltype, size, sqlnameformat, sqlnameparam ) \
        for( i = 0; i < size; ++i ) { \
            fprintf( file, "," print, features->name[ i ] ); \
        }
#   include "auto-vectorise-features-columns.h"
    fprintf(
        file,
        " );\n"
    );
}
/**
 * Print the features for a loop in XML mode.
 *
 * @param features the features to print
 * @param file the file to print to
 * @param loop the loop the features come from
 * @param benchmarkId a tag for the current application
 */
static void Features_printXML( Features* features, FILE* file, struct loop* loop, const char* benchmarkId ) {
    int i;
    assert( features ); assert( file ); assert( benchmarkId );
    fprintf(
        file,
        "\t<features benchmark='%s' filename='%s' function='%s' loop='%d'"
#       define COLUMN( name, ctype, print, sqltype ) " " #name "='" print "'"
#       include "auto-vectorise-features-columns.h"
        ,
        benchmarkId, main_input_filename, current_function_name(), loop->num
#       define COLUMN( name, ctype, print, sqltype ) , features->name
#       include "auto-vectorise-features-columns.h"
    );
#   define ARRAY_COLUMN( name, ctype, print, sqltype, size, sqlnameformat, sqlnameparam ) \
        for( i = 0; i < size; ++i ) { \
            fprintf( file, " " sqlnameformat "='" print "'\n", ( sqlnameparam ), features->name[ i ] ); \
        }
#   include "auto-vectorise-features-columns.h"
    fprintf(
        file,
        "/>\n"
    );
}
/**
 * Print the features for a loop.
 *
 * @param features the features to print
 * @param file the file to print to
 * @param loop the loop the features come from
 * @param benchmarkId a tag for the current application
 * @param outputMethod the mode to print in
 */
static void Features_print( Features* features, FILE* file, struct loop* loop, const char* benchmarkId, AutoVectorise_Features_OutputMethod outputMethod ) {
    assert( features ); assert( file ); assert( benchmarkId );
    switch( outputMethod ) {
        case AUTOVECTORISE_FEATURES_SQL :
            Features_printSQL( features, file, loop, benchmarkId, false );
            break;
        case AUTOVECTORISE_FEATURES_SQL_VERBOSECOMMENTS :
            Features_printSQL( features, file, loop, benchmarkId, true );
            break;
        case AUTOVECTORISE_FEATURES_XML :
            Features_printXML( features, file, loop, benchmarkId );
            break;
    }
}

/*----------------------------------------------------------------------------------------------------------------------
 * Feature computation code
 */

/**
 * Count the number of child loops a loop has (not recursive).
 *
 * @param loop the loop; may not be null
 * @return int the count
 */
static int Loop_countChildren( struct loop* loop ) {
    int count = 0;
    assert( loop );
    for( loop = loop->inner; loop; loop = loop->next ) count++;
    return count;
}

/**
 * Walk gimple operands to pick out bits of info
 */
static tree Features_treeWalker( tree* t, int* walk_subtrees __attribute__(( unused )), void* data ) {
    struct walk_stmt_info* info = ( struct walk_stmt_info* )data;
    Features* features = ( Features* )info->info;
    tree node = *t;
    enum tree_code code = TREE_CODE( node );

    /* Code counts */
    features->tree_code_total++;
    features->tree_code_counts[ code ]++;

    /* Unique vars */
    if( code == SSA_NAME ) {
        node = SSA_NAME_VAR( node );
        code = TREE_CODE( node );
    }
    if( code == VAR_DECL || code == PARM_DECL ) {
        void** slot = htab_find_slot( features->varTable, node, INSERT );
        if( *slot == NULL ) *slot = node;
    }

    return NULL;
}
/**
 * Walk gimple statements to pick out bits of info
 */
static tree Features_stmtWalker( gimple_stmt_iterator* i, bool* updateNodes __attribute__(( unused )), struct walk_stmt_info* info ) {
    Features* features = ( Features* )info->info;
    gimple stmt = gsi_stmt( *i );
    enum gimple_code code = gimple_code( stmt );

    features->gimple_code_total++;
    features->gimple_code_counts[ code ]++;

    switch( code ) {
        case GIMPLE_PHI :
            features->phi_arg_count += gimple_phi_num_args( stmt );
            break;
        case GIMPLE_COND :
        case GIMPLE_CALL :
        case GIMPLE_ASSIGN :
            features->tree_code_counts[ gimple_expr_code( stmt ) ]++;
            break;
        default :
            break;
    }
    return NULL;
}
/**
 * Walk statements and phi nodes of each bb gathering info
 * @param features the features; may not be null
 * @param loop the loop; may not be null
 */
static void Features_walk( Features* features, struct loop* loop ) {
    basic_block* body;
    unsigned i;
    assert( features ); assert( loop );

    body = get_loop_body( loop );
    for( i = 0; i < loop->num_nodes; i++ ) {
        basic_block bb = body[ i ];
        struct walk_stmt_info info;

        memset( &info, 0, sizeof( info ));
        info.info = features;
        walk_gimple_seq( bb->il.gimple->seq, Features_stmtWalker, Features_treeWalker, &info );

        memset( &info, 0, sizeof( info ));
        info.info = features;
        walk_gimple_seq( bb->il.gimple->phi_nodes, Features_stmtWalker, NULL, &info );
    }
    free( body );
}
/**
 * Get a histogram of the different types of edges that are internal to the loop
 *
 * @param features the features; may not be null
 * @param loop the loop; may not be null
 */
static void Features_cfgEdges( Features* features, struct loop* loop ) {
    basic_block* body;
    unsigned i;
    assert( features ); assert( loop );

    body = get_loop_body( loop );
    for( i = 0; i < loop->num_nodes; i++ ) {
        basic_block bb = body[ i ];
        edge e;
        edge_iterator ei;
        if( just_once_each_iteration_p( loop, bb )) features->num_once_bbs++;
        FOR_EACH_EDGE( e, ei, bb->succs ) {
            if( flow_bb_inside_loop_p( loop, e->dest )) {
                features->num_edges++;
                if( e->flags & EDGE_FALLTHRU ) features->num_edges_fallthru++;
                if( e->flags & EDGE_ABNORMAL ) features->num_edges_abnormal++;
                if( e->flags & EDGE_ABNORMAL_CALL ) features->num_edges_abnormalcall++;
                if( e->flags & EDGE_EH ) features->num_edges_eh++;
                if( e->flags & EDGE_FAKE ) features->num_edges_fake++;
                if( e->flags & EDGE_DFS_BACK ) features->num_edges_dfsback++;
                if( e->flags & EDGE_CAN_FALLTHRU ) features->num_edges_canfallthru++;
                if( e->flags & EDGE_IRREDUCIBLE_LOOP ) features->num_edges_irreducibleloop++;
                if( e->flags & EDGE_SIBCALL ) features->num_edges_sibcall++;
                if( e->flags & EDGE_LOOP_EXIT ) features->num_edges_loopexit++;
                if( e->flags & EDGE_TRUE_VALUE ) features->num_edges_truevalue++;
                if( e->flags & EDGE_FALSE_VALUE ) features->num_edges_falsevalue++;
                if( e->flags & EDGE_EXECUTABLE ) features->num_edges_executable++;
                if( e->flags & EDGE_CROSSING ) features->num_edges_crossing++;
                if( e->flags & EDGE_COMPLEX ) features->num_edges_complex++;
                if( EDGE_CRITICAL_P( e )) features->num_edges_critical++;
            }
        }
    }
    free( body );
}
/**
 * Hash table traverser to count the number of unique variable type references.  Used by Features_varCounts.
 */
static int Features_varCounts_trav( void** slot, void* data ) {
    Features* features = ( Features* )data;
    tree var = ( tree )*slot;

    if( TREE_CODE( var ) == VAR_DECL && ( TREE_STATIC( var ) || TREE_PUBLIC( var ))) {
        features->num_globals++;
        if( TREE_CODE( TREE_TYPE( var )) == POINTER_TYPE ) features->num_global_ptrs++;
    } else {
        features->num_locals++;
        if( TREE_CODE( TREE_TYPE( var )) == POINTER_TYPE ) features->num_local_ptrs++;
    }

    return TRUE;
}
/**
 * Count the number of unique variables of different types.
 *
 * Must already have called Features_walk
 *
 * @param features the features; may not be null
 */
static void Features_varCounts( Features* features ) {
    assert( features );
    htab_traverse_noresize( features->varTable, Features_varCounts_trav, features );
}

/**
 * Compute all the features on a loop
 *
 * @param features the features; may not be null
 * @param loop the loop; may not be null
 */
static void Features_compute( Features* features, struct loop* loop ) {
    assert( features ); assert( loop );

    features->depth = loop_depth( loop );
    features->can_duplicate = can_duplicate_loop_p( loop );
    features->num_child_loops = Loop_countChildren( loop );
    features->loop_level = get_loop_level( loop );
    features->expected_loop_iterations = expected_loop_iterations( loop );
    features->num_nodes = loop->num_nodes;
    features->num_loop_branches = num_loop_branches( loop );
    features->num_exits = VEC_length( edge, get_loop_exit_edges( loop ));

    Features_walk( features, loop );
    Features_cfgEdges( features, loop );
    Features_varCounts( features );
}

/*----------------------------------------------------------------------------------------------------------------------
 * Public API code - see auto-vectorise-features.h
 */

void AutoVectorise_Features_printHeader( FILE* file, AutoVectorise_Features_OutputMethod outputMethod ) {
    assert( file );
    switch( outputMethod ) {
        case AUTOVECTORISE_FEATURES_SQL :
        case AUTOVECTORISE_FEATURES_SQL_VERBOSECOMMENTS :
            Header_printSQL( file );
            break;
        case AUTOVECTORISE_FEATURES_XML :
            Header_printXML( file );
            break;
    }
}
void AutoVectorise_Features_printLoop( FILE* file, struct loop* loop, const char* benchmarkId, AutoVectorise_Features_OutputMethod outputMethod ) {
    Features features;
    assert( file ); assert( loop ); assert( benchmarkId );

    Features_init( &features );
    Features_compute( &features, loop );
    Features_print( &features, file, loop, benchmarkId, outputMethod );
    Features_destroy( &features );
}
void AutoVectorise_Features_printFooter( FILE* file, AutoVectorise_Features_OutputMethod outputMethod ) {
    assert( file );
    switch( outputMethod ) {
        case AUTOVECTORISE_FEATURES_SQL :
        case AUTOVECTORISE_FEATURES_SQL_VERBOSECOMMENTS :
            Footer_printSQL( file );
            break;
        case AUTOVECTORISE_FEATURES_XML :
            Footer_printXML( file );
            break;
    }
}
void AutoVectorise_Features_setup( void ) {
    const char* filename = getenv( AUTOVECTORISE_FEATURES_ENVNAME );
    if( filename != NULL ) {
        FILE* file = fopen( filename, "a" );
        if( file == NULL ) {
            fprintf( stderr, "Can't open %s\n", filename );
            return;
        }

        AutoVectorise_Features_printHeader( file, AUTOVECTORISE_FEATURES_SQL_VERBOSECOMMENTS );

        fclose( file );
    }
}
void AutoVectorise_Features_print( struct loop* loop ) {
    const char* filename = getenv( AUTOVECTORISE_FEATURES_ENVNAME );
    if( filename != NULL ) {
        const char* benchmarkId;
        FILE* file = fopen( filename, "a" );
        if( file == NULL ) {
            fprintf( stderr, "Can't open %s\n", filename );
            return;
        }

        benchmarkId = getenv( AUTOVECTORISE_FEATURES_BENCHMARKTAG_ENVNAME );
        if( benchmarkId == NULL ) benchmarkId = "@benchmark";

        AutoVectorise_Features_printLoop( file, loop, benchmarkId, AUTOVECTORISE_FEATURES_SQL_VERBOSECOMMENTS );

        fclose( file );
    }
}
void AutoVectorise_Features_shutdown( void ) {
    const char* filename = getenv( AUTOVECTORISE_FEATURES_ENVNAME );
    if( filename != NULL ) {
        FILE* file = fopen( filename, "a" );
        if( file == NULL ) {
            fprintf( stderr, "Can't open %s\n", filename );
            return;
        }

        AutoVectorise_Features_printFooter( file, AUTOVECTORISE_FEATURES_SQL_VERBOSECOMMENTS );

        fclose( file );
    }
}

