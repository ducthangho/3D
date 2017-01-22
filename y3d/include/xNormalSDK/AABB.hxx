#if 0
    This file is part of the xNormal 3 SDK, formatted for Doxygen/JavaDoc 
    documentation system. 
    (c) 2005-2015, S.Orgaz.
    Contact: granthill76 'at' yahoo.com
    Visit http://www.xnormal.net for more info
 
    FOR MORE INFO ABOUT THE LICENSE, CONSULT THE XNORMAL PDF DOCUMENTATION
#endif //0 comment


/**
* An axis-aligned bounding box ( aka AABB )
* For more info about AABBs see http://en.wikipedia.org/wiki/AABB
*/
class AABB XNORMAL_FINALCLASS
{
public:
    /**
    * Default constructor
    * @remarks This will initialize an infinite AABB
    */
    AABB()
    {
        this->MakeInfinite();
        m_bBeginUpdateOrUnion = false;
    }

    /**
    * Construct from pmin and pmax points
    * @param PMin [in] The min vector
    * @param PMax [in] The max vector
    */
    explicit AABB ( const Vector& PMin, const Vector& PMax )
    {
        this->SetMinMax(PMin,PMax);
        m_bBeginUpdateOrUnion = false;
    }

    /**
    * Copy constructor
    * @param aabb An AABB where to copy data from
    */
    AABB ( const AABB& aabb ) : m_vPMin(aabb.m_vPMin), m_vPMax(aabb.m_vPMax)
    {
        m_bBeginUpdateOrUnion = false;
    }

public:
    /**
    * Make this AABB infinite
    */
    void MakeInfinite () XNORMAL_NOTHROW
    {
    	m_vPMin = -FLT_MAX;
	    m_vPMax = FLT_MAX;
        m_bBeginUpdateOrUnion = false;
    }

    /**
    * Make this AABB empty
    */
    void MakeEmpty () XNORMAL_NOTHROW
    {
    	m_vPMin = m_vPMax = 0.0f;
        m_bBeginUpdateOrUnion = false;
    }

    /**
    * Test if this AABB if not infinite and is valid
    */
    bool IsValidAndNonInfinite() const XNORMAL_NOTHROW
    {
        xnUInt32 valid(0U);

        for ( xnUInt32 i=0U; i<3U; ++i )
        {
            if ( Utils::IsFinite(m_vPMin[i]) && fabsf(m_vPMin[i])!=FLT_MAX )
            {
                ++valid;
            }
	    
            if ( Utils::IsFinite(m_vPMax[i]) && fabsf(m_vPMax[i])!=FLT_MAX )
            {
                ++valid;
            }
        }

        return ( 6U==valid && m_vPMin.x<=m_vPMax.x && m_vPMin.y<=m_vPMax.y && m_vPMin.z<=m_vPMax.z );
    }

public:
    /**
    * Get the volume ( in world units ^ 3 ) of the AABB
    */
    xnSingle GetVolume () const XNORMAL_NOTHROW
    {
        return this->GetWidth() * this->GetHeight() * this->GetDepth();
    }

    /**
    * Get the area ( in world units ^ 2 ) of the AABB
    */
    xnSingle GetArea() const XNORMAL_NOTHROW
    {
        //From http://id.mind.net/~zona/mmts/geometrySection/surfaceAreasAndVolumes//areaVolumeBox1.html
        xnSingle area ( 2.0f * this->GetHeight() * this->GetWidth() );
        area += 2.0f * this->GetHeight() * this->GetDepth();
        area += 2.0f * this->GetWidth() * this->GetDepth();
        return area;
    }

public:
    /**
    * Get the width of this AABB ( in world units )
    */
    xnSingle GetWidth() const XNORMAL_NOTHROW
    {
        return m_vPMax.x - m_vPMin.x;
    }

    /**
    * Get the height of this AABB ( in world units )
    */
    xnSingle GetHeight() const XNORMAL_NOTHROW
    {
        return m_vPMax.y - m_vPMin.y;
    }

