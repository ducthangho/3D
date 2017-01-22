#if 0
    This file is part of the xNormal 3 SDK, formatted for Doxygen/JavaDoc 
    documentation system. 
    (c) 2005-2015, S.Orgaz.
    Contact: granthill76 'at' yahoo.com
    Visit http://www.xnormal.net for more info
 
    FOR MORE INFO ABOUT THE LICENSE, CONSULT THE XNORMAL PDF DOCUMENTATION
#endif //0 comment


//Avoid some recursive includes!
class Matrix3x3;

/**
* Basic 3D euclidean vector structure
*/
struct basic_XN_vector
{
    /**
    * The X cartesian component of the vector
    */
    xnSingle x;

    /**
    * The Y cartesian component of the vector
    */
    xnSingle y;

    /**
    * The Z cartesian component of the vector
    */
    xnSingle z;
};

/**
* A basic and very simple standard euclidean 3D vector (XYZ components)
*/
class Vector XNORMAL_FINALCLASS : public basic_XN_vector
{
public:
    /**
    * Default constructor ( empty at the moment )
    */
    Vector() XNORMAL_NOTHROW {}

    /**
    * Construct from XYZ parameters
    */
    explicit Vector ( const xnSingle X, const xnSingle Y, const xnSingle Z ) XNORMAL_NOTHROW
    {
        this->x = X;
        this->y = Y;
        this->z = Z;
    }

    /**
    * Construct from a scalar and replicate it into XYZ
    * Example: Vector myVec(8.0f) gives you a myVec(0.8f,0.8f,0.8f)
    */
    explicit Vector ( const xnSingle k ) XNORMAL_NOTHROW
    {
        this->x = this->y = this->z = k;
    }

    /**
    * Construct from a basic_XN_vector
    */
    explicit Vector ( const basic_XN_vector& v ) XNORMAL_NOTHROW
    {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
    }

    /**
    * Sonstruct from a float pointer
    */
    explicit Vector ( const xnSingle *ptr ) XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( ptr!=xnNULL );
        this->x = ptr[0];
        this->y = ptr[1];
        this->z = ptr[2];
    }

public:
    /**
    * Get the i-th element of the vector
    * @param i [in] The index of the vector element to retrieve, starting at 0,
    * in range [0,2]
    * @return The i-th element of the vector ( 0=x, 1=y, 2=z );
    * @remarks An assert will be fired if you specify an outbounds i-th index
    */
    xnSingle operator [] ( const xnUInt32 i ) const XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( i<3U );
        return reinterpret_cast<const xnSingle*>(this)[i];
    }

    /**
    * Get a reference to the i-th element of the vector
    * @param i [in] The index of the vector element to retrieve, starting at 0,
    * in range [0,2]
    * @return The i-th element of the vector ( 0=x, 1=y, 2=z );
    * @remarks An assert will be fired if you specify an outbounds i-th index
    */
    xnSingle& operator [] ( const xnUInt32 i ) XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( i<3U );
    	return reinterpret_cast<xnSingle*>(this)[i];
    }

public:
    /**
    * Test if all the vector's componentns (XYZ) are EXACTLY zero
    * @return true if X,Y and Z components are exactly zero or false if not
    */
    bool IsZero () const XNORMAL_NOTHROW
    {
        return (0.0f==x && 0.0f==y && 0.0f==z);
    }

    /**
    * Test if any of the vector's componentns (XYZ) is NOT zero
    * @return true if any of the X,Y and Z components are NOT zero
    */
    bool IsNotZero () const XNORMAL_NOTHROW
    {
        return ( x!=0.0f || y!=0.0f || z!=0.0f );
    }

    /**
    * Compare if a vector is aproximately equal to me
    * @param v The vector to compare
    * @param epsilon The comparison error metric (default param 1E-5f)
    * @return true if the specified vector is aprox equal to me or false if is
    * different than me
    */
    bool IsAproxEqual ( const basic_XN_vector& v, const xnSingle epsilon ) const XNORMAL_NOTHROW
    {
        return ( fabsf(x-v.x)<=epsilon && fabsf(y-v.y)<=epsilon && fabsf(z-v.z)<=epsilon );
    }

    static xnSingle SafeFloat ( const xnSingle k ) 
    {
        xnSingle l_fl;

        #ifdef XNORMAL_COMPILER_VSTUDIO
            const int fpc ( _fpclass(k) );
            switch(fpc)
            {
                default:
                    //NaN or denormalized number
                    l_fl = 0.0f;
                break;
                case _FPCLASS_NZ:
                    l_fl = 0.0f;
                break;
                case _FPCLASS_NINF:
                    l_fl = -FLT_MAX/2.0f;
                break;
                case _FPCLASS_PINF:
                    l_fl = +FLT_MAX/2.0f;
                break;
                case _FPCLASS_NN:
                case _FPCLASS_PN:
                case _FPCLASS_PZ:
                    //Good number, in good range and normalized. Copy it
                    l_fl = k;
                break;
            }
        #else
            const int fpc ( __builtin_fpclassify(FP_NAN, FP_INFINITE, FP_NORMAL, FP_SUBNORMAL, FP_ZERO, k) );
            switch(fpc)
            {
                case FP_ZERO:
                    l_fl = 0.0f;
                break;
                case FP_INFINITE:
                    l_fl = FLT_MAX/2.0f;
                break;
                case FP_SUBNORMAL:
                    l_fl = FLT_MIN;
                break;
                case FP_NORMAL:
                default:
                    // Good number, just copy it
                    l_fl = k;
                break;
            }
        #endif

        return l_fl;
    }

    /**
    * Fix neg.zero and NaN.
    **/
    Vector& Fix()
    {
        x = SafeFloat(x);
        y = SafeFloat(y);
        z = SafeFloat(z);
        return *this;
    }

