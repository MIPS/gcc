// Copyright 2015 The Go Authors.  All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

// MakeFunc ppc implementation.

package reflect

import (
	"runtime"
	"unsafe"
)

const ppc_arch_stack_slot_align uintptr = 8
const ppc_num_gr = 8
const ppc_num_fr = 13
type ppc_arch_gr_t uint64
type ppc_arch_fr_t uint64
type ppc_arg_t int

// The assembler stub will pass a pointer to this structure.
// This will come in holding the register values and the
// address of the parameters that might be on the stack.  
type ppc_regs struct {
	r3  ppc_arch_gr_t
	r4  ppc_arch_gr_t
	r5  ppc_arch_gr_t
	r6  ppc_arch_gr_t
	r7  ppc_arch_gr_t
	r8  ppc_arch_gr_t
	r9  ppc_arch_gr_t
	r10 ppc_arch_gr_t
	// stack_args is the address of the first parameter that
	// was stored on the stack. 
	stack_args ppc_arch_gr_t
	f1  ppc_arch_fr_t
	f2  ppc_arch_fr_t
	f3  ppc_arch_fr_t
	f4  ppc_arch_fr_t
	f5  ppc_arch_fr_t
	f6  ppc_arch_fr_t
	f7  ppc_arch_fr_t
	f8  ppc_arch_fr_t
	f9  ppc_arch_fr_t
	f10 ppc_arch_fr_t
	f11 ppc_arch_fr_t
	f12 ppc_arch_fr_t
	f13 ppc_arch_fr_t
}

const (
	ppc_general_reg ppc_arg_t = iota
	ppc_general_reg_pair
	ppc_float_reg
	// Argument passed as a pointer to an in-memory value.
	ppc_mem_ptr
	ppc_slice
	ppc_empty
)

// ppcClassifyParameter returns the register class needed to
// pass the value of type TYP.  ppc_empty means the register is
// not used.  The second and third return values are the offset of
// an rtype parameter passed in a register (second) or stack slot
// (third).
func ppcClassifyParameter(typ *rtype) (ppc_arg_t, uintptr, uintptr) {
	offset := uintptr(0)

	// For big endian, the pointer points to the start of the 
	// slot so if the size of the parameter does not fill the slot,
	// the offset must be adjusted to access the correct byte for a
	// memcpy.  For little endian this is not necessary.
	if runtime.GOARCH != "ppc64le" && typ.Size() < ppc_arch_stack_slot_align {
        	offset = ppc_arch_stack_slot_align - typ.Size()
	}
		
	switch typ.Kind() {
	default:
		panic("internal error--unknown kind in ppcClassifyParameter")
	case Bool, Int, Int8, Int16, Int32, Uint, Uint8, Uint16, Uint32:
		return ppc_general_reg, offset, offset
	case Int64, Uint64, Uintptr, Chan, Func, Map, Ptr, UnsafePointer:
		return ppc_general_reg, 0, 0
	case Float32, Float64:
		return ppc_float_reg, 0, offset
	case Complex64, Complex128:
		// Complex numbers are passed by reference.
		return ppc_mem_ptr, 0, 0
	case Array, Struct:
		var ityp *rtype
		var length int

		if typ.Size() == 0 {
			return ppc_empty, 0, 0
		}
		switch typ.Size() {
		default:
			// Pointer to memory.
			return ppc_mem_ptr, 0, 0
		case 1, 2:
			// Pass in an integer register.
			return ppc_general_reg, offset, offset

		case 4, 8, 16:
			// An array of up to 16 bytes could be passed in 
			// registers.  (Not sure about structures?)
		}
		if (typ.Kind() == Array) {
			atyp := (*arrayType)(unsafe.Pointer(typ))
			length = atyp.Len()
			ityp = atyp.elem
		} else {
			styp := (*structType)(unsafe.Pointer(typ))
			length = len(styp.fields)
			ityp = styp.fields[0].typ
		}
		if length == 1 {
			class, off_reg, off_slot := ppcClassifyParameter(ityp)
			if class == ppc_float_reg {
				return ppc_float_reg, off_reg, off_slot
			}
		}
		// Otherwise pass in an integer register or register pair
		switch typ.Size() {
		case 4, 8:
			return ppc_general_reg, 0, 0
		case 16:
			return ppc_general_reg_pair, 0, 0
		default:
			return ppc_general_reg, 0, 0
		}
	case Slice:
		return ppc_slice, 0, 0
	case Interface, String:
		return ppc_mem_ptr, 0, 0
	}
}

