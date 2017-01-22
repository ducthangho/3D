#if 0
    This file is part of the xNormal 3 SDK, formatted for Doxygen/JavaDoc 
    documentation system. 
    (c) 2005-2015, S.Orgaz.
    Contact: granthill76 'at' yahoo.com
    Visit http://www.xnormal.net for more info
 
    FOR MORE INFO ABOUT THE LICENSE, CONSULT THE XNORMAL PDF DOCUMENTATION
#endif //0 comment



/**
* A row-major-stored 3x3 matrix
*
* @remarks We decided not to template it to prevent syntax hell, improve the
* portability against compilers ( 
* http://www.mozilla.org/hacking/portable-cpp.html ), to improve SPEED, to 
* allow SIMD intrinsics and because we really don't need that flexibility here.
* The entire xNormal uses xnSingle for all the maths because 
*   "doubles" occupies too much memory ( and that don't gonna change ). 
*/
class Matrix3x3 XNORMAL_FINALCLASS
{
public:
    /**
    * Default constructor (empty at the moment)
    */
    Matrix3x3 () XNORMAL_NOTHROW {}

    /**
    * Contruct a matrix from nine elements
    */
    explicit Matrix3x3 ( const xnSingle _11, const xnSingle _12, const xnSingle _13,
                         const xnSingle _21, const xnSingle _22, const xnSingle _23,
                         const xnSingle _31, const xnSingle _32, const xnSingle _33 ) XNORMAL_NOTHROW :  e11(_11), e12(_12), e13(_13),
                                                                                                         e21(_21), e22(_22), e23(_23),
                                                                                                         e31(_31), e32(_32), e33(_33) {}

    /**
    * Construct a matrix from the three basis vectors
    */
    explicit Matrix3x3 ( const Vector& row1, const Vector& row2, const Vector& row3 ) XNORMAL_NOTHROW : XAxis(row1), YAxis(row2), ZAxis(row3) {}

public:
    /**
    * Set the matrix to an identity matrix
    * @return A reference to myself after setting me to the identity
    */
    Matrix3x3& SetIdentity () XNORMAL_NOTHROW
    {
        e11 = e22 = e33 = 1.0f;
        e12 = e13 = e21 = e23 = e31 = e32 = 0.0f;
        return *this;
    }

public:
    /**
    * Retrieve the value of the i-th element of the matrix.
    * The matrix elements are stored in row-major order as:\n
    * 0 1 2\n
    * 3 4 5\n
    * 6 7 8\n
    *
    * @remarks An assert will be fired if you specify a index outbounds
    */
    xnSingle operator [] ( const xnUInt32 i ) const XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( i<9U );
        return reinterpret_cast<const xnSingle*>(this)[i];
    }

    /**
    * Retrieve a reference to the i-th element of the matrix.
    * The matrix elements are stored in row-major order as:\n
    * 0 1 2\n
    * 3 4 5\n
    * 6 7 8\n
    *
    * @remarks An assert will be fired if you specify a index outbounds
    */
    xnSingle& operator [] ( const xnUInt32 i ) XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( i<9U );
        return reinterpret_cast<xnSingle*>(this)[i];
    }

    /**
    * Get an element of the matrix specifying the row and colums
    * @param row [in] Row of the element to retrieve in range [1,3]
    * @param col [in] Column of the element to retrieve in range [1,3]
    * @return The element of the matrix at [row,column]
    * @remarks An assert will be fired if you specify a row or column outbounds
    */
    xnSingle Get ( const xnUInt8 row, const xnUInt8 col ) const XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( row>=1 && row<=3 );
        XNORMAL_ASSERT ( col>=1 && col<=3 );
        return reinterpret_cast<const xnSingle*>(this)[((row-1U)*3U)+(col-1U)];
    }

    /**
    * Set an element of the matrix specifying the row and colums
    * @param row [in] Row of the element to retrieve in range [0,2]
    * @param col [in] Column of the element to retrieve in range [0,2]
    * @param v [in] The value to set in this matrix at [row,column]
    * @remarks An assert will be fired if you specify a row or column outbounds
    */
    void Set ( const xnUInt8 row, const xnUInt8 col, const xnSingle v ) XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( row>=0 && row<=2 );
        XNORMAL_ASSERT ( col>=0 && col<=2 );
        reinterpret_cast<xnSingle*>(this)[(row*3U)+col] = v;
    }

    /**
    * Get a matrix row
    * @param row [in] The row number in range [0,2]
    * @return A reference to the row vector
    */
    basic_XN_vector& GetRow ( const xnUInt32 row ) XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( row>=0U && row<=2U );
        return reinterpret_cast<basic_XN_vector*>(this)[row];
    }

