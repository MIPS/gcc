/* Test MIPS MSA ASE instructions */
/* { dg-do compile } */
/* { dg-options "-mfp64 -mhard-float -mmsa" } */
/* { dg-skip-if "madd and msub need combine" { *-*-* } { "-O0" "-flto" } { "" } } */

/* { dg-final { scan-assembler-times "\t.comm\tv16i8_\\d+,16,16" 3 } } */
/* { dg-final { scan-assembler-times "\t.comm\tv8i16_\\d+,16,16" 3 } } */
/* { dg-final { scan-assembler-times "\t.comm\tv4i32_\\d+,16,16" 3 } } */
/* { dg-final { scan-assembler-times "\t.comm\tv2i64_\\d+,16,16" 3 } } */
/* { dg-final { scan-assembler-times "\t.comm\tv16u8_\\d+,16,16" 3 } } */
/* { dg-final { scan-assembler-times "\t.comm\tv8u16_\\d+,16,16" 3 } } */
/* { dg-final { scan-assembler-times "\t.comm\tv4u32_\\d+,16,16" 3 } } */
/* { dg-final { scan-assembler-times "\t.comm\tv2u64_\\d+,16,16" 3 } } */
/* { dg-final { scan-assembler-times "\t.comm\tv4f32_\\d+,16,16" 3 } } */
/* { dg-final { scan-assembler-times "\t.comm\tv2f64_\\d+,16,16" 3 } } */