func ppcClassify(typ *rtype) (ppc_arg_t, ppc_arg_t) {
	switch typ.Kind() {
	default:
		panic("internal error--unknown kind in ppcClassify")

	case Bool, Int, Int8, Int16, Int32, Int64,
		Uint, Uint8, Uint16, Uint32, Uint64,
		Uintptr, Chan, Func, Map, Ptr, UnsafePointer:

		return ppc_general_reg, ppc_empty

	case Float32, Float64, Complex64:
		return ppc_float_reg, ppc_empty

	case Complex128:
		return ppc_float_reg, ppc_float_reg

	case Array:
		if typ.size == 0 {
			return ppc_empty, ppc_empty
		} else if typ.size > 16 {
			return ppc_mem_ptr, ppc_empty
		}
		atyp := (*arrayType)(unsafe.Pointer(typ))
		eclass1, eclass2 := ppcClassify(atyp.elem)
		if eclass1 == ppc_mem_ptr {
			return ppc_mem_ptr, ppc_empty
		}
		if eclass2 == ppc_empty && typ.size > 8 {
			eclass2 = eclass1
		}
		return eclass1, eclass2

	case Interface:
		return ppc_general_reg, ppc_general_reg

	case Slice:
		return ppc_mem_ptr, ppc_empty

	case String:
		return ppc_general_reg, ppc_general_reg

	case Struct:
		if typ.size == 0 {
			return ppc_empty, ppc_empty
		} else if typ.size > 16 {
			return ppc_mem_ptr, ppc_empty
		}
		var first, second ppc_arg_t
		f := ppc_empty
		onFirst := true
		styp := (*structType)(unsafe.Pointer(typ))
		for _, field := range styp.fields {
			if onFirst && field.offset >= 8 {
				first = f
				f = ppc_empty
				onFirst = false
			}
			fclass1, fclass2 := ppcClassify(field.typ)
			f = ppcMergeClasses(f, fclass1)
			if fclass2 != ppc_empty {
				if !onFirst {
					panic("ppcClassify inconsistent")
				}
				first = f
				f = fclass2
				onFirst = false
			}
		}
		if onFirst {
			first = f
			second = ppc_empty
		} else {
			second = f
		}
		if first == ppc_mem_ptr || second == ppc_mem_ptr {
			return ppc_mem_ptr, ppc_empty
		}
		return first, second
	}
}


// ppcMergeClasses merges two register classes as described in the
// ppc64 ELF ABI.
func ppcMergeClasses(c1, c2 ppc_arg_t) ppc_arg_t {
	switch {
	case c1 == c2:
		return c1
	case c1 == ppc_empty:
		return c2
	case c2 == ppc_empty:
		return c1
	case c1 == ppc_mem_ptr || c2 == ppc_mem_ptr:
		return ppc_mem_ptr
	case c1 == ppc_general_reg || c2 == ppc_general_reg:
		return ppc_general_reg
	default:
		return ppc_general_reg
	}
}