public:
    /**
    * Normalize this vector.
    * This makes all components to be in range [-1,1] and force the vector 
    * length to be the unit(1.0f)
    *
    * Example: Vector myVect(1,2,3)  -->normalize --> ( 0.267, 0.534, 0.8017 )
    * 
    * @return A reference to myself after normalizing me
    */
    Vector& Normalize () XNORMAL_NOTHROW
    {
        const xnDouble xx ( static_cast<xnDouble>(x) );
        const xnDouble yy ( static_cast<xnDouble>(y) );
        const xnDouble zz ( static_cast<xnDouble>(z) );
        const xnDouble len ( sqrt(xx*xx + yy*yy + zz*zz ) );
        if ( len>=DBL_MIN )
        {
            this->x = static_cast<float>(xx/len);
            this->y = static_cast<float>(yy/len);
            this->z = static_cast<float>(zz/len);
        }
        return *this;
    }

    /**
    * Normalize a specified vector.
    * @param v [in] The vector to normalize
    * @param outRes [in] A pointer to a vector variable where to copy 
    * the result
    * @return A new vector, normalized.
    */
    static void Normalize ( const basic_XN_vector& v, basic_XN_vector* outRes ) XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( outRes!=xnNULL );
        const xnDouble xx ( static_cast<xnDouble>(v.x) );
        const xnDouble yy ( static_cast<xnDouble>(v.y) );
        const xnDouble zz ( static_cast<xnDouble>(v.z) );
        const xnDouble len ( sqrt(xx*xx + yy*yy + zz*zz ) );
        if ( len>=DBL_MIN )
        {
            outRes->x = static_cast<float>(xx/len);
            outRes->y = static_cast<float>(yy/len);
            outRes->z = static_cast<float>(zz/len);
        }
    }

    /**
    * Get the euclidean length of this vector
    * @return The length of the vector (in generic units, without any 
    * specific format)
    */
    xnSingle Length () const XNORMAL_NOTHROW
    {
        return ::sqrtf ( x*x + y*y + z*z );
    }

    /**
    * Get the SQUARED euclidean length of this vector
    * Squared distances are much faster to compute than non-squared ones, 
    * because saves a expensive square root instruction. Use this when you can
    * but caution because squared numbers can be really LARGE and could provoke
    * a nice overflow, NaN or Infinite floating point number.
    */
    xnSingle LengthSq () const XNORMAL_NOTHROW
    {
        return x*x + y*y + z*z;
    }

    /**
    * Get the Manhattan length of this vector
    * @remarks This is useful for pathfinding and graphs
    */
    xnSingle ManhattanLength () const XNORMAL_NOTHROW
    {
        return fabsf(x) + fabsf(y) + fabsf(z);
    }