/* { dg-final { scan-assembler "test0_v16i8.*:.*v16i8_0.*test0_v16i8" } } */
/* { dg-final { scan-assembler "test0_v8i16.*:.*v8i16_0.*test0_v8i16" } } */
/* { dg-final { scan-assembler "test0_v4i32.*:.*v4i32_0.*test0_v4i32" } } */
/* { dg-final { scan-assembler "test0_v2i64.*:.*v2i64_0.*test0_v2i64" } } */
/* { dg-final { scan-assembler "test0_v16u8.*:.*v16u8_0.*test0_v16u8" } } */
/* { dg-final { scan-assembler "test0_v8u16.*:.*v8u16_0.*test0_v8u16" } } */
/* { dg-final { scan-assembler "test0_v4u32.*:.*v4u32_0.*test0_v4u32" } } */
/* { dg-final { scan-assembler "test0_v2u64.*:.*v2u64_0.*test0_v2u64" } } */
/* { dg-final { scan-assembler "test0_v4f32.*:.*v4f32_0.*test0_v4f32" } } */
/* { dg-final { scan-assembler "test0_v2f64.*:.*v2f64_0.*test0_v2f64" } } */
/* { dg-final { scan-assembler "test1_v16i8.*:.*st.b.*test1_v16i8" } } */
/* { dg-final { scan-assembler "test1_v8i16.*:.*st.h.*test1_v8i16" } } */
/* { dg-final { scan-assembler "test1_v4i32.*:.*st.w.*test1_v4i32" } } */
/* { dg-final { scan-assembler "test1_v2i64.*:.*st.d.*test1_v2i64" } } */
/* { dg-final { scan-assembler "test1_v16u8.*:.*st.b.*test1_v16u8" } } */
/* { dg-final { scan-assembler "test1_v8u16.*:.*st.h.*test1_v8u16" } } */
/* { dg-final { scan-assembler "test1_v4u32.*:.*st.w.*test1_v4u32" } } */
/* { dg-final { scan-assembler "test1_v2u64.*:.*st.d.*test1_v2u64" } } */
/* { dg-final { scan-assembler "test1_v4f32.*:.*st.w.*test1_v4f32" } } */
/* { dg-final { scan-assembler "test1_v2f64.*:.*st.d.*test1_v2f64" } } */
/* { dg-final { scan-assembler "test2_v16i8.*:.*addv.b.*test2_v16i8" } } */
/* { dg-final { scan-assembler "test2_v8i16.*:.*addv.h.*test2_v8i16" } } */
/* { dg-final { scan-assembler "test2_v4i32.*:.*addv.w.*test2_v4i32" } } */
/* { dg-final { scan-assembler "test2_v2i64.*:.*addv.d.*test2_v2i64" } } */
/* { dg-final { scan-assembler "test2_v16u8.*:.*addv.b.*test2_v16u8" } } */
/* { dg-final { scan-assembler "test2_v8u16.*:.*addv.h.*test2_v8u16" } } */
/* { dg-final { scan-assembler "test2_v4u32.*:.*addv.w.*test2_v4u32" } } */
/* { dg-final { scan-assembler "test2_v2u64.*:.*addv.d.*test2_v2u64" } } */
/* { dg-final { scan-assembler "test2_v4f32.*:.*fadd.w.*test2_v4f32" } } */
/* { dg-final { scan-assembler "test2_v2f64.*:.*fadd.d.*test2_v2f64" } } */
/* { dg-final { scan-assembler "test3_v16i8.*:.*subv.b.*test3_v16i8" } } */
/* { dg-final { scan-assembler "test3_v8i16.*:.*subv.h.*test3_v8i16" } } */
/* { dg-final { scan-assembler "test3_v4i32.*:.*subv.w.*test3_v4i32" } } */
/* { dg-final { scan-assembler "test3_v2i64.*:.*subv.d.*test3_v2i64" } } */
/* { dg-final { scan-assembler "test3_v16u8.*:.*subv.b.*test3_v16u8" } } */
/* { dg-final { scan-assembler "test3_v8u16.*:.*subv.h.*test3_v8u16" } } */
/* { dg-final { scan-assembler "test3_v4u32.*:.*subv.w.*test3_v4u32" } } */
/* { dg-final { scan-assembler "test3_v2u64.*:.*subv.d.*test3_v2u64" } } */
/* { dg-final { scan-assembler "test3_v4f32.*:.*fsub.w.*test3_v4f32" } } */
/* { dg-final { scan-assembler "test3_v2f64.*:.*fsub.d.*test3_v2f64" } } */
/* { dg-final { scan-assembler "test4_v16i8.*:.*mulv.b.*test4_v16i8" } } */
/* { dg-final { scan-assembler "test4_v8i16.*:.*mulv.h.*test4_v8i16" } } */
/* { dg-final { scan-assembler "test4_v4i32.*:.*mulv.w.*test4_v4i32" } } */
/* { dg-final { scan-assembler "test4_v2i64.*:.*mulv.d.*test4_v2i64" } } */
/* { dg-final { scan-assembler "test4_v16u8.*:.*mulv.b.*test4_v16u8" } } */
/* { dg-final { scan-assembler "test4_v8u16.*:.*mulv.h.*test4_v8u16" } } */
/* { dg-final { scan-assembler "test4_v4u32.*:.*mulv.w.*test4_v4u32" } } */
/* { dg-final { scan-assembler "test4_v2u64.*:.*mulv.d.*test4_v2u64" } } */
/* { dg-final { scan-assembler "test4_v4f32.*:.*fmul.w.*test4_v4f32" } } */
/* { dg-final { scan-assembler "test4_v2f64.*:.*fmul.d.*test4_v2f64" } } */
/* { dg-final { scan-assembler "test5_v16i8.*:.*div_s.b.*test5_v16i8" } } */
/* { dg-final { scan-assembler "test5_v8i16.*:.*div_s.h.*test5_v8i16" } } */
/* { dg-final { scan-assembler "test5_v4i32.*:.*div_s.w.*test5_v4i32" } } */
/* { dg-final { scan-assembler "test5_v2i64.*:.*div_s.d.*test5_v2i64" } } */
/* { dg-final { scan-assembler "test5_v16u8.*:.*div_u.b.*test5_v16u8" } } */
/* { dg-final { scan-assembler "test5_v8u16.*:.*div_u.h.*test5_v8u16" } } */
/* { dg-final { scan-assembler "test5_v4u32.*:.*div_u.w.*test5_v4u32" } } */
/* { dg-final { scan-assembler "test5_v2u64.*:.*div_u.d.*test5_v2u64" } } */
/* { dg-final { scan-assembler "test5_v4f32.*:.*fdiv.w.*test5_v4f32" } } */
/* { dg-final { scan-assembler "test5_v2f64.*:.*fdiv.d.*test5_v2f64" } } */
/* { dg-final { scan-assembler "test6_v16i8.*:.*mod_s.b.*test6_v16i8" } } */
/* { dg-final { scan-assembler "test6_v8i16.*:.*mod_s.h.*test6_v8i16" } } */
/* { dg-final { scan-assembler "test6_v4i32.*:.*mod_s.w.*test6_v4i32" } } */
/* { dg-final { scan-assembler "test6_v2i64.*:.*mod_s.d.*test6_v2i64" } } */
/* { dg-final { scan-assembler "test6_v16u8.*:.*mod_u.b.*test6_v16u8" } } */
/* { dg-final { scan-assembler "test6_v8u16.*:.*mod_u.h.*test6_v8u16" } } */
/* { dg-final { scan-assembler "test6_v4u32.*:.*mod_u.w.*test6_v4u32" } } */
/* { dg-final { scan-assembler "test6_v2u64.*:.*mod_u.d.*test6_v2u64" } } */
/* { dg-final { scan-assembler "test7_v16i8.*:.*subv.b.*test7_v16i8" } } */
/* { dg-final { scan-assembler "test7_v8i16.*:.*subv.h.*test7_v8i16" } } */
/* { dg-final { scan-assembler "test7_v4i32.*:.*subv.w.*test7_v4i32" } } */
/* { dg-final { scan-assembler "test7_v2i64.*:.*subv.d.*test7_v2i64" } } */
/* { dg-final { scan-assembler "test7_v16u8.*:.*subv.b.*test7_v16u8" } } */
/* { dg-final { scan-assembler "test7_v8u16.*:.*subv.h.*test7_v8u16" } } */
/* { dg-final { scan-assembler "test7_v4u32.*:.*subv.w.*test7_v4u32" } } */
/* { dg-final { scan-assembler "test7_v2u64.*:.*subv.d.*test7_v2u64" } } */
/* { dg-final { scan-assembler "test7_v4f32.*:.*fsub.w.*test7_v4f32" } } */
/* { dg-final { scan-assembler "test7_v2f64.*:.*fsub.d.*test7_v2f64" } } */
/* { dg-final { scan-assembler "test8_v16i8.*:.*xor.v.*test8_v16i8" } } */
/* { dg-final { scan-assembler "test8_v8i16.*:.*xor.v.*test8_v8i16" } } */
/* { dg-final { scan-assembler "test8_v4i32.*:.*xor.v.*test8_v4i32" } } */
/* { dg-final { scan-assembler "test8_v2i64.*:.*xor.v.*test8_v2i64" } } */
/* { dg-final { scan-assembler "test8_v16u8.*:.*xor.v.*test8_v16u8" } } */
/* { dg-final { scan-assembler "test8_v8u16.*:.*xor.v.*test8_v8u16" } } */
/* { dg-final { scan-assembler "test8_v4u32.*:.*xor.v.*test8_v4u32" } } */
/* { dg-final { scan-assembler "test8_v2u64.*:.*xor.v.*test8_v2u64" } } */
/* { dg-final { scan-assembler "test9_v16i8.*:.*or.v.*test9_v16i8" } } */
/* { dg-final { scan-assembler "test9_v8i16.*:.*or.v.*test9_v8i16" } } */
/* { dg-final { scan-assembler "test9_v4i32.*:.*or.v.*test9_v4i32" } } */
/* { dg-final { scan-assembler "test9_v2i64.*:.*or.v.*test9_v2i64" } } */
/* { dg-final { scan-assembler "test9_v16u8.*:.*or.v.*test9_v16u8" } } */
/* { dg-final { scan-assembler "test9_v8u16.*:.*or.v.*test9_v8u16" } } */
/* { dg-final { scan-assembler "test9_v4u32.*:.*or.v.*test9_v4u32" } } */
/* { dg-final { scan-assembler "test9_v2u64.*:.*or.v.*test9_v2u64" } } */
/* { dg-final { scan-assembler "test10_v16i8.*:.*and.v.*test10_v16i8" } } */
/* { dg-final { scan-assembler "test10_v8i16.*:.*and.v.*test10_v8i16" } } */
/* { dg-final { scan-assembler "test10_v4i32.*:.*and.v.*test10_v4i32" } } */
/* { dg-final { scan-assembler "test10_v2i64.*:.*and.v.*test10_v2i64" } } */
/* { dg-final { scan-assembler "test10_v16u8.*:.*and.v.*test10_v16u8" } } */
/* { dg-final { scan-assembler "test10_v8u16.*:.*and.v.*test10_v8u16" } } */
/* { dg-final { scan-assembler "test10_v4u32.*:.*and.v.*test10_v4u32" } } */
/* { dg-final { scan-assembler "test10_v2u64.*:.*and.v.*test10_v2u64" } } */
/* { dg-final { scan-assembler "test11_v16i8.*:.*nor.v.*test11_v16i8" } } */
/* { dg-final { scan-assembler "test11_v8i16.*:.*nor.v.*test11_v8i16" } } */
/* { dg-final { scan-assembler "test11_v4i32.*:.*nor.v.*test11_v4i32" } } */
/* { dg-final { scan-assembler "test11_v2i64.*:.*nor.v.*test11_v2i64" } } */
/* { dg-final { scan-assembler "test11_v16u8.*:.*nor.v.*test11_v16u8" } } */
/* { dg-final { scan-assembler "test11_v8u16.*:.*nor.v.*test11_v8u16" } } */
/* { dg-final { scan-assembler "test11_v4u32.*:.*nor.v.*test11_v4u32" } } */
/* { dg-final { scan-assembler "test11_v2u64.*:.*nor.v.*test11_v2u64" } } */
/* { dg-final { scan-assembler "test12_v16i8.*:.*sra.b.*test12_v16i8" } } */
/* { dg-final { scan-assembler "test12_v8i16.*:.*sra.h.*test12_v8i16" } } */
/* { dg-final { scan-assembler "test12_v4i32.*:.*sra.w.*test12_v4i32" } } */
/* { dg-final { scan-assembler "test12_v2i64.*:.*sra.d.*test12_v2i64" } } */
/* { dg-final { scan-assembler "test12_v16u8.*:.*srl.b.*test12_v16u8" } } */
/* { dg-final { scan-assembler "test12_v8u16.*:.*srl.h.*test12_v8u16" } } */
/* { dg-final { scan-assembler "test12_v4u32.*:.*srl.w.*test12_v4u32" } } */
/* { dg-final { scan-assembler "test12_v2u64.*:.*srl.d.*test12_v2u64" } } */
/* { dg-final { scan-assembler "test13_v16i8.*:.*sll.b.*test13_v16i8" } } */
/* { dg-final { scan-assembler "test13_v8i16.*:.*sll.h.*test13_v8i16" } } */
/* { dg-final { scan-assembler "test13_v4i32.*:.*sll.w.*test13_v4i32" } } */
/* { dg-final { scan-assembler "test13_v2i64.*:.*sll.d.*test13_v2i64" } } */
/* { dg-final { scan-assembler "test13_v16u8.*:.*sll.b.*test13_v16u8" } } */
/* { dg-final { scan-assembler "test13_v8u16.*:.*sll.h.*test13_v8u16" } } */
/* { dg-final { scan-assembler "test13_v4u32.*:.*sll.w.*test13_v4u32" } } */
/* { dg-final { scan-assembler "test13_v2u64.*:.*sll.d.*test13_v2u64" } } */
/* { dg-final { scan-assembler "test14_v16i8.*:.*ceq.b.*test14_v16i8" } } */
/* { dg-final { scan-assembler "test14_v8i16.*:.*ceq.h.*test14_v8i16" } } */
/* { dg-final { scan-assembler "test14_v4i32.*:.*ceq.w.*test14_v4i32" } } */
/* { dg-final { scan-assembler "test14_v2i64.*:.*ceq.d.*test14_v2i64" } } */
/* { dg-final { scan-assembler "test14_v16u8.*:.*ceq.b.*test14_v16u8" } } */
/* { dg-final { scan-assembler "test14_v8u16.*:.*ceq.h.*test14_v8u16" } } */
/* { dg-final { scan-assembler "test14_v4u32.*:.*ceq.w.*test14_v4u32" } } */
/* { dg-final { scan-assembler "test14_v2u64.*:.*ceq.d.*test14_v2u64" } } */
/* { dg-final { scan-assembler "test14_v4f32.*:.*fceq.w.*test14_v4f32" } } */
/* { dg-final { scan-assembler "test14_v2f64.*:.*fceq.d.*test14_v2f64" } } */
/* { dg-final { scan-assembler "test15_v16i8.*:.*ceq.b.*nor.v.*test15_v16i8" } } */
/* { dg-final { scan-assembler "test15_v8i16.*:.*ceq.h.*nor.v.*test15_v8i16" } } */
/* { dg-final { scan-assembler "test15_v4i32.*:.*ceq.w.*nor.v.*test15_v4i32" } } */
/* { dg-final { scan-assembler "test15_v2i64.*:.*ceq.d.*nor.v.*test15_v2i64" } } */
/* { dg-final { scan-assembler "test15_v16u8.*:.*ceq.b.*nor.v.*test15_v16u8" } } */
/* { dg-final { scan-assembler "test15_v8u16.*:.*ceq.h.*nor.v.*test15_v8u16" } } */
/* { dg-final { scan-assembler "test15_v4u32.*:.*ceq.w.*nor.v.*test15_v4u32" } } */
/* { dg-final { scan-assembler "test15_v2u64.*:.*ceq.d.*nor.v.*test15_v2u64" } } */
/* { dg-final { scan-assembler "test15_v4f32.*:.*fsne.w.*test15_v4f32" } } */
/* { dg-final { scan-assembler "test15_v2f64.*:.*fsne.d.*test15_v2f64" } } */
/* { dg-final { scan-assembler "test16_v16i8.*:.*ld.b\t\\\$w1.*clt_s.b.*test16_v16i8" } } */
/* { dg-final { scan-assembler "test16_v8i16.*:.*ld.h\t\\\$w1.*clt_s.h.*test16_v8i16" } } */
/* { dg-final { scan-assembler "test16_v4i32.*:.*ld.w\t\\\$w1.*clt_s.w.*test16_v4i32" } } */
/* { dg-final { scan-assembler "test16_v2i64.*:.*ld.d\t\\\$w1.*clt_s.d.*test16_v2i64" } } */
/* { dg-final { scan-assembler "test16_v16u8.*:.*ld.b\t\\\$w1.*clt_u.b.*test16_v16u8" } } */
/* { dg-final { scan-assembler "test16_v8u16.*:.*ld.h\t\\\$w1.*clt_u.h.*test16_v8u16" } } */
/* { dg-final { scan-assembler "test16_v4u32.*:.*ld.w\t\\\$w1.*clt_u.w.*test16_v4u32" } } */
/* { dg-final { scan-assembler "test16_v2u64.*:.*ld.d\t\\\$w1.*clt_u.d.*test16_v2u64" } } */
/* { dg-final { scan-assembler "test16_v4f32.*:.*ld.w\t\\\$w1.*fslt.w.*test16_v4f32" } } */
/* { dg-final { scan-assembler "test16_v2f64.*:.*ld.d\t\\\$w1.*fslt.d.*test16_v2f64" } } */
/* { dg-final { scan-assembler "test17_v16i8.*:.*ld.b\t\\\$w1.*cle_s.b.*test17_v16i8" } } */
/* { dg-final { scan-assembler "test17_v8i16.*:.*ld.h\t\\\$w1.*cle_s.h.*test17_v8i16" } } */
/* { dg-final { scan-assembler "test17_v4i32.*:.*ld.w\t\\\$w1.*cle_s.w.*test17_v4i32" } } */
/* { dg-final { scan-assembler "test17_v2i64.*:.*ld.d\t\\\$w1.*cle_s.d.*test17_v2i64" } } */
/* { dg-final { scan-assembler "test17_v16u8.*:.*ld.b\t\\\$w1.*cle_u.b.*test17_v16u8" } } */
/* { dg-final { scan-assembler "test17_v8u16.*:.*ld.h\t\\\$w1.*cle_u.h.*test17_v8u16" } } */
/* { dg-final { scan-assembler "test17_v4u32.*:.*ld.w\t\\\$w1.*cle_u.w.*test17_v4u32" } } */
/* { dg-final { scan-assembler "test17_v2u64.*:.*ld.d\t\\\$w1.*cle_u.d.*test17_v2u64" } } */
/* { dg-final { scan-assembler "test17_v4f32.*:.*ld.w\t\\\$w1.*fsle.w.*test17_v4f32" } } */
/* { dg-final { scan-assembler "test17_v2f64.*:.*ld.d\t\\\$w1.*fsle.d.*test17_v2f64" } } */
/* { dg-final { scan-assembler "test18_v16i8.*:.*ld.b\t\\\$w0.*clt_s.b.*test18_v16i8" } } */
/* { dg-final { scan-assembler "test18_v8i16.*:.*ld.h\t\\\$w0.*clt_s.h.*test18_v8i16" } } */
/* { dg-final { scan-assembler "test18_v4i32.*:.*ld.w\t\\\$w0.*clt_s.w.*test18_v4i32" } } */
/* { dg-final { scan-assembler "test18_v2i64.*:.*ld.d\t\\\$w0.*clt_s.d.*test18_v2i64" } } */
/* { dg-final { scan-assembler "test18_v16u8.*:.*ld.b\t\\\$w0.*clt_u.b.*test18_v16u8" } } */
/* { dg-final { scan-assembler "test18_v8u16.*:.*ld.h\t\\\$w0.*clt_u.h.*test18_v8u16" } } */
/* { dg-final { scan-assembler "test18_v4u32.*:.*ld.w\t\\\$w0.*clt_u.w.*test18_v4u32" } } */
/* { dg-final { scan-assembler "test18_v2u64.*:.*ld.d\t\\\$w0.*clt_u.d.*test18_v2u64" } } */
/* { dg-final { scan-assembler "test18_v4f32.*:.*ld.w\t\\\$w0.*fslt.w.*test18_v4f32" } } */
/* { dg-final { scan-assembler "test18_v2f64.*:.*ld.d\t\\\$w0.*fslt.d.*test18_v2f64" } } */
/* { dg-final { scan-assembler "test19_v16i8.*:.*ld.b\t\\\$w0.*cle_s.b.*test19_v16i8" } } */
/* { dg-final { scan-assembler "test19_v8i16.*:.*ld.h\t\\\$w0.*cle_s.h.*test19_v8i16" } } */
/* { dg-final { scan-assembler "test19_v4i32.*:.*ld.w\t\\\$w0.*cle_s.w.*test19_v4i32" } } */
/* { dg-final { scan-assembler "test19_v2i64.*:.*ld.d\t\\\$w0.*cle_s.d.*test19_v2i64" } } */
/* { dg-final { scan-assembler "test19_v16u8.*:.*ld.b\t\\\$w0.*cle_u.b.*test19_v16u8" } } */
/* { dg-final { scan-assembler "test19_v8u16.*:.*ld.h\t\\\$w0.*cle_u.h.*test19_v8u16" } } */
/* { dg-final { scan-assembler "test19_v4u32.*:.*ld.w\t\\\$w0.*cle_u.w.*test19_v4u32" } } */
/* { dg-final { scan-assembler "test19_v2u64.*:.*ld.d\t\\\$w0.*cle_u.d.*test19_v2u64" } } */
/* { dg-final { scan-assembler "test19_v4f32.*:.*ld.w\t\\\$w0.*fsle.w.*test19_v4f32" } } */
/* { dg-final { scan-assembler "test19_v2f64.*:.*ld.d\t\\\$w0.*fsle.d.*test19_v2f64" } } */
/* { dg-final { scan-assembler "test20_v16i8.*:.*addvi.b.*test20_v16i8" } } */
/* { dg-final { scan-assembler "test20_v8i16.*:.*addvi.h.*test20_v8i16" } } */
/* { dg-final { scan-assembler "test20_v4i32.*:.*addvi.w.*test20_v4i32" } } */
/* { dg-final { scan-assembler "test20_v2i64.*:.*addvi.d.*test20_v2i64" } } */
/* { dg-final { scan-assembler "test20_v16u8.*:.*addvi.b.*test20_v16u8" } } */
/* { dg-final { scan-assembler "test20_v8u16.*:.*addvi.h.*test20_v8u16" } } */
/* { dg-final { scan-assembler "test20_v4u32.*:.*addvi.w.*test20_v4u32" } } */
/* { dg-final { scan-assembler "test20_v2u64.*:.*addvi.d.*test20_v2u64" } } */
/* { dg-final { scan-assembler "test21_v16i8.*:.*subvi.b.*test21_v16i8" } } */
/* { dg-final { scan-assembler "test21_v8i16.*:.*subvi.h.*test21_v8i16" } } */
/* { dg-final { scan-assembler "test21_v4i32.*:.*subvi.w.*test21_v4i32" } } */
/* { dg-final { scan-assembler "test21_v2i64.*:.*subvi.d.*test21_v2i64" } } */
/* { dg-final { scan-assembler "test21_v16u8.*:.*subvi.b.*test21_v16u8" } } */
/* { dg-final { scan-assembler "test21_v8u16.*:.*subvi.h.*test21_v8u16" } } */
/* { dg-final { scan-assembler "test21_v4u32.*:.*subvi.w.*test21_v4u32" } } */
/* { dg-final { scan-assembler "test21_v2u64.*:.*subvi.d.*test21_v2u64" } } */
/* { dg-final { scan-assembler "test22_v16i8.*:.*ldi.b\t\\\$w\\d+,37.*mulv.b.*test22_v16i8" } } */
/* { dg-final { scan-assembler "test22_v8i16.*:.*ldi.h\t\\\$w\\d+,37.*mulv.h.*test22_v8i16" } } */
/* { dg-final { scan-assembler "test22_v4i32.*:.*ldi.w\t\\\$w\\d+,37.*mulv.w.*test22_v4i32" } } */
/* { dg-final { scan-assembler "test22_v2i64.*:.*ldi.d\t\\\$w\\d+,37.*mulv.d.*test22_v2i64" } } */
/* { dg-final { scan-assembler "test22_v16u8.*:.*ldi.b\t\\\$w\\d+,37.*mulv.b.*test22_v16u8" } } */
/* { dg-final { scan-assembler "test22_v8u16.*:.*ldi.h\t\\\$w\\d+,37.*mulv.h.*test22_v8u16" } } */
/* { dg-final { scan-assembler "test22_v4u32.*:.*ldi.w\t\\\$w\\d+,37.*mulv.w.*test22_v4u32" } } */
/* { dg-final { scan-assembler "test22_v2u64.*:.*ldi.d\t\\\$w\\d+,37.*mulv.d.*test22_v2u64" } } */
/* { dg-final { scan-assembler "test23_v16i8.*:.*ldi.b\t\\\$w\\d+,37.*div_s.b.*test23_v16i8" } } */
/* { dg-final { scan-assembler "test23_v8i16.*:.*ldi.h\t\\\$w\\d+,37.*div_s.h.*test23_v8i16" } } */
/* { dg-final { scan-assembler "test23_v4i32.*:.*ldi.w\t\\\$w\\d+,37.*div_s.w.*test23_v4i32" } } */
/* { dg-final { scan-assembler "test23_v2i64.*:.*ldi.d\t\\\$w\\d+,37.*div_s.d.*test23_v2i64" } } */
/* { dg-final { scan-assembler "test23_v16u8.*:.*ldi.b\t\\\$w\\d+,37.*div_u.b.*test23_v16u8" } } */
/* { dg-final { scan-assembler "test23_v8u16.*:.*ldi.h\t\\\$w\\d+,37.*div_u.h.*test23_v8u16" } } */
/* { dg-final { scan-assembler "test23_v4u32.*:.*ldi.w\t\\\$w\\d+,37.*div_u.w.*test23_v4u32" } } */
/* { dg-final { scan-assembler "test23_v2u64.*:.*ldi.d\t\\\$w\\d+,37.*div_u.d.*test23_v2u64" } } */
/* { dg-final { scan-assembler "test24_v16i8.*:.*ldi.b\t\\\$w\\d+,37.*mod_s.b.*test24_v16i8" } } */
/* { dg-final { scan-assembler "test24_v8i16.*:.*ldi.h\t\\\$w\\d+,37.*mod_s.h.*test24_v8i16" } } */
/* { dg-final { scan-assembler "test24_v4i32.*:.*ldi.w\t\\\$w\\d+,37.*mod_s.w.*test24_v4i32" } } */
/* { dg-final { scan-assembler "test24_v2i64.*:.*ldi.d\t\\\$w\\d+,37.*mod_s.d.*test24_v2i64" } } */
/* { dg-final { scan-assembler "test24_v16u8.*:.*ldi.b\t\\\$w\\d+,37.*mod_u.b.*test24_v16u8" } } */
/* { dg-final { scan-assembler "test24_v8u16.*:.*ldi.h\t\\\$w\\d+,37.*mod_u.h.*test24_v8u16" } } */
/* { dg-final { scan-assembler "test24_v4u32.*:.*ldi.w\t\\\$w\\d+,37.*mod_u.w.*test24_v4u32" } } */
/* { dg-final { scan-assembler "test24_v2u64.*:.*ldi.d\t\\\$w\\d+,37.*mod_u.d.*test24_v2u64" } } */
/* { dg-final { scan-assembler "test25_v16i8.*:.*xori.b.*test25_v16i8" } } */
/* { dg-final { scan-assembler "test25_v8i16.*:.*ldi.h\t\\\$w\\d+,37.*xor.v.*test25_v8i16" } } */
/* { dg-final { scan-assembler "test25_v4i32.*:.*ldi.w\t\\\$w\\d+,37.*xor.v.*test25_v4i32" } } */
/* { dg-final { scan-assembler "test25_v2i64.*:.*ldi.d\t\\\$w\\d+,37.*xor.v.*test25_v2i64" } } */
/* { dg-final { scan-assembler "test25_v16u8.*:.*xori.b.*test25_v16u8" } } */
/* { dg-final { scan-assembler "test25_v8u16.*:.*ldi.h\t\\\$w\\d+,37.*xor.v.*test25_v8u16" } } */
/* { dg-final { scan-assembler "test25_v4u32.*:.*ldi.w\t\\\$w\\d+,37.*xor.v.*test25_v4u32" } } */
/* { dg-final { scan-assembler "test25_v2u64.*:.*ldi.d\t\\\$w\\d+,37.*xor.v.*test25_v2u64" } } */
/* { dg-final { scan-assembler "test26_v16i8.*:.*ori.b.*test26_v16i8" } } */
/* { dg-final { scan-assembler "test26_v8i16.*:.*ldi.h\t\\\$w\\d+,37.*or.v.*test26_v8i16" } } */
/* { dg-final { scan-assembler "test26_v4i32.*:.*ldi.w\t\\\$w\\d+,37.*or.v.*test26_v4i32" } } */
/* { dg-final { scan-assembler "test26_v2i64.*:.*ldi.d\t\\\$w\\d+,37.*or.v.*test26_v2i64" } } */
/* { dg-final { scan-assembler "test26_v16u8.*:.*ori.b.*test26_v16u8" } } */
/* { dg-final { scan-assembler "test26_v8u16.*:.*ldi.h\t\\\$w\\d+,37.*or.v.*test26_v8u16" } } */
/* { dg-final { scan-assembler "test26_v4u32.*:.*ldi.w\t\\\$w\\d+,37.*or.v.*test26_v4u32" } } */
/* { dg-final { scan-assembler "test26_v2u64.*:.*ldi.d\t\\\$w\\d+,37.*or.v.*test26_v2u64" } } */
/* { dg-final { scan-assembler "test27_v16i8.*:.*andi.b.*test27_v16i8" } } */
/* { dg-final { scan-assembler "test27_v8i16.*:.*ldi.h\t\\\$w\\d+,37.*and.v.*test27_v8i16" } } */
/* { dg-final { scan-assembler "test27_v4i32.*:.*ldi.w\t\\\$w\\d+,37.*and.v.*test27_v4i32" } } */
/* { dg-final { scan-assembler "test27_v2i64.*:.*ldi.d\t\\\$w\\d+,37.*and.v.*test27_v2i64" } } */
/* { dg-final { scan-assembler "test27_v16u8.*:.*andi.b.*test27_v16u8" } } */
/* { dg-final { scan-assembler "test27_v8u16.*:.*ldi.h\t\\\$w\\d+,37.*and.v.*test27_v8u16" } } */
/* { dg-final { scan-assembler "test27_v4u32.*:.*ldi.w\t\\\$w\\d+,37.*and.v.*test27_v4u32" } } */
/* { dg-final { scan-assembler "test27_v2u64.*:.*ldi.d\t\\\$w\\d+,37.*and.v.*test27_v2u64" } } */
/* { dg-final { scan-assembler "test28_v16i8.*:.*srai.b.*test28_v16i8" } } */
/* { dg-final { scan-assembler "test28_v8i16.*:.*srai.h.*test28_v8i16" } } */
/* { dg-final { scan-assembler "test28_v4i32.*:.*srai.w.*test28_v4i32" } } */
/* { dg-final { scan-assembler "test28_v2i64.*:.*srai.d.*test28_v2i64" } } */
/* { dg-final { scan-assembler "test28_v16u8.*:.*srli.b.*test28_v16u8" } } */
/* { dg-final { scan-assembler "test28_v8u16.*:.*srli.h.*test28_v8u16" } } */
/* { dg-final { scan-assembler "test28_v4u32.*:.*srli.w.*test28_v4u32" } } */
/* { dg-final { scan-assembler "test28_v2u64.*:.*srli.d.*test28_v2u64" } } */
/* { dg-final { scan-assembler "test29_v16i8.*:.*slli.b.*test29_v16i8" } } */
/* { dg-final { scan-assembler "test29_v8i16.*:.*slli.h.*test29_v8i16" } } */
/* { dg-final { scan-assembler "test29_v4i32.*:.*slli.w.*test29_v4i32" } } */
/* { dg-final { scan-assembler "test29_v2i64.*:.*slli.d.*test29_v2i64" } } */
/* { dg-final { scan-assembler "test29_v16u8.*:.*slli.b.*test29_v16u8" } } */
/* { dg-final { scan-assembler "test29_v8u16.*:.*slli.h.*test29_v8u16" } } */
/* { dg-final { scan-assembler "test29_v4u32.*:.*slli.w.*test29_v4u32" } } */
/* { dg-final { scan-assembler "test29_v2u64.*:.*slli.d.*test29_v2u64" } } */
/* { dg-final { scan-assembler "test30_v16i8.*:.*ceqi.b.*test30_v16i8" } } */
/* { dg-final { scan-assembler "test30_v8i16.*:.*ceqi.h.*test30_v8i16" } } */
/* { dg-final { scan-assembler "test30_v4i32.*:.*ceqi.w.*test30_v4i32" } } */
/* { dg-final { scan-assembler "test30_v2i64.*:.*ceqi.d.*test30_v2i64" } } */
/* { dg-final { scan-assembler "test30_v16u8.*:.*ceqi.b.*test30_v16u8" } } */
/* { dg-final { scan-assembler "test30_v8u16.*:.*ceqi.h.*test30_v8u16" } } */
/* { dg-final { scan-assembler "test30_v4u32.*:.*ceqi.w.*test30_v4u32" } } */
/* { dg-final { scan-assembler "test30_v2u64.*:.*ceqi.d.*test30_v2u64" } } */
/* { dg-final { scan-assembler "test31_s_v16i8.*:.*clti_s.b.*test31_s_v16i8" } } */
/* { dg-final { scan-assembler "test31_s_v8i16.*:.*clti_s.h.*test31_s_v8i16" } } */
/* { dg-final { scan-assembler "test31_s_v4i32.*:.*clti_s.w.*test31_s_v4i32" } } */
/* { dg-final { scan-assembler "test31_s_v2i64.*:.*clti_s.d.*test31_s_v2i64" } } */
/* { dg-final { scan-assembler "test31_u_v16u8.*:.*clti_u.b.*test31_u_v16u8" } } */
/* { dg-final { scan-assembler "test31_u_v8u16.*:.*clti_u.h.*test31_u_v8u16" } } */
/* { dg-final { scan-assembler "test31_u_v4u32.*:.*clti_u.w.*test31_u_v4u32" } } */
/* { dg-final { scan-assembler "test31_u_v2u64.*:.*clti_u.d.*test31_u_v2u64" } } */
/* { dg-final { scan-assembler "test32_s_v16i8.*:.*clei_s.b.*test32_s_v16i8" } } */
/* { dg-final { scan-assembler "test32_s_v8i16.*:.*clei_s.h.*test32_s_v8i16" } } */
/* { dg-final { scan-assembler "test32_s_v4i32.*:.*clei_s.w.*test32_s_v4i32" } } */
/* { dg-final { scan-assembler "test32_s_v2i64.*:.*clei_s.d.*test32_s_v2i64" } } */
/* { dg-final { scan-assembler "test32_u_v16u8.*:.*clei_u.b.*test32_u_v16u8" } } */
/* { dg-final { scan-assembler "test32_u_v8u16.*:.*clei_u.h.*test32_u_v8u16" } } */
/* { dg-final { scan-assembler "test32_u_v4u32.*:.*clei_u.w.*test32_u_v4u32" } } */
/* { dg-final { scan-assembler "test32_u_v2u64.*:.*clei_u.d.*test32_u_v2u64" } } */
/* { dg-final { scan-assembler "test33_v4f32.*:.*fadd.w.*test33_v4f32" } } */
/* { dg-final { scan-assembler "test33_v2f64.*:.*fadd.d.*test33_v2f64" } } */
/* { dg-final { scan-assembler "test34_v4f32.*:.*fsub.w.*test34_v4f32" } } */
/* { dg-final { scan-assembler "test34_v2f64.*:.*fsub.d.*test34_v2f64" } } */
/* { dg-final { scan-assembler "test35_v4f32.*:.*fmul.w.*test35_v4f32" } } */
/* { dg-final { scan-assembler "test35_v2f64.*:.*fmul.d.*test35_v2f64" } } */
/* { dg-final { scan-assembler "test36_v4f32.*:.*fdiv.w.*test36_v4f32" } } */
/* { dg-final { scan-assembler "test36_v2f64.*:.*fdiv.d.*test36_v2f64" } } */
/* { dg-final { scan-assembler "test37_v16i8.*:.*vshf.b.*test37_v16i8" } } */
/* { dg-final { scan-assembler "test37_v8i16.*:.*vshf.h.*test37_v8i16" } } */
/* { dg-final { scan-assembler "test37_v4i32.*:.*vshf.w.*test37_v4i32" } } */
/* { dg-final { scan-assembler "test37_v2i64.*:.*vshf.d.*test37_v2i64" } } */
/* { dg-final { scan-assembler "test37_v16u8.*:.*vshf.b.*test37_v16u8" } } */
/* { dg-final { scan-assembler "test37_v8u16.*:.*vshf.h.*test37_v8u16" } } */
/* { dg-final { scan-assembler "test37_v4u32.*:.*vshf.w.*test37_v4u32" } } */
/* { dg-final { scan-assembler "test37_v2u64.*:.*vshf.d.*test37_v2u64" } } */
/* { dg-final { scan-assembler "test38_v16i8.*:.*vshf.b.*test38_v16i8" } } */
/* { dg-final { scan-assembler "test38_v8i16.*:.*vshf.h.*test38_v8i16" } } */
/* { dg-final { scan-assembler "test38_v4i32.*:.*vshf.w.*test38_v4i32" } } */
/* { dg-final { scan-assembler "test38_v2i64.*:.*vshf.d.*test38_v2i64" } } */
/* { dg-final { scan-assembler "test38_v16u8.*:.*vshf.b.*test38_v16u8" } } */
/* { dg-final { scan-assembler "test38_v8u16.*:.*vshf.h.*test38_v8u16" } } */
/* { dg-final { scan-assembler "test38_v4u32.*:.*vshf.w.*test38_v4u32" } } */
/* { dg-final { scan-assembler "test38_v2u64.*:.*vshf.d.*test38_v2u64" } } */
/* { dg-final { scan-assembler "test39_v4f32.*:.*vshf.w.*test39_v4f32" } } */
/* { dg-final { scan-assembler "test39_v2f64.*:.*vshf.d.*test39_v2f64" } } */
/* { dg-final { scan-assembler "test40_v4f32.*:.*vshf.w.*test40_v4f32" } } */
/* { dg-final { scan-assembler "test40_v2f64.*:.*vshf.d.*test40_v2f64" } } */
/* { dg-final { scan-assembler "test41_v16i8.*:.*maddv.b.*test41_v16i8" } } */
/* { dg-final { scan-assembler "test41_v8i16.*:.*maddv.h.*test41_v8i16" } } */
/* { dg-final { scan-assembler "test41_v4i32.*:.*maddv.w.*test41_v4i32" } } */
/* { dg-final { scan-assembler "test41_v2i64.*:.*maddv.d.*test41_v2i64" } } */
/* { dg-final { scan-assembler "test41_v16u8.*:.*maddv.b.*test41_v16u8" } } */
/* { dg-final { scan-assembler "test41_v8u16.*:.*maddv.h.*test41_v8u16" } } */
/* { dg-final { scan-assembler "test41_v4u32.*:.*maddv.w.*test41_v4u32" } } */
/* { dg-final { scan-assembler "test41_v2u64.*:.*maddv.d.*test41_v2u64" } } */
/* { dg-final { scan-assembler "test41_v4f32.*:.*fmadd.w.*test41_v4f32" } } */
/* { dg-final { scan-assembler "test41_v2f64.*:.*fmadd.d.*test41_v2f64" } } */
/* { dg-final { scan-assembler "test42_v16i8.*:.*msubv.b.*test42_v16i8" } } */
/* { dg-final { scan-assembler "test42_v8i16.*:.*msubv.h.*test42_v8i16" } } */
/* { dg-final { scan-assembler "test42_v4i32.*:.*msubv.w.*test42_v4i32" } } */
/* { dg-final { scan-assembler "test42_v2i64.*:.*msubv.d.*test42_v2i64" } } */
/* { dg-final { scan-assembler "test42_v16u8.*:.*msubv.b.*test42_v16u8" } } */
/* { dg-final { scan-assembler "test42_v8u16.*:.*msubv.h.*test42_v8u16" } } */
/* { dg-final { scan-assembler "test42_v4u32.*:.*msubv.w.*test42_v4u32" } } */
/* { dg-final { scan-assembler "test42_v2u64.*:.*msubv.d.*test42_v2u64" } } */
/* { dg-final { scan-assembler "test42_v4f32.*:.*fmsub.w.*test42_v4f32" } } */
/* { dg-final { scan-assembler "test42_v2f64.*:.*fmsub.d.*test42_v2f64" } } */
/* { dg-final { scan-assembler "test43_builtin.*:.*cfcmsa.*test43_builtin" } } */
/* { dg-final { scan-assembler "test44_builtin.*:.*ctcmsa.*test44_builtin" } } */
/* { dg-final { scan-assembler "test45_builtin_v16i8.*:.*ld.b.*test45_builtin_v16i8" } } */
/* { dg-final { scan-assembler "test45_builtin_v8i16.*:.*ld.h.*test45_builtin_v8i16" } } */
/* { dg-final { scan-assembler "test45_builtin_v4i32.*:.*ld.w.*test45_builtin_v4i32" } } */
/* { dg-final { scan-assembler "test45_builtin_v2i64.*:.*ld.d.*test45_builtin_v2i64" } } */
/* { dg-final { scan-assembler "test45_v16i8.*:.*ld.b.*test45_v16i8" } } */
/* { dg-final { scan-assembler "test45_v8i16.*:.*ld.h.*test45_v8i16" } } */
/* { dg-final { scan-assembler "test45_v4i32.*:.*ld.w.*test45_v4i32" } } */
/* { dg-final { scan-assembler "test45_v2i64.*:.*ld.d.*test45_v2i64" } } */
/* { dg-final { scan-assembler "test46_builtin_v16i8.*:.*ldi.b.*,123.*test46_builtin_v16i8" } } */
/* { dg-final { scan-assembler "test46_builtin_v8i16.*:.*ldi.h.*,123.*test46_builtin_v8i16" } } */
/* { dg-final { scan-assembler "test46_builtin_v4i32.*:.*ldi.w.*,123.*test46_builtin_v4i32" } } */
/* { dg-final { scan-assembler "test46_builtin_v2i64.*:.*ldi.d.*,123.*test46_builtin_v2i64" } } */
/* { dg-final { scan-assembler "test46_min_v16i8.*:.*ldi.b\t\\\$w\\d+,-128.*test46_min_v16i8" } } */
/* { dg-final { scan-assembler "test46_min_v8i16.*:.*ldi.h\t\\\$w\\d+,-512.*test46_min_v8i16" } } */
/* { dg-final { scan-assembler "test46_min_v4i32.*:.*ldi.w\t\\\$w\\d+,-512.*test46_min_v4i32" } } */
/* { dg-final { scan-assembler "test46_min_v2i64.*:.*ldi.d\t\\\$w\\d+,-512.*test46_min_v2i64" } } */
/* { dg-final { scan-assembler "test46_max_v16i8.*:.*ldi.b\t\\\$w\\d+,127.*test46_max_v16i8" } } */
/* { dg-final { scan-assembler "test46_max_v8i16.*:.*ldi.h\t\\\$w\\d+,511.*test46_max_v8i16" } } */
/* { dg-final { scan-assembler "test46_max_v4i32.*:.*ldi.w\t\\\$w\\d+,511.*test46_max_v4i32" } } */
/* { dg-final { scan-assembler "test46_max_v2i64.*:.*ldi.d\t\\\$w\\d+,511.*test46_max_v2i64" } } */
/* { dg-final { scan-assembler "test47_builtin_v16i8.*:.*splat.b.*test47_builtin_v16i8" } } */
/* { dg-final { scan-assembler "test47_builtin_v8i16.*:.*splat.h.*test47_builtin_v8i16" } } */
/* { dg-final { scan-assembler "test47_builtin_v4i32.*:.*splat.w.*test47_builtin_v4i32" } } */
/* { dg-final { scan-assembler "test47_builtin_v2i64.*:.*splat.d.*test47_builtin_v2i64" } } */
/* { dg-final { scan-assembler "test48_builtin_v16i8.*:.*splati.b.*test48_builtin_v16i8" } } */
/* { dg-final { scan-assembler "test48_builtin_v8i16.*:.*splati.h.*test48_builtin_v8i16" } } */
/* { dg-final { scan-assembler "test48_builtin_v4i32.*:.*splati.w.*test48_builtin_v4i32" } } */
/* { dg-final { scan-assembler "test48_builtin_v2i64.*:.*splati.d.*test48_builtin_v2i64" } } */
/* { dg-final { scan-assembler "test49_v16i8.*:.*fill.b.*test49_v16i8" } } */
/* { dg-final { scan-assembler "test49_v8i16.*:.*fill.h.*test49_v8i16" } } */
/* { dg-final { scan-assembler "test49_v4i32.*:.*fill.w.*test49_v4i32" } } */
/* { dg-final { scan-assembler "test49_v2i64.*:.*fill.d.*test49_v2i64" { target mips64-*-* } } } */
/* { dg-final { scan-assembler "test49_v2i64.*:\(?=.*sra\)\(?=.*fill.w\).*test49_v2i64" { target mips-*-* } } } */
/* { dg-final { scan-assembler "test50_v16i8.*:.*insert.b.*test50_v16i8" } } */
/* { dg-final { scan-assembler "test50_v8i16.*:.*insert.h.*test50_v8i16" } } */
/* { dg-final { scan-assembler "test50_v4i32.*:.*insert.w.*test50_v4i32" } } */
/* { dg-final { scan-assembler "test50_v2i64.*:.*insert.d.*test50_v2i64" { target mips64-*-* } } } */
/* { dg-final { scan-assembler "test50_v2i64.*:.*sra\(.*insert.w\)\{2\}.*test50_v2i64" { target mips-*-* } } } */
/* { dg-final { scan-assembler "test51_builtin_v16i8.*:.*insve.b.*test51_builtin_v16i8" } } */
/* { dg-final { scan-assembler "test51_builtin_v8i16.*:.*insve.h.*test51_builtin_v8i16" } } */
/* { dg-final { scan-assembler "test51_builtin_v4i32.*:.*insve.w.*test51_builtin_v4i32" } } */
/* { dg-final { scan-assembler "test51_builtin_v2i64.*:.*insve.d.*test51_builtin_v2i64" } } */
/* { dg-final { scan-assembler "test51_v16i8.*:.*insve.b.*test51_v16i8" } } */
/* { dg-final { scan-assembler "test51_v8i16.*:.*insve.h.*test51_v8i16" } } */
/* { dg-final { scan-assembler "test51_v4i32.*:.*insve.w.*test51_v4i32" } } */
/* { dg-final { scan-assembler "test51_v2i64.*:.*insve.d.*test51_v2i64" } } */
/* { dg-final { scan-assembler "test52_builtin_v16i8.*:.*copy_s.b.*test52_builtin_v16i8" } } */
/* { dg-final { scan-assembler "test52_builtin_v8i16.*:.*copy_s.h.*test52_builtin_v8i16" } } */
/* { dg-final { scan-assembler "test52_builtin_v4i32.*:.*copy_s.w.*test52_builtin_v4i32" } } */
/* { dg-final { scan-assembler "test52_builtin_v2i64.*:.*copy_s.d.*test52_builtin_v2i64" { target mips64-*-* } } } */
/* { dg-final { scan-assembler "test52_builtin_v2i64.*:.*copy_s.w\t\\\$\\d+,\\\$w\\d+\\\[2\\\].*test52_builtin_v2i64" { target mips-*-* } } } */
/* { dg-final { scan-assembler "test52_v16i8.*:.*copy_s.b.*test52_v16i8" } } */
/* { dg-final { scan-assembler "test52_v8i16.*:.*copy_s.h.*test52_v8i16" } } */
/* { dg-final { scan-assembler "test52_v4i32.*:.*copy_s.w.*test52_v4i32" } } */
/* { dg-final { scan-assembler "test52_v2i64.*:.*copy_s.d.*test52_v2i64" { target mips64-*-* } } } */
/* { dg-final { scan-assembler "test52_v2i64.*:.*copy_s.w\t\\\$\\d+,\\\$w\\d+\\\[2\\\].*test52_v2i64" { target mips-*-* } } } */
/* { dg-final { scan-assembler "test53_builtin_v16i8.*:.*copy_u.b.*test53_builtin_v16i8" } } */
/* { dg-final { scan-assembler "test53_builtin_v8i16.*:.*copy_u.h.*test53_builtin_v8i16" } } */
/* { dg-final { scan-assembler "test53_builtin_v4i32.*:.*copy_u.w.*test53_builtin_v4i32" } } */
/* { dg-final { scan-assembler "test53_builtin_v2i64.*:.*copy_u.d.*test53_builtin_v2i64" { target mips64-*-* } } } */
/* { dg-final { scan-assembler "test53_builtin_v2i64.*:.*copy_u.w\t\\\$\\d+,\\\$w\\d+\\\[2\\\].*test53_builtin_v2i64" { target mips-*-* } } } */
/* { dg-final { scan-assembler "test53_v16i8.*:.*copy_u.b.*test53_v16i8" } } */
/* { dg-final { scan-assembler "test53_v8i16.*:.*copy_u.h.*test53_v8i16" } } */
/* { dg-final { scan-assembler "test53_v4i32.*:.*copy_u.w.*test53_v4i32" } } */
/* { dg-final { scan-assembler "test53_v2i64.*:.*copy_u.d.*test53_v2i64" { target mips64-*-* } } } */
/* { dg-final { scan-assembler "test53_v2i64.*:.*copy_u.w\t\\\$\\d+,\\\$w\\d+\\\[2\\\].*test53_v2i64" { target mips-*-* } } } */
/* { dg-final { scan-assembler "test54_builtin_v16i8.*:.*st.b.*test54_builtin_v16i8" } } */
/* { dg-final { scan-assembler "test54_builtin_v8i16.*:.*st.h.*test54_builtin_v8i16" } } */
/* { dg-final { scan-assembler "test54_builtin_v4i32.*:.*st.w.*test54_builtin_v4i32" } } */
/* { dg-final { scan-assembler "test54_builtin_v2i64.*:.*st.d.*test54_builtin_v2i64" } } */
/* { dg-final { scan-assembler "test54_v16i8.*:.*st.b.*test54_v16i8" } } */
/* { dg-final { scan-assembler "test54_v8i16.*:.*st.h.*test54_v8i16" } } */
/* { dg-final { scan-assembler "test54_v4i32.*:.*st.w.*test54_v4i32" } } */
/* { dg-final { scan-assembler "test54_v2i64.*:.*st.d.*test54_v2i64" } } */

