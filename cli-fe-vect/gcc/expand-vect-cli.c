#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "ggc.h"
#include "tree.h"
#include "target.h"
#include "rtl.h"
#include "basic-block.h"
#include "diagnostic.h"
#include "tree-flow.h"
#include "tree-dump.h"
#include "timevar.h"
#include "cfgloop.h"
#include "cfglayout.h"
#include "expr.h"
#include "recog.h"
#include "optabs.h"
#include "params.h"
#include "toplev.h"
#include "tree-chrec.h"
#include "tree-data-ref.h"
#include "tree-scalar-evolution.h"
#include "input.h"
#include "hashtab.h"
#include "tree-vectorizer.h"
#include "tree-pass.h"
#include "langhooks.h"

bool double_not_supported = false;

static tree
add_cli_function (const char *name,
                  tree type)
{
  tree   id = get_identifier (name);
  tree decl = build_decl (UNKNOWN_LOCATION, FUNCTION_DECL, id, type);

  TREE_PUBLIC (decl)         = 1;
  DECL_EXTERNAL (decl)       = 1;
/*  DECL_FUNCTION_CODE (decl)  = NULL;*/
  return decl;
}

tree cli_functions[1024];

static void 
create_cli_fn_table (void)
{
  
  cli_functions[0] = add_cli_function ("genvec_support_VQI_stride_System_UInt32", unsigned_type_node);
  cli_functions[1] = add_cli_function ("genvec_support_VHI_stride_System_UInt32", unsigned_type_node);
  cli_functions[2] = add_cli_function ("genvec_support_VSI_stride_System_UInt32", unsigned_type_node);
  cli_functions[3] = add_cli_function ("genvec_support_VDI_stride_System_UInt32", unsigned_type_node);
  cli_functions[4] = add_cli_function ("genvec_support_VSF_stride_System_UInt32", unsigned_type_node);
  cli_functions[5] = add_cli_function ("genvec_support_VDF_stride_System_UInt32", unsigned_type_node);

  cli_functions[6] = add_cli_function ("genvec_support_VQI_VQI_align_System_UInt32", unsigned_type_node);
  cli_functions[7] = add_cli_function ("genvec_support_VHI_VHI_align_System_UInt32", unsigned_type_node);
  cli_functions[8] = add_cli_function ("genvec_support_VSI_VSI_align_System_UInt32", unsigned_type_node);
  cli_functions[9] = add_cli_function ("genvec_support_VDI_VDI_align_System_UInt32", unsigned_type_node);
  cli_functions[10] = add_cli_function ("genvec_support_VSF_VSF_align_System_UInt32", unsigned_type_node);
  cli_functions[11] = add_cli_function ("genvec_support_VDF_VDF_align_System_UInt32", unsigned_type_node);

  cli_functions[12] = add_cli_function ("genvec_support_VQI_VQI_uniform_vec_System_SByte_Mono_Simd_Vector16sb", unsigned_type_node);
  cli_functions[13] = add_cli_function ("genvec_support_VHI_VHI_uniform_vec_System_Int16_Mono_Simd_Vector8s", unsigned_type_node);
  cli_functions[14] = add_cli_function ("genvec_support_VSI_VSI_uniform_vec_System_Int32_Mono_Simd_Vector4i", unsigned_type_node);
  cli_functions[15] = add_cli_function ("genvec_support_VDI_VDI_uniform_vec_System_Int64_Mono_Simd_Vector2l", unsigned_type_node);
  cli_functions[16] = add_cli_function ("genvec_support_VSF_VSF_uniform_vec_System_Single_Mono_Simd_Vector4f", unsigned_type_node);
  cli_functions[17] = add_cli_function ("genvec_support_VDF_VDF_uniform_vec_System_Double_Mono_Simd_Vector2d", unsigned_type_node);

  cli_functions[18] = add_cli_function ("genvec_support_VQI_VQI_affine_vec_System_SByte_System_SByte_Mono_Simd_Vector16sb", unsigned_type_node);
  cli_functions[19] = add_cli_function ("genvec_support_VHI_VHI_affine_vec_System_Int16_System_Int16_Mono_Simd_Vector8s", unsigned_type_node);
  cli_functions[20] = add_cli_function ("genvec_support_VSI_VSI_affine_vec_System_Int32_System_Int32_Mono_Simd_Vector4i", unsigned_type_node);
  cli_functions[21] = add_cli_function ("genvec_support_VDI_VDI_affine_vec_System_Int64_System_Int32_Mono_Simd_Vector2l", unsigned_type_node);
  cli_functions[22] = add_cli_function ("genvec_support_VSF_VSF_affine_vec_System_Single_Mono_Simd_Vector4f", unsigned_type_node);
  cli_functions[23] = add_cli_function ("genvec_support_VDF_VDF_affine_vec_System_Double_Mono_Simd_Vector2d", unsigned_type_node);

  cli_functions[24] = add_cli_function ("genvec_support_VQI_VQI_init_vec_System_SByte_System_SByte_Mono_Simd_Vector16sb", unsigned_type_node);
  cli_functions[25] = add_cli_function ("genvec_support_VHI_VHI_init_vec_System_Int16_System_Int16_Mono_Simd_Vector8s", unsigned_type_node);
  cli_functions[26] = add_cli_function ("genvec_support_VSI_VSI_init_vec_System_Int32_System_Int32_Mono_Simd_Vector4i", unsigned_type_node);
  cli_functions[27] = add_cli_function ("genvec_support_VDI_VDI_init_vec_System_Int64_System_Int32_Mono_Simd_Vector2l", unsigned_type_node);
  cli_functions[28] = add_cli_function ("genvec_support_VSF_VSF_init_vec_System_Single_Mono_Simd_Vector4f", unsigned_type_node);
  cli_functions[29] = add_cli_function ("genvec_support_VDF_VDF_init_vec_System_Double_Mono_Simd_Vector2d", unsigned_type_node);

  cli_functions[30] = add_cli_function ("genvec_support_VQI_VQI_reduc_max_epilogue_Mono_Simd_Vector16sb_Mono_Simd_Vector16sb", unsigned_type_node);
  cli_functions[31] = add_cli_function ("genvec_support_VHI_VHI_reduc_max_epilogue_Mono_Simd_Vector8s_Mono_Simd_Vector8s", unsigned_type_node);
  cli_functions[32] = add_cli_function ("genvec_support_VSI_VSI_reduc_max_epilogue_Mono_Simd_Vector4i_Mono_Simd_Vector4i", unsigned_type_node);
  cli_functions[33] = add_cli_function ("genvec_support_VDI_VDI_reduc_max_epilogue_Mono_Simd_Vector2l_Mono_Simd_Vector2l", unsigned_type_node);
  cli_functions[34] = add_cli_function ("genvec_support_VSF_VSF_reduc_max_epilogue_Mono_Simd_Vector4f_Mono_Simd_Vector4f", unsigned_type_node);
  cli_functions[35] = add_cli_function ("genvec_support_VDF_VDF_reduc_max_epilogue_Mono_Simd_Vector2f_Mono_Simd_Vector2f", unsigned_type_node);

  cli_functions[36] = add_cli_function ("genvec_support_VQI_VQI_reduc_min_epilogue_Mono_Simd_Vector16sb_Mono_Simd_Vector16sb", unsigned_type_node);
  cli_functions[37] = add_cli_function ("genvec_support_VHI_VHI_reduc_min_epilogue_Mono_Simd_Vector8s_Mono_Simd_Vector8s", unsigned_type_node);
  cli_functions[38] = add_cli_function ("genvec_support_VSI_VSI_reduc_min_epilogue_Mono_Simd_Vector4i_Mono_Simd_Vector4i", unsigned_type_node);
  cli_functions[39] = add_cli_function ("genvec_support_VDI_VDI_reduc_min_epilogue_Mono_Simd_Vector2l_Mono_Simd_Vector2l", unsigned_type_node);
  cli_functions[40] = add_cli_function ("genvec_support_VSF_VSF_reduc_min_epilogue_Mono_Simd_Vector4f_Mono_Simd_Vector4f", unsigned_type_node);
  cli_functions[41] = add_cli_function ("genvec_support_VDF_VDF_reduc_min_epilogue_Mono_Simd_Vector2f_Mono_Simd_Vector2f", unsigned_type_node);

  cli_functions[42] = add_cli_function ("genvec_support_VQI_VQI_reduc_plus_epilogue_Mono_Simd_Vector16sb_Mono_Simd_Vector16sb", unsigned_type_node);
  cli_functions[43] = add_cli_function ("genvec_support_VHI_VHI_reduc_plus_epilogue_Mono_Simd_Vector8s_Mono_Simd_Vector8s", unsigned_type_node);
  cli_functions[44] = add_cli_function ("genvec_support_VSI_VSI_reduc_plus_epilogue_Mono_Simd_Vector4i_Mono_Simd_Vector4i", unsigned_type_node);
  cli_functions[45] = add_cli_function ("genvec_support_VDI_VDI_reduc_plus_epilogue_Mono_Simd_Vector2l_Mono_Simd_Vector2l", unsigned_type_node);
  cli_functions[46] = add_cli_function ("genvec_support_VSF_VSF_reduc_plus_epilogue_Mono_Simd_Vector4f_Mono_Simd_Vector4f", unsigned_type_node);
  cli_functions[47] = add_cli_function ("genvec_support_VDF_VDF_reduc_plus_epilogue_Mono_Simd_Vector2f_Mono_Simd_Vector2f", unsigned_type_node);

  cli_functions[48] = add_cli_function ("genvec_support_VQI_RealignLoad_Mono_Simd_Vector16sb_Mono_Simd_Vector16sb_Mono_Simd_Vector16sb_Mono_Simd_Vector16sb__System_Int32_System_Int32_Mono_Simd_Vector16sb", unsigned_type_node);
  cli_functions[49] = add_cli_function ("genvec_support_VHI_RealignLoad_Mono_Simd_Vector8s_Mono_Simd_Vector8s_Mono_Simd_Vector8s_Mono_Simd_Vector8s__System_Int32_System_Int32_Mono_Simd_Vector8s", unsigned_type_node);
  cli_functions[50] = add_cli_function ("genvec_support_VSI_RealignLoad_Mono_Simd_Vector4i_Mono_Simd_Vector4i_Mono_Simd_Vector4i_Mono_Simd_Vector4i__System_Int32_System_Int32_Mono_Simd_Vector4i", unsigned_type_node);
  cli_functions[51] = add_cli_function ("genvec_support_VDI_RealignLoad_Mono_Simd_Vector2l_Mono_Simd_Vector2l_Mono_Simd_Vector2l_Mono_Simd_Vector2l__System_Int32_System_Int32_Mono_Simd_Vector2l", unsigned_type_node);
  cli_functions[52] = add_cli_function ("genvec_support_VSF_RealignLoad_Mono_Simd_Vector4f_Mono_Simd_Vector4f_Mono_Simd_Vector4f_Mono_Simd_Vector4f__System_Int32_System_Int32_Mono_Simd_Vector4f", unsigned_type_node);
  cli_functions[53] = add_cli_function ("genvec_support_VDF_RealignLoad_Mono_Simd_Vector2f_Mono_Simd_Vector2f_Mono_Simd_Vector2f_Mono_Simd_Vector2f__System_Int32_System_Int32_Mono_Simd_Vector2f", unsigned_type_node);

  cli_functions[54] = add_cli_function ("genvec_support_VQI_VI_get_loop_niters_System_Int32_System_Int32_System_Int32", unsigned_type_node);

  cli_functions[55] = add_cli_function ("genvec_support_VHI_VHI_dot_product_Mono_Simd_Vector16sb_Mono_Simd_Vector16sb_Mono_Simd_Vector8s_Mono_Simd_Vector8s", unsigned_type_node);
  cli_functions[56] = add_cli_function ("genvec_support_VSI_VSI_dot_product_Mono_Simd_Vector8s_Mono_Simd_Vector8s_Mono_Simd_Vector4i_Mono_Simd_Vector4i", unsigned_type_node);

  cli_functions[57] = add_cli_function ("genvec_support_VQI_VQI_realign_offset_System_UInt32", unsigned_type_node);
  cli_functions[58] = add_cli_function ("genvec_support_VHI_VHI_realign_offset_System_UInt32", unsigned_type_node);
  cli_functions[59] = add_cli_function ("genvec_support_VSI_VSI_realign_offset_System_UInt32", unsigned_type_node);
  cli_functions[60] = add_cli_function ("genvec_support_VDI_VHI_realign_offset_System_UInt32", unsigned_type_node);
  cli_functions[61] = add_cli_function ("genvec_support_VSF_VSF_realign_offset_System_UInt32", unsigned_type_node);
  cli_functions[62] = add_cli_function ("genvec_support_VDF_VDF_realign_offset_System_UInt32", unsigned_type_node);

  cli_functions[63] = add_cli_function ("genvec_support_VQI_VQI_mask_for_load_Mono_Simd_Vector16sb__System_Int32_System_Int32_Mono_Simd_Vector16sb", unsigned_type_node);
  cli_functions[64] = add_cli_function ("genvec_support_VHI_VHI_mask_for_load_Mono_Simd_Vector8s_System_UInt32_Mono_Simd_Vector8s", unsigned_type_node);
  cli_functions[65] = add_cli_function ("genvec_support_VSI_VSI_mask_for_load_Mono_Simd_Vector16sb_Mono_Simd_Vector16sb_Mono_Simd_Vector16sb", unsigned_type_node);
  cli_functions[66] = add_cli_function ("genvec_support_VDI_VDI_mask_for_load_System_UInt32_Mono_Simd_Vector2l_Mono_Simd_Vector2l", unsigned_type_node);
  cli_functions[67] = add_cli_function ("genvec_support_VSF_VSF_mask_for_load_System_UInt32_Mono_Simd_Vector4f_Mono_Simd_Vector4f", unsigned_type_node);
  cli_functions[68] = add_cli_function ("genvec_support_VDF_VDF_mask_for_load_System_UInt32_Mono_Simd_Vector2f_Mono_Simd_Vector2f", unsigned_type_node);

  cli_functions[69] = add_cli_function ("genvec_support_VQI_get_vec_size_System_UInt32", unsigned_type_node);
  cli_functions[70] = add_cli_function ("genvec_support_VHI_get_vec_size_System_UInt32", unsigned_type_node);
  cli_functions[71] = add_cli_function ("genvec_support_VSI_get_vec_size_System_UInt32", unsigned_type_node);
  cli_functions[72] = add_cli_function ("genvec_support_VDI_get_vec_size_System_UInt32", unsigned_type_node);
  cli_functions[73] = add_cli_function ("genvec_support_VSF_get_vec_size_System_UInt32", unsigned_type_node);
  cli_functions[74] = add_cli_function ("genvec_support_VDF_get_vec_size_System_UInt32", unsigned_type_node);

  cli_functions[75] = add_cli_function ("genvec_support_VQI_ALoad_Mono_Simd_Vector16sb__Mono_Simd_Vector16sb", unsigned_type_node);
  cli_functions[76] = add_cli_function ("genvec_support_VHI_ALoad_Mono_Simd_Vector8s__Mono_Simd_Vector8s", unsigned_type_node);
  cli_functions[77] = add_cli_function ("genvec_support_VSI_ALoad_Mono_Simd_Vector4i__Mono_Simd_Vector4i", unsigned_type_node);
  cli_functions[78] = add_cli_function ("genvec_support_VDI_ALoad_Mono_Simd_Vector2l__Mono_Simd_Vector2l", unsigned_type_node);
  cli_functions[79] = add_cli_function ("genvec_support_VSF_ALoad_Mono_Simd_Vector4f__Mono_Simd_Vector4f", unsigned_type_node);
  cli_functions[80] = add_cli_function ("genvec_support_VDF_ALoad_Mono_Simd_Vector2f__Mono_Simd_Vector2f", unsigned_type_node);


  cli_functions[81] = add_cli_function ("genvec_support_VSI_VSI_int_to_float_signed_Mono_Simd_Vector4i_Mono_Simd_Vector4f", unsigned_type_node);
  cli_functions[82] = add_cli_function ("genvec_support_VSI_VSI_int_to_float_unsigned_Mono_Simd_Vector4i_Mono_Simd_Vector4f", unsigned_type_node);
  cli_functions[83] = add_cli_function ("genvec_support_VSI_VSI_float_to_int_signed_Mono_Simd_Vector4f_Mono_Simd_Vector4i", unsigned_type_node);
  cli_functions[84] = add_cli_function ("genvec_support_VSI_VSI_float_to_int_signed_Mono_Simd_Vector4f_Mono_Simd_Vector4i", unsigned_type_node);

  cli_functions[85] = add_cli_function ("genvec_support_VQI_VQI_interleave_high_Mono_Simd_Vector16sb_Mono_Simd_Vector8s_Mono_Simd_Vector16sb", unsigned_type_node);
  cli_functions[86] = add_cli_function ("genvec_support_VHI_VHI_interleave_high_Mono_Simd_Vector8s_Mono_Simd_Vector8s_Mono_Simd_Vector8s", unsigned_type_node);
  cli_functions[87] = add_cli_function ("genvec_support_VSI_VSI_interleave_high_Mono_Simd_Vector4i_Mono_Simd_Vector4i_Mono_Simd_Vector4i", unsigned_type_node);

  cli_functions[88] = add_cli_function ("genvec_support_VQI_VQI_interleave_low_Mono_Simd_Vector16sb_Mono_Simd_Vector8s_Mono_Simd_Vector16sb", unsigned_type_node);
  cli_functions[89] = add_cli_function ("genvec_support_VHI_VHI_interleave_low_Mono_Simd_Vector8s_Mono_Simd_Vector8s_Mono_Simd_Vector8s", unsigned_type_node);
  cli_functions[90] = add_cli_function ("genvec_support_VSI_VSI_interleave_low_Mono_Simd_Vector4i_Mono_Simd_Vector4i_Mono_Simd_Vector4i", unsigned_type_node);

  cli_functions[91] = add_cli_function ("genvec_support_VQI_VQI_extract_even_Mono_Simd_Vector16sb_Mono_Simd_Vector8s_Mono_Simd_Vector16sb", unsigned_type_node);
  cli_functions[92] = add_cli_function ("genvec_support_VHI_VHI_extract_even_Mono_Simd_Vector8s_Mono_Simd_Vector8s_Mono_Simd_Vector8s", unsigned_type_node);
  cli_functions[93] = add_cli_function ("genvec_support_VSI_VSI_extract_even_Mono_Simd_Vector4i_Mono_Simd_Vector4i_Mono_Simd_Vector4i", unsigned_type_node);

  cli_functions[94] = add_cli_function ("genvec_support_VQI_VQI_extract_odd_Mono_Simd_Vector16sb_Mono_Simd_Vector8s_Mono_Simd_Vector16sb", unsigned_type_node);
  cli_functions[95] = add_cli_function ("genvec_support_VHI_VHI_extract_odd_Mono_Simd_Vector8s_Mono_Simd_Vector8s_Mono_Simd_Vector8s", unsigned_type_node);
  cli_functions[96] = add_cli_function ("genvec_support_VSI_VSI_extract_odd_Mono_Simd_Vector4i_Mono_Simd_Vector4i_Mono_Simd_Vector4i", unsigned_type_node);

  cli_functions[97] = add_cli_function ("genvec_support_VQI_VQI_pack_Mono_Simd_Vector8s_Mono_Simd_Vector8s_Mono_Simd_Vector16sb", unsigned_type_node);
  cli_functions[98] = add_cli_function ("genvec_support_VHI_VHI_pack_Mono_Simd_Vector4i_Mono_Simd_Vector4i_Mono_Simd_Vector8s", unsigned_type_node);

  cli_functions[99] = add_cli_function ("genvec_support_VHI_VHI_widen_mult_hi_Mono_Simd_Vector16sb_Mono_Simd_Vector8s_Mono_Simd_Vector8s", unsigned_type_node);
  cli_functions[100] = add_cli_function ("genvec_support_VSI_VSI_widen_mult_hi_Mono_Simd_Vector8s_Mono_Simd_Vector8s_Mono_Simd_Vector4i", unsigned_type_node);
  cli_functions[101] = add_cli_function ("genvec_support_VHI_VHI_widen_mult_lo_Mono_Simd_Vector16sb_Mono_Simd_Vector8s_Mono_Simd_Vector8s", unsigned_type_node);
  cli_functions[102] = add_cli_function ("genvec_support_VSI_VSI_widen_mult_lo_Mono_Simd_Vector8s_Mono_Simd_Vector8s_Mono_Simd_Vector4i", unsigned_type_node);
  cli_functions[103] = add_cli_function ("genvec_support_VSI_VSI_double_supported_System_SByte", unsigned_type_node);
  cli_functions[104] = add_cli_function ("genvec_support_VSI_VSI_unpack_high_Mono_Simd_Vector8s_Mono_Simd_Vector4i", unsigned_type_node);
  cli_functions[105] = add_cli_function ("genvec_support_VHI_VHI_unpack_high_Mono_Simd_Vector16sb_Mono_Simd_Vector8s", unsigned_type_node);
  cli_functions[106] = add_cli_function ("genvec_support_VSI_VSI_unpack_low_Mono_Simd_Vector8s_Mono_Simd_Vector4i", unsigned_type_node);
  cli_functions[107] = add_cli_function ("genvec_support_VHI_VHI_unpack_low_Mono_Simd_Vector16sb_Mono_Simd_Vector8s", unsigned_type_node);
  cli_functions[108] = add_cli_function ("genvec_support_VQI_VQI_shift_right_Mono_Simd_Vector16sb_Mono_Simd_Vector16sb_System_UInt32_Mono_Simd_Vector16sb", unsigned_type_node);
  cli_functions[109] = add_cli_function ("genvec_support_VHI_VHI_shift_right_Mono_Simd_Vector8s_Mono_Simd_Vector8s_System_UInt32_Mono_Simd_Vector8s", unsigned_type_node);
  cli_functions[110] = add_cli_function ("genvec_support_VSI_VSI_shift_right_Mono_Simd_Vector4i_Mono_Simd_Vector4i_System_UInt32_Mono_Simd_Vector4i", unsigned_type_node);
  cli_functions[111] = add_cli_function ("genvec_support_VQI_VQI_left_right_Mono_Simd_Vector16sb_Mono_Simd_Vector16sb_System_UInt32_Mono_Simd_Vector16sb", unsigned_type_node);
  cli_functions[112] = add_cli_function ("genvec_support_VHI_VHI_shift_left_Mono_Simd_Vector8s_Mono_Simd_Vector8s_System_UInt32_Mono_Simd_Vector8s", unsigned_type_node);
  cli_functions[113] = add_cli_function ("genvec_support_VSI_VSI_shift_left_Mono_Simd_Vector4i_Mono_Simd_Vector4i_System_UInt32_Mono_Simd_Vector4i", unsigned_type_node);

  cli_functions[114] = add_cli_function ("genvec_support_VSF_VSF_interleave_high_Mono_Simd_Vector4f_Mono_Simd_Vector4f_Mono_Simd_Vector4f", unsigned_type_node);
  cli_functions[115] = add_cli_function ("genvec_support_VDF_VDF_interleave_high_Mono_Simd_Vector2d_Mono_Simd_Vector2d_Mono_Simd_Vector2d", unsigned_type_node);
  cli_functions[116] = add_cli_function ("genvec_support_VSF_VSF_interleave_low_Mono_Simd_Vector4f_Mono_Simd_Vector4f_Mono_Simd_Vector4f", unsigned_type_node);
  cli_functions[117] = add_cli_function ("genvec_support_VDF_VDF_interleave_low_Mono_Simd_Vector2d_Mono_Simd_Vector2d_Mono_Simd_Vector2d", unsigned_type_node);
  cli_functions[118] = add_cli_function ("genvec_support_VSF_VSF_extract_even_Mono_Simd_Vector4f_Mono_Simd_Vector4f_Mono_Simd_Vector4f", unsigned_type_node);
  cli_functions[119] = add_cli_function ("genvec_support_VDF_VDF_extract_even_Mono_Simd_Vector2d_Mono_Simd_Vector2d_Mono_Simd_Vector2d", unsigned_type_node);
  cli_functions[120] = add_cli_function ("genvec_support_VSF_VSF_extract_odd_Mono_Simd_Vector4f_Mono_Simd_Vector4f_Mono_Simd_Vector4f", unsigned_type_node);
  cli_functions[121] = add_cli_function ("genvec_support_VDF_VDF_extract_odd_Mono_Simd_Vector2d_Mono_Simd_Vector2d_Mono_Simd_Vector2d", unsigned_type_node);

  cli_functions[122] = add_cli_function ("genvec_support_VQI_VQI_bit_field_ref_Mono_Simd_Vector16sb_System_UInt32_System_UInt32_System_Int8", unsigned_type_node);
  cli_functions[123] = add_cli_function ("genvec_support_VHI_VHI_bit_field_ref_Mono_Simd_Vector8s_System_UInt32_System_UInt32_System_Int16", unsigned_type_node);
  cli_functions[124] = add_cli_function ("genvec_support_VSI_VSI_bit_field_ref_Mono_Simd_Vector4i_System_UInt32_System_UInt32_System_Int32", unsigned_type_node);
  cli_functions[125] = add_cli_function ("genvec_support_VSF_VSF_bit_field_ref_Mono_Simd_Vector4f_System_UInt32_System_UInt32_System_Single", unsigned_type_node);
  cli_functions[126] = add_cli_function ("genvec_support_VDF_VDF_bit_field_ref_Mono_Simd_Vector2d_System_UInt32_System_UInt32_System_Double", unsigned_type_node);
}

