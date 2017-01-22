#if 0
    This file is part of the xNormal 3 SDK, formatted for Doxygen/JavaDoc 
    documentation system. 
    (c) 2005-2015, S.Orgaz.
    Contact: granthill76 'at' yahoo.com
    Visit http://www.xnormal.net for more info
 
    FOR MORE INFO ABOUT THE LICENSE, CONSULT THE XNORMAL PDF DOCUMENTATION
#endif //0 comment

/**
* A basic and very simple RGBA xnSingleing-point color class with RGBA elements
* all in range [0.0f,1.0f]
*/
class Color XNORMAL_FINALCLASS
{
public:
    /**
    * Default constructor ( empty atm )
    */
    Color () XNORMAL_NOTHROW {}

    /**
    * Construct from a scalar
    * @param k [in] Scalar value to replicate in the RGBA components
    */
    explicit Color ( const xnSingle k ) XNORMAL_NOTHROW : r(k), g(k), b(k), a(k) {}

    /**
    * Construct from a four-floats pointer. R is ptr[0], G is ptr[1], 
    * B is ptr[2] and A is ptr[3]
    * @param ptr [in] The pointer
    */
    explicit Color ( const xnSingle *ptr ) XNORMAL_NOTHROW
    {
        this->Set(ptr);
    }

	/**
    * Construct from a 0xAARRGGBB 32bits unsigned integer
    * @param argb [in] The 0xAARRGGBB 32bits unsigned integer
    */
    explicit Color ( const xnUInt32 argb ) XNORMAL_NOTHROW
    {
        this->operator =(argb);
    }

    /**
    * Construct from the RGBA components
    * @param R [in] The RED component of the color
    * @param G [in] The GREEN component of the color
    * @param B [in] The BLUE component of the color
    * @param A [in] The ALPHA(opacity) component of the color
    */
    explicit Color ( const xnSingle R, const xnSingle G, const xnSingle B, const xnSingle A ) XNORMAL_NOTHROW : r(R), g(G), b(B), a(A) {}

    /**
    * Copy constructor
    * @param c [in] A color reference where to copy the values from
    */
    Color ( const Color& c ) XNORMAL_NOTHROW : r(c.r), g(c.g), b(c.b), a(c.a) {}

public:
    /**
    * Get the i-th element of the color
    * @param i [in] An index in range [0,3]. 0=red, 1=green, 2=blue, 3=alpha
    * @return The i-th element of the color. 
    */
    xnSingle operator [] ( const xnUInt32 i ) const XNORMAL_NOTHROW
    {
    	XNORMAL_ASSERT ( i<4U );
        return reinterpret_cast<const xnSingle*>(this)[i];
    }

    /**
    * Get a reference to the i-th element of the color
    * @param i [in] An index in range [0,3]. 0=red, 1=green, 2=blue, 3=alpha
    * @return The i-th element of the color. 
    */
    xnSingle& operator [] ( const xnUInt32 i ) XNORMAL_NOTHROW
    {
    	XNORMAL_ASSERT ( i<4U );
        return reinterpret_cast<xnSingle*>(this)[i];
    }

public:
    /**
    * Equal operator
    * @param c [in] The vector to compare this color
    * @return TRUE if this vector and the specified color 'c' are identical or
    * FALSE if they are different
    */
    bool operator == ( const Color& c ) const XNORMAL_NOTHROW
    {
        return ( this->r==c.r && this->g==c.g && this->b==c.b && this->a==c.a );
    }

    /**
    * Non-equal operator
    * @param c [in] The vector to compare this color
    * @return TRUE if this vector and the specified color 'c' are NOT identical 
    * or FALSE if they are identical
    */
    bool operator != ( const Color& c ) const XNORMAL_NOTHROW
    {
        return ( this->r!=c.r || this->g!=c.g || this->b!=c.b || this->a==c.a );
    }

    /**
    * Test if the color is approximately the same than the specified one
    * @param c [in] The color to compare
    * @param maxDiff [in] Maximum difference ( in absolute value... so must be >=-0.0f )
    * @param testAlpha [in] "true" to test the alpha channel or "false" to ignore it.
    * @return TRUE if this color and the specified color differs more or equal
    * than maxDiff
    */
    bool IsAproxEqual ( const Color& c, const xnSingle maxDiff, const bool testAlpha ) const XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( maxDiff>=-0.0f );
        if ( testAlpha )
        {
            return ( fabsf(r-c.r)<=maxDiff && fabsf(g-c.g)<=maxDiff && fabsf(b-c.b)<=maxDiff && fabsf(a-c.a)<=maxDiff );
        }
        return ( fabsf(r-c.r)<=maxDiff && fabsf(g-c.g)<=maxDiff && fabsf(b-c.b)<=maxDiff );
    }