typedef signed char v16i8 __attribute__ ((vector_size(16)));
typedef short v8i16 __attribute__ ((vector_size(16)));
typedef int v4i32 __attribute__ ((vector_size(16)));
typedef long long v2i64 __attribute__ ((vector_size(16)));
typedef unsigned char v16u8 __attribute__ ((vector_size(16)));
typedef unsigned short v8u16 __attribute__ ((vector_size(16)));
typedef unsigned int v4u32 __attribute__ ((vector_size(16)));
typedef unsigned long long v2u64 __attribute__ ((vector_size(16)));
typedef float v4f32 __attribute__ ((vector_size(16)));
typedef double v2f64 __attribute__ ((vector_size(16)));

/*
typedef signed char v8i8 __attribute__ ((vector_size(8)));
typedef short v4i16 __attribute__ ((vector_size(8)));
typedef int v2i32 __attribute__ ((vector_size(8)));
typedef float v2f32 __attribute__ ((vector_size(8)));

typedef signed char v4i8 __attribute__ ((vector_size(4)));
typedef short v2i16 __attribute__ ((vector_size(4)));
*/

typedef long long i64;
typedef int i32;
typedef short i16;
typedef signed char i8;
typedef double f64;
typedef float f32;

float imm_f = 37.0;

#define v16i8_DF b
#define v8i16_DF h
#define v4i32_DF w
#define v2i64_DF d
#define v16u8_DF b
#define v8u16_DF h
#define v4u32_DF w
#define v2u64_DF d