public:
    /**
    * Perform a dot product between this vector and other vector
    * @param v The vector to dot-product with me
    * @returns A float with the dot product result as dotP = (*this)· v
    * @remarks We inline here this function for speed
    */
    xnSingle Dot ( const basic_XN_vector& v ) const XNORMAL_NOTHROW
    {
        return x*v.x + y*v.y + z*v.z;
    }

    /**
    * Cross-product this vector with a specified vector
    * @param v [in] The vector to cross-product with this vector
    * @return A reference to myself as ((*this) x v)
    */
    Vector& Cross ( const basic_XN_vector& v ) XNORMAL_NOTHROW
    {
        const xnSingle xx (this->x);
        const xnSingle yy (this->y);
        const xnSingle zz (this->z);

        this->x = (yy*v.z) - (zz*v.y);
        this->y = (zz*v.x) - (xx*v.z);
        this->z = (xx*v.y) - (yy*v.x);
        return *this;
    }

    /**
    * Perform cross product between two specified vectors. 
    * This is a syntax sugar for the instanced Cross() method
    * @param v1 [in] The first vector
    * @param v2 [in] The second vector
    * @param outRes [out] A pointer to a vector variable where to copy the 
    * result
    * @remarks v1 and v2 can be the same vector, this method is protected
    * against that condition
    */
    static void Cross ( const basic_XN_vector& v1, const basic_XN_vector& v2, basic_XN_vector* outRes ) XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( outRes!=xnNULL );

        Vector l_vT;
        l_vT.x = (v1.y*v2.z) - (v1.z*v2.y);
        l_vT.y = (v1.z*v2.x) - (v1.x*v2.z);
        l_vT.z = (v1.x*v2.y) - (v1.y*v2.x);
        *outRes = l_vT;
    }

public:
    /**
    * Equal operator
    * @param v [in] The vector to compare this vector
    * @return TRUE if this vector and the specified vector 'v' are identical
    * or FALSE if they are different
    */
    bool operator == ( const Vector& v ) const
    {
        return ( this->x==v.x && this->y==v.y && this->z==v.z );
    }

    /**
    * Non-equal operator
    * @param v [in] The vector to compare this vector
    * @return TRUE if this vector and the specified vector 'v' are NOT 
    * identical or FALSE if they are identical
    */
    bool  operator != ( const Vector& v ) const
    {
        return ( this->x!=v.x || this->y!=v.y || this->z!=v.z );
    }

public:
    /**
    * Negate operator
    * @return A new vector as V' = ( -this->x, -this->y, -this->z )
    */
    Vector operator - () const XNORMAL_NOTHROW
    {
        return Vector(-x,-y,-z);
    }

    Vector&  operator = ( const Vector& v ) XNORMAL_NOTHROW
    {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
        return *this;
    }

    Vector& operator = ( const xnSingle k ) XNORMAL_NOTHROW
    {
        x = y = z = k;
        return *this;
    }

    Vector operator + ( const basic_XN_vector& v ) const XNORMAL_NOTHROW
    {
        return Vector ( x+v.x, y+v.y, z+v.z );
    }

    Vector operator + ( const xnSingle k ) const XNORMAL_NOTHROW
    {
        return Vector ( x+k, y+k, z+k );
    }

    Vector operator - ( const basic_XN_vector& v ) const XNORMAL_NOTHROW
    {
        return Vector ( x-v.x, y-v.y, z-v.z );
    }

    Vector operator - ( const xnSingle k ) const XNORMAL_NOTHROW
    {
        return Vector ( x-k, y-k, z-k );
    }

    Vector operator * ( const basic_XN_vector& v ) const XNORMAL_NOTHROW
    {
        return Vector ( x*v.x, y*v.y, z*v.z );
    }

    Vector operator * ( const xnSingle k ) const XNORMAL_NOTHROW
    {
        return Vector ( x*k, y*k, z*k );
    }

    Vector operator / ( const xnSingle k ) const XNORMAL_NOTHROW
    {
        return Vector ( x/k, y/k, z/k );
    }

public:
    Vector& operator += ( const basic_XN_vector& v ) XNORMAL_NOTHROW
    {
        this->x += v.x;
        this->y += v.y;
        this->z += v.z;
        return *this;
    }

    Vector& operator += ( const xnSingle k ) XNORMAL_NOTHROW
    {
        this->x += k;
        this->y += k;
        this->z += k;
        return *this;
    }

    Vector& operator -= ( const basic_XN_vector& v ) XNORMAL_NOTHROW
    {
        this->x -= v.x;
        this->y -= v.y;
        this->z -= v.z;
        return *this;
    }

    Vector& operator -= ( const xnSingle k ) XNORMAL_NOTHROW
    {
        this->x -= k;
        this->y -= k;
        this->z -= k;
        return *this;
    }

    Vector& operator *= ( const basic_XN_vector& v ) XNORMAL_NOTHROW
    {
        this->x *= v.x;
        this->y *= v.y;
        this->z *= v.z;
        return *this;
    }

    Vector& operator *= ( const xnSingle k ) XNORMAL_NOTHROW
    {
        this->x *= k;
        this->y *= k;
        this->z *= k;
        return *this;
    }

    Vector& operator /= ( const xnSingle k ) XNORMAL_NOTHROW
    {
        this->x /= k;
        this->y /= k;
        this->z /= k;
        return *this;
    }

