;; AMD Athlon Scheduling
;;
;; The Athlon does contain three pipelined FP units, three integer units and
;; three address generation units. 
;;
;; The predecode logic is determining boundaries of instructions in the 64
;; byte cache line. So the cache line straddling problem of K6 might be issue
;; here as well, but it is not noted in the documentation.
;;
;; Three DirectPath instructions decoders and only one VectorPath decoder
;; is available. They can decode three DirectPath instructions or one VectorPath
;; instruction per cycle.
;; Decoded macro instructions are then passed to 72 entry instruction control
;; unit, that passes
;; it to the specialized integer (18 entry) and fp (36 entry) schedulers.
;;
;; The load/store queue unit is not attached to the schedulers but
;; communicates with all the execution units separately instead.

(define_attr "athlon_decode" "direct,vector"
  (cond [(eq_attr "type" "call,imul,idiv,other,multi,fcmov,fpspc,str,pop,cld")
	   (const_string "vector")
         (and (eq_attr "type" "push")
              (match_operand 1 "memory_operand" ""))
	   (const_string "vector")
         (and (eq_attr "type" "fmov")
	      (and (eq_attr "memory" "load,store")
		   (eq_attr "mode" "XF")))
	   (const_string "vector")]
	(const_string "direct")))

;;
;;           decode0 decode1 decode2
;;                 \    |   /
;;    instruction control unit (72 entry scheduler)
;;                |                        |
;;      integer scheduler (18)         stack map
;;     /  |    |    |    |   \        stack rename
;;  ieu0 agu0 ieu1 agu1 ieu2 agu2      scheduler
;;    |  agu0  |   agu1      agu2    register file
;;    |      \ |    |       /         |     |     |
;;     \      /\    |     /         fadd  fmul  fstore
;;       \  /    \  |   /           fadd  fmul  fstore
;;       imul  load/store (2x)      fadd  fmul  fstore

(define_automaton "athlon,athlon_mult,athlon_fp")
(define_cpu_unit "athlon-decode0" "athlon")
(define_cpu_unit "athlon-decode1" "athlon")
(define_cpu_unit "athlon-decode2" "athlon")
(define_reservation "athlon-vector" "(athlon-decode0 + athlon-decode1
				     + athlon-decode2)")
(define_reservation "athlon-direct" "(athlon-decode0 | athlon-decode1
				     | athlon-decode2)")

;; Agu and ieu unit results in extremly large automatons and
;; in our approximation they are hardly filled in.  Only ieu
;; unit can, as issue rate is 3 and agu unit is always used
;; first in the insn reservations.  Skip the models.

;(define_cpu_unit "athlon-ieu0" "athlon_ieu")
;(define_cpu_unit "athlon-ieu1" "athlon_ieu")
;(define_cpu_unit "athlon-ieu2" "athlon_ieu")
;(define_reservation "athlon-ieu" "(athlon-ieu0 | athlon-ieu1 | athlon-ieu2)")
(define_reservation "athlon-ieu" "nothing")
;(define_cpu_unit "athlon-agu0" "athlon_agu")
;(define_cpu_unit "athlon-agu1" "athlon_agu")
;(define_cpu_unit "athlon-agu2" "athlon_agu")
;(define_reservation "athlon-agu" "(athlon-agu0 | athlon-agu1 | athlon-agu2)")
(define_reservation "athlon-agu" "nothing,nothing")

(define_cpu_unit "athlon-mult" "athlon_mult")

(define_cpu_unit "athlon-load0" "athlon")
(define_cpu_unit "athlon-load1" "athlon")
(define_reservation "athlon-load" "athlon-agu,
				   (athlon-load0 | athlon-load1)")
(define_reservation "athlon-store" "nothing")

;; The three fp units are fully pipelined with latency of 3
(define_cpu_unit "athlon-fadd" "athlon_fp")
(define_cpu_unit "athlon-fmul" "athlon_fp")
(define_cpu_unit "athlon-fstore" "athlon_fp")
(define_reservation "athlon-fany" "(athlon-fadd | athlon-fmul | athlon-fstore)")
(define_reservation "athlon-faddmul" "(athlon-fadd | athlon-fmul)")


;; Latency of push operation is 3 cycles, but ESP value is available
;; earlier
(define_insn_reservation "athlon_push" 2
			 (and (eq_attr "cpu" "athlon")
			      (eq_attr "type" "push"))
			 "athlon-direct,nothing,athlon-store")
(define_insn_reservation "athlon_pop" 4
			 (and (eq_attr "cpu" "athlon")
			      (eq_attr "type" "push"))
			 "athlon-vector,athlon-ieu,athlon-load")
(define_insn_reservation "athlon_leave" 3
			 (and (eq_attr "cpu" "athlon")
			      (eq_attr "type" "push"))
			 "athlon-vector,athlon-load")

;; Lea executes in AGU unit with 2 cycles latency.
(define_insn_reservation "athlon_lea" 2
			 (and (eq_attr "cpu" "athlon")
			      (eq_attr "type" "lea"))
			 "athlon-direct,athlon-agu")

;; Mul executes in special multiplier unit attached to IEU0
(define_insn_reservation "athlon_imul" 5
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "type" "imul")
				   (eq_attr "memory" "none")))
			 "athlon-vector,athlon-ieu,athlon-mult,nothing,nothing,athlon-ieu")