public:
    /**
    * Multiply the matrix by other matrix
    * @param m [in] The matrix to multiply by
    * @return A new matrix M=(*this) * m
    */
    Matrix3x3 operator * ( const Matrix3x3& m ) const XNORMAL_NOTHROW
    {
        return Matrix3x3 (
                e11*m.e11 + e12*m.e21 + e13*m.e31,
                e11*m.e12 + e12*m.e22 + e13*m.e32,
                e11*m.e13 + e12*m.e23 + e13*m.e33,

                e21*m.e11 + e22*m.e21 + e23*m.e31,
                e21*m.e12 + e22*m.e22 + e23*m.e32,
                e21*m.e13 + e22*m.e23 + e23*m.e33,

                e31*m.e11 + e32*m.e21 + e33*m.e31,
                e31*m.e12 + e32*m.e22 + e33*m.e32,
                e31*m.e13 + e32*m.e23 + e33*m.e33 );
    }

    /**
    * Multiply the matrix by other matrix
    * @param m [in] The matrix to multiply by
    * @return This matrix multiplied by the specified matrix
    */
    Matrix3x3& operator *= ( const Matrix3x3& m ) XNORMAL_NOTHROW
    {
        *this = (*this) * m;
        return *this;
    }

    /**
    * Multiply the matrix by a scalar
    * @param k [in] The scalar to multiply by
    * @return This matrix multiplied by the scalar
    */
    Matrix3x3& operator *= ( const xnSingle k ) XNORMAL_NOTHROW
    {
        e11 *= k;
        e12 *= k;
        e13 *= k;

        e21 *= k;
        e22 *= k;
        e23 *= k;

        e31 *= k;
        e32 *= k;
        e33 *= k;

        return *this;
    }

public:
    /**
    * Create a rotation matrix around an axis(like a quaterion-to-matrix)
    * @param axis [in] The rotation axis in world space and normalized
    * @param rads [in] Radians to rotate
    * @param outRes [out] A pointer to a matrix variable where to copy the 
    * result data. You can use v'=v*outRes to rotate a point around the 
    * specified axis
    */
    static void GetRotateAxis ( const Vector& axis, const xnSingle rads, Matrix3x3* outRes ) XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( outRes!=xnNULL );
	
        #if 0
            Generate a rotation matrix to rotate around a specified (normalized)
            axis
         
            For more info see:
                1) Realtime rendering, ed.AKA Peters, auth: Tomas Moller and 
                    Eric Haines, chaper 3.3 Quaterions, pag42, figure 3.30
               
                 2) http://www.cprogramming.com/tutorial/3d/rotation.html 
                    left-handed figure.
				    Notice left-handed here means v2=v*M, and right-handed v2=M*v,
                    is NOT referred to coordinate system, is REFERRED to matrix
                    concatenation.
             
            This function is specially useful to perform local rotations
        #endif	
    
	    //Local variables		
        const xnSingle	lc_flCos = cosf(rads);
        const xnSingle	lc_flSin = sinf(rads);
        const xnSingle	lc_flOneCos = 1.0f - lc_flCos;
        Vector  l_vAxis (axis);

        l_vAxis.Normalize();
            
        outRes->e11 = (lc_flOneCos*l_vAxis.x*l_vAxis.x) + lc_flCos;
        outRes->e12 = (lc_flOneCos*l_vAxis.x*l_vAxis.y) - (lc_flSin*l_vAxis.z);
        outRes->e13 = (lc_flOneCos*l_vAxis.x*l_vAxis.z) + (lc_flSin*l_vAxis.y);
					
        outRes->e21 = (lc_flOneCos*l_vAxis.x*l_vAxis.y) + (lc_flSin*l_vAxis.z);
        outRes->e22 = (lc_flOneCos*l_vAxis.y*l_vAxis.y) + lc_flCos;
        outRes->e23 = (lc_flOneCos*l_vAxis.y*l_vAxis.z) - lc_flSin*l_vAxis.x;
					
        outRes->e31 = (lc_flOneCos*l_vAxis.x*l_vAxis.z) - lc_flSin*l_vAxis.y;
        outRes->e32 = (lc_flOneCos*l_vAxis.y*l_vAxis.z) + lc_flSin*l_vAxis.x;
        outRes->e33 = (lc_flOneCos*l_vAxis.z*l_vAxis.z) + lc_flCos;
    }

    /**
    * Create a rotation matrix around the X world axis
    * @param rads [in] Radians to rotate
    * @param outRes [out] A pointer to a matrix variable where to copy the 
    * result. You can use v'=v*outRes to rotate a point around the world X axis
    */
    static void GetGlobalRotX ( const xnSingle rads, Matrix3x3* outRes ) XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( outRes!=xnNULL );

        const xnSingle lc_flSin ( sinf(rads) );
        const xnSingle lc_flCos ( cosf(rads) );

        outRes->e11 = 1.0f;
        outRes->e12 = 0.0f;
        outRes->e13 = 0.0f;

        outRes->e21 = 0.0f;
        outRes->e22 = lc_flCos;
        outRes->e23 = lc_flSin;
					   
        outRes->e31 = 0.0f;
        outRes->e32 = -lc_flSin;
        outRes->e33 = lc_flCos;
    }

    /**
    * Create a rotation matrix around the Y world axis
    * @param rads [in] Radians to rotate
    * @param outRes [out] A pointer to a matrix variable where to copy the 
    * result. You can use v'=v*outRes to rotate a point around the world Y axis
    */
    static void GetGlobalRotY ( const xnSingle rads, Matrix3x3* outRes ) XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( outRes!=xnNULL );

        const xnSingle lc_flSin ( sinf(rads) );
        const xnSingle lc_flCos ( cosf(rads) );

        outRes->e11 = lc_flCos;
        outRes->e12 = 0.0f;
        outRes->e13 = -lc_flSin;

        outRes->e21 = 0.0f;
        outRes->e22 = 1.0f;
        outRes->e23 = 0.0f;

        outRes->e31 = lc_flSin;
        outRes->e32 = 0.0f;
        outRes->e33 = lc_flCos;
    }

