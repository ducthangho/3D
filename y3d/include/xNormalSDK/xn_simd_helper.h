#ifndef __XNORMAL__SIMD__HELPER__HEADER__
    #define __XNORMAL__SIMD__HELPER__HEADER__
    
    #if 1
        // turn those verbose intrinsics into something readable.
        #define loadps(mem)		_mm_load_ps((const float * const)(mem))
        #define storess(ss,mem)	_mm_store_ss((float * const)(mem),(ss))
        #define minss			_mm_min_ss
        #define maxss			_mm_max_ss
        #define minps			_mm_min_ps
        #define maxps			_mm_max_ps
        #define mulps			_mm_mul_ps
        #define subps			_mm_sub_ps
        #define mulss(ps1,ps2)		_mm_mul_ss((ps1),(ps2))
        #define subss(ps1,ps2)		_mm_sub_ss((ps1),(ps2))
        #define set1ps(f)			_mm_set1_ps((f))
        #define load1ps(mem)		_mm_load1_ps((mem))
        #define cmplt(ps1,ps2)		_mm_cmplt_ps((ps1),(ps2))
        #define cmpgt(ps1,ps2)		_mm_cmpgt_ps((ps1),(ps2))
        #define orps(ps1,ps2)		_mm_or_ps((ps1),(ps2))		// 3 cycles
        #define movemask(ps)		_mm_movemask_ps((ps))
        #define mask_any(ps)		(movemask(ps) != 0)
        #define mask_all(ps)		(movemask(ps) == 15)
        #define rotatelps(ps)		_mm_shuffle_ps((ps),(ps), 0x39)	// a,b,c,d -> b,c,d,a
        #define muxhps(low,high)	_mm_movehl_ps((low),(high))	// low{a,b,c,d}|high{e,f,g,h} = {c,d,g,h}
    #endif

    #ifdef XNORMAL_COMPILER_MINGW64
        // old stuff for old compilers :D
        #define _MM_GET_F32(a,ith) ((float*)&a)[ith]
        #define _MM_GET_F32_0(a) *((float*)&a)
        #define _MM_SET_F32(a,ith,f) ((float*)&a)[ith] = f
        #define _MM_SET_F32_0(a,f) *((float*)&a) = f

        #define _MM_GET_I32(a,ith) ((int*)&a)[ith]
        #define _MM_GET_I32_0(a) *((int*)&a)
        #define _MM_SET_I32(a,ith,i) ((int*)&a)[ith] = i
        #define _MM_SET_I32_0(a,i) *((int*)&a) = i
    
        #define _MM_GET_U32(a,ith) ((unsigned int*)&a)[ith]
        #define _MM_GET_U32_0(a) *((unsigned int*)&a)
        #define _MM_SET_U32(a,ith,u) ((unsigned int*)&a)[ith] = u
        #define _MM_SET_U32_0(a,u) *((unsigned int*)&a) = u
    #else
        #define _MM_GET_F32(a,ith) a.m128_f32[ith]
        #define _MM_GET_F32_0(a) a.m128_f32[0]
        #define _MM_SET_F32(a,ith,f) a.m128_f32[ith] = f
        #define _MM_SET_F32_0(a,f) a.m128_f32[ith] = f

        #define _MM_GET_U32(a,ith) a.m128_u32[ith]
        #define _MM_GET_U32_0(a) a.m128_u32[0]
        #define _MM_SET_U32(a,ith,u) a.m128_u32[ith] = u
        #define _MM_SET_U32_0(a,u) a.m128_u32[0] = u

        #define _MMI_GET_I32(a,ith) a.m128i_i32[ith]
        #define _MMI_GET_I32_0(a) a.m128i_i32[0]
        #define _MMI_SET_I32(a,ith,i) a.m128i_i32[ith] = i
        #define _MMI_SET_I32_0(a,i) a.m128i_i32[ith] = i
    #endif // XNORMAL_COMPILER_MINGW64

#endif // __XNORMAL__SIMD__HELPER__HEADER__