// Given a value of type *rtype left aligned in an iword, reload
// the value so that it can be stored in a general or floating
// point register.  For general registers the value is sign extend
// and right aligned.
func ppcReloadForRegister(typ *rtype, w iword, offset uintptr) (iword) {
	var do_sign_extend bool = false
	var gr ppc_arch_gr_t

	switch typ.Kind() {
		case Int, Int8, Int16, Int32, Int64:
			do_sign_extend = true
		default:
			// Handle all other cases in the next switch.
	}
	switch (typ.size) {
		case 1:
			if (do_sign_extend == true) {
				se := int64(*(*int8)(unsafe.Pointer(&w)))
				gr = *(*ppc_arch_gr_t)(unsafe.Pointer(&se))
			} else {
				e := int64(*(*uint8)(unsafe.Pointer(&w)))
				gr = *(*ppc_arch_gr_t)(unsafe.Pointer(&e))
			}
		case 2:
			if (do_sign_extend == true) {
				se := int64(*(*int16)(unsafe.Pointer(&w)))
				gr = *(*ppc_arch_gr_t)(unsafe.Pointer(&se))
			} else {
				e := int64(*(*uint16)(unsafe.Pointer(&w)))
				gr = *(*ppc_arch_gr_t)(unsafe.Pointer(&e))
			}
		case 4:
			if (do_sign_extend == true) {
				se := int64(*(*int32)(unsafe.Pointer(&w)))
				gr = *(*ppc_arch_gr_t)(unsafe.Pointer(&se))
			} else {
				e := int64(*(*uint32)(unsafe.Pointer(&w)))
				gr = *(*ppc_arch_gr_t)(unsafe.Pointer(&e))
			}
		default:
			panic("reflect: bad size in ppcReloadForRegister")
	}

	return *(*iword)(unsafe.Pointer(&gr))
}