#define MAX_CLI_FN 127 

static tree
get_vectype (tree scalar_type, bool *ignore_stmt)
{
  enum machine_mode inner_mode = TYPE_MODE (scalar_type);
  int nbytes = GET_MODE_SIZE (inner_mode);
  int nunits;
  tree vectype;

  if (scalar_type == double_type_node && double_not_supported)
    {
      *ignore_stmt = true;
      return NULL_TREE;
    }

  if (nbytes == 0 || nbytes >= UNITS_PER_SIMD_WORD (inner_mode))
    return NULL_TREE;

  /* FORNOW: Only a single vector size per mode (UNITS_PER_SIMD_WORD)
     is expected.  */
  nunits = UNITS_PER_SIMD_WORD (inner_mode) / nbytes;

  vectype = build_vector_type (scalar_type, nunits);

  if (!(vectype
        && (VECTOR_MODE_P (TYPE_MODE (vectype))
            || INTEGRAL_MODE_P (TYPE_MODE (vectype)))))
    return NULL_TREE;

  return vectype;
}

static void
finish_replacement (tree new_rhs, gimple stmt, gimple new_call_stmt)
{
  gimple_stmt_iterator gsi;
  gimple use_stmt;
  imm_use_iterator imm_iter;
  use_operand_p use_p;
  tree lhs = gimple_call_lhs (stmt);
  tree new_lhs = duplicate_ssa_name (lhs, NULL);
  gimple new_stmt;

  if (new_call_stmt)
    new_stmt = new_call_stmt;
  else
    new_stmt = gimple_build_assign (new_lhs, new_rhs);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "==> created new statement: ");
      print_gimple_stmt (dump_file, new_stmt, 0, TDF_SLIM);
      fprintf (dump_file, "\n");
    }

  if (new_call_stmt)
    gimple_call_set_lhs (new_stmt, new_lhs);
  else
    gimple_assign_set_lhs (new_stmt, new_lhs);

  gsi = gsi_for_stmt (stmt);
  gsi_insert_before (&gsi, new_stmt, GSI_NEW_STMT);
  gsi = gsi_for_stmt (stmt);
  gsi_remove (&gsi, true);

  mark_symbols_for_renaming (new_stmt);

  FOR_EACH_IMM_USE_STMT (use_stmt, imm_iter, lhs)
    FOR_EACH_IMM_USE_ON_STMT (use_p, imm_iter)
      SET_USE (use_p, new_lhs);
}