public:
    /**
    * Replicate a specified scalar into the RGBA component of the Color
    * @param k [in] The scalar to replicate into all the color's components
    * @return A reference to this color with the scalar replicated into all
    * its components
    */
    Color& operator = ( const xnSingle k ) XNORMAL_NOTHROW
    {
    	r = g = b = a = k;
        return *this;
    }

public:
    Color operator + ( const Color& c ) const XNORMAL_NOTHROW
    {
        return Color ( this->r+c.r, this->g+c.g, this->b+c.b, this->a+c.a );
    }

    Color operator + ( const xnSingle k ) const XNORMAL_NOTHROW
    {
        return Color ( this->r+k, this->g+k, this->b+k, this->a+k );
    }

    Color operator - ( const Color& c ) const XNORMAL_NOTHROW
    {
        return Color ( this->r-c.r, this->g-c.g, this->b-c.b, this->a-c.a );
    }

    Color operator - ( const xnSingle k ) const XNORMAL_NOTHROW
    {
        return Color ( this->r+k, this->g+k, this->b+k, this->a+k );
    }

    Color operator * ( const Color& c ) const XNORMAL_NOTHROW
    {
        return Color ( this->r*c.r, this->g*c.g, this->b*c.b, this->a*c.a );
    }

    Color operator * ( const xnSingle k ) const XNORMAL_NOTHROW
    {
         return Color ( this->r*k, this->g*k, this->b*k, this->a*k );
    }

    Color operator / ( const Color& c ) const XNORMAL_NOTHROW
    {
        return Color ( r/c.r, g/c.g, b/c.b, a/c.a );

    }

    Color operator / ( const xnSingle k ) const XNORMAL_NOTHROW
    {
        return Color ( r/k, g/k, b/k, a/k );
    }

public:
    Color& operator += ( const Color& c ) XNORMAL_NOTHROW
    {
        r += c.r;
        g += c.g;
        b += c.b;
        a += c.a;
        return *this;
    }

    Color& operator += ( const xnSingle k ) XNORMAL_NOTHROW
    {
        r += k;
        g += k;
        b += k;
        a += k;
        return *this;
    }

    Color& operator -= ( const Color& c ) XNORMAL_NOTHROW
    {
        r -= c.r;
        g -= c.g;
        b -= c.b;
        a -= c.a;
        return *this;
    }

    Color& operator -= ( const xnSingle k ) XNORMAL_NOTHROW
    {
        r -= k;
        g -= k;
        b -= k;
        a -= k;
        return *this;
    }

    Color& operator *= ( const Color& c ) XNORMAL_NOTHROW
    {
        this->r *= c.r;
        this->g *= c.g;
        this->b *= c.b;
        this->a *= c.a;
        return *this;
    }

    Color& operator *= ( const xnSingle k ) XNORMAL_NOTHROW
    {
        this->r *= k;
        this->g *= k;
        this->b *= k;
        this->a *= k;
        return *this;
    }
     
    Color& operator /= ( const Color& c ) XNORMAL_NOTHROW
    {
        r /= c.r;
        g /= c.g;
        b /= c.b;
        a /= c.a;
        return *this;
    }

    Color& operator /= ( const xnSingle k ) XNORMAL_NOTHROW
    {
        r /= k;
        g /= k;
        b /= k;
        a /= k;
        return *this;
    }

public:
    /**
    * Convert the color to a luminance value
    * @return The luminance in range [0.0f,1.0f]
    */
    xnSingle GetLuminance () const XNORMAL_NOTHROW
    {
    	return (r*0.3f) + (g*0.59f) + (b*0.11f);
    }

    /**
    * Convert the color to gray scale
    * @return The gray-scale
    */
    xnSingle GetGray () const XNORMAL_NOTHROW
    {
    	return (r+g+b)/3.0f;
    }