(define_insn_reservation "athlon_imul_mem" 8
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "type" "imul")
				   (eq_attr "memory" "load,both")))
			 "athlon-vector,athlon-load,athlon-ieu,athlon-mult,nothing,nothing,athlon-ieu")
(define_insn_reservation "athlon_idiv" 42
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "type" "idiv")
				   (eq_attr "memory" "none")))
			 "athlon-vector,athlon-ieu*42")
(define_insn_reservation "athlon_idiv_mem" 45
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "type" "idiv")
				   (eq_attr "memory" "load,both")))
			 "athlon-vector,athlon-load,athlon-ieu*42")
(define_insn_reservation "athlon_str" 15
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "type" "str")
				   (eq_attr "memory" "load,both,store")))
			 "athlon-vector,athlon-load,athlon-ieu*10")

(define_insn_reservation "athlon_idirect" 1
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "athlon_decode" "direct")
				   (and (eq_attr "unit" "integer,unknown")
					(eq_attr "memory" "none"))))
			 "athlon-direct,athlon-ieu")
(define_insn_reservation "athlon_ivector" 2
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "athlon_decode" "vector")
				   (and (eq_attr "unit" "integer,unknown")
					(eq_attr "memory" "none"))))
			 "athlon-vector,athlon-ieu,athlon-ieu")
(define_insn_reservation "athlon_idirect_loadmov" 3
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "type" "imov")
				   (eq_attr "memory" "load")))
			 "athlon-direct,athlon-load")
(define_insn_reservation "athlon_idirect_load" 4
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "athlon_decode" "direct")
				   (and (eq_attr "unit" "integer,unknown")
					(eq_attr "memory" "load"))))
			 "athlon-direct,athlon-load,athlon-ieu")
(define_insn_reservation "athlon_ivector_load" 6
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "athlon_decode" "vector")
				   (and (eq_attr "unit" "integer,unknown")
					(eq_attr "memory" "load"))))
			 "athlon-vector,athlon-load,athlon-ieu,athlon-ieu")
(define_insn_reservation "athlon_idirect_movstore" 3
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "type" "imov")
				   (eq_attr "memory" "store")))
			 "athlon-direct,athlon-agu,athlon-store")
(define_insn_reservation "athlon_idirect_both" 4
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "athlon_decode" "direct")
				   (and (eq_attr "unit" "integer,unknown")
					(eq_attr "memory" "both"))))
			 "athlon-direct,athlon-load,athlon-ieu,
			  athlon-store")
(define_insn_reservation "athlon_ivector_both" 6
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "athlon_decode" "vector")
				   (and (eq_attr "unit" "integer,unknown")
					(eq_attr "memory" "both"))))
			 "athlon-vector,athlon-load,athlon-ieu,athlon-ieu,
			  athlon-store")
(define_insn_reservation "athlon_idirect_store" 1
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "athlon_decode" "direct")
				   (and (eq_attr "unit" "integer,unknown")
					(eq_attr "memory" "store"))))
			 "athlon-direct,athlon-ieu,
			  athlon-store")
(define_insn_reservation "athlon_ivector_store" 2
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "athlon_decode" "vector")
				   (and (eq_attr "unit" "integer,unknown")
					(eq_attr "memory" "store"))))
			 "athlon-vector,athlon-ieu,athlon-ieu,
			  athlon-store")

;; Athlon floatin point unit
(define_insn_reservation "athlon_fldxf" 12
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "type" "fmov")
				   (and (eq_attr "memory" "load")
					(eq_attr "mode" "XF"))))
			 "athlon-vector,athlon-fany")
(define_insn_reservation "athlon_fld" 6
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "type" "fmov")
				   (eq_attr "memory" "load")))
			 "athlon-direct,athlon-fany,nothing,athlon-load")
(define_insn_reservation "athlon_fstxf" 10
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "type" "fmov")
				   (and (eq_attr "memory" "store,both")
					(eq_attr "mode" "XF"))))
			 "athlon-vector,athlon-fstore")