static bool
replace_init_builtin (gimple stmt)
{
  tree new_rhs, scalar_type, vectype, arg0, arg1, t;
  int i, nunits;
  bool ignore_stmt;

  arg0 = gimple_call_arg (stmt, 0);
  arg1 = gimple_call_arg (stmt, 1);
  scalar_type = TREE_TYPE (arg0);
  vectype = get_vectype (scalar_type, &ignore_stmt);
  if (!vectype)
    {
      if (!ignore_stmt)
        return false;

      new_rhs = integer_zero_node;
      finish_replacement (new_rhs, stmt, NULL);
      return true;
    }

  nunits = TYPE_VECTOR_SUBPARTS (vectype);
  t = NULL_TREE;
  for (i = nunits - 2; i >= 0; --i)
    t = tree_cons (NULL_TREE, unshare_expr (arg1), t);
  t = tree_cons (NULL_TREE, unshare_expr (arg0), t);
  if (CONSTANT_CLASS_P (arg0))
    new_rhs = build_vector (vectype, t);
  else
    new_rhs = build_constructor_from_list (vectype, t);
  finish_replacement (new_rhs, stmt, NULL);
  return true;
}

static bool
replace_get_vec_size_builtin (int index, gimple stmt)
{
  tree type, vectype;
  tree new_rhs;
  bool ignore_stmt;

  switch (index)
    {
      case get_vec_size_vqi:
        type = intQI_type_node;
        break;

      case get_vec_size_vhi:
        type = intHI_type_node;
        break;

      case get_vec_size_vsi:
        type = intSI_type_node;
        break;

      case get_vec_size_vdi:
        type = intDI_type_node;
        break;

      case get_vec_size_vsf:
        type = float_type_node;
        break;

      case get_vec_size_vdf:
        type = double_type_node;
        break;

      default:
        return false;
    }

  vectype = get_vectype (type, &ignore_stmt);
  if (!vectype)
    return false;

  new_rhs = fold_convert (unsigned_type_node, TYPE_SIZE_UNIT (vectype));
  finish_replacement (new_rhs, stmt, NULL);
  return true;
}

