#if 0
    This file is part of the xNormal 3 SDK, formatted for Doxygen/JavaDoc 
    documentation system. 
    (c) 2005-2015, S.Orgaz.
    Contact: granthill76 'at' yahoo.com
    Visit http://www.xnormal.net for more info
 
    FOR MORE INFO ABOUT THE LICENSE, CONSULT THE XNORMAL PDF DOCUMENTATION
#endif //0 comment

/**
* A generic class that provides miscellaneous tool functions for paths,
* numerical conversions, tokenization, etc...
* \todo Sepparate into different classes ( Path, Random, etc... )
*/
class Utils XNORMAL_FINALCLASS
{
public:
    /**
    * Retrieves the time (in seconds) past from the operating system's boot
    * @return The seconds past from the operating system's boot
    */
    static xnDouble Time()
    {
        return static_cast<xnDouble>(clock()) / static_cast<xnDouble>(CLOCKS_PER_SEC);
    }

public:
    /**
    * Test is a value a power-of-two and optionally return the steps
    * (mipmap count)
    * @param val [in] The value to test
    * @param outSteps [inout] A pointer to a variable to be filled with the 
    * steps(mipmap count). You can pass NULL if you don't need this. 
    * This will be filled only if this method return TRUE. 
    * If this method returns FALSE then WON'T be filled.
    * 
    * @return TRUE is the value is power-of-two(and then the 'outSteps' 
    * parameter will be filled). FALSE if the value is NOT power-of-two ( 
    * and, then, the 'outSteps' parameter WON'T be filled )
    */
    static bool Log2 ( const xnUInt32 val, xnUInt32* outSteps ) XNORMAL_NOTHROW
    {
        //ALWAYS initialize out params
        if ( outSteps!=xnNULL )
        {
            *outSteps = 1;
        }

        xnUInt32 v(1U);
        while ( v<val )
        {
            v<<=1U;

            if ( outSteps!=xnNULL )
            {
                *outSteps = (*outSteps)+1U;
            }
        }

        return (val==v);
    }

public:
    /**
    * Get the file extension in form ".XXX" of the specified file
    * @param s [in] The filename
    *
    * @param outRes [out] A reference to a string to be filled in the form 
    * ".XXX" with the extension. 
    * For example, if you specify the file "C:/games/myModel.obj" this function
    * will return ".obj" or ".OBJ" if you specify the uppercase.
    * If the file has no extension then a "." string will be returned. 
    * For example: "C:/games/myModel" and "C:/games/myModel." returns both "." 
    *
    * @param toUppercase [in] TRUE if you want the extension returned in 
    * uppercase or FALSE if you want the extension in lowercase
    */
    static void GetFileExtension ( const IString& s, IString& outRes, const bool toUppercase )
    {
        outRes.Clear();
        std::wstring file ( s.GetPtr() );

        for ( int i=static_cast<int>(file.length())-1; i>=0; --i )
        {
            if ( L'/'==file[i] || L'\\'==file[i] )
            {
                //If a folder bar appears before we get a '.' then there's no extension. Example: "c:\peter.asberg\myDir\myObj"
                break;
            }

            if ( L'.'==file[i] )
            {
                outRes.SetW ( file.substr(i,file.length()-i).c_str() );
                break;
            }
        }

        //pass to uppercase and return
        if ( true==outRes.IsEmpty() )
        {
            outRes.SetW ( L"." );
        }

        if ( true==toUppercase )
        {
            outRes.ToUpper();
        }
    }

    /**
    * Get the filename without extesion 
    * @param s [in] The filename
    *
    * @param outRes [out] A reference to a string with the filename WITHOUT
    * extension. For example, if you specify the file "C:/games/myModel.obj" 
    * this function will return "C:/games/myModel"
    */          
    static void GetFileNameWithoutExtension ( const IString& s, IString& outRes )
    {
        std::wstring res ( s.GetPtr() );

        for ( int i=static_cast<int>(res.length())-1; i>=0; --i )
        {
            if ( L'/'==res[i] || L'\\'==res[i] )
            {
                //If a folder bar appears before we get a '.' then there's no 
                //extension. Example: c:\peter.asberg\myDir\myObj
                break;
            }

            if ( L'.'==res[i] )
            {
                outRes.SetW ( res.substr(0,i).c_str() );
                break;
            }
        }
    }