// MakeFuncStubGo implements the ppc calling convention for
// MakeFunc.  This should not be called.  It is exported so that
// assembly code can call it.
func MakeFuncStubGo(regs *ppc_regs, c *makeFuncImpl) {
	ftyp := c.typ
	gr := 0
	fr := 0
	ap := uintptr(regs.stack_args)

	// See if the result requires a struct.  If it does, the first
	// parameter is a pointer to the struct.
	var ret_class, ret_class_2, ret_class_dummy ppc_arg_t
	var ret_off_reg uintptr
	var ret_type *rtype

	switch len(ftyp.out) {
	case 0:
		ret_type = nil
		ret_class, ret_class_2, ret_off_reg = ppc_empty, ppc_empty, 0
	case 1:
		ret_type = ftyp.out[0]
		ret_class, ret_class_2 = ppcClassify(ret_type)
	case 2:
		if runtime.GOARCH == "ppc64le" {
			ret_class, ret_class_dummy = ppcClassify(ftyp.out[0])
			ret_class_2, ret_class_dummy = ppcClassify(ftyp.out[1])
		} else {
			ret_class, ret_class_2, ret_off_reg = ppc_mem_ptr, ppc_empty, 0
		}
	default:
		ret_type = nil
		ret_class, ret_class_2, ret_off_reg = ppc_mem_ptr, ppc_empty, 0
	}
	in := make([]Value, 0, len(ftyp.in))
	if ret_class == ppc_mem_ptr {
		// We are returning a value in memory, which means
		// that the first argument is a hidden parameter
		// pointing to that return area.
		gr++
	}

argloop:
	for _, rt := range ftyp.in {
		class, off_reg, off_slot := ppcClassifyParameter(rt)
		fl := flag(rt.Kind()) << flagKindShift
		switch class {
		case ppc_empty:
			v := Value{rt, nil, fl | flagIndir}
			in = append(in, v)
			continue argloop
		case ppc_general_reg_pair:
			if gr < ppc_num_gr {
				var vals[2] uintptr
				vals[0] = ppc_general_reg_val(regs, gr)
				vals[1] = ppc_general_reg_val(regs, gr+1)
				ix := uintptr(unsafe.Pointer(&vals[0]))
				iw := unsafe.Pointer(ix)
				fl |= flagIndir
				v := Value{rt, iw, fl}
				in = append(in, v)
				gr += 2
			} else {
				in, ap = ppc_add_stackreg(
                                        in, ap, rt, off_slot)
			}
			continue argloop
		case ppc_general_reg:
			// Values stored in a general register are right
			// aligned.
			if gr < ppc_num_gr {
				val := ppc_general_reg_val(regs, gr)
				iw := unsafe.Pointer(val)
				k := rt.Kind()
				if k != Ptr && k != UnsafePointer {
					ix := uintptr(unsafe.Pointer(&val))
					ix += off_reg
					iw = unsafe.Pointer(ix)
					fl |= flagIndir
				}
				v := Value{rt, iw, fl}
				in = append(in, v)
				gr++
			} else {
				in, ap = ppc_add_stackreg(
					in, ap, rt, off_slot)
			}
			continue argloop
		case ppc_float_reg:
			// In a register, floats are left aligned, but in a
			// stack slot they are right aligned.
			if fr < ppc_num_fr {
				val := ppc_float_reg_val(regs, fr)
				ix := uintptr(unsafe.Pointer(&val))
				if rt.Size() == 4 {
					// For single precision float, the value was saved as a 
					// double precision bit pattern so must be first
					// converted to double then cast as a single float.
					var f32 float32 = float32(*(*float64)(unsafe.Pointer(&val)))
					ix = uintptr(unsafe.Pointer(&f32))
				}
				v := Value {
					rt, unsafe.Pointer(unsafe.Pointer(ix)),
					fl | flagIndir,
				}
				in = append(in, v)
				fr++
				gr++
			} else {
				in, ap = ppc_add_stackreg(
					in, ap, rt, off_slot)
			}
			continue argloop
		case ppc_mem_ptr:
			if gr < ppc_num_gr && unsafe.Pointer(ppc_general_reg_val(regs, gr)) != nil {
				// Register holding a pointer to memory.
				val := ppc_general_reg_val(regs, gr)
				v := Value{
					rt, unsafe.Pointer(val), fl | flagIndir}
				in = append(in, v)
				gr++
			} else {
				// Stack slot holding a pointer to memory.
				in, ap = ppc_add_memarg(in, ap, rt)
			}
			continue argloop
		case ppc_slice:
			if gr < ppc_num_gr {
				// Register holding a pointer to memory.
				val := ppc_general_reg_val(regs, gr)
				cnt := ppc_general_reg_val(regs, gr+1)
				cap := ppc_general_reg_val(regs, gr+2)

				s := SliceHeader{val, int(cnt), int(cap)}
				v := Value{
					rt, unsafe.Pointer(&s), fl | flagIndir}
				in = append(in, v)
				gr+=3
			} else {
				// Stack slot holding a pointer to memory.
				in, ap = ppc_add_memarg(in, ap, rt)
			}
			continue argloop
		}
		panic("reflect: argtype not handled in MakeFunc:argloop")
	}

	// All the real arguments have been found and turned into
	// Values.  Call the real function.

	out := c.call(in)

	if len(out) != len(ftyp.out) {
		panic("reflect: wrong return count from function created by MakeFunc")
	}

	for i, typ := range ftyp.out {
		v := out[i]
		if v.typ != typ {
			panic(
				"reflect: function created by MakeFunc using " +
				funcName(c.fn) + " returned wrong type: have " +
				out[i].typ.String() + " for " + typ.String())
		}
		if v.flag&flagRO != 0 {
			panic(
				"reflect: function created by MakeFunc using " +
				 funcName(c.fn) + " returned value obtained " +
				"from unexported field")
		}
	}

	switch (ret_class) {
	case ppc_general_reg, ppc_float_reg:
		// Single return value in a general or floating point register.
		v := out[0]
		if (ret_class_2 == ppc_general_reg) {
		   var buf [2]unsafe.Pointer
		   ptr := unsafe.Pointer(&buf[0])
		   off := uintptr(0)
		   for i, typ := range ftyp.out {
		       v := out[i]
		       off = align(off, uintptr(typ.fieldAlign))
		       addr := unsafe.Pointer(uintptr(ptr) + off)
		       if v.flag&flagIndir == 0 && (v.kind() == Ptr || v.kind() == UnsafePointer) {
			  storeIword(addr, iword(v.val), typ.size)
		       } else {
		       	  memmove(addr, v.val, typ.size)
		       }
		       off += uintptr(typ.size)
		   }
		   regs.r3 = *(*ppc_arch_gr_t)(unsafe.Pointer(&buf[0]))
		   regs.r4 = *(*ppc_arch_gr_t)(unsafe.Pointer(&buf[1]))
		} else {
		    w := v.iword()
		    if v.Kind() != Ptr && v.Kind() != UnsafePointer {
		       w = loadIword(unsafe.Pointer(w), v.typ.size)
		       if (ret_off_reg != 0) {
		       	  w = ppcReloadForRegister(ret_type, w, ret_off_reg)
		    	  }
		    }
		    if (ret_class == ppc_float_reg) {
		        regs.f1 = ppc_arch_fr_t(uintptr(w))
		    } else {
		        regs.r3 = ppc_arch_gr_t(uintptr(w))
		    }
		}

	case ppc_mem_ptr:
		// The address of the memory area was passed as a hidden
		// parameter in %r2.  Multiple return values are always returned
		// in an in-memory structure.
		ptr := unsafe.Pointer(uintptr(regs.r3))
		off := uintptr(0)
		for i, typ := range ftyp.out {
			v := out[i]
			off = align(off, uintptr(typ.fieldAlign))
			addr := unsafe.Pointer(uintptr(ptr) + off)
			if v.flag&flagIndir == 0 && (v.kind() == Ptr || v.kind() == UnsafePointer) {
				storeIword(addr, iword(v.val), typ.size)
			} else {
				memmove(addr, v.val, typ.size)
			}
			off += typ.size
		}

	case ppc_empty:
	}

	return
}