static bool
replace_aligned_load_builtin (int index, gimple stmt)
{
  tree scalar_type, vectype, new_rhs;
  tree dataref_ptr;
  bool ignore_stmt;

  switch (index)
    {
      case aligned_load_vsi:
        scalar_type = intSI_type_node;
        break;

      case aligned_load_vdi:
        scalar_type = intDI_type_node;
        break;

      case aligned_load_vhi:
        scalar_type = intHI_type_node;
        break;

      case aligned_load_vqi:
        scalar_type = intQI_type_node;
        break;

      case aligned_load_vsf:
        scalar_type = float_type_node;
        break;

      case aligned_load_vdf:
        scalar_type = double_type_node;
        break;

      default:
        return false;
    }

  vectype = get_vectype (scalar_type, &ignore_stmt);
  if (!vectype)
    return false;

  dataref_ptr = gimple_call_arg (stmt, 0);
  new_rhs = build1 (ALIGN_INDIRECT_REF, vectype, dataref_ptr);
  finish_replacement (new_rhs, stmt, NULL);
  return true;
}

static bool
replace_stride_builtin (int index, gimple stmt)
{
  tree scalar_type, vectype, new_rhs;
  bool ignore_stmt;

  switch (index)
    {
      case stride_vsi:
        scalar_type = intSI_type_node;
        break;

      case stride_vdi:
        scalar_type = intDI_type_node;
        break;

      case stride_vhi:
        scalar_type = intHI_type_node;
        break;
        
      case stride_vqi:
        scalar_type = intQI_type_node;
        break;

      case stride_vsf:
        scalar_type = float_type_node;
        break;
     
      case stride_vdf:
        scalar_type = double_type_node;
        break;

      default:
        return false;
    }

  vectype = get_vectype (scalar_type, &ignore_stmt);
  if (!vectype)
    return false;
  new_rhs = build_int_cst (unsigned_type_node, TYPE_VECTOR_SUBPARTS (vectype));
  finish_replacement (new_rhs, stmt, NULL);
  return true;
}

static bool
replace_align_builtin (int index, gimple stmt)
{
  tree new_rhs, scalar_type, vectype;
  bool ignore_stmt;

  switch (index)
    {
      case align_vsi:
        scalar_type = intSI_type_node;
        break;

      case align_vdi:
        scalar_type = intDI_type_node;
        break;

      case align_vhi:
        scalar_type = intHI_type_node;
        break;

      case align_vqi:
        scalar_type = intQI_type_node;
        break;

      case align_vsf:
        scalar_type = float_type_node;
        break;

      case align_vdf:
        scalar_type = double_type_node;
        break;

      default:
        return false;
    }

  vectype = get_vectype (scalar_type, &ignore_stmt);
  if (!vectype)
    return false;
  new_rhs = build_int_cst (unsigned_type_node, TYPE_ALIGN (vectype)/BITS_PER_UNIT);
  finish_replacement (new_rhs, stmt, NULL);
  return true;
}

static bool
replace_uniform_builtin (gimple stmt)
{
  tree new_rhs, scalar_type, vectype, arg, t;
  int i, nunits;
  bool ignore_stmt;

  arg = gimple_call_arg (stmt, 0);
  scalar_type = TREE_TYPE (arg);
  vectype = get_vectype (scalar_type, &ignore_stmt);
  if (!vectype)
    return false;

  nunits = TYPE_VECTOR_SUBPARTS (vectype);
  t = NULL_TREE;
  for (i = 0; i < nunits; i++)
    t = tree_cons (NULL_TREE, unshare_expr (arg), t);
  if (CONSTANT_CLASS_P (arg))
    new_rhs = build_vector (vectype, t);
  else
    new_rhs = build_constructor_from_list (vectype, t);
  finish_replacement (new_rhs, stmt, NULL);
  return true;
}

static bool
replace_affine_builtin (gimple stmt)
{
  tree new_rhs, scalar_type, vectype, arg0, arg1, t, new_name, var;
  int i, nunits;
  gimple init_stmt;
  gimple_stmt_iterator gsi;
  bool ignore_stmt;

  arg0 = gimple_call_arg (stmt, 0);
  arg1 = gimple_call_arg (stmt, 1);
  scalar_type = TREE_TYPE (arg0);
  vectype = get_vectype (scalar_type, &ignore_stmt);
  if (!vectype)
    return false;

  nunits = TYPE_VECTOR_SUBPARTS (vectype);
  t = NULL_TREE;
  new_name = arg0;
  t = tree_cons (NULL_TREE, new_name, t);
  var = create_tmp_var (scalar_type, "var_");
  add_referenced_var (var);
  for (i = 1; i < nunits; i++)
    {
      enum tree_code code = POINTER_TYPE_P (scalar_type)
                               ? POINTER_PLUS_EXPR : PLUS_EXPR;
      init_stmt = gimple_build_assign_with_ops (code, var,
                                                new_name, arg1);
      new_name = make_ssa_name (var, init_stmt);
      gimple_assign_set_lhs (init_stmt, new_name);
      gsi = gsi_for_stmt (stmt);
      gsi_insert_before (&gsi, init_stmt, GSI_NEW_STMT);
      t = tree_cons (NULL_TREE, new_name, t);
    }

  new_rhs = build_constructor_from_list (vectype, nreverse (t));
  finish_replacement (new_rhs, stmt, NULL);
  return true;
}