(define_insn_reservation "athlon_fst" 4
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "type" "fmov")
				   (eq_attr "memory" "store,both")))
			 "athlon-direct,athlon-fstore,nothing,athlon-store")
(define_insn_reservation "athlon_fist" 4
			 (and (eq_attr "cpu" "athlon")
			      (eq_attr "type" "fistp"))
			 "athlon-direct,athlon-fstore,nothing")
(define_insn_reservation "athlon_fmov" 2
			 (and (eq_attr "cpu" "athlon")
			      (eq_attr "type" "fmov"))
			 "athlon-direct,athlon-faddmul")
(define_insn_reservation "athlon_fadd_load" 7
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "type" "fop")
				   (eq_attr "memory" "load")))
			 "athlon-direct,athlon-load,athlon-fadd")
(define_insn_reservation "athlon_fadd" 4
			 (and (eq_attr "cpu" "athlon")
			      (eq_attr "type" "fop"))
			 "athlon-direct,athlon-fadd")
(define_insn_reservation "athlon_fmul_load" 7
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "type" "fmul")
				   (eq_attr "memory" "load")))
			 "athlon-direct,athlon-load,athlon-fmul")
(define_insn_reservation "athlon_fmul" 4
			 (and (eq_attr "cpu" "athlon")
			      (eq_attr "type" "fmul"))
			 "athlon-direct,athlon-fmul")
(define_insn_reservation "athlon_fsgn" 2
			 (and (eq_attr "cpu" "athlon")
			      (eq_attr "type" "fsgn"))
			 "athlon-direct,athlon-fmul")
(define_insn_reservation "athlon_fdiv_load" 24
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "type" "fdiv")
				   (eq_attr "memory" "load")))
			 "athlon-direct,athlon-load,athlon-fmul")
(define_insn_reservation "athlon_fdiv" 24
			 (and (eq_attr "cpu" "athlon")
			      (eq_attr "type" "fdiv"))
			 "athlon-direct,athlon-fmul")
(define_insn_reservation "athlon_fpspc_load" 103
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "type" "fpspc")
				   (eq_attr "memory" "load")))
			 "athlon-vector,athlon-load,athlon-fmul")
(define_insn_reservation "athlon_fpspc" 100
			 (and (eq_attr "cpu" "athlon")
			      (eq_attr "type" "fpspc"))
			 "athlon-vector,athlon-fmul")
(define_insn_reservation "athlon_fcmov_load" 10
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "type" "fcmov")
				   (eq_attr "memory" "load")))
			 "athlon-vector,athlon-load,athlon-fmul")
(define_insn_reservation "athlon_fcmov" 7
			 (and (eq_attr "cpu" "athlon")
			      (eq_attr "type" "fcmov"))
			 "athlon-vector,athlon-fmul")
(define_insn_reservation "athlon_fcomi_load" 6
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "type" "fcmp")
				   (and (eq_attr "athlon_decode" "vector")
				        (eq_attr "memory" "load"))))
			 "athlon-vector,athlon-load,athlon-fadd")
(define_insn_reservation "athlon_fcomi" 3
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "athlon_decode" "vector")
				   (eq_attr "type" "fcmp")))
			 "athlon-vector,athlon-fadd")
(define_insn_reservation "athlon_fcom_load" 5
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "type" "fcmp")
				   (eq_attr "memory" "load")))
			 "athlon-direct,athlon-load,athlon-fadd")
(define_insn_reservation "athlon_fcom" 2
			 (and (eq_attr "cpu" "athlon")
			      (eq_attr "type" "fcmp"))
			 "athlon-direct,athlon-fadd")
(define_insn_reservation "athlon_fxch" 2
			 (and (eq_attr "cpu" "athlon")
			      (eq_attr "type" "fxch"))
			 "athlon-direct,athlon-fany")
;; Athlon handle MMX operations in the FPU unit with shorter latencies
(define_insn_reservation "athlon_mmxsseld" 4
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "type" "mmxmov,ssemov")
				   (eq_attr "memory" "load")))
			 "athlon-direct,athlon-fany,athlon-load")
(define_insn_reservation "athlon_mmxssest" 2
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "type" "mmxmov,ssemov")
				   (eq_attr "memory" "store,both")))
			 "athlon-direct,athlon-store")
(define_insn_reservation "athlon_mmxssemov" 2
			 (and (eq_attr "cpu" "athlon")
			      (eq_attr "type" "mmxmov,ssemov"))
			 "athlon-direct,athlon-faddmul")
(define_insn_reservation "athlon_mmxmul_load" 6
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "type" "mmxmul")
				   (eq_attr "memory" "load")))
			 "athlon-direct,athlon-load,athlon-fmul")
