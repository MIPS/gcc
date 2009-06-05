#define ARC_FP_DEBUG 1
#define FINE_GRAINED_LIBRARIES
#if !defined (__ARC_NORM__) || ARC_FP_DEBUG
#define L_pack_sf
#define L_unpack_sf
#define L_make_sf
#define L_thenan_sf
#endif
#ifndef __ARC_NORM__
#define L_addsub_sf
#define L_mul_sf
#define L_div_sf
#define L_sf_to_df
#define L_si_to_sf
#define L_sf_to_si
#define L_usi_to_sf
#elif ARC_FP_DEBUG
#define L_addsub_sf
#define __addsf3 __addsf3_c
#define __subsf3 __subsf3_c
#define L_mul_sf
#define __mulsf3 __mulsf3_c
#define L_div_sf
#define __divsf3 __divsf3_c
#define L_sf_to_df
#define __extendsfdf2 __extendsfdf2_c
#define L_si_to_sf
#define __floatsisf __floatsisf_c
#define L_sf_to_si
#define __fixsfsi __fixsfsi_c
#define L_usi_to_sf
#define __floatunsisf __floatunsisf_c
#endif
#ifndef __ARC_NORM__
#define L_fpcmp_parts_sf
#define L_compare_sf
#define L_eq_sf
#define L_ne_sf
#define L_gt_sf
#define L_ge_sf
#define L_lt_sf
#define L_le_sf
#define L_unord_sf
#define L_negate_sf
#elif ARC_FP_DEBUG
#define L_fpcmp_parts_sf
#define L_eq_sf
#define __eqsf2 __eqsf2_c
#define L_gt_sf
#define __gtsf2 __gtsf2_c
#define L_ge_sf
#define __gesf2 __gesf2_c
#define L_unord_sf
#define __unordsf2 __unordsf2_c
#endif