static bool
replace_reduc_epilog_builtin (int index, gimple stmt)
{
  enum tree_code reduc_code;
  tree new_rhs, vectype, arg;
  enum tree_code shift_code = ERROR_MARK;
  int bit_offset;
  tree scalar_type;
  tree bitsize;
  int element_bitsize;
  int vec_size_in_bits;
  tree vec_temp, vec_dest, new_temp, new_name, new_scalar_dest, rhs;
  gimple epilog_stmt;
  gimple_stmt_iterator gsi;
  optab reduc_optab;
  enum machine_mode vec_mode;
  enum tree_code code;
  gimple use_stmt;
  imm_use_iterator imm_iter;
  use_operand_p use_p;

  arg = gimple_call_arg (stmt, 0);
  vectype = TREE_TYPE (arg);
  vec_mode = TYPE_MODE (vectype);

  switch (index)
    {
      case reduc_max_vsi:
        scalar_type = integer_type_node;
        reduc_code = REDUC_MAX_EXPR;
        code = MAX_EXPR;
        break;
      case reduc_max_vdi:
        scalar_type = long_long_integer_type_node;
        reduc_code = REDUC_MAX_EXPR;
        code = MAX_EXPR;
        break;
      case reduc_max_vdf:
        scalar_type = double_type_node;
        reduc_code = REDUC_MAX_EXPR;
        code = MAX_EXPR;
        break;
      case reduc_max_vhi:
        scalar_type = short_integer_type_node;
        reduc_code = REDUC_MAX_EXPR;
        code = MAX_EXPR;
        break;
      case reduc_max_vqi:
        scalar_type = char_type_node;
        reduc_code = REDUC_MAX_EXPR;
        code = MAX_EXPR;
        break;
      case reduc_max_vsf:
        scalar_type = float_type_node;
        reduc_code = REDUC_MAX_EXPR;
        code = MAX_EXPR;
        break;

      case reduc_min_vsi:
        scalar_type = integer_type_node;
        reduc_code = REDUC_MIN_EXPR;
        code = MIN_EXPR;
        break;
      case reduc_min_vdi:
        scalar_type = long_long_integer_type_node;
        reduc_code = REDUC_MIN_EXPR;
        code = MIN_EXPR;
        break;
      case reduc_min_vdf:
        scalar_type = double_type_node;
        reduc_code = REDUC_MIN_EXPR;
        code = MIN_EXPR;
        break;
      case reduc_min_vhi:
        scalar_type = short_integer_type_node;
        reduc_code = REDUC_MIN_EXPR;
        code = MIN_EXPR;
        break;
      case reduc_min_vqi:
        scalar_type = char_type_node;
        reduc_code = REDUC_MIN_EXPR;
        code = MIN_EXPR;
        break;
      case reduc_min_vsf:
        scalar_type = float_type_node;
        reduc_code = REDUC_MIN_EXPR;
        code = MIN_EXPR;
        break;

      case reduc_plus_vsi:
        scalar_type = integer_type_node;
        reduc_code = REDUC_PLUS_EXPR;
        code = PLUS_EXPR;
        break;
      case reduc_plus_vdi:
        scalar_type = long_long_integer_type_node;
        reduc_code = REDUC_PLUS_EXPR;
        code = PLUS_EXPR;
        break;
      case reduc_plus_vdf:
        scalar_type = double_type_node;
        reduc_code = REDUC_PLUS_EXPR;
        code = PLUS_EXPR;
        break;
      case reduc_plus_vhi:
        scalar_type = short_integer_type_node;
        reduc_code = REDUC_PLUS_EXPR;
        code = PLUS_EXPR;
        break;
      case reduc_plus_vqi:
        scalar_type = char_type_node;
        reduc_code = REDUC_PLUS_EXPR;
        code = PLUS_EXPR;
        break;
      case reduc_plus_vsf:
        scalar_type = float_type_node;
        reduc_code = REDUC_PLUS_EXPR;
        code = PLUS_EXPR;
        break;

      default:
        return false;
    }

  bitsize = TYPE_SIZE (scalar_type);
  element_bitsize = tree_low_cst (bitsize, 1);
  vec_size_in_bits = tree_low_cst (TYPE_SIZE (vectype), 1);

  gsi = gsi_for_stmt (stmt);

  gcc_assert (reduc_code < NUM_TREE_CODES);
  reduc_optab = optab_for_tree_code (reduc_code, vectype,
                                         optab_default);
  if (reduc_optab 
      && optab_handler (reduc_optab, vec_mode)->insn_code
          != CODE_FOR_nothing) 
    {
      /*** Case 1:  Create:
           v_out2 = reduc_expr <v_out1>  */

      new_rhs = build1 (reduc_code, vectype, arg);
      finish_replacement (new_rhs, stmt, NULL);
      return true;
    }

  if (optab_handler (vec_shr_optab, vec_mode)->insn_code != CODE_FOR_nothing)
    {
      shift_code = VEC_RSHIFT_EXPR;
          
      /*** Case 2: Create:
        for (offset = VS/2; offset >= element_size; offset/=2)
          {
            Create:  va' = vec_shift <va, offset>
            Create:  va = vop <va, va'>
          }  */

      vec_dest = create_tmp_var (vectype, "var_");
      add_referenced_var (vec_dest);
      DECL_GIMPLE_REG_P (vec_dest) = true;
      new_temp = arg;

      for (bit_offset = vec_size_in_bits/2;
           bit_offset >= element_bitsize;
           bit_offset /= 2)
        {
          tree bitpos = size_int (bit_offset);

          epilog_stmt = gimple_build_assign_with_ops (shift_code, vec_dest,
                                                      new_temp, bitpos);
          new_name = make_ssa_name (vec_dest, epilog_stmt);
          gimple_assign_set_lhs (epilog_stmt, new_name);
          gsi_insert_before (&gsi, epilog_stmt, GSI_SAME_STMT);

          epilog_stmt = gimple_build_assign_with_ops (code, vec_dest,
                                                      new_name, new_temp);
          new_temp = make_ssa_name (vec_dest, epilog_stmt);
          gimple_assign_set_lhs (epilog_stmt, new_temp);
          gsi_insert_before (&gsi, epilog_stmt, GSI_SAME_STMT);
        }

      finish_replacement (new_temp, stmt, NULL);
      return true;
    }

  /*** Case 3: Create:
    s = extract_field <v_out2, 0>
    for (offset = element_size;
         offset < vector_size;
         offset += element_size;)
     {
        Create:  s' = extract_field <v_out2, offset>
        Create:  s = op <s, s'>
     }  */

  vec_temp = arg;
  vec_size_in_bits = tree_low_cst (TYPE_SIZE (vectype), 1);
  new_scalar_dest = create_tmp_var (scalar_type, "var_");
  add_referenced_var (new_scalar_dest);
  rhs = build3 (BIT_FIELD_REF, scalar_type, vec_temp, bitsize,
                bitsize_zero_node);
  epilog_stmt = gimple_build_assign (new_scalar_dest, rhs);
  new_temp = make_ssa_name (new_scalar_dest, epilog_stmt);
  gimple_assign_set_lhs (epilog_stmt, new_temp);
  gsi_insert_before (&gsi, epilog_stmt, GSI_SAME_STMT);

  for (bit_offset = element_bitsize;
       bit_offset < vec_size_in_bits;
       bit_offset += element_bitsize)
    {
      tree bitpos = bitsize_int (bit_offset);
      tree rhs = build3 (BIT_FIELD_REF, scalar_type, vec_temp, bitsize,
                         bitpos);

      epilog_stmt = gimple_build_assign (new_scalar_dest, rhs);
      new_name = make_ssa_name (new_scalar_dest, epilog_stmt);
      gimple_assign_set_lhs (epilog_stmt, new_name);
      gsi_insert_before (&gsi, epilog_stmt, GSI_SAME_STMT);

      epilog_stmt = gimple_build_assign_with_ops (code, new_scalar_dest,
                                                  new_name, new_temp);
      new_temp = make_ssa_name (new_scalar_dest, epilog_stmt);
      gimple_assign_set_lhs (epilog_stmt, new_temp);
      gsi_insert_before (&gsi, epilog_stmt, GSI_SAME_STMT);
    }

  FOR_EACH_IMM_USE_STMT (use_stmt, imm_iter, gimple_call_lhs (stmt))
    FOR_EACH_IMM_USE_ON_STMT (use_p, imm_iter)
      SET_USE (use_p, new_temp);
  
  gsi = gsi_for_stmt (stmt);
  gsi_remove (&gsi, true);
 
  return true;
}