public:
    /**
    * Linear interpolate two vectors and copy the result to this vector
    * @param vSrc [in] A pointer to the "source" vector
    * @param vDst [in] A pointer to the "target"-destination vector
    * @param pctSrc [in] Percentage for the "source" vector ( 1.0f-dstPct )
    * @param pctDst [in] Percentage for the "target"-destination 
    * vector ( dstPct )
    * @param outRes [out] A pointer to a vector where to put the result
    */
    static void LInterpolate ( const basic_XN_vector& vSrc, 
        const basic_XN_vector& vDst, const xnSingle pctSrc, 
        const xnSingle pctDst, basic_XN_vector* outRes ) XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( outRes!=xnNULL );

        outRes->x = (vSrc.x*pctSrc) + (vDst.x*pctDst);
        outRes->y = (vSrc.y*pctSrc) + (vDst.y*pctDst);
        outRes->z = (vSrc.z*pctSrc) + (vDst.z*pctDst);
    }

    /**
    * Tri-linear interpolate three Vector2 and copy the result to this vector
    * @param v1 [in] A pointer to the first vector
    * @param v2 [in] A pointer to the second vector
    * @param v3 [in] A pointer to the third vector
    * @param bar [in] The three barycentric factors to use in X(1st vector),
    * Y(2nd vector), Z(3rd vector)
    * @param outRes [out] A pointer to a vector where to put the result as:
    *   outRes->x = (v1.x*bar.x) + (v2.x*bar.y) + (v3.x*bar.z);
	*   outRes->y = (v1.y*bar.x) + (v2.y*bar.y) + (v3.y*bar.z);
	*   outRes->z = (v1.z*bar.x) + (v2.z*bar.y) + (v3.z*bar.z);
    */
    static void TriLInterpolate ( const basic_XN_vector& v1, 
        const basic_XN_vector& v2, const basic_XN_vector& v3, 
        const basic_XN_vector& bar, basic_XN_vector* outRes ) XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( outRes!=xnNULL );

        Vector l_vT;
        l_vT.x = (v1.x*bar.x) + (v2.x*bar.y) + (v3.x*bar.z);
        l_vT.y = (v1.y*bar.x) + (v2.y*bar.y) + (v3.y*bar.z);
        l_vT.z = (v1.z*bar.x) + (v2.z*bar.y) + (v3.z*bar.z);
        *outRes = l_vT;
    }

public:
    /**
    * Set this Vector using a float pointer
    * @param ptr [in] A pointer to, almost, three floats
    */
    void Set ( const xnSingle* ptr )
    {
        XNORMAL_ASSERT ( ptr!=xnNULL );
        this->x = ptr[0];
        this->y = ptr[1];
        this->z = ptr[2];
    }