    /**
    * Get the filename without path
    * @param s [in] The filename
    *
    * @param outRes [out] A reference to a string to be filled with the 
    * filename without the path. For example, if you specify the file 
    * "C:/games/myModel.obj" this function will return "myModel.obj"
    */
    static void GetFileNameWithoutPath ( const IString& s, IString& outRes )
    {
        std::wstring res ( s.GetPtr() );

        for ( int i=static_cast<int>(res.length())-1; i>=0; --i )
        {
            if ( L'\\'==res[i] || L'/'==res[i] )
            {
                outRes.SetW ( res.substr(i+1,res.length()-i-1).c_str() );
                return;
            }
        }

        outRes.SetW ( res.c_str() );
    }

public:
    /**
    * Clamp/Tile two pixel (XY) coordinates using an image width and height
    *
    * @param x [inout] The X coordinate of the pixel
    * @param y [inout] The Y coordinate of the pixel
    * @param width [in] The width of the image(in pixels, well in the same 
    * unit that the 'x' param really)
    * @param height [in] The height of the image(in pixels, well in the 
    * same unit that the 'x' param really)
    * @param clamp [in] TRUE if you wanna clamp or FALSE to tile/wrap
    */
    static void Fix ( xnInt32& x, xnInt32& y, const xnInt32 width, const xnInt32 height, const bool clamp ) XNORMAL_NOTHROW
    {
        if ( clamp )
        {
            x = std::max<xnInt32>(0,std::min<xnInt32>(width-1,x));
            y = std::max<xnInt32>(0,std::min<xnInt32>(height-1,y));
        }
        else
        {
            if ( x<0 )
            {
                x += width;
            }
            else
            {
                if ( x>=width )
                {
                    x-= width;
                }
            }

	    if ( y<0 )
            {
                y += height;
            }
            else
            {
                if ( y>=height )
                {
                    y-= height;
                }
            }
        }
    }

    /**
    * Clamp a floating point number in range [min,max]
    *
    * @param number [in] The floating point number to clamp
    * @param maxv [in] Maximum number allowed
    * @param minv [in] Minimum number allowed
    *
    * @return The clamped number as:
    * \code
    *       if ( k<min )
    *       {
    *           return min;
    *       }
    *       else
    *       {
    *           if ( k>max )
    *           {
    *               return max;
    *           }
    *           else
    *           {
    *               return k;
    *           }
    *       }
    * \endcode
    */
    static xnSingle Clamp ( const xnSingle number, const xnSingle maxv, const xnSingle minv ) XNORMAL_NOTHROW
    {
        if ( number<minv )
        {
            return minv;
        }
        else
        {
            if ( number>maxv )
            {
                return maxv;
            }
            return number;
        }
    }

    /**
    * Convert an ASCII string to a float
    * @param t [in] The ASCII text to convert to a floating point number
    * @return The floating point number extracted from the passed string or 0.0f if any error
    * @remarks This is an improved atof() C function more fault-tolerant
    */
    static xnSingle SafeAtof ( char* t )
    {
        XNORMAL_ASSERT ( t!=0 );
        XNORMAL_ASSERT ( strlen(t)>0 );

        xnSingle l_fl(0.0f);

        //replace all commas by points for atof!
        for ( size_t i=0U; i<strlen(t); ++i )
        {
            if ( ','==t[i] )
            {
                t[i] = '.';
            }
        }

        //Do the atof
        l_fl = static_cast<xnSingle>(atof(t));
        #ifdef XNORMAL_COMPILER_VSTUDIO
            if ( 0==_finite(l_fl) || _FPCLASS_NZ==_fpclass(l_fl) )
        #else
            if ( false==Utils::IsFinite(l_fl) ||
                 FP_ZERO==__builtin_fpclassify(FP_NAN, FP_INFINITE,FP_NORMAL,FP_SUBNORMAL,FP_ZERO,l_fl) )
        #endif
        {
            //remove infinite, NaN and -0.00000 numbers
            l_fl = 0.0f;
        }
	    
        return l_fl;
    }

    /**
    * Convert an ASCII string to a signed integer
    * @param t [in] The ASCII text to convert to a signed integer number
    * @return The signed integer number extracted from the passed string or 
    * 0 if any error
    * @remarks This is an improved atoi() C function more fault-tolerant
    */
    static xnInt32 SafeAtoi ( const char* t )
    {
        return static_cast<xnInt32>(atoi(t));
    }

    /**
    * Convert an ASCII string to a signed integer
    * @param t [in] The ASCII text to convert to a signed integer number
    * @return The signed integer number extracted from the passed string or
    * 0 if any error
    * @remarks This is an improved _ultoa() C function more fault-tolerant
    */
    static xnUInt32 SafeAtoui ( const char* t )
    {
        unsigned int l_ui(0U);
        sscanf(t,"%u",&l_ui);
        return static_cast<xnUInt32>(l_ui);
    }

    /**
    * Filter a floating point value
    * @param k [in] A floating point number
    * @return A floating point number or 0.0f if the passed number is a NaN, 
    * infinite, -0.00000f or any other evil one!
    */
    static xnSingle SafeFloat ( const xnSingle k )
    {
        return Vector::SafeFloat ( k );
    }