VEC (gimple, heap) *removed_stmts = NULL;
static bool
replace_realign_load_builtin (int index, gimple stmt)
{
  enum machine_mode mode;
  tree scalar_type, vectype, new_rhs;
  tree msq, lsq, realignment_token, dataref_ptr;
  tree misalign, alignment;
  tree tmis;
  bool ignore_stmt;

  switch (index)
    {
      case realign_load_vsi:
        scalar_type = intSI_type_node;
        break;

      case realign_load_vdi:
        scalar_type = intSI_type_node;
        break;

      case realign_load_vhi:
        scalar_type = intHI_type_node;
        break;

      case realign_load_vqi:
        scalar_type = intQI_type_node;
        break;

      case realign_load_vsf:
        scalar_type = float_type_node;
        break;

      case realign_load_vdf:
        scalar_type = float_type_node;
        break;


      default:
        return false;
    }

  vectype = get_vectype (scalar_type, &ignore_stmt);
  if (!vectype)
    return false;

  mode = TYPE_MODE (vectype);
  if (optab_handler (vec_realign_load_optab, mode)->insn_code 
       != CODE_FOR_nothing)
    {
      msq = gimple_call_arg (stmt, 0);
      lsq = gimple_call_arg (stmt, 1);
      realignment_token = gimple_call_arg (stmt, 2);
      new_rhs = build3 (REALIGN_LOAD_EXPR, vectype, msq, lsq,
                        realignment_token);
      finish_replacement (new_rhs, stmt, NULL);
      return true;
    }
   
  if (optab_handler (movmisalign_optab, mode)->insn_code != CODE_FOR_nothing)
    { 
      gimple def_stmt;
      gimple_stmt_iterator gsi;

      misalign = fold_convert (sizetype, gimple_call_arg (stmt, 4));
      alignment = gimple_call_arg (stmt, 5);

      if (!TREE_INT_CST_LOW (alignment))
        tmis = size_zero_node;
      else
        {
          alignment = size_int (TYPE_ALIGN (vectype)/BITS_PER_UNIT);
          tmis = size_binop (FLOOR_MOD_EXPR, misalign, alignment);
        }

      tmis = size_binop (MULT_EXPR, tmis, size_int(BITS_PER_UNIT));
      dataref_ptr = gimple_call_arg (stmt, 3);
      new_rhs = build2 (MISALIGNED_INDIRECT_REF, vectype, dataref_ptr, tmis);
      finish_replacement (new_rhs, stmt, NULL);

      realignment_token = gimple_call_arg (stmt, 2);
      def_stmt = SSA_NAME_DEF_STMT (realignment_token);
      if (def_stmt && is_gimple_assign (def_stmt)
          && gimple_assign_rhs_code (def_stmt) == VIEW_CONVERT_EXPR)
        {
          int i;
          gimple rem_stmt;
          bool removed = false;

          if (!removed_stmts)
            removed_stmts = VEC_alloc (gimple, heap, 3);
          else
            {
              for (i = 0; VEC_iterate (gimple, removed_stmts, i, rem_stmt); i++)
                if (def_stmt == rem_stmt)
                  {
                    removed = true;
                    break;
                  }
            }
          
          if (!removed)
            {
              VEC_safe_push (gimple, heap, removed_stmts, def_stmt);
              gsi = gsi_for_stmt (def_stmt);
              gsi_remove (&gsi, true);
            }
        }

      return true;
    }

  return false;
}

static bool
replace_get_loop_niters_builtin (gimple stmt)
{
  tree new_rhs;

  new_rhs = gimple_call_arg (stmt, 0);
  finish_replacement (new_rhs, stmt, NULL);
  return true;
}

static bool
replace_dot_product_builtin (gimple stmt)
{
  tree vectype = TREE_TYPE (gimple_call_lhs (stmt));
  tree new_rhs;
  enum machine_mode mode = TYPE_MODE (vectype);

  optab dot_prod_optab =
    optab_for_tree_code (DOT_PROD_EXPR, vectype, optab_default);
  if (!dot_prod_optab 
      || optab_handler (dot_prod_optab, mode)->insn_code == CODE_FOR_nothing)
    return false; 

  new_rhs = build3 (DOT_PROD_EXPR, vectype, gimple_call_arg (stmt, 0),
                    gimple_call_arg (stmt, 1), gimple_call_arg (stmt, 2));
  finish_replacement (new_rhs, stmt, NULL);
  return true;
}

static bool
replace_realign_offset_builtin (int index, gimple stmt)
{
  tree scalar_type, vectype, new_rhs;
  bool ignore_stmt;

  switch (index)
    {
      case realign_offset_vsi:
        scalar_type = intSI_type_node;
        break;

      case realign_offset_vdi:
        scalar_type = intDI_type_node;
        break;

      case realign_offset_vhi:
        scalar_type = intHI_type_node;
        break;

      case realign_offset_vqi:
        scalar_type = intQI_type_node;
        break;

      case realign_offset_vsf:
        scalar_type = float_type_node;
        break;

      case realign_offset_vdf:
        scalar_type = double_type_node;
        break;

      default:
        return false;
    }

  vectype = get_vectype (scalar_type, &ignore_stmt);
  if (!vectype)
    return false;

  new_rhs = build_int_cst (TREE_TYPE (gimple_call_lhs (stmt)), 
                           TYPE_VECTOR_SUBPARTS (vectype) - 1);
  finish_replacement (new_rhs, stmt, NULL);
  return true;
}

static bool
replace_mask_for_load_builtin (gimple stmt)
{
  gimple_stmt_iterator gsi;
  tree builtin_decl;

  if (targetm.vectorize.builtin_mask_for_load
      && (builtin_decl = targetm.vectorize.builtin_mask_for_load()))
    {
      gimple new_stmt = gimple_build_call (builtin_decl, 1, 
                                           gimple_call_arg (stmt, 0));
      finish_replacement (NULL, stmt, new_stmt);
      return true;
    }

  gsi = gsi_for_stmt (stmt);
  gsi_remove (&gsi, true);
  return true;
}

static bool
replace_int_float_cvt (int index, gimple stmt)
{
  enum tree_code code;
  tree integral_type, builtin_decl, vectype;
  gimple new_stmt;
  bool ignore_stmt;

  switch (index)
    {
      case int2float_signed:
        code = FLOAT_EXPR;
        integral_type = intSI_type_node;
        break; 

      case int2float_unsigned:
        code = FLOAT_EXPR;
        integral_type = unsigned_intSI_type_node;
        break;

      case float2int_signed:
        code = FIX_TRUNC_EXPR;
        integral_type = intSI_type_node;
        break;

      case float2int_unsigned:
        code = FIX_TRUNC_EXPR;
        integral_type = unsigned_intSI_type_node;
        break;

      default:
        return false;
    }

  vectype = get_vectype (integral_type, &ignore_stmt);
  if (!vectype)
    return false;

  if (!targetm.vectorize.builtin_conversion
      || !(builtin_decl = targetm.vectorize.builtin_conversion 
                                              (code, vectype)))
    return false; 
   
  new_stmt = gimple_build_call (builtin_decl, 1, gimple_call_arg (stmt, 0));
  finish_replacement (NULL, stmt, new_stmt);
  return true;
}

static bool
replace_interleaving (int index, gimple stmt)
{
  optab optab;
  int mode;
  enum tree_code code;
  tree scalar_type, vectype, new_rhs;
  bool ignore_stmt;

  switch (index)
    {
      case interleave_high_vqi:
	code = VEC_INTERLEAVE_HIGH_EXPR;
        scalar_type = intQI_type_node;
        break;

      case interleave_high_vhi:
	code = VEC_INTERLEAVE_HIGH_EXPR;
        scalar_type = intHI_type_node;
        break;

      case interleave_high_vsi:
	code = VEC_INTERLEAVE_HIGH_EXPR;
        scalar_type = intSI_type_node;
        break;

      case interleave_low_vqi:
	code = VEC_INTERLEAVE_LOW_EXPR;
        scalar_type = intQI_type_node;
        break;

      case interleave_low_vhi:
	code = VEC_INTERLEAVE_LOW_EXPR;
        scalar_type = intHI_type_node;
        break;

      case interleave_low_vsi:
	code = VEC_INTERLEAVE_LOW_EXPR;
        scalar_type = intSI_type_node;
        break;

      case extract_even_vqi:
	code = VEC_EXTRACT_EVEN_EXPR;
        scalar_type = intQI_type_node;
        break;

      case extract_even_vhi:
	code = VEC_EXTRACT_EVEN_EXPR;
        scalar_type = intHI_type_node;
        break;

      case extract_even_vsi:
	code = VEC_EXTRACT_EVEN_EXPR;
        scalar_type = intSI_type_node;
        break;

      case extract_odd_vqi:
	code = VEC_EXTRACT_ODD_EXPR;
        scalar_type = intQI_type_node;
        break;

      case extract_odd_vhi:
	code = VEC_EXTRACT_ODD_EXPR;
        scalar_type = intHI_type_node;
        break;

      case extract_odd_vsi:
	code = VEC_EXTRACT_ODD_EXPR;
        scalar_type = intSI_type_node;
        break;

      case interleave_high_vsf:
        code = VEC_INTERLEAVE_HIGH_EXPR;
        scalar_type = float_type_node;
        break;

      case interleave_high_vdf:
        code = VEC_INTERLEAVE_HIGH_EXPR;
        scalar_type = double_type_node;
        break;

      case interleave_low_vsf:
        code = VEC_INTERLEAVE_LOW_EXPR;
        scalar_type = float_type_node;
        break;

      case interleave_low_vdf:
        code = VEC_INTERLEAVE_LOW_EXPR;
        scalar_type = double_type_node;
        break;

      case extract_even_vsf:
        code = VEC_EXTRACT_EVEN_EXPR;
        scalar_type = float_type_node;
        break;

      case extract_even_vdf:
        code = VEC_EXTRACT_EVEN_EXPR;
        scalar_type = double_type_node;
        break;

      case extract_odd_vsf:
        code = VEC_EXTRACT_ODD_EXPR;
        scalar_type = float_type_node;
        break;

      case extract_odd_vdf:
        code = VEC_EXTRACT_ODD_EXPR;
        scalar_type = double_type_node;
        break;

      default:
        return false;
    }

  vectype = get_vectype (scalar_type, &ignore_stmt);
  if (!vectype)
    return false;

  mode = (int) TYPE_MODE (vectype);

  optab = optab_for_tree_code (code, vectype, optab_default);
  if (!optab || optab_handler (optab, mode)->insn_code == CODE_FOR_nothing)
    return false;

  new_rhs = build2 (code, vectype, gimple_call_arg (stmt, 0), 
                                   gimple_call_arg (stmt, 1));
  finish_replacement (new_rhs, stmt, NULL);
  return true;
}

