
// BEGIN PREAMBLE
	.version	3.1
	.target	sm_30
	.address_size 64
	.extern .shared .u8 sdata[];
// END PREAMBLE

// BEGIN VAR DEF: libgomp_ptx_lock
.global .align 4 .u32 libgomp_ptx_lock;

// BEGIN GLOBAL FUNCTION DECL: GOMP_atomic_start
.visible .func GOMP_atomic_start;
// BEGIN GLOBAL FUNCTION DEF: GOMP_atomic_start
.visible .func GOMP_atomic_start
{
	.reg .pred 	%p<2>;
	.reg .s32 	%r<2>;
	.reg .s64 	%rd<2>;
BB5_1:
	mov.u64 	%rd1, libgomp_ptx_lock;
	atom.global.cas.b32 	%r1, [%rd1], 0, 1;
	setp.ne.s32	%p1, %r1, 0;
	@%p1 bra 	BB5_1;
	ret;
	}
// BEGIN GLOBAL FUNCTION DECL: GOMP_atomic_end
.visible .func GOMP_atomic_end;
// BEGIN GLOBAL FUNCTION DEF: GOMP_atomic_end
.visible .func GOMP_atomic_end
{
	.reg .s32 	%r<2>;
	.reg .s64 	%rd<2>;
	mov.u64 	%rd1, libgomp_ptx_lock;
	atom.global.exch.b32 	%r1, [%rd1], 0;
	ret;
	}