#define v16i8_INITV V16
#define v8i16_INITV V8
#define v4i32_INITV V4
#define v2i64_INITV V2
#define v16u8_INITV V16
#define v8u16_INITV V8
#define v4u32_INITV V4
#define v2u64_INITV V2

#define v16i8_LDI_MIN -128
#define v16i8_LDI_MAX 127
#define v8i16_LDI_MIN -512
#define v8i16_LDI_MAX 511
#define v4i32_LDI_MIN -512
#define v4i32_LDI_MAX 511
#define v2i64_LDI_MIN -512
#define v2i64_LDI_MAX 511

#define VE2(VALUE) (VALUE), (VALUE)
#define VE4(VALUE) VE2(VALUE), VE2(VALUE)
#define VE8(VALUE) VE4(VALUE), VE4(VALUE)
#define VE16(VALUE) VE8(VALUE), VE8(VALUE)

#define V16(TYPE, VALUE) (TYPE) { VE16(VALUE) }
#define V8(TYPE, VALUE) (TYPE) { VE8(VALUE) }
#define V4(TYPE, VALUE) (TYPE) { VE4(VALUE) }
#define V2(TYPE, VALUE) (TYPE) { VE2(VALUE) }

#define PASTE_BUILTIN(NAME, DF) __builtin_msa_ ## NAME ## _ ## DF
#define EVAL_BUILTIN(NAME, DF) PASTE_BUILTIN(NAME, DF)
#define BUILD_BUILTIN(NAME, DF) EVAL_BUILTIN(NAME, DF)