public:
    /**
    * Converts Euler angles to a 3x3 matrix
    * @param yawHeading [in] Yaw-heading angle in radians
    * @param pitchAttitude [in] Pitch-attitude angle in radians
    * @param rollBank [in] Roll-bank angle in radians
    * @param outRes [out] A pointer to a matrix variable where to copy the
    * result. 
    */
    static void MatrixFromEuler ( const xnSingle yawHeading, 
        const xnSingle pitchAttitude, const xnSingle rollBank, 
        Matrix3x3* outRes ) XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( outRes!=xnNULL );

        // Assuming the angles are in radians.
        //http://www.euclideanspace.com/maths/geometry/rotations/conversions/
        //eulerToMatrix/index.htm
        //http://www.euclideanspace.com/maths/geometry/rotations/euler/

        /*
         [m00 m01 m02]
         [m10 m11 m12] * v ->Right handed
         [m20 m21 m22]
         m00 = ch * ca;
         m01 = sh*sb - ch*sa*cb;
         m02 = ch*sa*sb + sh*cb;
         m10 = sa;
         m11 = ca*cb;
         m12 = -ca*sb;
         m20 = -sh*ca;
         m21 = sh*sa*cb + ch*sb;
         m22 = -sh*sa*sb + ch*cb;
	*/

        const xnSingle ca = cosf(pitchAttitude);
        const xnSingle sa = sinf(pitchAttitude);
        const xnSingle ch = cosf(yawHeading);
        const xnSingle sh = sinf(yawHeading);
        const xnSingle cb= cosf(rollBank);
        const xnSingle sb = sinf(rollBank);

        outRes->e11 = ch * ca;
        outRes->e12 = sh*sb - ch*sa*cb;
        outRes->e13 = ch*sa*sb + sh*cb;

        outRes->e21 = sa;
        outRes->e22 = ca*cb;
        outRes->e23 = -ca*sb;

        outRes->e31 = -sh*ca;
        outRes->e32 = sh*sa*cb + ch*sb;
        outRes->e33 = -sh*sa*sb + ch*cb;
    }

    /**
    * Pass this matrix to euler angles
    * @param yawHeading [in] Yaw-heading angle in radians
    * @param pitchAttitude [in] Pitch-attitude angle in radians
    * @param rollBank [in] Roll-bank angle in radians
    */
    void ToEuler ( xnSingle& yawHeading, xnSingle& pitchAttitude, xnSingle& rollBank ) const XNORMAL_NOTHROW
    {
        //see http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToEuler/

        // Assuming the angles are in radians.
        if (e21 > 0.998f)
        {
            // singularity at north pole
            yawHeading = atan2f(e13,e33);
            pitchAttitude = 3.141592f * 0.5f;
            rollBank = 0.0f;
        }
        else
        {
            if (e21 < -0.998f)
            {
                // singularity at south pole
                yawHeading = atan2f(e13, e33);
                pitchAttitude = -3.141592f * 0.5f;
                rollBank = 0.0f;
            }
            else
            {
                yawHeading = atan2f(-e31, e11);
                pitchAttitude = asinf(e21);
                rollBank = atan2f(-e23, e22);
            }
        }
    }

