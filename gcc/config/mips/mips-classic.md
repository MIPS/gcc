;;  Mips.md	     Machine Description for MIPS based processors
;;  Copyright (C) 1989-2016 Free Software Foundation, Inc.
;;  Contributed by   A. Lichnewsky, lich@inria.inria.fr
;;  Changes by       Michael Meissner, meissner@osf.org
;;  64-bit r4000 support by Ian Lance Taylor, ian@cygnus.com, and
;;  Brendan Eich, brendan@microunity.com.

;; This file is part of GCC.

;; GCC is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 3, or (at your option)
;; any later version.

;; GCC is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.

(define_enum "processor" [
  r3000
  4kc
  4kp
  5kc
  5kf
  20kc
  24kc
  24kf2_1
  24kf1_1
  74kc
  74kf2_1
  74kf1_1
  74kf3_2
  interaptiv_mr2
  loongson_2e
  loongson_2f
  loongson_3a
  m4k
  octeon
  octeon2
  octeon3
  r3900
  r6000
  r4000
  r4100
  r4111
  r4120
  r4130
  r4300
  r4600
  r4650
  r4700
  r5000
  r5400
  r5500
  r5900
  r7000
  r8000
  r9000
  r10000
  sb1
  sb1a
  sr71000
  xlr
  xlp
  p5600
  m5100
  i6400
  m6200
  p6600
])

(include "mips.md")

(include "i6400.md")
(include "p5600.md")
(include "m5100.md")
(include "m6200.md")
(include "p6600.md")
(include "4k.md")
(include "5k.md")
(include "20kc.md")
(include "24k.md")
(include "74k.md")
(include "3000.md")
(include "4000.md")
(include "4100.md")
(include "4130.md")
(include "4300.md")
(include "4600.md")
(include "5000.md")
(include "5400.md")
(include "5500.md")
(include "6000.md")
(include "7000.md")
(include "9000.md")
(include "10000.md")
(include "loongson2ef.md")
(include "loongson3a.md")
(include "octeon.md")
(include "sb1.md")
(include "sr71k.md")
(include "xlr.md")
(include "xlp.md")

;; Synchronization instructions.

(include "sync.md")

; The MIPS Paired-Single Floating Point and MIPS-3D Instructions.

(include "mips-ps-3d.md")

; The MIPS DSP Instructions.

(include "mips-dsp.md")

; The MIPS DSP REV 2 Instructions.

(include "mips-dspr2.md")

; MIPS fixed-point instructions.
(include "mips-fixed.md")

; microMIPS patterns.
(include "micromips.md")

;; Synchronization instructions.

(include "sync.md")

; The MIPS Paired-Single Floating Point and MIPS-3D Instructions.

(include "mips-ps-3d.md")

; The MIPS DSP Instructions.

(include "mips-dsp.md")

; The MIPS DSP REV 2 Instructions.

(include "mips-dspr2.md")

; MIPS fixed-point instructions.
(include "mips-fixed.md")

; ST-Microelectronics Loongson-2E/2F-specific patterns.
(include "loongson.md")

; The MIPS MSA Instructions.
(include "mips-msa.md")

(define_c_enum "unspec" [
  UNSPEC_ADDRESS_FIRST
])