#define INIT_VECTOR(TYPE, VALUE) TYPE ## _INITV (TYPE, VALUE)


#define DECLARE(TYPE) TYPE TYPE ## _0, TYPE ## _1, TYPE ## _2;
#define RETURN(TYPE) NOMIPS16 TYPE test0_ ## TYPE () { return TYPE ## _0; }
#define ASSIGN(TYPE) NOMIPS16 void test1_ ## TYPE (TYPE i) { TYPE ## _1 = i; }
#define ADD(TYPE) NOMIPS16 TYPE test2_ ## TYPE (TYPE i, TYPE j) { return i + j; }
#define SUB(TYPE) NOMIPS16 TYPE test3_ ## TYPE (TYPE i, TYPE j) { return i - j; }
#define MUL(TYPE) NOMIPS16 TYPE test4_ ## TYPE (TYPE i, TYPE j) { return i * j; }
#define DIV(TYPE) TYPE test5_ ## TYPE (TYPE i, TYPE j) { return i / j; }
#define MOD(TYPE) TYPE test6_ ## TYPE (TYPE i, TYPE j) { return i % j; }
#define MINUS(TYPE) TYPE test7_ ## TYPE (TYPE i) { return -i; }
#define XOR(TYPE) TYPE test8_ ## TYPE (TYPE i, TYPE j) { return i ^ j; }
#define OR(TYPE) TYPE test9_ ## TYPE (TYPE i, TYPE j) { return i | j; }
#define AND(TYPE) TYPE test10_ ## TYPE (TYPE i, TYPE j) { return i & j; }
#define BIT_COMPLEMENT(TYPE) TYPE test11_ ## TYPE (TYPE i) { return ~i; }
#define SHIFT_RIGHT(TYPE) TYPE test12_ ## TYPE (TYPE i, TYPE j) { return i >> j; }
#define SHIFT_LEFT(TYPE) TYPE test13_ ## TYPE (TYPE i, TYPE j) { return i << j; }
#define EQ(TYPE) TYPE test14_ ## TYPE (TYPE i, TYPE j) { return i == j; }
#define NEQ(TYPE) TYPE test15_ ## TYPE (TYPE i, TYPE j) { return i != j; }
#define LT(TYPE) TYPE test16_ ## TYPE (TYPE i, TYPE j) { return i < j; }
#define LEQ(TYPE) TYPE test17_ ## TYPE (TYPE i, TYPE j) { return i <= j; }
#define GT(TYPE) TYPE test18_ ## TYPE (TYPE i, TYPE j) { return i > j; }
#define GEQ(TYPE) TYPE test19_ ## TYPE (TYPE i, TYPE j) { return i >= j; }