public:
    /**
    * Sets the matrix to a look-at-point rotation matrix
    * @param pos [in] The position of the eye
    * @param lookAtPoint [in] The position where you want to look
    * @param outRes [out] A pointer to a matrix variable where to copy the
    * result. 
    *
    * @param upVector [in] An optional "up" vector required in the maths 
    * involved
    *
    * @return A new matrix aiming at the specified point
    *
    * @remarks This will use the lookAt vector as OpenGL, not like Direct3D
    */
    static void LookAt ( const Vector& pos, const Vector& lookAtPoint, 
        Matrix3x3* outRes, const Vector& upVector ) XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( outRes!=xnNULL );

        const Vector lc_vZa ( (pos-lookAtPoint).Normalize() );
        Vector l_vXa, l_vYa, l_vUp(upVector);
		
        Vector::Cross(l_vUp.Normalize(),lc_vZa, &l_vXa);
        Vector::Cross(lc_vZa,l_vXa.Normalize(), &l_vYa);
	
        outRes->XAxis = l_vXa;
        outRes->YAxis = l_vYa.Normalize();
        outRes->ZAxis = lc_vZa;	
    }

public:
    /**
    * Transforms a point using a matrix
    * @param tm [in] The matrix to use
    * @param v [in] The point to transform
    * @param outRes [out] A pointer to a vector variable where to copy the
    * result (v'=M*v)
    */
    static void Transform(const Matrix3x3& tm,const Vector& v, Vector* outRes) XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( outRes!=xnNULL );

        Vector l_vT;
        l_vT.x = tm.e11*v.x + tm.e12*v.y + tm.e13*v.z;
        l_vT.y = tm.e21*v.x + tm.e22*v.y + tm.e23*v.z;
        l_vT.z = tm.e31*v.x + tm.e32*v.y + tm.e33*v.z;
        *outRes = l_vT;
    }

    /**
    * Transform a specified vector by a matrix as v'=v*tm
    * @param v [in] The vector to transform
    * @param tm [in] The matrix to apply
    * @param outRes [out] A pointer to a vector variable where to copy the result (v*tm)
    */
    static void TransformVector ( const basic_XN_vector& v, const Matrix3x3& tm, basic_XN_vector* outRes ) XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( outRes!=xnNULL );

        Vector l_vT;
        l_vT.x = v.x*tm.e11 + v.y*tm.e21 + v.z*tm.e31;
        l_vT.y = v.x*tm.e12 + v.y*tm.e22 + v.z*tm.e32;
        l_vT.z = v.x*tm.e13 + v.y*tm.e23 + v.z*tm.e33;
        *outRes = l_vT;
    }


public:
    /**
    * Transposes ( swap rows by columns ) the specified matrix
    * @param m [in] The matrix to transpose
    * @param outRes [out] A pointer to a matrix variable where to copy the 
    * result (outRes=transpose(M))
    */
    static void Transpose ( const Matrix3x3& m, Matrix3x3* outRes ) XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( outRes!=xnNULL );

        Matrix3x3 l_mT;

        l_mT.e11 = m.e11;
        l_mT.e12 = m.e21;
        l_mT.e13 = m.e31;

        l_mT.e21 = m.e12;
        l_mT.e22 = m.e22;
        l_mT.e23 = m.e32;
	
        l_mT.e31 = m.e13;
        l_mT.e32 = m.e23;
        l_mT.e33 = m.e33;

        *outRes = l_mT;
    }

public:
    /**
    * Calculates the parity of the matrix
    * See http://mathworld.wolfram.com/ParityCheckMatrix.html for more info
    * @return FALSE if all is OK. TRUE if any axis is scaled negatively so 
    * breaks the left/right-handed orthogonality
    */
    bool Parity () const XNORMAL_NOTHROW
    {
        Vector cp;
        Vector::Cross(this->YAxis,this->ZAxis,&cp);
        if ( cp.Dot(this->XAxis)<-0.0f )
        {
            return true;
        }
        return false;
    }