public:
    /**
    * Get the maximum component index of the vector ( 0=x, 1=y, 2=z )
    * @remarks Vector element signs are taken into consideration
    */
    xnUInt32 GetMaxComponent () const XNORMAL_NOTHROW
    {
        if ( x>y && x>z )
        {
            return 0;
        }
        else
        {
            if ( y>x && y>z )
            {
                return 1;
            }
    	    return 2;
        }
    }

    /**
    * Get the maximum component index of the vector ( 0=x, 1=y, 2=z )
    * @remarks Vector element signs are IGNORED
    * @remarks This is useful for cubic texture mapping
    */
    xnUInt32 GetMaxAbsComponent () const XNORMAL_NOTHROW
    {
        const xnSingle xx(fabsf(x));
        const xnSingle yy(fabsf(y));
        const xnSingle zz(fabsf(z));

        if ( xx>yy && xx>zz )
        {
            return 0;
        }
        else
        {
            if ( yy>xx && yy>zz )
            {
                return 1;
            }
            return 2;
        }
    }

    /**
    * Get the minimum component index of the vector ( 0=x, 1=y, 2=z )
    * @remarks Vector element signs are taken into consideration
    */
    xnUInt32 GetMinComponent () const XNORMAL_NOTHROW
    {
        if ( x<y && x<z )
        {
            return 0;
        }
        else
        {
            if ( y<x && y<z )
            {
                return 1;
            }
            return 2;
        }
    }

    /**
    * Get the minimum component index of the vector ( 0=x, 1=y, 2=z )
    * @remarks Vector element signs are IGNORED
    */
    xnUInt32 GetMinAbsComponent () const XNORMAL_NOTHROW
    {
        const xnSingle xx(fabsf(x));
        const xnSingle yy(fabsf(y));
        const xnSingle zz(fabsf(z));

        if ( xx<yy && xx<zz )
        {
            return 0;
        }
        else
        {
            if ( yy<xx && yy<zz )
            {
                return 1;
            }
            return 2;
        }
    }

    /**
    * Get the minimum components of two vectors
    * @param a [in] The first vector
    * @param b [in] The second vector
    * @param res [out] A pointer to a vector to be filled as
    *   [min(a.x,b.x),min(a.y,b.y),min(a.z,b.z)]
    */
    static void GetMinVector ( const Vector& a, const Vector& b, Vector* res )
    {
        XNORMAL_ASSERT ( res!=xnNULL );
        const register __m128 aa(_mm_set_ps(1.0f,a.z,a.y,a.x));
        const register __m128 bb(_mm_set_ps(1.0f,b.z,b.y,b.x));
        const register __m128 minv(_mm_min_ps(aa,bb));
        res->x = _MM_GET_F32(minv,0);
        res->y = _MM_GET_F32(minv,1);
        res->z = _MM_GET_F32(minv,2);
    }

    /**
    * Get the maximum components of two vectors
    * @param a [in] The first vector
    * @param b [in] The second vector
    * @param res [out] A pointer to a vector to be filled as
    *   [max(a.x,b.x),max(a.y,b.y),max(a.z,b.z)]
    */
    static void GetMaxVector ( const Vector& a, const Vector& b, Vector* res )
    {
        XNORMAL_ASSERT ( res!=xnNULL );
        const register __m128 aa(_mm_set_ps(1.0f,a.z,a.y,a.x));
        const register __m128 bb(_mm_set_ps(1.0f,b.z,b.y,b.x));
        const register __m128 maxv(_mm_max_ps(aa,bb));
        res->x = _MM_GET_F32(maxv,0);
        res->y = _MM_GET_F32(maxv,1);
        res->z = _MM_GET_F32(maxv,2);
    }

public:
    /**
    * Clamp all the vector components into a range of values
    * @param v [in] the vector to clamp
    * @param minv [in] The minimum value
    * @param maxv [in] The maximum value
    * @param outRes [out] A pointer to a vector to be written with the
    *   clamped values. It can be the same than "v". Cannot be NULL.
    */
    static void Clamp ( const Vector& v, const xnSingle minv, const xnSingle maxv, Vector *outRes )
    {
        XNORMAL_ASSERT ( outRes!=xnNULL );

        const register __m128 xmm(_mm_set_ps(0.0f,v.z,v.y,v.x));
    
        const register __m128 minvx(_mm_set_ps1(minv));
        const register __m128 maxvx(_mm_set_ps1(maxv));

        const register __m128 res ( _mm_min_ps(_mm_max_ps(xmm,minvx),maxvx) );

        outRes->Set((const xnSingle*)&res);
    }

public:
    /**
    * Compute the reflected vector given a direction and a normal:
    * 
    * dir  n    ^ reflected vector
    *  \   |   /
    *   \  |  /
    *    \ | /
    *     >|/
    * -----|------- surface
    * 
    * @param dir [in] Input (normalized) direction
    * @param n [in] Surface normal(normalized)
    * @param outRes [out] A pointer to a vector where to place the result
    */
    static void Reflect ( const Vector& dir, const Vector& n, Vector* outRes )
    {
        XNORMAL_ASSERT ( outRes!=xnNULL );
        *outRes = (dir - (n*2.0f*dir.Dot(n))).Normalize();
    }


