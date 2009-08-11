#include <assert.h>
#include <fnmatch.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "obstack.h"
#include "cfgloop.h"
#include "function.h"

#include "auto-vectorise-override.h"

/**
 * Linked list element, holding each of the loop target specifications
 */
typedef struct Override Override;
struct Override {
    /**
     * Glob pattern for the main_input_filename match; will never be null and will be heap allocated
     */
	char* mainInputFileName;
	/**
	 * Glob pattern for the current_function_name match; will never be null and will be heap allocated
	 */
	char* functionName;
	/**
	 * Loop number match (will be -1 for match any)
	 */
	int loopNumber;
	/**
	 * The target to vectorise loops onto (-2=use default heuristic;-1=don't vectorise;+ve=a target id)
	 */
	int target;
	/**
	 * Next element in the linked list
	 */
	Override* next;
};
/**
 * Head of the loop specifications list
 */
static Override* Override_head = NULL;
/**
 * Delete the loop specifications list
 */
static void Override_deleteAll( void ) {
	while( Override_head ) {
		Override* next = Override_head->next;
        free( Override_head->mainInputFileName );
		free( Override_head->functionName );
		free( Override_head );
		Override_head = next;
	}
}
/**
 * Determine if a loop is matched by a particular specification
 *
 * @param self the loop specification; may not be NULL
 * @param loop the loop; may not be NULL
 * @return TRUE if the loop matches; FALSE otherwise
 */
static bool Override_matches( Override* self, struct loop* loop ) {
	assert( self ); assert( loop );

	if( self->loopNumber != -1 && self->loopNumber != loop->num ) return FALSE;
	if( !fnmatch( self->mainInputFileName, main_input_filename, 0 ) == 0 ) return FALSE;
	if( !fnmatch( self->functionName, current_function_name( ), 0 ) == 0 ) return FALSE;

	return TRUE;
}
/**
 * Load a list of specifications from a file
 *
 * @param file the file to load from
 * @return TRUE on success; FALSE otherwise
 */
static bool Override_loadFromFile( FILE* file ) {
    /* BTW, this is not 100% robust.  The plugin system does this in a bullet proof way */
#   define BUF_LEN 4096
    char filename[ BUF_LEN ];
    char function[ BUF_LEN ];
    char loop[ BUF_LEN ];
    char target[ BUF_LEN ];

    assert( file );

    while( fscanf( file, "filename=%s function=%s loop=%s target=%s\n", filename, function, loop, target ) == 4 ) {
        Override* override = ( Override* )xcalloc( sizeof( Override ), 1 );
        override->mainInputFileName = xstrdup( filename );
        override->functionName = xstrdup( function );
        override->loopNumber = -1;
        override->next = Override_head;
        if( strcmp( loop, "*" ) != 0 ) override->loopNumber = atoi( loop );
        if( strcmp( target, "default" ) == 0 ) override->target = AUTOVECTORISE_OVERRIDE_DEFAULT;
        else if( strcmp( target, "none" ) == 0 ) override->target = AUTOVECTORISE_OVERRIDE_DONT;
        else override->target = atoi( target );
        Override_head = override;
    }
	return TRUE;
}

/* Public API functions - see auto-vectorise-override.h for details */
bool AutoVectorise_Override_setup( void ) {
    char* filename;
    FILE* file;
    bool success;

    filename = getenv( AUTOVECTORISE_OVERRIDE_ENVNAME );
    if( filename == NULL ) return TRUE;

    file = fopen( filename, "r" );
    if( file == NULL ) return FALSE;

    success = Override_loadFromFile( file );
    fclose( file );
    return success;
}
int AutoVectorise_Override_getTarget( struct loop* loop ) {
    Override* p;
    for( p = Override_head; p != NULL; p = p->next ) {
        if( Override_matches( p, loop )) return p->target;
    }
    return AUTOVECTORISE_OVERRIDE_DEFAULT;
}
void AutoVectorise_Override_shutdown( void ) {
    Override_deleteAll();
}