#define ADD_I(TYPE) TYPE test20_ ## TYPE (TYPE i) { return i + 31; }
#define SUB_I(TYPE) TYPE test21_ ## TYPE (TYPE i) { return i - 31; }
#define MUL_I(TYPE) TYPE test22_ ## TYPE (TYPE i) { return i * 37; }
#define DIV_I(TYPE) TYPE test23_ ## TYPE (TYPE i) { return i / 37; }
#define MOD_I(TYPE) TYPE test24_ ## TYPE (TYPE i) { return i % 37; }
#define XOR_I(TYPE) TYPE test25_ ## TYPE (TYPE i) { return i ^ 37; }
#define OR_I(TYPE) TYPE test26_ ## TYPE (TYPE i) { return i | 37; }
#define AND_I(TYPE) TYPE test27_ ## TYPE (TYPE i) { return i & 37; }
#define SHIFT_RIGHT_I(TYPE) TYPE test28_ ## TYPE (TYPE i) { return i >> 3; }
#define SHIFT_LEFT_I(TYPE) TYPE test29_ ## TYPE (TYPE i) { return i << 3; }
#define EQ_I(TYPE) TYPE test30_ ## TYPE (TYPE i) { return i == 5; }
#define LT_S_I(TYPE) TYPE test31_s_ ## TYPE (TYPE i) { return i < 5; }
#define LT_U_I(TYPE) TYPE test31_u_ ## TYPE (TYPE i) { return i < (unsigned) 5; }
#define LEQ_S_I(TYPE) TYPE test32_s_ ## TYPE (TYPE i) { return i <= 5; }
#define LEQ_U_I(TYPE) TYPE test32_u_ ## TYPE (TYPE i) { return i <= (unsigned) 5; }