    /**
    * Get the depth of this AABB ( in world units )
    */
    xnSingle GetDepth() const XNORMAL_NOTHROW
    {
        return m_vPMax.z - m_vPMin.z;
    }

    /**
    * Get the bbox size in one axis
    * @param axis [in] Use 0 for X axis, 1 for Y axis or 2 for Z axis
    */
    xnSingle GetSize(const xnUInt32 axis) const XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( axis<3U );
        return m_vPMax[axis]-m_vPMin[axis];
    }

public:
    /**
    * Begin update ( prepare AABB to be updated/unioned )
    */
    void BeginUpdateOrUnion() XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( false==m_bBeginUpdateOrUnion ); // You forgot to call EndUpdateOrUnion() in a previous call!

        m_vPMin = FLT_MAX;
        m_vPMax = -FLT_MAX;
        m_bBeginUpdateOrUnion = true;
    }

    /**
    * Update AABB limits based on a point ( will test if the point is less 
    * than the pmin or greater than pmax, updating bounding box limits only
    * if required
    * @param p [in] A point to add to this AABB
    * @return A reference to this AABB
    * @remarks You should call ResetUpdate() first... unless the AABB has been
    * updated before...
    */
    AABB& Update ( const Vector& p ) XNORMAL_NOTHROW
    {
        const register __m128 pp(_mm_set_ps(0.0f,p.z,p.y,p.x));
        const register __m128 pmin(_mm_set_ps(0.0f,m_vPMin.z,m_vPMin.y,m_vPMin.x));
        const register __m128 pmax(_mm_set_ps(0.0f,m_vPMax.z,m_vPMax.y,m_vPMax.x));
        const register __m128 pminDef(_mm_min_ps(pp,pmin));
        const register __m128 pmaxDef(_mm_max_ps(pp,pmax));
        m_vPMin.Set((float*)&pminDef);
        m_vPMax.Set((float*)&pmaxDef);
        return *this;
    }

    /**
    * Expand this AABB joining other AABB
    * @param aabb [in] Other AABB to mix with this AABB
    * @return A reference to this AABB
    * @remarks You should call ResetUpdate() first... unless the AABB has been
    * updated before...
    */
    AABB& Union ( const AABB& aabb ) XNORMAL_NOTHROW
    {
        const register __m128 pminA(_mm_set_ps(0.0f,aabb.m_vPMin.z,aabb.m_vPMin.y,aabb.m_vPMin.x));
        const register __m128 pmaxA(_mm_set_ps(0.0f,aabb.m_vPMax.z,aabb.m_vPMax.y,aabb.m_vPMax.x));
        const register __m128 pmin(_mm_set_ps(0.0f,m_vPMin.z,m_vPMin.y,m_vPMin.x));
        const register __m128 pmax(_mm_set_ps(0.0f,m_vPMax.z,m_vPMax.y,m_vPMax.x));
        const register __m128 pminDef(_mm_min_ps(pmin,pminA));
        const register __m128 pmaxDef(_mm_max_ps(pmax,pmaxA));
        m_vPMin.Set((float*)&pminDef);
        m_vPMax.Set((float*)&pmaxDef);
        return *this;
    }

    /**
    * End update or union
    * @remarks If no data were added using Update() or Union() then an empty
    * AABB will be formed...
    */
    void EndUpdateOrUnion() XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( true==m_bBeginUpdateOrUnion ); // Call BeginUpdateOrUnion() first!;
    
        if ( FLT_MAX==m_vPMin.x  || FLT_MAX==m_vPMin.y  || FLT_MAX==m_vPMin.z ||
             -FLT_MAX==m_vPMax.x || -FLT_MAX==m_vPMax.y || -FLT_MAX==m_vPMax.z )
        {
            this->SetMinMax(Vector(0.0f),Vector(0.0f));
        }
        else
        {
            m_vCenter = (m_vPMin+m_vPMax)*0.5f;
            m_vHalfSize = (m_vPMax-m_vPMin)*0.5f;
        }

        m_bBeginUpdateOrUnion = false;        
    }