public:
    /**
    * Normalize color
    * @param epsilon [in] Error value. If the total intensity of the color is
    * less than this value won't be normalized.
    *
    * @param takeAlphaIntoConsideration [in] TRUE to take alpha channel into
    * the calculation or FALSE to use only thr RGB channels.
    *
    * @return A self instance reference
    */
    Color& Normalize( const xnSingle epsilon=1e-5f, const bool takeAlphaIntoConsideration=false ) XNORMAL_NOTHROW
    {
        xnSingle l_flLen;
        if ( takeAlphaIntoConsideration )
        {
            l_flLen = sqrtf ( r*r + g*g + b*b + a*a );
        }
        else
        {
            l_flLen = sqrtf ( r*r + g*g + b*b );
        }

        if ( l_flLen>=epsilon )
        {
            const xnSingle lc_flInvLen = 1.0f/l_flLen;
            this->r *= lc_flInvLen;
            this->g *= lc_flInvLen;
            this->b *= lc_flInvLen;
            this->a *= lc_flInvLen;
        }
    
        return *this;
    }

    /**
    * Normalize a color
    * @param c [in] The color to normalize
    * @param epsilon [in] Error value. If the total intensity of the color is
    * less than this value won't be normalized.
    *
    * @param takeAlphaIntoConsideration [in] TRUE to take alpha channel into
    * the calculation or FALSE to use only thr RGB channels.
    *
    * @return A new color, normalized
    */
    static Color Normalize ( const Color& c, const xnSingle epsilon=1e-5f, const bool takeAlphaIntoConsideration=false ) XNORMAL_NOTHROW
    {
        xnSingle l_flLen;
        if ( takeAlphaIntoConsideration )
        {
            l_flLen = sqrtf ( c.r*c.r + c.g*c.g + c.b*c.b + c.a*c.a );
        }
        else
        {
            l_flLen = sqrtf ( c.r*c.r + c.g*c.g + c.b*c.b );
        }

        if ( l_flLen>=epsilon )
        {
            const xnSingle lc_flInvLen ( 1.0f/l_flLen );
            return c*lc_flInvLen;
        }
    
        return c;
    }

public:
    /**
    * Get the maximum component index of the color ( 0=r, 1=g, 2=b )
    */
    xnUInt32 GetMaxRGBComponent () const XNORMAL_NOTHROW
    {
        if ( r>g && r>b )
        {
            return 0;
        }
        else
        {
            if ( g>r && g>b )
            {
                return 1;
            }
            return 2;
        }
    }

    /**
    * Get the maximum absolute component index of the color ( 0=r, 1=g, 2=b )
    */
    xnUInt32 GetMaxAbsoluteRGBComponent () const  XNORMAL_NOTHROW
    {
        const xnSingle ar(fabsf(r));
        const xnSingle ag(fabsf(g));
        const xnSingle ab(fabsf(b));

        if ( ar>ag && ar>ab )
        {
            return 0;
        }
        else
        {
            if ( ag>ar && ag>ab )
            {
                return 1;
            }
            return 2;
        }
    }

    /**
    * Get the minimum component index of the color ( 0=r, 1=g, 2=b )
    */
    xnUInt32 GetMinRGBComponent () const XNORMAL_NOTHROW
    {
        if ( r<g && r<b )
        {
            return 0;
        }
        else
        {
            if ( g<r && g<b )
            {
                return 1;
            }
            return 2;
        }
    }

    /**
    * Get the minimum absolute component index of the color ( 0=r, 1=g, 2=b )
    */
    xnUInt32 GetMinAbsoluteRGBComponent () const XNORMAL_NOTHROW
    {
        const xnSingle ar(fabsf(r));
        const xnSingle ag(fabsf(g));
        const xnSingle ab(fabsf(b));

        if ( ar<ag && ar<ab )
        {
            return 0;
        }
        else
        {
            if ( ag<ar && ag<ab )
            {
                return 1;
            }
            else
            {
                return 2;
            }
        }
    }

    /**
    * Make all the color components in range [0,1]
    */
    static void Saturate(const Color& c, Color* outRes) XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( outRes!=xnNULL );
        const register __m128 gsc_0(_mm_setzero_ps());
        const register __m128 gsc_1(_mm_set_ps1(1.0f));

        const register __m128 xmm(_mm_set_ps(c.a,c.b,c.g,c.r));
        const register __m128 res ( _mm_min_ps(_mm_max_ps(xmm,gsc_0),gsc_1) );
        outRes->Set((const xnSingle*)&res);
    }

public:
    /**
    * Copy operator
    * @param c [in] the color to copy into me
    * @return A reference to me
    */
    Color& operator = ( const Color& c ) XNORMAL_NOTHROW
    {
        this->r = c.r;
        this->g = c.g;
        this->b = c.b;
        this->a = c.a;
        return *this;
    }

    /**
    * Copy operator
    * @param argb [in] the 32bpp color(Intel order) to copy into me 
    * @return A reference to me
    */
    Color& operator = ( const xnUInt32 argb ) XNORMAL_NOTHROW
    {
    	const register xnUInt8 B ( static_cast<xnUInt8>(argb&0x000000FFU) );
        const register xnUInt8 G ( static_cast<xnUInt8>((argb&0x0000FF00U)>>8U) );
        const register xnUInt8 R ( static_cast<xnUInt8>((argb&0x00FF0000U)>>16U) );
        const register xnUInt8 A ( static_cast<xnUInt8>((argb&0xFF000000U)>>24U) );

        const register xnSingle lc_flInv255(1.0f/255.0f);
        this->r = static_cast<xnSingle>(R)*lc_flInv255;
        this->g = static_cast<xnSingle>(G)*lc_flInv255;
        this->b = static_cast<xnSingle>(B)*lc_flInv255;
        this->a = static_cast<xnSingle>(A)*lc_flInv255;

        return *this;
    }

    /**
    * Set the value of the color using a floating point pointer
    * @param ptr [in] A floating point pointer with 4 floats where to read 
    * the data from
    */
    Color& Set ( const xnSingle* ptr ) XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( ptr!=xnNULL );
        
        this->r = ptr[0];
        this->g = ptr[1];
        this->b = ptr[2];
        this->a = ptr[3];
        
        return *this;
    }