public:
    /**
    * Swizzle vector XYZ coordinates using a mask
    * @param swizzle [in] A string containing the swizzle mask. 
    * For example, a mask equal to "X+Y+Z+" won't perform any operation. 
    * A mask like "Z+Y+X+" will swap X and Z coordinates.
    * Notice this mask must be in UPPERCASE and in format
    * [CoordX][Sign][CoordY][Sign][CoordZ][Sign]
    */
    void Swizzle ( const char* swizzle ) XNORMAL_NOTHROW
    {
        if ( 0==strcmp(swizzle,"X+Y+Z+") )
        {
            return;
        }

        char c[3] = {'X','Y','Z'}, s[3] = {'+','+','+'};
        xnSingle ret[3];

        #ifdef XNORMAL_DEBUG
            const int nRet = sscanf ( swizzle, "%c%c%c%c%c%c", &c[0], &s[0], &c[1], &s[1], &c[2], &s[2] );
            XNORMAL_ASSERT ( 6==nRet );
        #else
            sscanf ( swizzle, "%c%c%c%c%c%c", &c[0], &s[0], &c[1], &s[1], &c[2], &s[2] );
        #endif

        for ( int i=0; i<3; i++ )
        {
            if ( 'X'==c[i] )
            {
                ret[i] = ('-'==s[i]) ? -x : x;
            }
            else
            {
                if ( 'Y'==c[i] )
                {
                    ret[i] = ('-'==s[i]) ? -y : y;
                }
                else
                {
                    ret[i] = ('-'==s[i]) ? -z : z;
                }
            }
        }

        this->x = ret[0];
        this->y = ret[1];
        this->z = ret[2];
    }

    template <typename T>
    void ToStringStd ( T& outRes ) const
    {
        char c[255];
        const int n ( sprintf ( c, "%4.4f;%4.4f;%4.4f",x,y,z) );
        if ( n>2 )
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
    * Pass the vector to a string
    * @param outRes [out] A STL string with the vector converted to a
    * string as ( x;y;z )
    */
    XNORMAL_INLINE void ToString ( IString& outRes ) const XNORMAL_NOTHROW
    {
        char c[255];
        const int n ( sprintf ( c, "%4.4f;%4.4f;%4.4f",x,y,z) );
        if ( n>2 )
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
};

/**
* Scalar-by-vector multiplication(left operator)
* @param k [in] A scalar
* @param v [in] A vector
* @return The scalar multiplied by the specified vector as v'=k*v
*/
XNORMAL_INLINE Vector operator * ( const xnSingle k, const basic_XN_vector& v ) XNORMAL_NOTHROW
{
    return Vector ( k*v.x, k*v.y, k*v.z );
}

/**
* Scalar-by-vector addition(left operator)
* @param k [in] A scalar
* @param v [in] A vector
* @return The scalar added to the specified vector as v'=k+v
*/
XNORMAL_INLINE Vector operator + ( const xnSingle k, const basic_XN_vector& v ) XNORMAL_NOTHROW
{
    return Vector ( k+v.x, k+v.y, k+v.z );
}

/**
* A 2D vector
*/
class Vector2
{
public:
    /**
    * Default constructor (empty atm)
    */
    Vector2() XNORMAL_NOTHROW
    {
    }

    /**
    * Construct from the two components
    * @param X [in] The x component
    * @param Y [in] The y component
    */
    Vector2 ( const xnSingle X, const xnSingle Y ) XNORMAL_NOTHROW : x(X), y(Y)
    {
    }

    Vector2 ( const xnSingle* f ) XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( f!=xnNULL );
         //note: 'f' should have enough room for, almost, two floats
        this->x = f[0];
        this->y = f[1];
    }

public:
    /**
    * Assignment operator
    * @param v [in] The vector to replicate in the X and Y components
    * @return A reference to this vector with the copied scalar in XY
    */
    Vector2& operator = ( const Vector2& v ) XNORMAL_NOTHROW
    {
        this->x = v.x;
        this->y = v.y;
        return *this;
    }

    /**
    * Scalar-assignment operator
    * @param k [in] The scalar to replicate in the X and Y components
    * @return A reference to this vector with the copied scalar in XY
    */
    Vector2& operator = ( const xnSingle k ) XNORMAL_NOTHROW
    {
        x = y = k;
        return *this;
    }

    void Set ( const xnSingle* f ) XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( f!=xnNULL );
        this->x = f[0];
        this->y = f[1];
    }

