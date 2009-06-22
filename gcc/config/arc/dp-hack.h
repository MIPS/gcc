#define FINE_GRAINED_LIBRARIES
#define ARC_DP_DEBUG 1
#if !defined (__ARC_NORM__) || ARC_DP_DEBUG
#define L_pack_df
#define L_unpack_df
#define L_make_df
#define L_thenan_df
#define L_sf_to_df
#endif
#ifndef __ARC_NORM__
#define L_addsub_df
#elif ARC_DP_DEBUG
#define L_addsub_df
#define __adddf3 __adddf3_c
#define __subdf3 __subdf3_c
#endif
#ifndef __ARC_NORM__
#define L_mul_df
#elif ARC_DP_DEBUG
#define L_mul_df
#define __muldf3 __muldf3_c
#endif
#ifndef __ARC_NORM__
#define L_div_df
#define L_df_to_sf
#define L_si_to_df
#define L_df_to_si
#define L_tf_to_usi /* need to defined this instead of df_to_usi */
#define L_usi_to_df
#elif ARC_DP_DEBUG
#define L_div_df
#define __divdf3 __divdf3_c
#define L_df_to_sf
#define __truncdfsf2 __truncdfsf2_c
#define L_si_to_df
#define __floatsidf __floatsidf_c
#define L_df_to_si
#define __fixdfsi __fixdfsi_c
#define L_tf_to_usi
#define __fixunsdfsi __fixunsdfsi_c
#define L_usi_to_df
#define __floatunsidf __floatunsidf_c
#endif
#ifndef __ARC_NORM__
#define L_fpcmp_parts_df
#define L_compare_df
#define L_eq_df
#define L_ne_df
#define L_gt_df
#define L_ge_df
#define L_lt_df
#define L_le_df
#define L_unord_df
#define L_negate_df
#elif ARC_DP_DEBUG
#define L_fpcmp_parts_df
#define L_eq_df
#define __eqdf2 __eqdf2_c
#define L_gt_df
#define __gtdf2 __gtdf2_c
#define L_ge_df
#define __gedf2 __gedf2_c
#define L_unord_df
#define __unorddf2 __unorddf2_c
#endif