static bool
replace_pack (int index, gimple stmt)
{
  enum tree_code code = VEC_PACK_TRUNC_EXPR;
  tree scalar_type, vectype, new_rhs;
  optab optab;
  int mode;
  bool ignore_stmt;
 
  switch (index)
    {
      case pack_vqi:
        scalar_type = intHI_type_node;
        break;

      case pack_vhi:
        scalar_type = intSI_type_node;
        break;

      default:
        return false;
    }

  vectype = get_vectype (scalar_type, &ignore_stmt);
  if (!vectype)
    return false;

  mode = (int) TYPE_MODE (vectype);

  optab = optab_for_tree_code (code, vectype, optab_default);
  if (!optab || optab_handler (optab, mode)->insn_code == CODE_FOR_nothing)
    return false;

  new_rhs = build2 (code, vectype, gimple_call_arg (stmt, 0),
                                   gimple_call_arg (stmt, 1));
  finish_replacement (new_rhs, stmt, NULL);
  return true;
}

static bool
replace_widen_mult (int index, gimple stmt)
{
  enum tree_code code;
  tree scalar_type, vectype, new_rhs;
  optab optab;
  int mode;
  bool ignore_stmt;

  switch (index)
    {
      case widen_mult_hi_vhi:
        code = VEC_WIDEN_MULT_HI_EXPR;
        scalar_type = intQI_type_node;
        break;

      case widen_mult_hi_vsi:
        code = VEC_WIDEN_MULT_HI_EXPR;
        scalar_type = intHI_type_node;
        break;

      case widen_mult_lo_vhi:
        code = VEC_WIDEN_MULT_LO_EXPR;
        scalar_type = intQI_type_node;
        break;

      case widen_mult_lo_vsi:
        code = VEC_WIDEN_MULT_LO_EXPR;
        scalar_type = intHI_type_node;
        break;

      default:
        return false;
    }

  vectype = get_vectype (scalar_type, &ignore_stmt);
  if (!vectype)
    return false;

  mode = (int) TYPE_MODE (vectype);

  optab = optab_for_tree_code (code, vectype, optab_default);
  if (!optab || optab_handler (optab, mode)->insn_code == CODE_FOR_nothing)
    return false;

  new_rhs = build2 (code, vectype, gimple_call_arg (stmt, 0),
                                   gimple_call_arg (stmt, 1));
  finish_replacement (new_rhs, stmt, NULL);
  return true;
}


static bool
replace_shift (int index, gimple stmt)
{
  enum tree_code code;
  tree scalar_type, vectype, new_rhs;
  optab optab;
  int mode;
  tree scalar_arg = gimple_call_arg (stmt, 2);
  tree vector_arg = gimple_call_arg (stmt, 1);
  gimple def_stmt;
  bool invariant = true;
  tree shift_arg = NULL_TREE;
  bool ignore_stmt;

  switch (index)
    {
      case shift_right_vqi:
        code = RSHIFT_EXPR;
        scalar_type = intQI_type_node;
        break;

      case shift_right_vhi:
        code = RSHIFT_EXPR;
        scalar_type = intHI_type_node;
        break;

      case shift_right_vsi:
        code = RSHIFT_EXPR;
        scalar_type = intSI_type_node;
        break;

      case shift_left_vqi:
        code = LSHIFT_EXPR;
        scalar_type = intQI_type_node;
        break;

      case shift_left_vhi:
        code = LSHIFT_EXPR;
        scalar_type = intHI_type_node;
        break;

      case shift_left_vsi:
        code = LSHIFT_EXPR;
        scalar_type = intSI_type_node;
        break;

      default:
        return false;
    }

  vectype = get_vectype (scalar_type, &ignore_stmt);
  if (!vectype)
    return false;

  mode = (int) TYPE_MODE (vectype);

  if (TREE_CODE (vector_arg) == SSA_NAME)
    {
      def_stmt = SSA_NAME_DEF_STMT (vector_arg);
      if (gimple_bb (stmt) == gimple_bb (def_stmt))
        invariant = false; 
    }

  if (invariant)
    {
      optab = optab_for_tree_code (code, vectype, optab_scalar);
      if (optab
          && (optab_handler (optab, TYPE_MODE (vectype))->insn_code
              != CODE_FOR_nothing)) 
        shift_arg = scalar_arg;
      else
        {
          optab = optab_for_tree_code (code, vectype, optab_vector);
          if (optab
              && (optab_handler (optab, TYPE_MODE (vectype))->insn_code
                  != CODE_FOR_nothing))
            shift_arg = vector_arg;
        }
    }
  else
    {
      optab = optab_for_tree_code (code, vectype, optab_vector);
      if (optab
          && (optab_handler (optab, TYPE_MODE (vectype))->insn_code
              != CODE_FOR_nothing)) 
        shift_arg = vector_arg;
    }

  if (!shift_arg)
    return false;

  new_rhs = build2 (code, vectype, gimple_call_arg (stmt, 0), shift_arg);
  finish_replacement (new_rhs, stmt, NULL);
  return true;
}

static bool
replace_double_supported (gimple stmt)
{
  tree vectype, new_rhs;
  bool ignore_stmt;

  vectype = get_vectype (double_type_node, &ignore_stmt);
  if (vectype)
    new_rhs = build_int_cst (signed_char_type_node, 1);
  else
    new_rhs = build_int_cst (signed_char_type_node, 0);

  finish_replacement (new_rhs, stmt, NULL);
  double_not_supported = true;
  return true;
}

static bool
replace_unpack (int index, gimple stmt)
{
  enum tree_code code;
  tree scalar_type, vectype, new_rhs;
  optab optab;
  int mode;
  bool ignore_stmt;

  switch (index)
    {
      case unpack_hi_vqi:
        code = VEC_UNPACK_HI_EXPR;
        scalar_type = intQI_type_node;
        break;

      case unpack_hi_vhi:
        code = VEC_UNPACK_HI_EXPR;
        scalar_type = intHI_type_node;
        break;

      case unpack_lo_vqi:
        code = VEC_UNPACK_LO_EXPR;
        scalar_type = intQI_type_node;
        break;

      case unpack_lo_vhi:
        code = VEC_UNPACK_LO_EXPR;
        scalar_type = intHI_type_node;
        break;

      default:
        return false;
    }

  vectype = get_vectype (scalar_type, &ignore_stmt);
  if (!vectype)
    return false;

  mode = (int) TYPE_MODE (vectype);

  optab = optab_for_tree_code (code, vectype, optab_default);
  if (!optab || optab_handler (optab, mode)->insn_code == CODE_FOR_nothing)
    return false;

  new_rhs = build1 (code, vectype, gimple_call_arg (stmt, 0));
  finish_replacement (new_rhs, stmt, NULL);
  return true;
}