public:
    /**
    * Multiply operator by other Vector2
    * @param v [in] A vector to multiply me by
    * @return A new vector as V'=(*this) * v
    */
    Vector2 operator * ( const Vector2& v ) const XNORMAL_NOTHROW
    {
        return Vector2 ( x*v.x, y*v.y );
    }

    /**
    * Multiply operator by a scalar
    * @param k [in] A scalar to multiply me by
    * @return A new vector as V'=(*this) * k
    */
    Vector2 operator * ( const xnSingle k ) const XNORMAL_NOTHROW
    {
        return Vector2 ( x*k, y*k );
    }

    /**
    * Division operator by a scalar
    * @param k [in] A scalar to divide me by
    * @return A new vector as V'=(*this) / k
    */
    Vector2 operator / ( const xnSingle k ) const XNORMAL_NOTHROW
    {
        //Test a possible divide-by-zero... Btw, IEEE754 makes +-INF on
        //k close to zero .
        XNORMAL_ASSERT ( fabsf(k)>=1e-6f );
        return Vector2 ( x/k, y/k );
    }

    /**
    * Addition operator by other Vector2
    * @param v [in] A vector to add me by
    * @return A new vector as V'=(*this) + v
    */
    Vector2 operator + ( const Vector2& v ) const XNORMAL_NOTHROW
    {
        return Vector2 ( x+v.x, y+v.y );
    }

    /**
    * Subtract operator by other Vector2
    * @param v [in] A vector to subtract me by
    * @return A new vector as V'=(*this) - v
    */
    Vector2 operator - ( const Vector2& v ) const XNORMAL_NOTHROW
    {
        return Vector2 ( x-v.x, y-v.y );
    }

    /**
    * Subtract operator by a scalar
    * @param k [in] A scalar constant to be subtraced from me
    * @return A new vector as v'=(*this) - v
    */
    Vector2 operator - ( const xnSingle k ) const XNORMAL_NOTHROW
    {
        return Vector2 ( x-k, y-k );
    }

public:
    Vector2& operator += ( const Vector2& v ) XNORMAL_NOTHROW
    {
        x += v.x;
        y += v.y;
        return *this;
    }

    Vector2& operator += ( const xnSingle k ) XNORMAL_NOTHROW
    {
        x += k;
        y += k;
        return *this;
    }

    Vector2& operator -= ( const Vector2& v ) XNORMAL_NOTHROW
    {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    Vector2& operator -= ( const xnSingle k ) XNORMAL_NOTHROW
    {
        x -= k;
        y -= k;
        return *this;
    }

    Vector2& operator *= ( const xnSingle k ) XNORMAL_NOTHROW
    {
        x *= k;
        y *= k;
        return *this;
    }

public:
    /**
    * Compare if a vector is aproximately equal to me
    * @param v The vector to compare
    * @param epsilon The comparison error metric (default param 1E-5f)
    * @return true if the specified vector is aprox equal to me or false if is
    * different than me
    */
    bool IsAproxEqual ( const Vector2& v, const xnSingle epsilon ) const XNORMAL_NOTHROW
    {
        return ( fabsf(x-v.x)<=epsilon && fabsf(y-v.y)<=epsilon );
    }

    bool operator == ( const Vector2& v ) const XNORMAL_NOTHROW
    {
        return (x==v.x && y==v.y);
    }

public:
    /**
    * Linear interpolate two vectors and copy the result to this vector
    * @param vSrc [in] A pointer to the "source" vector
    * @param vDst [in] A pointer to the "target"-destination vector
    * @param pctSrc [in] Percentage for the "source" vector ( 1.0f-dstPct )
    * @param pctDst [in] Percentage for the "target"-destination 
    * vector ( dstPct )
    * @param outRes [out] A pointer to a Vector2 variable where to copy 
    * the result
    */
    static void LInterpolate ( const Vector2& vSrc, const Vector2& vDst, 
        const xnSingle pctSrc, const xnSingle pctDst, 
        Vector2* outRes ) XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( outRes!=xnNULL );
        const Vector2 lc_vT ( (vSrc*pctSrc) + (vDst*pctDst) );
        *outRes = lc_vT;
    }

    /**
    * Tri-linear interpolate three Vector2 and copy the result to this vector
    * @param v1 [in] A reference to the first Vector2
    * @param v2 [in] A reference to the second Vector2
    * @param v3 [in] A reference to the third Vector2
    * @param bar [in] The barycentric factors to use in X(1st vector), 
    * Y(2nd vector), Z(3rd vector)
    * @param outRes [out] A pointer to a Vector2 variable where to copy
    * the result
    */
    static void TriLInterpolate ( const Vector2& v1, const Vector2& v2,
        const Vector2& v3, const Vector& bar, Vector2* outRes ) XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( outRes!=xnNULL );

        Vector2 l_vT;
        l_vT.x = (v1.x*bar.x) + (v2.x*bar.y) + (v3.x*bar.z);
        l_vT.y = (v1.y*bar.x) + (v2.y*bar.y) + (v3.y*bar.z);
        *outRes = l_vT;
    }