public:
    /**
    * Get an orthogonal basis based on a vector
    * @param n [in] A vector, normalized(unit length).
    * @param outRes [out] A pointer where to store the output orthogonal 
    * basis(orthogonalized too).
    * This basis will be X-aligned with the normal vector as:
    *
    *      outRes.Za
    *       ^     
    *       |     outRes.Ya
    *       |   / 
    *       | / 
    *       *--------> outRes.Xa == n
    *    
    * right handed, OpenGL's coordinate system style.
    */
    static void GetOrthogonalBasisXAlign( const Vector& n, Matrix3x3* outRes )
    {
        XNORMAL_ASSERT ( n.Length()>=0.99f && n.Length()<=1.01f );
        XNORMAL_ASSERT ( outRes!=xnNULL );
        #if 0
            const xnUInt32 lc_uiMinc(n.GetMinAbsComponent());

            outRes->XAxis = n;

            switch(lc_uiMinc)
            {
            default:
            case 0:
                Vector::Cross(n,Vector(1.0f,0.0f,0.0f),&outRes->YAxis);
            break;
            case 1:
                Vector::Cross(n,Vector(0.0f,1.0f,0.0f),&outRes->YAxis);
            break;
            case 2:
                Vector::Cross(n,Vector(0.0f,0.0f,1.0f),&outRes->YAxis);
            break;
            };

            Vector::Cross(outRes->XAxis,outRes->YAxis,&outRes->ZAxis);
            Vector::Normalize(outRes->ZAxis,&outRes->ZAxis);
        #else
            //From http://mathworld.wolfram.com/SphericalCoordinates.html
            //, thee the theta and phi tangent VECTORS! Notice they use a Z up coordsys though.
            //That provide no flips for 100% perfect sphere trackball! 
            //Not like those crappy vector cross products!
            Vector r(n.z,n.x,n.y);
            const xnSingle theta ( atan2f(r.y,r.x) );//caution: VS intellisense says  (_X,_Y) but it's (_Y,_X) really as the docs says!
            const xnSingle phi ( acosf(r.z) );
            const xnSingle lc_flSinTheta(sinf(theta));
            const xnSingle lc_flCosTheta(cosf(theta));
            const xnSingle lc_flSinPhi(sinf(phi));
            const xnSingle lc_flCosPhi(cosf(phi));

            //x axis will be the "r" vector on the Wolfram's image... our normal
            outRes->XAxis = n;
            Vector::Normalize(outRes->XAxis,&outRes->XAxis);

            //y axis will be the phi "vector" on the Wolfram's image
            outRes->YAxis.z = lc_flCosTheta*lc_flCosPhi;
            outRes->YAxis.x = lc_flSinTheta*lc_flCosPhi;
            outRes->YAxis.y = -lc_flSinPhi;
            //Vector::Normalize(outRes->YAxis,&outRes->YAxis);

            //z axis will be the theta "vector" on the Wolfram's image.. and our tangent vector
            outRes->ZAxis.z = -lc_flSinTheta;
            outRes->ZAxis.x = lc_flCosTheta;
            outRes->ZAxis.y = 0.0f;
            //Vector::Normalize(outRes->ZAxis,&outRes->ZAxis);
        #endif
    }

public:
    void ToString ( IString& outRes ) const XNORMAL_NOTHROW
    {
        char c[255];
        const int n ( sprintf ( c, "%4.3f;%4.3f;%4.3f;%4.3f;%4.3f;%4.3f;%4.3f;%4.3f;%4.3f", e11, e12, e13, e21, e22, e23, e31, e32, e33 ) );
        if ( 9==n )
        {
            outRes.Set ( const_cast<const char*>(c) );
        }
        else
        {
            XNORMAL_ASSERT ( false );
            outRes.Clear();
        }
    }

public:
    union
    {
        struct
        {
            xnSingle e11, e12, e13;
            xnSingle e21, e22, e23;
            xnSingle e31, e32, e33;
        };

        struct
        {
            xnSingle m00, m01, m02;
            xnSingle m10, m11, m12;
            xnSingle m20, m21, m22;
        };

        struct
        {
            basic_XN_vector XAxis, YAxis, ZAxis; 
        };
    };
};