#define ADD_F(TYPE) TYPE test33_ ## TYPE (TYPE i) { return i + imm_f; }
#define SUB_F(TYPE) TYPE test34_ ## TYPE (TYPE i) { return i - imm_f; }
#define MUL_F(TYPE) TYPE test35_ ## TYPE (TYPE i) { return i * imm_f; }
#define DIV_F(TYPE) TYPE test36_ ## TYPE (TYPE i) { return i / imm_f; }

#define SHUFFLE1(TYPE) TYPE test37_ ## TYPE (TYPE i, TYPE mask) { return __builtin_shuffle (i, mask); }
#define SHUFFLE2(TYPE) TYPE test38_ ## TYPE (TYPE i, TYPE j, TYPE mask) { return __builtin_shuffle (i, j, mask); }

#define REAL_SHUFFLE1(TYPE, MASK_TYPE) TYPE test39_ ## TYPE (TYPE i, MASK_TYPE mask) { return __builtin_shuffle (i, mask); }
#define REAL_SHUFFLE2(TYPE, MASK_TYPE) TYPE test40_ ## TYPE (TYPE i, TYPE j, MASK_TYPE mask) { return __builtin_shuffle (i, j, mask); }

#define MADD(TYPE) TYPE test41_ ## TYPE (TYPE i, TYPE j, TYPE k) { return i * j + k; }
#define MSUB(TYPE) TYPE test42_ ## TYPE (TYPE i, TYPE j, TYPE k) { return k - i * j; }

/* MSA Load/Store and Move instructions */
#define CFCMSA_BUILTIN() int test43_builtin () { return __builtin_msa_cfcmsa(0x1f); }
#define CTCMSA_BUILTIN() void test44_builtin (int i) { return __builtin_msa_ctcmsa(0x1f, i); }
#define LOAD_V_BUILTIN(TYPE) TYPE test45_builtin_ ## TYPE (TYPE *i) { return BUILD_BUILTIN(ld, TYPE ## _DF) (i, 0); }
#define LOAD_V(TYPE) TYPE test45_ ## TYPE (TYPE *i) { return *i; }
#define LOAD_I_BUILTIN(TYPE) TYPE test46_builtin_ ## TYPE (TYPE *i) { return BUILD_BUILTIN(ldi, TYPE ## _DF) (123); }
#define LOAD_I_MIN(TYPE) TYPE test46_min_ ## TYPE (TYPE *i) { return INIT_VECTOR(TYPE, TYPE ## _LDI_MIN); }
#define LOAD_I_MAX(TYPE) TYPE test46_max_ ## TYPE (TYPE *i) { return INIT_VECTOR(TYPE, TYPE ## _LDI_MAX); }
#define SPLAT_BUILTIN(TYPE) TYPE test47_builtin_ ## TYPE (TYPE i, int j) { return BUILD_BUILTIN(splat, TYPE ## _DF) (i, j); }
#define SPLATI_BUILTIN(TYPE) TYPE test48_builtin_ ## TYPE (TYPE i, int j) { return BUILD_BUILTIN(splati, TYPE ## _DF) (i, 1); }
#define FILL(TYPE) TYPE test49_ ## TYPE (int i) { return INIT_VECTOR(TYPE, i); }
#define INSERT(TYPE) TYPE test50_ ## TYPE (int i) { TYPE a = INIT_VECTOR(TYPE, 0); a[1] = i; return a; }
#define INSVE_BUILTIN(TYPE) TYPE test51_builtin_ ## TYPE (TYPE i, TYPE j) { return BUILD_BUILTIN(insve, TYPE ## _DF) (i, 1, j); }
#define INSVE(TYPE) TYPE test51_ ## TYPE (TYPE i) { TYPE a = INIT_VECTOR(TYPE, 0); a[1] = i[0]; return a; }
#define COPY_S_BUILTIN(TYPE) signed int test52_builtin_ ## TYPE (TYPE i) { return BUILD_BUILTIN(copy_s, TYPE ## _DF) (i, 1); }
#define COPY_S(TYPE) signed int test52_ ## TYPE (TYPE i) { return i[1]; }
#define COPY_U_BUILTIN(TYPE) unsigned int test53_builtin_ ## TYPE (TYPE i) { return BUILD_BUILTIN(copy_u, TYPE ## _DF) (i, 1); }
#define COPY_U(TYPE) unsigned int test53_ ## TYPE (TYPE i) { return (unsigned int) i[1]; }
#define STORE_V_BUILTIN(TYPE) void test54_builtin_ ## TYPE (TYPE i, TYPE *j) { BUILD_BUILTIN(st, TYPE ## _DF) (i, j, -64); }
#define STORE_V(TYPE) void test54_ ## TYPE (TYPE i) { TYPE ## _0 = i; }