static bool
replace_bit_field_ref (int index, gimple stmt)
{
  tree scalar_type, vectype, new_rhs, bitpos, bitsize;
  bool ignore_stmt;

  switch (index)
    {
      case bit_field_ref_vqi:
        scalar_type = intQI_type_node;
        break;

      case bit_field_ref_vhi:
        scalar_type = intHI_type_node;
        break;

      case bit_field_ref_vsi:
        scalar_type = intSI_type_node;
        break;

      case bit_field_ref_vsf:
        scalar_type = float_type_node;
        break;

      case bit_field_ref_vdf:
        scalar_type = double_type_node;
        break;
     
      default:
        return false;
    }

  vectype = get_vectype (scalar_type, &ignore_stmt);
  if (!vectype)
    return false;

  bitsize = TYPE_SIZE (scalar_type);
  gcc_assert (!tree_int_cst_compare (bitsize, gimple_call_arg (stmt, 1)));
  bitpos = gimple_call_arg (stmt, 2);

  if (BYTES_BIG_ENDIAN)
    bitpos = size_binop (MULT_EXPR,
                         bitsize_int (TYPE_VECTOR_SUBPARTS (vectype) - 1 -
                                      TREE_INT_CST_LOW (bitpos)),
                         TYPE_SIZE (scalar_type));

  new_rhs = build3 (BIT_FIELD_REF, scalar_type, gimple_call_arg (stmt, 0), 
                    bitsize, fold_convert (bitsizetype, bitpos));
  finish_replacement (new_rhs, stmt, NULL);
  return true;
}

static bool
replace_cli_fn (int index, gimple stmt)
{
  switch (index)
    {
      case stride_vsi:
      case stride_vhi:
      case stride_vqi:
      case stride_vsf:
      case stride_vdf:
      case stride_vdi:
        return replace_stride_builtin (index, stmt);

      case align_vsi:
      case align_vhi:
      case align_vqi:
      case align_vsf:
      case align_vdf:
      case align_vdi:
        return replace_align_builtin (index, stmt);

      case uniform_vsi:
      case uniform_vhi:
      case uniform_vqi:
      case uniform_vsf:
      case uniform_vdf:
      case uniform_vdi:
        return replace_uniform_builtin (stmt);

      case affine_vsi:
      case affine_vhi:
      case affine_vqi:
      case affine_vsf:
      case affine_vdf:
      case affine_vdi:
        return replace_affine_builtin (stmt);

      case init_vsi:
      case init_vhi:
      case init_vqi:
      case init_vsf:
      case init_vdf:
      case init_vdi:
        return replace_init_builtin (stmt);

      case reduc_max_vsi:
      case reduc_max_vhi:
      case reduc_max_vqi:
      case reduc_max_vsf:
      case reduc_max_vdf:
      case reduc_max_vdi:
      case reduc_min_vsi:
      case reduc_min_vhi:
      case reduc_min_vqi:
      case reduc_min_vsf:
      case reduc_min_vdf:
      case reduc_min_vdi:
      case reduc_plus_vsi:
      case reduc_plus_vhi:
      case reduc_plus_vqi:
      case reduc_plus_vsf:
      case reduc_plus_vdf:
      case reduc_plus_vdi:
        return replace_reduc_epilog_builtin (index, stmt);

      case realign_load_vsi:
      case realign_load_vhi:
      case realign_load_vqi:
      case realign_load_vsf:
      case realign_load_vdi:
      case realign_load_vdf:
        return replace_realign_load_builtin (index, stmt);

      case get_loop_niters:
        return replace_get_loop_niters_builtin (stmt);

      case dot_product_vhi:
      case dot_product_vqi:
        return replace_dot_product_builtin (stmt);

      case realign_offset_vsi:
      case realign_offset_vhi:
      case realign_offset_vqi:
      case realign_offset_vsf:
      case realign_offset_vdi:
      case realign_offset_vdf:
        return replace_realign_offset_builtin (index, stmt);

      case mask_for_load_vsi:
      case mask_for_load_vhi:
      case mask_for_load_vqi:
      case mask_for_load_vsf:
      case mask_for_load_vdi:
      case mask_for_load_vdf:
        return replace_mask_for_load_builtin (stmt);

      case get_vec_size_vsi:
      case get_vec_size_vhi:
      case get_vec_size_vqi:
      case get_vec_size_vsf:
      case get_vec_size_vdi:
      case get_vec_size_vdf:
        return replace_get_vec_size_builtin (index, stmt);

      case aligned_load_vsi:
      case aligned_load_vhi:
      case aligned_load_vqi:
      case aligned_load_vsf:
      case aligned_load_vdi:
      case aligned_load_vdf:
        return replace_aligned_load_builtin (index, stmt);

      case int2float_signed:
      case int2float_unsigned:
      case float2int_signed:
      case float2int_unsigned:
        return replace_int_float_cvt (index, stmt);

      case interleave_high_vqi:
      case interleave_high_vhi:
      case interleave_high_vsi:
      case interleave_low_vqi:
      case interleave_low_vhi:
      case interleave_low_vsi:
      case extract_even_vqi:
      case extract_even_vhi:
      case extract_even_vsi:
      case extract_odd_vqi:
      case extract_odd_vhi:
      case extract_odd_vsi:
      case interleave_high_vsf:
      case interleave_high_vdf:
      case interleave_low_vsf:
      case interleave_low_vdf:
      case extract_even_vsf:
      case extract_even_vdf:
      case extract_odd_vsf:
      case extract_odd_vdf:
	return replace_interleaving (index, stmt);

      case pack_vqi:
      case pack_vhi:
	return replace_pack (index, stmt);

      case widen_mult_hi_vhi:
      case widen_mult_hi_vsi:
      case widen_mult_lo_vhi:
      case widen_mult_lo_vsi:
	return replace_widen_mult (index, stmt);

      case double_supported:
        return replace_double_supported (stmt);

      case unpack_hi_vqi:
      case unpack_hi_vhi:
      case unpack_lo_vqi:
      case unpack_lo_vhi:
        return replace_unpack (index, stmt);

      case shift_right_vqi:
      case shift_right_vhi:
      case shift_right_vsi:
      case shift_left_vqi:
      case shift_left_vhi:
      case shift_left_vsi:
        return replace_shift (index, stmt);

      case bit_field_ref_vqi:
      case bit_field_ref_vhi:
      case bit_field_ref_vsi:
      case bit_field_ref_vsf:
      case bit_field_ref_vdf:
        return replace_bit_field_ref (index, stmt);

      default:
        if (dump_file && (dump_flags & TDF_DETAILS))
          fprintf (dump_file, "unsupported index of cli fn %d\n", index);
        return false;
    }
}

static void 
replace_cli_builtins (basic_block bb)
{
  gimple_stmt_iterator si;
  tree node;
  int i;

  for (si = gsi_start_bb (bb); !gsi_end_p (si); gsi_next (&si))
    {
      gimple stmt = gsi_stmt (si);

      if (dump_file && (dump_flags & TDF_DETAILS))
        {
          fprintf (dump_file, "==> examining statement: ");
          print_gimple_stmt (dump_file, stmt, 0, TDF_SLIM);
          fprintf (dump_file, "\n");
        }

      if (!is_gimple_call (stmt))
        continue;

      node = gimple_call_fndecl (stmt);

      for (i = 0; i < MAX_CLI_FN; i++)
        {
          if (DECL_NAME (node) == DECL_NAME (cli_functions[i]))
            {
              if (replace_cli_fn (i, stmt))
                {
                  if (dump_file && (dump_flags & TDF_DETAILS))
                    {
                      fprintf (dump_file, "RECOGNIZED %d ", i);
                      print_generic_expr (dump_file, node, TDF_SLIM);
                      fprintf (dump_file, "\n");
                    }
                }
              break;
            }
        }
    }
}

static unsigned int
scan_loops (void)
{
  unsigned int vect_loops_num;
  basic_block bb;

  vect_loops_num = number_of_loops ();

  /* Bail out if there are no loops.  */
  if (vect_loops_num <= 1)
    return 0;

  create_cli_fn_table ();

  FOR_EACH_BB (bb)
    replace_cli_builtins (bb);

  if (removed_stmts)
    VEC_free (gimple, heap, removed_stmts);

  mark_sym_for_renaming (gimple_vop (cfun));
  return 1;
}

static unsigned int
expand_vect_cli (void)
{
  if (number_of_loops () <= 1)
    return 0;

  return scan_loops ();
}

static bool
gate_expand_vect_cli (void)
{
  return flag_tree_vectorize;
}

struct gimple_opt_pass pass_expand_vect_cli = 
{
 {
  GIMPLE_PASS,
  "expcli",                               /* name */
  gate_expand_vect_cli,                  /* gate */
  expand_vect_cli,                       /* execute */
  NULL,                                 /* sub */
  NULL,                                 /* next */
  0,                                    /* static_pass_number */
  TV_TREE_VECTORIZATION,                /* tv_id */
  PROP_cfg | PROP_ssa,                  /* properties_required */
  0,                                    /* properties_provided */
  0,                                    /* properties_destroyed */
  TODO_verify_loops,                    /* todo_flags_start */
  TODO_dump_func | TODO_update_ssa
    | TODO_ggc_collect                  /* todo_flags_finish */
 }
};