public:
    xnSingle LengthSq() const XNORMAL_NOTHROW
    {
        return (x*x) + (y*y);
    }

public:
    /**
    * Fix neg.zero and NaN.
    **/
    Vector2& Fix()
    {
        x = Vector::SafeFloat(x);
        y = Vector::SafeFloat(y);
        return *this;
    }

public:
    template <typename T>
    void ToStringStd ( T& outRes ) const
    {
        char c[255];
        const int n ( sprintf ( c, "%4.4f;%4.4f", x, y ) );
        if ( n>1 )
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
    * Pass the vector to a string
    * @return A STL string with the vector converted to a string as ( x;y )
    */
    XNORMAL_INLINE void ToString ( IString& outRes ) const XNORMAL_NOTHROW
    {
        char c[255];
        const int n ( sprintf ( c, "%4.4f;%4.4f", x, y ) );
        if ( n>1 )
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


public:
    /**
    * The X component
    */
    xnSingle x;

    /**
    * The Y component
    */
    xnSingle y;
};

/**
* A 2d-integer very simple vector
*/
struct Vector2I
{
    /**
    * The X(horizontal) component
    */
    xnInt32 x;

    /**
    * The Y(vertical) component
    */
    xnInt32 y;

    /**
    * Default constructor (empty)
    */
    Vector2I() {}

    /**
    * Construct from X and Y
    */
    Vector2I ( const xnInt32 X, const xnInt32 Y ) XNORMAL_NOTHROW : x(X), y(Y)
    {
    }
};


//---
struct VectorD
{
    VectorD() {}

    explicit VectorD ( const xnDouble _x, const xnDouble _y, const xnDouble _z )  XNORMAL_NOTHROW : x(_x), y(_y), z(_z)
    {
    }

    explicit VectorD ( const Vector &v ) XNORMAL_NOTHROW : x(static_cast<xnDouble>(v.x)), y(static_cast<xnDouble>(v.y)), z(static_cast<xnDouble>(v.z)) 
    {
    }

    VectorD ( const VectorD &v ) XNORMAL_NOTHROW : x(v.x), y(v.y), z(v.z)
    {
    }

    VectorD operator - ( const VectorD &v ) const XNORMAL_NOTHROW
    {   
        return VectorD ( x-v.x, y-v.y, z-v.z ); 
    }

    VectorD& operator += ( const VectorD &v ) XNORMAL_NOTHROW
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    static void Cross ( const VectorD &v1, const VectorD &v2, VectorD &outRes ) XNORMAL_NOTHROW
    {
        VectorD res;
        res.x = (v1.y*v2.z) - (v1.z*v2.y);
        res.y = (v1.z*v2.x) - (v1.x*v2.z);
        res.z = (v1.x*v2.y) - (v1.y*v2.x);
        outRes = res;
    }

    void Normalize () XNORMAL_NOTHROW
    {
        const xnDouble lc_dLen ( sqrt(x*x + y*y + z*z) );
        if ( lc_dLen > DBL_MIN )
        {
            x /= lc_dLen;
            y /= lc_dLen;
            z /= lc_dLen;
        }
    }

    xnDouble Length() const XNORMAL_NOTHROW
    {
        return sqrt(x*x + y*y + z*z);
    }

    xnDouble x, y, z;
};

//---
struct Vector2D
{
    Vector2D() XNORMAL_NOTHROW
    {
    }

    explicit Vector2D ( const xnDouble _x, const xnDouble _y ) XNORMAL_NOTHROW : x(_x), y(_y)
    {
    }

    explicit Vector2D ( const Vector2 &v ) XNORMAL_NOTHROW : x(static_cast<xnDouble>(v.x)), y(static_cast<xnDouble>(v.y)) 
    {
    }

    Vector2D ( const Vector2D &v ) XNORMAL_NOTHROW : x(v.x), y(v.y)
    {
    }

    Vector2D operator - ( const Vector2D &v ) const XNORMAL_NOTHROW
    {   
        return Vector2D ( x-v.x, y-v.y ); 
    }

    Vector2D& operator = ( const xnDouble k ) XNORMAL_NOTHROW
    {
        this->x = this->y = k;
        return *this;
    }

    Vector2D& operator += ( const Vector2D &v ) XNORMAL_NOTHROW
    {
        x += v.x;
        y += v.y;
        return *this;
    }

    xnDouble Length() const XNORMAL_NOTHROW
    {
        return sqrt(x*x + y*y);
    }

    xnDouble x, y;
};

