#if 0
    This file is part of the xNormal 3 SDK, formatted for Doxygen/JavaDoc 
    documentation system. 
    (c) 2005-2015, S.Orgaz.
    Contact: granthill76 'at' yahoo.com
    Visit http://www.xnormal.net for more info
 
    FOR MORE INFO ABOUT THE LICENSE, CONSULT THE XNORMAL PDF DOCUMENTATION
#endif //0 comment


#ifndef __XNORMAL__HEADER__68E4C94A__923A__43ee__9042__9DE13C20404B__
    #define __XNORMAL__HEADER__68E4C94A__923A__43ee__9042__9DE13C20404B__

    //DOxygen main page info
    /**
     * \mainpage
     * \image html xNormalLogoForDoc_BIG_SDK.png
     * \section intro_sec Introduction
     * The xNormal C++ SDK allows you to write plugins and can be used also
     * as an useful library to compute vertex normals, tangent basis, etc... 
     * or to re-use the existing xNormal plugins!
    */

    #ifndef __cplusplus
        #error xNormal SDK : A C++ compiler is required
    #endif

    //define the platform OS macros.
    #define XNORMAL_WINDOWS
    #if defined(WIN64) || defined(_WIN64)
        #define XNORMAL_WINDOWS64
    #endif
    #include <windows.h>
    #include <shlobj.h>
    #include <shellapi.h>

    //define the compiler detection macros
    #ifdef _MSC_VER
        #if (_MSC_VER<=1800)
            #error xNormal requires Visual Studio 2013
        #else
            #define XNORMAL_COMPILER_VSTUDIO
        #endif
    #else
        #error xNormal requires Visual Studio 2013
    #endif

    //define the XNORMAL_ASSERT macro to easily enable/disable/redirect asserts to our own function
    #if defined(DEBUG) || defined(_DEBUG)
        #include <assert.h>
        #define XNORMAL_DEBUG
        #define XNORMAL_ASSERT(x) assert(x);
    #else
        #define XNORMAL_ASSERT(x)
    #endif

    //define inline and calling convention
    #define XNORMAL_INLINE inline
    #define XNORMAL_DLLEXPORT extern "C" __declspec(dllexport)
    
    //C++11 support
    #define XNORMAL_CPP11_SUPPORT
    #define XNORMAL_FINALCLASS final
    #define XNORMAL_OVERRIDE override
    #define XNORMAL_STATIC_ASSERT(x) static_assert(x);

    //OpenMP support
    #include <time.h>

    #ifdef _OPENMP
        #include <omp.h>

        #define XNORMAL_OPENMP _OPENMP

        //redirect omp_XXXXXX functions to a macro 
        //(like we did with the assert) so we can easily 
        //enable/disable/redirect the calls for the compilers without omp.h 
        //OMP pragmas are supposed to be automatically ignored if the 
        //compiler lacks support. 
        #define XNORMAL_OPENMP_DESTROY_LOCK ::omp_destroy_lock()
        #define XNORMAL_OPENMP_DESTROY_NEST_LOCK ::omp_destroy_nest_lock()
        #define XNORMAL_OPENMP_GET_DYNAMIC ::omp_get_dynamic()
        #define XNORMAL_OPENMP_GET_MAX_THREADS ::omp_get_max_threads()
        #define XNORMAL_OPENMP_GET_NESTED ::omp_get_nested()
        #define XNORMAL_OPENMP_GET_NUM_PROCS ::omp_get_num_procs()
        #define XNORMAL_OPENMP_GET_NUM_THREADS ::omp_get_num_threads()
        #define XNORMAL_OPENMP_THREADID ::omp_get_thread_num()
        #define XNORMAL_OPENMP_GET_WTICK ::omp_get_wtick()
        #define XNORMAL_OPENMP_GET_WTIME ::omp_get_wtime()
        #define XNORMAL_OPENMP_IN_PARALLEL ::omp_in_parallel()
        #define XNORMAL_OPENMP_INIT_LOCK ::omp_init_lock()
        #define XNORMAL_OPENMP_INIT_NEST_LOCK ::omp_init_nest_lock()
        #define XNORMAL_OPENMP_SET_DYNAMIC ::omp_set_dynamic()
        #define XNORMAL_OPENMP_SET_LOCK ::omp_set_lock()
        #define XNORMAL_OPENMP_SET_NEST_LOCK ::omp_set_nest_lock()
        #define XNORMAL_OPENMP_SET_NESTED ::omp_set_nested()
        #define XNORMAL_OPENMP_SET_NUM_THREADS ::omp_set_num_threads()
        #define XNORMAL_OPENMP_TEST_LOCK ::omp_test_lock()
        #define XNORMAL_OPENMP_TEST_NEST_LOCK ::omp_test_nest_lock()
        #define XNORMAL_OPENMP_UNSET_LOCK ::omp_unset_lock()
        #define XNORMAL_OPENMP_UNSET_NEST_LOCK ::omp_unset_nest_lock()
    #else
        #define XNORMAL_OPENMP_DESTROY_LOCK
        #define XNORMAL_OPENMP_DESTROY_NEST_LOCK
        #define XNORMAL_OPENMP_GET_DYNAMIC 0
        #define XNORMAL_OPENMP_GET_MAX_THREADS 1
        #define XNORMAL_OPENMP_GET_NESTED 0
        #define XNORMAL_OPENMP_GET_NUM_PROCS 1
        #define XNORMAL_OPENMP_GET_NUM_THREADS 1
        #define XNORMAL_OPENMP_THREADID 0
        #define XNORMAL_OPENMP_GET_WTICK 1.0 / static_cast<double>(CLOCKS_PER_SEC)

        #define XNORMAL_OPENMP_GET_WTIME static_cast<double>(clock()) / static_cast<double>(CLOCKS_PER_SEC)

        #define XNORMAL_OPENMP_IN_PARALLEL 0
        #define XNORMAL_OPENMP_INIT_LOCK
        #define XNORMAL_OPENMP_INIT_NEST_LOCK
        #define XNORMAL_OPENMP_SET_DYNAMIC
        #define XNORMAL_OPENMP_SET_LOCK
        #define XNORMAL_OPENMP_SET_NEST_LOCK
        #define XNORMAL_OPENMP_SET_NESTED
        #define XNORMAL_OPENMP_SET_NUM_THREADS
        #define XNORMAL_OPENMP_TEST_LOCK 0
        #define XNORMAL_OPENMP_TEST_NEST_LOCK 0
        #define XNORMAL_OPENMP_UNSET_LOCK
        #define XNORMAL_OPENMP_UNSET_NEST_LOCK
    #endif

    //define the xNormal SDK version and namespace macros(to change 
    //namespace name very easy on conflicts, to disable namespaces if the
    //compiler doesn't support them or just to avoid different
    //xNormal SDKs mixing!)
    //#define XNORMAL_SDK_VERSION 60 //v3.10.3
    //#define XNORMAL_SDK_VERSION 61 //v3.10.4 Beta 1
    //#define XNORMAL_SDK_VERSION 62 //v3.10.4
    //#define XNORMAL_SDK_VERSION 63 //v3.10.5
    //#define XNORMAL_SDK_VERSION 64 //v3.10.6
    //#define XNORMAL_SDK_VERSION 65 //v3.11.0 Beta 1
    //#define XNORMAL_SDK_VERSION 66 //v3.11.0 
    //#define XNORMAL_SDK_VERSION 67 //v3.11.1 Beta 1, 3.11.1 Final
    //#define XNORMAL_SDK_VERSION 68 //v3.12.0 Beta 1
    //#define XNORMAL_SDK_VERSION 69 //v3.12.0 Final
    //#define XNORMAL_SDK_VERSION 70 //v3.14.0 Beta 1
    //#define XNORMAL_SDK_VERSION 71 //v3.14.0 Final
    //#define XNORMAL_SDK_VERSION 72 //v3.14.1 Final, v3.14.2 Final
    //#define XNORMAL_SDK_VERSION 73 //v3.14.3 Final
    //#define XNORMAL_SDK_VERSION 74 //v3.14.4 Final
    //#define XNORMAL_SDK_VERSION 75 //v3.14.5 Final
    //#define XNORMAL_SDK_VERSION 76 //v3.14.6 Final
    //#define XNORMAL_SDK_VERSION 80 //v3.15.0 Final
    //#define XNORMAL_SDK_VERSION 81 //v3.15.1 Beta 1, 3.15.1 final
    //#define XNORMAL_SDK_VERSION 82 //v3.15.2 Final
    //#define XNORMAL_SDK_VERSION 83 //v3.15.3 Beta 1, xNormal 3.15.3 Beta 2
    //#define XNORMAL_SDK_VERSION 84 //v3.15.3 Beta 3, xNormal 3.15.3 Beta 4
    //#define XNORMAL_SDK_VERSION 85 //v3.15.3 Beta 5
    //#define XNORMAL_SDK_VERSION 86 //v3.15.3 Beta 6, v3.15.3 Beta 7
    //#define XNORMAL_SDK_VERSION 87 //v3.15.3 RC1, v3.16.0 Beta 1
    //#define XNORMAL_SDK_VERSION 88 //v3.16.0 Beta 2, v3.16.0, v3.16.1
    //#define XNORMAL_SDK_VERSION 89 //v3.16.2 Beta 1, v3.16.2 Beta 2
    //#define XNORMAL_SDK_VERSION 90 //v3.16.2 Beta 3
    //#define XNORMAL_SDK_VERSION 91 //v3.16.2 RC1, v3.16.2 Final
    //#define XNORMAL_SDK_VERSION 92 //v3.16.3/4/5/6/7/8/9/10/11/12
    //#define XNORMAL_SDK_VERSION 93 //v3.17.0 Beta 1
    //#define XNORMAL_SDK_VERSION 94 //v3.17.0 Beta 2/3/3b
    //#define XNORMAL_SDK_VERSION 95 //v3.17.0 Beta 4
    //#define XNORMAL_SDK_VERSION 96 //v3.17.0 Beta 5/Final, 3.17.1, 3.17.2 Beta 3/4, 3.17.2 Final, 3.17.3 Beta 1/2, 3.17.3 Final, v3.17.4 Beta 1
    //#define XNORMAL_SDK_VERSION 97 //v3.17.4 Beta 2, 3.17.4 Final, 3.17.5 Beta 1/2, 3.17.5 Final, 3.17.6 Beta 1
    //#define XNORMAL_SDK_VERSION 98 //v3.17.6 Beta 2
    //#define XNORMAL_SDK_VERSION 99 //v3.17.6 Final, v3.17.7 Final, v3.17.8 Final, v3.17.9 Final
    //#define XNORMAL_SDK_VERSION 100  //v3.17.10 Final, v3.17.11 Final, v3.17.12 Final, v3.17.13 Final
    //#define XNORMAL_SDK_VERSION 101  //v3.17.14 Beta 1, v3.17.14 Final, v3.17.15 Final, v3.17.16 Final
    //#define XNORMAL_SDK_VERSION 102  //v3.18.0 Beta 1, v3.18.0 Beta 2
    //#define XNORMAL_SDK_VERSION 103  //v3.18.0 Final
    //#define XNORMAL_SDK_VERSION 104  //v3.18.1 Final
    //#define XNORMAL_SDK_VERSION 105  //v3.18.2 Final, v3.18.3 Final, v.3.18.4 Final, v.3.18.5 Final, v.3.18.6 Final, v.3.18.7 Final, v3.18.8 Final, v3.18.9 Final, v3.18.10 Final
    #define XNORMAL_SDK_VERSION 106  //v3.19.0 Final, v3.19.1 Final, v3.19.2 Final

    //define the XNORMAL SDK namespace "name"
    #define XNORMAL_SDK_NAMESPACE xNormalC_SDK_106

    #define xnNULL nullptr
	
    //define the safe delete/release
    #define XNORMAL_SAFE_DELETE(x)\
        if(xnNULL!=x)\
        {\
            delete x;\
            x = xnNULL;\
        }

    #define XNORMAL_SAFE_RELEASE(x)\
        if(xnNULL!=x)\
        {\
            x->Release();\
            x = xnNULL;\
        }

    #define XNORMAL_SAFE_DELETE_ARRAY(x)\
        if(xnNULL!=x)\
        {\
            delete [] x;\
            x = xnNULL;\
        }

    #define XNORMAL_SAFE_FREE(x)\
        if(xnNULL!=x)\
        {\
            ::free(x);\
            x = xnNULL;\
        }

    //Set basic types because C doesn't determine fixed sizes for 
    //unsigned long, unsigned int, bool, etc... to avoid type problems.
    namespace XNORMAL_SDK_NAMESPACE
    {
        typedef unsigned char xnByte; //1byte
        typedef char xnInt8; //1byte
        typedef unsigned char xnUInt8; //1byte
        typedef short xnInt16; //2bytes
        typedef unsigned short xnUInt16; //2bytes
        typedef int xnInt32; //4bytes
        typedef unsigned int xnUInt32; //4bytes
        typedef signed long long xnInt64; //8 bytes
        typedef long unsigned long xnUInt64; //8 bytes
        typedef float xnSingle;//4 bytes, single precission
        typedef double xnDouble;//8 bytes, double precission
        typedef void* xnHandle;//null == invalid handle
    }

    //Now include all the headers like a "precompiled header"
    #include <stdio.h>
    #include <stdlib.h>
    #include <stddef.h>//for size_t
    #include <float.h>
    #include <math.h>
    #include <string.h>
    #include <memory.h>
    #include <malloc.h>
    #include <stdarg.h>
    #include <xmmintrin.h>
    #include <emmintrin.h>

    #include <string>
    #include <vector>
    #include <list>
    #include <map>
    #include <memory>
    #include <functional>
    #include <algorithm>
    #include <iostream>
    #include <stack>
    #include <sstream>
    #include <fstream>
    #include <locale>
    #ifdef XNORMAL_COMPILER_VSTUDIO
        #include <codecvt>
    #endif // XNORMAL_COMPILER_VSTUDIO

    //Define exception things
    #if (defined(XNORMAL_COMPILER_VSTUDIO) && defined(_CPPUNWIND)) ||\
        (defined(XNORMAL_COMPILER_MINGW64) && defined(__EXCEPTIONS))

        #include <exception>        

        #define XNORMAL_HAS_EXCEPTIONS

        #define XNORMAL_THROW(x) throw(x)
        #define XNORMAL_ANYTHROW throw(...)
        #define XNORMAL_NOTHROW /*noexcept*/throw()
        #define XNORMAL_NOTHROWOVR /*noexcept*/throw() override
        #define XNORMAL_RETHROW throw;

        #define XNORMAL_TRY try
        #define XNORMAL_CATCH(x) catch (x)
        #define XNORMAL_CATCH_ALL catch (...)
        #define XNORMAL_CATCH_ALL_EMPTY catch (...) {}

        #define XNORMAL_SUPERCATCH_ERRSTR(errStr)\
            catch(std::exception &e){\
                errStr.Set ( e.what() );\
            }\
            catch(char *c){\
                errStr.Set ( const_cast<const char*>(c) );\
            }\
            catch(const char *c){\
                errStr.Set ( c );\
            }\
            catch(...){\
                errStr.Set ( "Unhandled exception" );\
            }\

    #else
        #define XNORMAL_THROW(x)
        #define XNORMAL_ANYTHROW
        #define XNORMAL_NOTHROW
        #define XNORMAL_NOTHROWOVR override
        #define XNORMAL_RETHROW

        #define XNORMAL_TRY
        #define XNORMAL_CATCH(x)
        #define XNORMAL_CATCH_ALL
        #define XNORMAL_CATCH_ALL_EMPTY
    #endif

    namespace XNORMAL_SDK_NAMESPACE
    {
        #include "xn_simd_helper.h"

        #include "basic_XN_trait.hxx"
        #include "pair.hxx"

        #include "IArray.hxx"
        #include "IString.hxx"

        #include "auto_ptr_release.hxx"

        #include "Version.hxx"
        #include "Guid.hxx"
        #include "Vector.hxx"
        #include "Color.hxx"
        #include "Matrix.hxx"
        #include "Plane.hxx"

        #include "IProgressCallback.hxx"

        #include "Utils.hxx"
        
        #include "IImage.hxx"

        class IPlugin;
        class IImageImporter;
        class IImageExporter;
        class IMeshImporter;
        class IMeshExporter;
        class ITangentBasisCalculator;
        class IGraphicsDrv;
        class IVideoMaker;
        class ITriangulator;
        #include "IHost.hxx"

        #include "IPlugin.hxx"

        #include "AODistribution.hxx"

        #include "AABB.hxx"

        #include "IImageImporter.hxx"
        #include "IImageExporter.hxx"

        class IGeometry;
        class IGraphicsDrvMesh;
        #include "IPolyMesh.hxx"
        #include "IGeometry.hxx"
        #include "ITriangulator.hxx"
        //#include "Triangulator_Default.hxx"
        #include "IMeshImporter.hxx"
        #include "IMeshExporter.hxx"
        
        #include "ITangentBasisCalculator.hxx"
        
        #include "IVideoMaker.hxx"
        #include "IGraphicsDrv.hxx"

        #ifdef XNORMAL_HAS_EXCEPTIONS
            #include "Exception.hxx"
        #endif // XNORMAL_HAS_EXCEPTIONS
    }

#endif //__XNORMAL__HEADER__68E4C94A__923A__43ee__9042__9DE13C20404B__