    /**
    * Test if is a good floating point number or a NaN. negative zero or infinite.
    * @param v [in] A floating point number
    */    
    static bool IsGoodFloat ( const xnSingle v ) XNORMAL_NOTHROW
    {   
        #ifdef XNORMAL_COMPILER_VSTUDIO
            const int fpc ( _fpclass(v) );
            return ( _FPCLASS_NN==fpc || _FPCLASS_PN==fpc || _FPCLASS_PZ==fpc );
        #else
            return ( FP_NORMAL==__builtin_fpclassify(FP_NAN, FP_INFINITE,FP_NORMAL,FP_SUBNORMAL,FP_ZERO,v) );
        #endif
    }
    
    static bool IsFinite ( const xnSingle k )
    {
        #ifdef XNORMAL_COMPILER_VSTUDIO
            return ( _finite(k)!=0 );
        #else
            const int fpc ( __builtin_fpclassify(FP_NAN, FP_INFINITE,FP_NORMAL,FP_SUBNORMAL,FP_ZERO,k) );
            return ( FP_ZERO==fpc || FP_NORMAL==fpc );
        #endif
    }

public:
    static std::string toUTF8 ( const wchar_t *s )
    {
        #ifdef XNORMAL_COMPILER_VSTUDIO
            std::string res;
            if ( xnNULL!=s && wcslen(s)>0U )
            {
                res = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(s);
            }
            return res;
        #else
            // MinGW64 does not support wstring_convert apparently so...
            const std::wstring src ( s );
            std::string dest;
            for (size_t i = 0U; i < src.size(); i++)
            {
                wchar_t w ( src[i] );
                if (w <= 0x7f)
                {
                    dest.push_back ( static_cast<char>(w) );
                }
                else
                {
                    if (w <= 0x7ff)
                    {
                        dest.push_back(0xc0 | ((w >> 6)& 0x1f));
                        dest.push_back(0x80| (w & 0x3f));
                    }
                    else
                    {
                        if (w <= 0xffff)
                        {
                            dest.push_back(0xe0 | ((w >> 12)& 0x0f));
                            dest.push_back(0x80| ((w >> 6) & 0x3f));
                            dest.push_back(0x80| (w & 0x3f));
                        }
                        else
                        {
                            if (w <= 0x10ffff)
                            {
                                dest.push_back(0xf0 | ((w >> 18)& 0x07));
                                dest.push_back(0x80| ((w >> 12) & 0x3f));
                                dest.push_back(0x80| ((w >> 6) & 0x3f));
                                dest.push_back(0x80| (w & 0x3f));
                            }
                            else
                            {
                                dest.push_back('?');
                            }
                        }
                    }
                }
            }

            return dest;            
        #endif // XNORMAL_COMPILER_VSTUDIO
    }

    static std::wstring toUNICODE ( const char *utf8 )
    {
        #ifdef XNORMAL_COMPILER_VSTUDIO
            std::wstring res;
            if ( xnNULL!=utf8 && strlen(utf8)>0U )
            {
                res = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(utf8);
            }
            return res;
        #else
            // MinGW64 does not support wstring_convert apparently so...
            std::wstring dest;
            const std::string src ( utf8 );

            wchar_t w ( 0 );
            int bytes ( 0 );
            wchar_t err ( L'?' );
            for (size_t i = 0U; i < src.size(); i++)
            {
                unsigned char c = (unsigned char)src[i];
                if (c <= 0x7f)
                {
                    //first byte
                    if (bytes)
                    {
                        dest.push_back(err);
                        bytes = 0;
                    }
                    dest.push_back((wchar_t)c);
                }
                else if (c <= 0xbf)
                {
                    //second/third/etc byte
                    if (bytes)
                    {
                        w = ((w << 6)|(c & 0x3f));
                        bytes--;
                        if ( 0==bytes )
                        {
                            dest.push_back(w);
                        }
                    }
                    else
                    {
                        dest.push_back(err);
                    }
                }
                else if (c <= 0xdf){//2byte sequence start
                    bytes = 1;
                    w = c & 0x1f;
                }
                else if (c <= 0xef){//3byte sequence start
                    bytes = 2;
                    w = c & 0x0f;
                }
                else if (c <= 0xf7){//3byte sequence start
                    bytes = 3;
                    w = c & 0x07;
                }
                else{
                    dest.push_back(err);
                    bytes = 0;
                }
            }
            if (bytes>0)
            {
                dest.push_back(err);
            }

            return dest;
        #endif  // XNORMAL_COMPILER_VSTUDIO
    }
};