#define ITERATE_FOR_ALL_SIGNED_INT_VECTOR_TYPES(FUNC) \
  FUNC (v16i8) \
  FUNC (v8i16) \
  FUNC (v4i32) \
  FUNC (v2i64)

#define ITERATE_FOR_ALL_UNSIGNED_INT_VECTOR_TYPES(FUNC) \
  FUNC (v16u8) \
  FUNC (v8u16) \
  FUNC (v4u32) \
  FUNC (v2u64)

/*
  FUNC (v8i8) \
  FUNC (v4i16) \
  FUNC (v2i32) \
  FUNC (v4i8) \
  FUNC (v2i16)
*/

#define ITERATE_FOR_ALL_INT_SCALAR_TYPES(FUNC) \
  FUNC (i64) \
  FUNC (i32) \
  FUNC (i16) \
  FUNC (i8)

#define ITERATE_FOR_ALL_INT_VECTOR_TYPES(FUNC) \
 ITERATE_FOR_ALL_SIGNED_INT_VECTOR_TYPES(FUNC) \
 ITERATE_FOR_ALL_UNSIGNED_INT_VECTOR_TYPES(FUNC)

#define ITERATE_FOR_ALL_INT_TYPES(FUNC) \
  ITERATE_FOR_ALL_INT_VECTOR_TYPES(FUNC) \

/*
  ITERATE_FOR_ALL_INT_SCALAR_TYPES(FUNC)
*/

#define ITERATE_FOR_ALL_REAL_VECTOR_TYPES(FUNC) \
  FUNC (v4f32) \
  FUNC (v2f64) \

/*
  FUNC (v2f32)
*/

#define ITERATE_FOR_ALL_REAL_SCALAR_TYPES(FUNC) \
  FUNC (f64) \
  FUNC (f32)

#define ITERATE_FOR_ALL_REAL_TYPES(FUNC) \
  ITERATE_FOR_ALL_REAL_VECTOR_TYPES(FUNC) \

/*
  ITERATE_FOR_ALL_REAL_SCALAR_TYPES(FUNC)
*/

#define ITERATE_FOR_ALL_TYPES(FUNC) \
  ITERATE_FOR_ALL_INT_TYPES(FUNC) \
  ITERATE_FOR_ALL_REAL_TYPES(FUNC)

ITERATE_FOR_ALL_TYPES (ADD)
ITERATE_FOR_ALL_TYPES (SUB)
ITERATE_FOR_ALL_TYPES (MUL)
ITERATE_FOR_ALL_TYPES (DIV)
ITERATE_FOR_ALL_INT_TYPES (MOD)
ITERATE_FOR_ALL_INT_TYPES (XOR)
ITERATE_FOR_ALL_INT_TYPES (OR)
ITERATE_FOR_ALL_INT_TYPES (AND)
ITERATE_FOR_ALL_INT_TYPES (SHIFT_RIGHT)
ITERATE_FOR_ALL_INT_TYPES (SHIFT_LEFT)
ITERATE_FOR_ALL_TYPES (MINUS)
ITERATE_FOR_ALL_INT_TYPES (BIT_COMPLEMENT)
ITERATE_FOR_ALL_TYPES (MADD)
ITERATE_FOR_ALL_TYPES (MSUB)
ITERATE_FOR_ALL_INT_VECTOR_TYPES (SHUFFLE1)
ITERATE_FOR_ALL_INT_VECTOR_TYPES (SHUFFLE2)
REAL_SHUFFLE1 (v2f64, v2i64)
REAL_SHUFFLE2 (v2f64, v2i64)
REAL_SHUFFLE1 (v4f32, v4i32)
REAL_SHUFFLE2 (v4f32, v4i32)

ITERATE_FOR_ALL_TYPES (DECLARE)
ITERATE_FOR_ALL_TYPES (RETURN)
ITERATE_FOR_ALL_TYPES (ASSIGN)
ITERATE_FOR_ALL_INT_TYPES (ADD_I)
ITERATE_FOR_ALL_INT_TYPES (SUB_I)
ITERATE_FOR_ALL_INT_TYPES (MUL_I)
ITERATE_FOR_ALL_INT_TYPES (DIV_I)
ITERATE_FOR_ALL_INT_TYPES (MOD_I)
ITERATE_FOR_ALL_INT_TYPES (XOR_I)
ITERATE_FOR_ALL_INT_TYPES (OR_I)
ITERATE_FOR_ALL_INT_TYPES (AND_I)
ITERATE_FOR_ALL_INT_TYPES (SHIFT_RIGHT_I)
ITERATE_FOR_ALL_INT_TYPES (SHIFT_LEFT_I)
ITERATE_FOR_ALL_REAL_TYPES (ADD_F)
ITERATE_FOR_ALL_REAL_TYPES (SUB_F)
ITERATE_FOR_ALL_REAL_TYPES (MUL_F)
ITERATE_FOR_ALL_REAL_TYPES (DIV_F)
ITERATE_FOR_ALL_TYPES (EQ)
ITERATE_FOR_ALL_TYPES (EQ_I)
ITERATE_FOR_ALL_TYPES (NEQ)
ITERATE_FOR_ALL_TYPES (LT)
ITERATE_FOR_ALL_SIGNED_INT_VECTOR_TYPES(LT_S_I)
ITERATE_FOR_ALL_UNSIGNED_INT_VECTOR_TYPES(LT_U_I)
ITERATE_FOR_ALL_TYPES (LEQ)
ITERATE_FOR_ALL_SIGNED_INT_VECTOR_TYPES(LEQ_S_I)
ITERATE_FOR_ALL_UNSIGNED_INT_VECTOR_TYPES(LEQ_U_I)
ITERATE_FOR_ALL_TYPES (GT)
ITERATE_FOR_ALL_TYPES (GEQ)

CFCMSA_BUILTIN()
CTCMSA_BUILTIN()
ITERATE_FOR_ALL_SIGNED_INT_VECTOR_TYPES(LOAD_V_BUILTIN)
ITERATE_FOR_ALL_SIGNED_INT_VECTOR_TYPES(LOAD_V)
ITERATE_FOR_ALL_SIGNED_INT_VECTOR_TYPES(LOAD_I_BUILTIN)
ITERATE_FOR_ALL_SIGNED_INT_VECTOR_TYPES(LOAD_I_MIN)
ITERATE_FOR_ALL_SIGNED_INT_VECTOR_TYPES(LOAD_I_MAX)
ITERATE_FOR_ALL_SIGNED_INT_VECTOR_TYPES(SPLAT_BUILTIN)
ITERATE_FOR_ALL_SIGNED_INT_VECTOR_TYPES(SPLATI_BUILTIN)
ITERATE_FOR_ALL_SIGNED_INT_VECTOR_TYPES(FILL)
ITERATE_FOR_ALL_SIGNED_INT_VECTOR_TYPES(INSERT)
ITERATE_FOR_ALL_SIGNED_INT_VECTOR_TYPES(INSVE_BUILTIN)
ITERATE_FOR_ALL_SIGNED_INT_VECTOR_TYPES(INSVE)
ITERATE_FOR_ALL_SIGNED_INT_VECTOR_TYPES(COPY_S_BUILTIN)
ITERATE_FOR_ALL_SIGNED_INT_VECTOR_TYPES(COPY_S)
ITERATE_FOR_ALL_SIGNED_INT_VECTOR_TYPES(COPY_U_BUILTIN)
ITERATE_FOR_ALL_UNSIGNED_INT_VECTOR_TYPES(COPY_U)
ITERATE_FOR_ALL_SIGNED_INT_VECTOR_TYPES(STORE_V_BUILTIN)
ITERATE_FOR_ALL_SIGNED_INT_VECTOR_TYPES(STORE_V)

