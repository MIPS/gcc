/* Used by upc-crtbegin to define the beginning of the shared section.  */
#ifndef GUPCR_SHARED_BEGIN_NAME
#define GUPCR_SHARED_BEGIN_NAME __upc_shared_start
#endif

/* Used by upc-crtend to define the end of the shared section.  */
#ifndef GUPCR_SHARED_END_NAME
#define GUPCR_SHARED_END_NAME __upc_shared_end
#endif
 
/* Used by upc-crtbegin to define the beginning of the shared section.  */
#ifndef GUPCR_PGM_INFO_BEGIN_NAME
#define GUPCR_PGM_INFO_BEGIN_NAME __upc_pgm_info_start
#endif

/* Used by upc-crtend to define the end of the shared section.  */
#ifndef GUPCR_PGM_INFO_END_NAME
#define GUPCR_PGM_INFO_END_NAME __upc_pgm_info_end
#endif
 
/* Used by upc-crtbegin to define the beginning of UPC init. array section.  */
#ifndef GUPCR_INIT_ARRAY_BEGIN_NAME
#define GUPCR_INIT_ARRAY_BEGIN_NAME __upc_init_array_start
#endif

/* Used by upc-crtend to define the beginning of UPC init. array section.  */
#ifndef GUPCR_INIT_ARRAY_END_NAME
#define GUPCR_INIT_ARRAY_END_NAME __upc_init_array_end
#endif