// The ppc_add_stackreg function adds an argument passed on the
// stack that could be passed in a register.
func ppc_add_stackreg(
	in []Value, ap uintptr, rt *rtype, offset uintptr) ([]Value, uintptr) {
	// If we're not already at the beginning of a stack slot, round up to
	// the beginning of the next one.
	ap = align(ap, ppc_arch_stack_slot_align)
	// If offset is > 0, the data is right aligned on the stack slot.
	ap += offset

	// We have to copy the argument onto the heap in case the
	// function hangs onto the reflect.Value we pass it.
	p := unsafe_New(rt)
	memmove(p, unsafe.Pointer(ap), rt.size)

	v := Value{rt, p, flag(rt.Kind()<<flagKindShift) | flagIndir}
	in = append(in, v)
	ap += rt.size
	ap = align(ap, ppc_arch_stack_slot_align)

	return in, ap
}

// The ppc_add_memarg function adds an argument passed in memory.
func ppc_add_memarg(in []Value, ap uintptr, rt *rtype) ([]Value, uintptr) {
	// If we're not already at the beginning of a stack slot,
	// round up to the beginning of the next one.
	ap = align(ap, ppc_arch_stack_slot_align)

	// We have to copy the argument onto the heap in case the
	// function hangs onto the reflect.Value we pass it.
	p := unsafe_New(rt)
	memmove(p, unsafe.Pointer(ap), rt.size)

	v := Value{rt, p, flag(rt.Kind()<<flagKindShift) | flagIndir}
	in = append(in, v)
	ap += ppc_arch_stack_slot_align

	return in, ap
}

// The ppc_general_reg_val function returns the value of integer register GR.
func ppc_general_reg_val(regs *ppc_regs, gr int) uintptr {
	var r ppc_arch_gr_t
	switch gr {
	case 0:
		r = regs.r3
	case 1:
		r = regs.r4
	case 2:
		r = regs.r5
	case 3:
		r = regs.r6
	case 4:
		r = regs.r7
	case 5:
		r = regs.r8
	case 6:
		r = regs.r9
	case 7:
		r = regs.r10
	default:
		panic("ppc_general_reg_val: bad integer register")
	}
	return uintptr(r)
}

// The ppc_float_reg_val function returns the value of float register FR.
func ppc_float_reg_val(regs *ppc_regs, fr int) uintptr {
	var r ppc_arch_fr_t
	switch fr {
	case 0:
		r = regs.f1
	case 1:
		r = regs.f2
	case 2:
		r = regs.f3
	case 3:
		r = regs.f4
	case 4:
		r = regs.f5
	case 5:
		r = regs.f6
	case 6:
		r = regs.f7
	case 7:
		r = regs.f8
	case 8:
		r = regs.f9
	case 9:
		r = regs.f10
	case 10:
		r = regs.f11
	case 11:
		r = regs.f12
	case 12:
		r = regs.f13
	default:
		panic("ppc_float_reg_val: bad floating point register")
	}
	return uintptr(r)
}