public:
    /**
    * Test if a point is inside this AABB
    * @param v [in] The point to test
    * @return TRUE if the point is inside this AABB or FALSE if not
    */
    bool IsPointInside ( const Vector& v ) const XNORMAL_NOTHROW
    {
        const register __m128 vv(_mm_set_ps(0.0f,v.z,v.y,v.x));
        const register __m128 mi(_mm_set_ps(0.0f,m_vPMin.z,m_vPMin.y,m_vPMin.x));
        const register __m128 ma(_mm_set_ps(0.0f,m_vPMax.z,m_vPMax.y,m_vPMax.x));
        const register __m128 mask1(_mm_cmpge_ps(vv,mi));
        const register __m128 mask2(_mm_cmple_ps(vv,ma));
        return (15==_mm_movemask_ps(_mm_and_ps(mask1,mask2)));
    }

public:
    /**
    * Get the i-th point
    * @param ith [in] The point to retrieve ( 0==pmin, 1==pmax )
    * @return A reference to the i-th point
    * @todo In the future, modify it in a way the user could access the 
    * 8 AABB points
    */
    const Vector& GetPoint ( const xnUInt32 ith ) const XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( ith<2U );
        return reinterpret_cast<const Vector*>(this)[ith];
    }

    /**
    * Get the min point
    * @return A vector with the min point
    */
    const Vector& GetPMin() const XNORMAL_NOTHROW
    {
        return m_vPMin;
    }

    /**
    * Get the max point
    * @return A vector with the max point
    */
    const Vector& GetPMax() const XNORMAL_NOTHROW
    {
        return m_vPMax;
    }

    /**
    * Get the AABB center point
    * @return The AABB's center point
    */
    const Vector& GetCenter() const XNORMAL_NOTHROW
    {
        return m_vCenter;
    }

    /**
    * Set the min/max points
    * @param pmin [in] The min point
    * @param pmax [in] The max point
    * @remarks And assert will be fired if there are not correct ( 
    *   all the pmax components must be greater or equal to the pmin ones )
    */
    void SetMinMax ( const Vector& pmin, const Vector& pmax ) XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( pmin.x<=pmax.x );
        XNORMAL_ASSERT ( pmin.y<=pmax.y );
        XNORMAL_ASSERT ( pmin.z<=pmax.z );

        m_vPMin = pmin;
        m_vPMax = pmax;

        m_vCenter = (pmin+pmax)*0.5f;
        m_vHalfSize = (pmax-pmin)*0.5f;

        m_bBeginUpdateOrUnion = false;
    }

protected:
    /**
    * Min point of the AABB
    * PMin(x,y,z) should satisfy that PMin.x<=PMax.x, PMin.y<=PMax.y, 
    * PMin.z<=PMax.z 
    */
    Vector m_vPMin;

    /**
    * Max point of the AABB
    * PMax(x,y,z) should that PMax.x>=PMin.x, PMax.y>=PMin.y, PMax.z>=PMin.z
    */
    Vector m_vPMax;

    /**
    * Center point
    */
    Vector m_vCenter;

    /**
    * Half size of the box(used in the "triangle in aabb" test).
    */
    Vector m_vHalfSize;

    /**
    * Property to indicate if the proper BeginUpdateOrUnion() or
    * EndUpdateOrUnion() pair is ok
    */
    bool m_bBeginUpdateOrUnion;

private:
    static bool planeBoxOverlap ( const Vector& normal, const xnSingle d, const Vector& maxbox )
    {
        register xnUInt32 q;
        Vector vmin, vmax;

        for ( q=0; q<3; ++q )
        {
            if ( normal[q]>0.0f )
            {
                vmin[q] = -maxbox[q];
                vmax[q] = maxbox[q];
            }
            else
            {
                vmin[q] = maxbox[q];
                vmax[q] = -maxbox[q];
            }
        }

        if ( normal.Dot(vmin)+d>0.0f )
        {
            return 0;
        }

        if ( normal.Dot(vmax)+d>=0.0f )
        {
            return true;
        }

        return false;
    }
};