public:
    /**
    * Linear interpolate two color and copy the result to this color
    * @param cSrc [in] A pointer to the "source" color
    * @param cDst [in] A pointer to the "target"-destination color
    * @param pctSrc [in] Percentage for the "source" color ( 1.0f-dstPct )
    * @param pctDst [in] Percentage for the "target"-destination color
    * ( dstPct )
    * @param outRes [out] A pointer to a color where to put the result
    */
    static void  LInterpolate ( const Color& cSrc, const Color& cDst, const xnSingle pctSrc, const xnSingle pctDst, Color* outRes ) XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( outRes!=xnNULL );

        outRes->r = (cSrc.r*pctSrc) + (cDst.r*pctDst);
        outRes->g = (cSrc.g*pctSrc) + (cDst.g*pctDst);
        outRes->b = (cSrc.b*pctSrc) + (cDst.b*pctDst);
        outRes->a = (cSrc.a*pctSrc) + (cDst.a*pctDst);
    }

    /**
    * Tri-linear interpolate three Vector2 and copy the result to this color
    * @param c1 [in] A pointer to the first color
    * @param c2 [in] A pointer to the second color
    * @param c3 [in] A pointer to the third color
    * @param bar [in] The three barycentric factors to use in X(1st vector),
    * Y(2nd vector), Z(3rd vector)
    * @param outRes [out] A pointer to a vector where to put the result
    */
    static void TriLInterpolate ( const Color& c1, const Color& c2, const Color& c3, const basic_XN_vector& bar, Color* outRes ) XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( outRes!=xnNULL );

        Color l_vT;
        l_vT.r = (c1.r*bar.x) + (c2.r*bar.y) + (c3.r*bar.z);
        l_vT.g = (c1.g*bar.x) + (c2.g*bar.y) + (c3.g*bar.z);
        l_vT.b = (c1.b*bar.x) + (c2.b*bar.y) + (c3.b*bar.z);
        l_vT.a = (c1.a*bar.x) + (c2.a*bar.y) + (c3.a*bar.z);

        *outRes = l_vT;
    }

public:
    template <typename T>
    void ToString ( T& outRes ) const
    {
        char c[255];
        const int n ( sprintf ( c, "%4.4f;%4.4f;%4.4f;%4.4f", r, g, b, a ) );
        if ( n>3 )
        {
            outRes = c;
        }
        else
        {
            XNORMAL_ASSERT ( false );
            outRes.clear();
        }
    }

    /**
    * Pass the color to a string
    * @param outRes [out] A reference to a string where to write the color's value
    */
    XNORMAL_INLINE void ToString ( IString& outRes ) const XNORMAL_NOTHROW
    {
        char c[255];
        const int n ( sprintf ( c, "%4.4f;%4.4f;%4.4f;%4.4f", r, g, b, a ) );
        if ( n>3 )
        {
            #ifdef XNORMAL_DEBUG
                const bool res ( outRes.Set ( c ) );
                XNORMAL_ASSERT ( true==res );
            #else
                outRes.Set ( c );
            #endif            
        }
        else
        {
            XNORMAL_ASSERT ( false );
            outRes.Clear();
        }
    }

    /**
    * Pass the color to a 0xAARRGGBB 32-bits number(Intel order)
    */
    xnUInt32 ToARGB () const
    {
        xnUInt32 res ( static_cast<xnUInt32>(this->b*255.0f) );
        res |= static_cast<xnUInt32>(this->g*255.0f)<<8U;
        res |= static_cast<xnUInt32>(this->r*255.0f)<<16U;
        res |= static_cast<xnUInt32>(this->a*255.0f)<<24U;
        return res;
    }

public:
	/**
	* Blue color component in range [0.0f,1.0f]
	*/
	xnSingle r;

	/**
	* Green color component in range [0.0f,1.0f]
	*/
	xnSingle g;

	/**
	* Red color component in range [0.0f,1.0f]
	*/
	xnSingle b;

	/**
	* Alpha color component in range [0.0f=transparent,1.0f=opaque]
	*/
	xnSingle a;
};