(define_insn_reservation "athlon_mmxmul" 3
			 (and (eq_attr "cpu" "athlon")
			      (eq_attr "type" "mmxmul"))
			 "athlon-direct,athlon-fmul")
(define_insn_reservation "athlon_mmx_load" 5
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "unit" "mmx")
				   (eq_attr "memory" "load")))
			 "athlon-direct,athlon-load,athlon-faddmul")
(define_insn_reservation "athlon_mmx" 2
			 (and (eq_attr "cpu" "athlon")
			      (eq_attr "unit" "mmx"))
			 "athlon-direct,athlon-faddmul")
;; SSE operations are handled by the i387 unit as well.  The latnecy
;; is same as for i387 operations for scalar operations
(define_insn_reservation "athlon_sselog_load" 6
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "type" "sselog")
				   (eq_attr "memory" "load")))
			 "athlon-vector,athlon-load,athlon-fmul")
(define_insn_reservation "athlon_sselog" 3
			 (and (eq_attr "cpu" "athlon")
			      (eq_attr "type" "sselog"))
			 "athlon-vector,athlon-fmul")
(define_insn_reservation "athlon_ssecmp_load" 5
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "type" "ssecmp")
				   (and (eq_attr "mode" "SF,DF")
					(eq_attr "memory" "load"))))
			 "athlon-vector,athlon-load,athlon-fadd")
(define_insn_reservation "athlon_ssecmp" 2
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "type" "ssecmp")
				   (eq_attr "mode" "SF,DF")))
			 "athlon-direct,athlon-fadd")
(define_insn_reservation "athlon_ssecmpvector_load" 6
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "type" "ssecmp")
				   (eq_attr "memory" "load")))
			 "athlon-vector,athlon-fadd")
(define_insn_reservation "athlon_ssecmpvector" 3
			 (and (eq_attr "cpu" "athlon")
			      (eq_attr "type" "ssecmp"))
			 "athlon-vector,athlon-fadd")
(define_insn_reservation "athlon_sseadd_load" 7
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "type" "sseadd")
				   (and (eq_attr "mode" "SF,DF")
					(eq_attr "memory" "load"))))
			 "athlon-direct,athlon-load,athlon-fadd")
(define_insn_reservation "athlon_sseadd" 4
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "type" "sseadd")
				   (eq_attr "mode" "SF,DF")))
			 "athlon-direct,athlon-fadd")
(define_insn_reservation "athlon_sseaddvector_load" 8
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "type" "sseadd")
				   (eq_attr "memory" "load")))
			 "athlon-vector,athlon-load,athlon-fadd")
(define_insn_reservation "athlon_sseaddvector" 5
			 (and (eq_attr "cpu" "athlon")
			      (eq_attr "type" "sseadd"))
			 "athlon-vector,athlon-fadd")
(define_insn_reservation "athlon_ssemul_load" 7
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "type" "ssemul")
				   (and (eq_attr "mode" "SF,DF")
					(eq_attr "memory" "load"))))
			 "athlon-direct,athlon-load,athlon-fmul")
(define_insn_reservation "athlon_ssemul" 4
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "type" "ssemul")
				   (eq_attr "mode" "SF,DF")))
			 "athlon-direct,athlon-fmul")
(define_insn_reservation "athlon_ssemulvector_load" 8
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "type" "ssemul")
				   (eq_attr "memory" "load")))
			 "athlon-vector,athlon-load,athlon-fmul")
(define_insn_reservation "athlon_ssemulvector" 5
			 (and (eq_attr "cpu" "athlon")
			      (eq_attr "type" "ssemul"))
			 "athlon-vector,athlon-fmul")
(define_insn_reservation "athlon_ssediv_load" 19
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "type" "ssediv")
				   (and (eq_attr "mode" "SF,DF")
					(eq_attr "memory" "load"))))
			 "athlon-direct,athlon-load,athlon-fmul")
(define_insn_reservation "athlon_ssediv" 16
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "type" "ssediv")
				   (eq_attr "mode" "SF,DF")))
			 "athlon-direct,athlon-fmul")
(define_insn_reservation "athlon_ssedivvector_load" 32
			 (and (eq_attr "cpu" "athlon")
			      (and (eq_attr "type" "ssediv")
				   (eq_attr "memory" "load")))
			 "athlon-vector,athlon-load,athlon-fmul")
(define_insn_reservation "athlon_ssedivvector" 29
			 (and (eq_attr "cpu" "athlon")
			      (eq_attr "type" "ssediv"))
			 "athlon-vector,athlon-fmul")
