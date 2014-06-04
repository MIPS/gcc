;; Machine Description for MIPS MSA ASE
;; Contributed by Chao-ying Fu (fu@mips.com), MIPS Technologies, Inc.
;;
;; Copyright (C) 2014 Free Software Foundation, Inc.
;;
;; This file is part of GCC.
;;
;; GCC is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 3, or (at your option)
;; any later version.
;;
;; GCC is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.
;;

;; Attributes to categorize MSA instructions based on execution units
(define_attr "msa_execunit"
  "unknown, msa_eu_div, msa_eu_float2, msa_eu_float2_l,
  msa_eu_float4, msa_eu_float5, msa_eu_float8, msa_eu_logic,
  msa_eu_logic3, msa_eu_logic_l, msa_eu_mult, msa_eu_cmp,
  msa_eu_store4, msa_eu_int_add, msa_eu_fdiv"
  (const_string "unknown"))

