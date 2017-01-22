#if 0
    This file is part of the xNormal 3 SDK, formatted for Doxygen/JavaDoc 
    documentation system. 
    (c) 2005-2015, S.Orgaz.
    Contact: granthill76 'at' yahoo.com
    Visit http://www.xnormal.net for more info
 
    FOR MORE INFO ABOUT THE LICENSE, CONSULT THE XNORMAL PDF DOCUMENTATION
#endif //0 comment



/**
* A polygonal mesh ( it can contain triangular and quad faces )
*/
class IPolyMesh
{
public:
    virtual void Release() XNORMAL_NOTHROW = 0;

public:
    /**
    * Set the polyMesh's name
    * @param name [in] A pointer to an UNICODE string. Should not be NULL usually.
    * return TRUE if the name was set of FALSE if not ( for instance, due to an out of memory )
    */
    virtual bool SetName ( const wchar_t *name ) XNORMAL_NOTHROW = 0;

    /**
    * Get the polyMesh's name
    * @return An UNICODE string
    */
    virtual const wchar_t* GetName() const XNORMAL_NOTHROW = 0;

    /**
    * Geometry where is inserted the mesh or NULL if the mesh is NOT inserted into any geometry
    */
    virtual const IGeometry* GetGeometryConst() const XNORMAL_NOTHROW = 0;
    virtual IGeometry* GetGeometry() XNORMAL_NOTHROW = 0;

public:
    /**
    * Get the number of point vertices 
    * @return The total number of vertex points in the mesh
    */
    virtual xnUInt32 GetNumPoints () const XNORMAL_NOTHROW = 0;
    
    /**
    * Get the position of a vertex
    * For example, to iterate over all the polyMesh's vertex positions you
    * could do:
    * \code
    * xnUInt32 i;
    * Vector pos;
    * for ( i=0; i<this->GetNumPoints(); ++i )
    * {
    *    pos = this->GetVertex(i);
    * }
    * \endcode
    *
    * @param ith [in] The index of the vertex. This should be in range 
    * [0,this->GetNumPoints()-1]
    * @return A const reference to the vertex point
    */
    const Vector& GetPoint ( const xnUInt32 ith ) const XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( ith<this->GetNumPoints() );
        return this->GetPointP ( ith );
    }

    virtual const Vector* GetPointsPtrConst() const XNORMAL_NOTHROW = 0;
    virtual Vector* GetPointsPtr() XNORMAL_NOTHROW = 0;

    /**
    * Set vertex point value
    *
    * @param ith [in] The index of the vertex. This should be in range
    * [0,this->GetNumPoints()-1]
    * @param v [in] The value to set
    * @remarks All the semantics/vertex channels need a 3-float result... 
    * but the UVs(2 floats .xy) and the per-vertex AO(1 float,.x).
    */
    void SetPoint ( const xnUInt32 ith, const Vector& v ) XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( ith<this->GetNumPoints() );
        this->SetPointP ( ith, v );
    }

    /*
    * Adds a vertex point to the vertex array
    *
    * @return TRUE if the point was added or FALSE if not ( usually due to an out of memory ).
    *
    * @remarks This will grow a few the internal reserve buffer. Usually it's a 
    * good thing to call ReserveVertices() before this or , even better, if you 
    * know the exact number of faces then use the ResizeVertices()/SetVertex() 
    * combo. It works like the STL's vector.
    */
    virtual bool AddPoint ( const Vector& v ) XNORMAL_NOTHROW = 0;

    /**
    * Clear all the vertices points
    */
    virtual void ClearPoints () XNORMAL_NOTHROW = 0;

    /**
    * Reserve(for adding) certain number of points ( without destroying
    * the current vertices )
    *
    * @param nVerts [in] The number of vertices to reserve
    *
    * @return TRUE if we could reserve the points or FALSE if not ( usually due to an out of memory ).
    *
    * @remarks Each time you call AddPoint() and the internal reserve is full,
    * xNormal will grow the internal buffer and it will re-allocate the stored 
    * vertices.
    * For this motive, it's important that you set a memory reserve before 
    * adding vertices with the AddVertex() method... or the memory used can
    * grow a lot.
    * Of course, if you know the exact # of vertices better use the 
    * ResizePoints()/SetPoint() combo. It works like the STL's vector
    * container.
    */
    virtual bool ReservePoints ( const xnUInt32 nVerts ) XNORMAL_NOTHROW = 0;

    /**
    * Allocate certain number of vertices ( destroying the previous buffer ).
    *
    * @param nVerts [in] The number of vertices to preallocate
    *
    * @return TRUE if we could reserve the points or FALSE if not ( usually due to an out of memory ).
    *
    * @remarks The difference between ReservePoints() and this method is that
    *   the ReservePoints() does not modify the current # of vertices while
    *   ResizePoints() does. The ReservePoints() is good when you don't
    *   know the exact vertex count... so you need to use the AddPoint(). 
    *   If you know the exact vertex count then better use ResizePoints()
    *   /SetPoint(). It works like the STL's vector container.
    */
    virtual bool ResizePoints ( const xnUInt32 nVerts ) XNORMAL_NOTHROW = 0;
    
    /**
    * Copy points to other polymesh and clear them from this mesh
    * @param pmsh [out] The destination mesh where to copy the vertices to
    * @return TRUE if we could reserve the points or FALSE if not ( usually due to an out of memory ).
    * @remarks This mesh's points will be cleared after you call this.
    */
    bool CopyPointsTo ( IPolyMesh& pmsh ) XNORMAL_NOTHROW
    {
        if ( 0U==this->GetNumPoints() )
        {
            pmsh.ClearPoints();
            return true;
        }

        if ( false==pmsh.ResizePoints(this->GetNumPoints()) )
        {
            return false;
        }
        //copy data. We'll use dangerous "memcpy" here because we really NEED speed.
        memcpy ( reinterpret_cast<void*>(pmsh.GetPointsPtr()), reinterpret_cast<const void*>(this->GetPointsPtr()), sizeof(Vector)*static_cast<size_t>(this->GetNumPoints()) ); 
        return true;
    }

public:
    virtual xnUInt32 GetNumNormals () const XNORMAL_NOTHROW = 0;
    
    const Vector& GetNormal ( const xnUInt32 ith ) const XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( ith<this->GetNumNormals() );
        return this->GetNormalP ( ith );
    }

    virtual const Vector* GetNormalsPtrConst() const XNORMAL_NOTHROW = 0;
    virtual Vector* GetNormalsPtr() XNORMAL_NOTHROW = 0;

    void SetNormal ( const xnUInt32 ith, const Vector& v )
    {
        XNORMAL_ASSERT ( ith<this->GetNumNormals() );
        this->SetNormalP ( ith, v );
    }

    virtual bool AddNormal ( const Vector& v ) XNORMAL_NOTHROW = 0;

    virtual void ClearNormals () XNORMAL_NOTHROW = 0;
    
    virtual bool ReserveNormals ( const xnUInt32 nVerts ) XNORMAL_NOTHROW = 0;

    virtual bool ResizeNormals ( const xnUInt32 nVerts ) XNORMAL_NOTHROW = 0;
    
    bool CopyNormalsTo ( IPolyMesh& pmsh ) XNORMAL_NOTHROW
    {
        if ( 0U==this->GetNumNormals() )
        {
            pmsh.ClearNormals();
            return true;
        }

        if ( false==pmsh.ResizeNormals(this->GetNumNormals()) )
        {
            return false;
        }
        //copy data. We'll use dangerous "memcpy" here because we really NEED speed.
        memcpy ( reinterpret_cast<void*>(pmsh.GetNormalsPtr()), reinterpret_cast<const void*>(this->GetNormalsPtr()), sizeof(Vector)*static_cast<size_t>(this->GetNumNormals()) ); 
        return true;
    }

public:
    virtual xnUInt32 GetNumBiNormals () const XNORMAL_NOTHROW = 0;
    
    const Vector& GetBiNormal ( const xnUInt32 ith ) const XNORMAL_NOTHROW 
    {
        XNORMAL_ASSERT ( ith<this->GetNumBiNormals() );
        return this->GetBiNormalP(ith);
    }

    virtual const Vector* GetBiNormalsPtrConst() const XNORMAL_NOTHROW = 0;
    virtual Vector* GetBiNormalsPtr() XNORMAL_NOTHROW = 0;

    void SetBiNormal ( const xnUInt32 ith, const Vector& v ) XNORMAL_NOTHROW 
    {
        XNORMAL_ASSERT ( ith<this->GetNumBiNormals() );
        this->SetBiNormalP ( ith, v );        
    }

    virtual bool AddBiNormal ( const Vector& v ) XNORMAL_NOTHROW = 0;
    
    virtual void ClearBiNormals () XNORMAL_NOTHROW = 0;

    virtual bool ReserveBiNormals ( const xnUInt32 nVerts ) XNORMAL_NOTHROW = 0;

    virtual bool ResizeBiNormals ( const xnUInt32 nVerts ) XNORMAL_NOTHROW = 0;
    
    bool CopyBiNormalsTo ( IPolyMesh& pmsh ) XNORMAL_NOTHROW
    {
        if ( 0U==this->GetNumBiNormals() )
        {
            pmsh.ClearBiNormals();
            return true;
        }

        if ( false==pmsh.ResizeBiNormals(this->GetNumBiNormals()) )
        {
            return false;
        }
        //copy data. We'll use dangerous "memcpy" here because we really NEED speed.
        memcpy ( reinterpret_cast<void*>(pmsh.GetBiNormalsPtr()), reinterpret_cast<const void*>(this->GetBiNormalsPtr()), sizeof(Vector)*static_cast<size_t>(this->GetNumBiNormals()) ); 
        return true;
    }

public:
    virtual xnUInt32 GetNumTangents () const XNORMAL_NOTHROW = 0;
    
    const Vector& GetTangent ( const xnUInt32 ith ) const XNORMAL_NOTHROW  
    {
        XNORMAL_ASSERT ( ith<this->GetNumTangents() );
        return this->GetTangentP(ith);
    }

    virtual const Vector* GetTangentsPtrConst() const XNORMAL_NOTHROW = 0;
    virtual Vector* GetTangentsPtr() XNORMAL_NOTHROW = 0;
    
    void SetTangent ( const xnUInt32 ith, const Vector& v ) XNORMAL_NOTHROW 
    {
        XNORMAL_ASSERT ( ith<this->GetNumTangents() );
        this->SetTangentP ( ith, v );        
    }

    virtual bool AddTangent ( const Vector& v ) XNORMAL_NOTHROW  = 0;
    
    virtual void ClearTangents () XNORMAL_NOTHROW  = 0;
    
    virtual bool ReserveTangents ( const xnUInt32 nVerts ) XNORMAL_NOTHROW  = 0;
    
    virtual bool ResizeTangents ( const xnUInt32 nVerts ) XNORMAL_NOTHROW = 0;
    
    bool CopyTangentsTo ( IPolyMesh& pmsh ) XNORMAL_NOTHROW
    {
        if ( 0U==this->GetNumTangents() )
        {
            pmsh.ClearTangents();
            return true;
        }

        if ( false==pmsh.ResizeTangents(this->GetNumTangents()) )
        {
            return false;
        }
        //copy data. We'll use dangerous "memcpy" here because we really NEED speed.
        memcpy ( reinterpret_cast<void*>(pmsh.GetTangentsPtr()), reinterpret_cast<const void*>(this->GetTangentsPtr()), sizeof(Vector)*static_cast<size_t>(this->GetNumTangents()) ); 
        return true;
    }

public:
    virtual xnUInt32 GetNumUVs () const XNORMAL_NOTHROW = 0;
    
    const Vector2& GetUV ( const xnUInt32 ith ) const XNORMAL_NOTHROW 
    {
        XNORMAL_ASSERT ( ith<this->GetNumUVs() );
        return this->GetUVP(ith);
    }

    virtual const Vector2* GetUVsPtrConst() const XNORMAL_NOTHROW = 0;
    virtual Vector2* GetUVsPtr() XNORMAL_NOTHROW = 0;
    
    void SetUV ( const xnUInt32 ith, const Vector2& uv ) XNORMAL_NOTHROW 
    {
        XNORMAL_ASSERT ( ith<this->GetNumUVs() );
        this->SetUVP ( ith, uv );
    }

    virtual bool AddUV ( const Vector2& uv ) XNORMAL_NOTHROW = 0;
    
    virtual void ClearUVs () XNORMAL_NOTHROW = 0;
    
    virtual bool ReserveUVs ( const xnUInt32 nVerts ) XNORMAL_NOTHROW = 0;
    
    virtual bool ResizeUVs ( const xnUInt32 nVerts ) XNORMAL_NOTHROW = 0;
    
    bool CopyUVsTo ( IPolyMesh& pmsh ) XNORMAL_NOTHROW
    {
        if ( 0U==this->GetNumUVs() )
        {
            pmsh.ClearUVs();
            return true;
        }

        if ( false==pmsh.ResizeUVs(this->GetNumUVs()) )
        {
            return false;
        }
        //copy data. We'll use dangerous "memcpy" here because we really NEED speed.
        memcpy ( reinterpret_cast<void*>(pmsh.GetUVsPtr()), reinterpret_cast<const void*>(this->GetUVsPtr()), sizeof(Vector2)*static_cast<size_t>(this->GetNumUVs()) ); 
        return true;
    }

public:
    virtual xnUInt32 GetNumVColors () const XNORMAL_NOTHROW = 0;
    
    /**
    * Get a vertex color
    * @param ith [in] The vertex index in range [0,this->GetNumVColors()-1]
    * @return The vertex color as 0xAARRGGBB. You can use the Color class to 
    * convert to floating point values using the Color constructor with
    * a xnUInt32 parameter.
    */
    xnUInt32 GetVColor ( const xnUInt32 ith ) const XNORMAL_NOTHROW 
    {
        XNORMAL_ASSERT ( ith<this->GetNumVColors() );
        return this->GetVColorP(ith);
    }

    virtual const xnUInt32* GetVColorsPtrConst() const XNORMAL_NOTHROW = 0;
    virtual xnUInt32* GetVColorsPtr() XNORMAL_NOTHROW = 0;

    /**
    * Set a vertex color
    * @param ith [in] The vertex index in range [0,this->GetNumVColors()-1]
    * @param vcol [in] The vertex color as 0xAARRGGBB. You can use the 
    * Color class to convert from floating point values using the 
    * Color::ToARGB() method
    */
    void SetVColor ( const xnUInt32 ith, const xnUInt32 vcol ) XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( ith<this->GetNumVColors() );
        this->SetVColorP ( ith, vcol );
    }
    
    virtual bool AddVColor ( const xnUInt32& vcol ) XNORMAL_NOTHROW = 0;
    
    virtual void ClearVColors () XNORMAL_NOTHROW = 0;
    
    virtual bool ReserveVColors ( const xnUInt32 nVerts ) XNORMAL_NOTHROW = 0;
    
    virtual bool ResizeVColors ( const xnUInt32 nVerts ) XNORMAL_NOTHROW = 0;
    
    bool CopyVColorsTo ( IPolyMesh& pmsh ) XNORMAL_NOTHROW
    {
        if ( 0U==this->GetNumVColors() )
        {
            pmsh.ClearVColors();
            return true;
        }

        if ( false==pmsh.ResizeVColors(this->GetNumVColors()) )
        {
            return false;
        }
        //copy data. We'll use dangerous "memcpy" here because we really NEED speed.
        memcpy ( reinterpret_cast<void*>(pmsh.GetVColorsPtr()), reinterpret_cast<const void*>(this->GetVColorsPtr()), sizeof(xnUInt32)*static_cast<size_t>(this->GetNumVColors()) ); 
        return true;
    }

public:
    /**
    * Mesh face structure to support triangles and quads
    * xNormal uses CCW-defined faces like:
    * \code
    *       // 0----3
    *       // |    |
    *       // |    |
    *       // 1----2
    * \endcode
    * or
    * \code
    *       //    0
    *       //  /   \
    *       // /     \
    *       //1-------2
    *   
    * \endcode
    */
    class Face
    {
    public:
        Face() XNORMAL_NOTHROW 
        {
            this->SetNVerts(3U);
        }

        /**
        * Get the number of vertices for this face ( 3 or 4 )
        * @return The number of vertices of this face ( 3 or 4 )
        */
        xnUInt32 GetNVerts() const XNORMAL_NOTHROW 
        {
            return ((m_arrIdx[0]&1U)!=0U) ? 4U : 3U;
        }

        /**
        * Set the number of vertices fort this face
        * @param nv [in] 3 or 4 vertices. N-gons are not supported
        */
        void SetNVerts ( const xnUInt32 nv ) XNORMAL_NOTHROW 
        {
            XNORMAL_ASSERT ( 3U==nv || 4U==nv );
            if ( 4U == nv )
            {
                // set quad-face bit
                m_arrIdx[0] |= 1U;
            }
            else
            {
                // clear quad-face bit
                m_arrIdx[0U] &= 0xFFFFFFFEU;
            }
        }

        /**
        * Get a vertex index in this face.
        *        
        * @param ith [in] The i-th vertex on this face. Pass here 0, 1, 2 or 3(
        *   this last in case it's a quad face)
        *
        * @return The vertex index ( notice we use 31-bits indices and not
        * 32bits, so the maximum index is 0xFFFFFFFE )
        */
        xnUInt32 GetVertexIdx ( const xnUInt32 ith ) const XNORMAL_NOTHROW 
        {
            XNORMAL_ASSERT ( ith>=0U && ith<this->GetNVerts() );
            return (m_arrIdx[ith]&0xFFFFFFFEU)>>1U;
        }
        
        /**
        * Set a vertex index in this face
        * @param ith [in] The i-th vertex on this face. Pass here 0, 1, 2 or 3(
        *   this last in case it's a quad face)
        * @param idx [in] The vertex index. You can use 31 of the 32 bits, the 
        * lowest one is used to indicate if this the face is a quad face or a
        * tri face
        *
        */
        void SetVertexIdx ( const xnUInt32 ith, const xnUInt32 idx ) XNORMAL_NOTHROW 
        {
            XNORMAL_ASSERT ( ith>=0U && ith<this->GetNVerts() );
            m_arrIdx[ith] &= 1U; // clear all the bits but the quad bit
            m_arrIdx[ith] |= (idx<<1U);
        }

    public:
        /**
        * Test if a face is degenerated ( with a very weak but fast method )
        */
        bool IsDegenerated () const XNORMAL_NOTHROW 
        {
            return ( this->GetVertexIdx(0U)==this->GetVertexIdx(1U) ||
                this->GetVertexIdx(0U)==this->GetVertexIdx(2U) ||
                this->GetVertexIdx(1U)==this->GetVertexIdx(2U) );
        }

    protected:
        xnUInt32 m_arrIdx[4];
    };
    
    /**
    * Get the number of point faces
    */
    virtual xnUInt32 GetNumPointFaces () const XNORMAL_NOTHROW = 0;
    
    /**
    * Get a position face in the mesh
    * @param ith [in] The face index
    * @return A const reference to the face
    */
    const Face& GetPointFace ( const xnUInt32 ith ) const XNORMAL_NOTHROW 
    {
        XNORMAL_ASSERT ( ith<this->GetNumPointFaces() );
        return this->GetPointFaceP ( ith );
    }

    virtual const Face* GetPointFacesPtrConst() const XNORMAL_NOTHROW = 0;
    virtual Face* GetPointFacesPtr() XNORMAL_NOTHROW = 0;

    /**
    * Set a point face in the mesh
    * @param ith [in] The face index
    * @param f [in] The face value to set
    */
    void SetPointFace ( const xnUInt32 ith, const Face& f ) XNORMAL_NOTHROW 
    {
        XNORMAL_ASSERT ( ith<this->GetNumPointFaces() );
        this->SetPointFaceP ( ith, f );
    }

    /**
    * Add a new face at the end of the mesh's face list.
    * @param f [in] The face to add
    */
    virtual bool AddPointFace ( const Face& f ) XNORMAL_NOTHROW = 0;

    /**
    * Reserve(for adding) certain number of faces ( without destroying
    * the current ones)
    *
    * @param nFaces [in] The number of faces to reserve
    *
    * @remarks Each time you call AddFace() and the internal reserve buffer
    * is full, xNormal will grow the internal reserve buffer and it will 
    * re-allocate the stored vertices.
    * For this motive, it's important that you set a memory reserve before 
    * adding  new faces with the AddFace() method... or the memory used can 
    * grow a lot.
    * Of course, if you know the exact # of faces better use the 
    * ResizeFaces()/SetFace() combo. It works like the STL's vector container.
    */
    virtual bool ReservePointFaces ( const xnUInt32 nFaces ) XNORMAL_NOTHROW = 0;
    
    /**
    * Allocate certain number of face ( and destroy previous buffer ).
    *
    * @param nFaces [in] The number of vertices to preallocate
    *
    * @remarks The difference between ReservePointFaces() and this method is
    * that the ReservedPointFaces() does not modify the current # of faces
    * while ResizePointFaces() does. The ReservePointFaces() is good when
    * you don't know the exact face count.. If you know the exact vertex count
    * then better use ResizePointVertices()/SetPointVertex() combo.
    * This works like the STL's vector container.
    */
    virtual bool ResizePointFaces ( const xnUInt32 nFaces ) XNORMAL_NOTHROW = 0;
    
    /**
    * Erase point faces
    */
    virtual void ClearPointFaces () XNORMAL_NOTHROW = 0;
    
    /**
    * Copy faces to other polymesh and clear them from this mesh
    * @param pmsh [out] The destination mesh where to copy the face to
    */
    bool CopyPointFacesTo ( IPolyMesh& pmsh ) XNORMAL_NOTHROW 
    {
        if ( 0U==this->GetNumPointFaces() )
        {
            pmsh.ClearPointFaces();
            return true;
        }

        if ( false==pmsh.ResizePointFaces(this->GetNumPointFaces()) )
        {
            return false;
        }
        //copy data. We'll use dangerous "memcpy" here because we really NEED speed.
        memcpy ( reinterpret_cast<void*>(pmsh.GetPointFacesPtr()), reinterpret_cast<const void*>(this->GetPointFacesPtr()), sizeof(Face)*static_cast<size_t>(this->GetNumPointFaces()) ); 
        return true;
    }

public:
    virtual xnUInt32 GetNumNormalFaces () const XNORMAL_NOTHROW = 0;
    
    const Face& GetNormalFace ( const xnUInt32 ith ) const XNORMAL_NOTHROW 
    {
        XNORMAL_ASSERT ( ith<this->GetNumNormalFaces() );
        return this->GetNormalFaceP(ith);
    }

    virtual const Face* GetNormalFacesPtrConst() const XNORMAL_NOTHROW = 0;
    virtual Face* GetNormalFacesPtr() XNORMAL_NOTHROW = 0;

    void SetNormalFace ( const xnUInt32 ith, const Face& f ) XNORMAL_NOTHROW 
    {
        XNORMAL_ASSERT ( ith<this->GetNumNormalFaces() );
        this->SetNormalFaceP ( ith, f );
    }

    virtual bool AddNormalFace ( const Face& f ) XNORMAL_NOTHROW = 0;
    
    virtual bool ReserveNormalFaces ( const xnUInt32 nFaces ) XNORMAL_NOTHROW = 0;
    
    virtual bool ResizeNormalFaces ( const xnUInt32 nFaces ) XNORMAL_NOTHROW = 0;
    
    virtual void ClearNormalFaces () XNORMAL_NOTHROW = 0;
    
    bool CopyNormalFacesTo ( IPolyMesh& pmsh ) XNORMAL_NOTHROW 
    {
        if ( 0U==this->GetNumNormalFaces() )
        {
            pmsh.ClearNormalFaces();
            return true;
        }

        if ( false==pmsh.ResizeNormalFaces(this->GetNumNormalFaces()) )
        {
            return false;
        }
        //copy data. We'll use dangerous "memcpy" here because we really NEED speed.
        memcpy ( reinterpret_cast<void*>(pmsh.GetNormalFacesPtr()), reinterpret_cast<const void*>(this->GetNormalFacesPtr()), sizeof(Face)*static_cast<size_t>(this->GetNumNormalFaces()) ); 
        return true;
    }

public:
    virtual xnUInt32 GetNumTangentFaces () const XNORMAL_NOTHROW = 0;
    
    const Face& GetTangentFace ( const xnUInt32 ith ) const XNORMAL_NOTHROW 
    {
        XNORMAL_ASSERT ( ith<this->GetNumTangentFaces() );
        return this->GetTangentFaceP ( ith );
    }

    virtual const Face* GetTangentFacesPtrConst() const XNORMAL_NOTHROW = 0;
    virtual Face* GetTangentFacesPtr() XNORMAL_NOTHROW = 0;

    void SetTangentFace ( const xnUInt32 ith, const Face& f ) XNORMAL_NOTHROW 
    {
        XNORMAL_ASSERT ( ith<this->GetNumTangentFaces() );
        this->SetTangentFaceP ( ith, f );
    }

    virtual bool AddTangentFace ( const Face& f ) XNORMAL_NOTHROW = 0;
    
    virtual bool ReserveTangentFaces ( const xnUInt32 nFaces ) XNORMAL_NOTHROW = 0;
    
    virtual bool ResizeTangentFaces ( const xnUInt32 nFaces ) XNORMAL_NOTHROW = 0;
    
    virtual void ClearTangentFaces () XNORMAL_NOTHROW = 0;
    
    bool CopyTangentFacesTo ( IPolyMesh& pmsh ) XNORMAL_NOTHROW 
    {
        if ( 0U==this->GetNumTangentFaces() )
        {
            pmsh.ClearTangentFaces();
            return true;
        }

        if ( false==pmsh.ResizeTangentFaces(this->GetNumTangentFaces()) )
        {
            return false;
        }
        //copy data. We'll use dangerous "memcpy" here because we really NEED speed.
        memcpy ( reinterpret_cast<void*>(pmsh.GetTangentFacesPtr()), reinterpret_cast<const void*>(this->GetTangentFacesPtr()), sizeof(Face)*static_cast<size_t>(this->GetNumTangentFaces()) ); 
        return true;
    }

public:
    virtual xnUInt32 GetNumBiNormalFaces () const XNORMAL_NOTHROW = 0;
    
    const Face& GetBiNormalFace ( const xnUInt32 ith ) const XNORMAL_NOTHROW 
    {
        XNORMAL_ASSERT ( ith<this->GetNumBiNormalFaces() );
        return this->GetBiNormalFaceP ( ith );        
    }

    virtual const Face* GetBiNormalFacesPtrConst() const XNORMAL_NOTHROW = 0;
    virtual Face* GetBiNormalFacesPtr() XNORMAL_NOTHROW = 0;
    
    void SetBiNormalFace ( const xnUInt32 ith, const Face& f ) XNORMAL_NOTHROW 
    {
        XNORMAL_ASSERT ( ith<this->GetNumBiNormalFaces() );
        this->SetBiNormalFaceP ( ith, f );
    }

    virtual bool AddBiNormalFace ( const Face& f ) XNORMAL_NOTHROW = 0;
    
    virtual bool ReserveBiNormalFaces ( const xnUInt32 nFaces ) XNORMAL_NOTHROW = 0;
    
    virtual bool ResizeBiNormalFaces ( const xnUInt32 nFaces ) XNORMAL_NOTHROW = 0;
    
    virtual void ClearBiNormalFaces () XNORMAL_NOTHROW = 0;
    
    bool CopyBiNormalFacesTo ( IPolyMesh& pmsh ) XNORMAL_NOTHROW 
    {
        if ( 0U==this->GetNumBiNormalFaces() )
        {
            pmsh.ClearBiNormalFaces();
            return true;
        }

        if ( false==pmsh.ResizeBiNormalFaces(this->GetNumBiNormalFaces()) )
        {
            return false;
        }
        //copy data. We'll use dangerous "memcpy" here because we really NEED speed.
        memcpy ( reinterpret_cast<void*>(pmsh.GetBiNormalFacesPtr()), reinterpret_cast<const void*>(this->GetBiNormalFacesPtr()), sizeof(Face)*static_cast<size_t>(this->GetNumBiNormalFaces()) ); 
        return true;
    }

public:
    virtual xnUInt32 GetNumUVFaces () const XNORMAL_NOTHROW = 0;
    
    const Face& GetUVFace ( const xnUInt32 ith ) const XNORMAL_NOTHROW 
    {
        XNORMAL_ASSERT ( ith<this->GetNumUVFaces() );
        return this->GetUVFaceP(ith);
    }

    virtual const Face* GetUVFacesPtrConst() const XNORMAL_NOTHROW = 0;
    virtual Face* GetUVFacesPtr() XNORMAL_NOTHROW = 0;

    void SetUVFace ( const xnUInt32 ith, const Face& f ) XNORMAL_NOTHROW 
    {
        XNORMAL_ASSERT ( ith<this->GetNumUVFaces() );
        this->SetUVFaceP ( ith, f );
    }

    virtual bool AddUVFace ( const Face& f ) XNORMAL_NOTHROW = 0;
    
    virtual bool ReserveUVFaces ( const xnUInt32 nFaces ) XNORMAL_NOTHROW = 0;
    
    virtual bool ResizeUVFaces ( const xnUInt32 nFaces ) XNORMAL_NOTHROW = 0;
    
    virtual void ClearUVFaces () XNORMAL_NOTHROW = 0;
    
    bool CopyUVFacesTo ( IPolyMesh& pmsh ) XNORMAL_NOTHROW 
    {
        if ( 0U==this->GetNumUVFaces() )
        {
            pmsh.ClearUVFaces();
            return true;
        }

        if ( false==pmsh.ResizeUVFaces(this->GetNumUVFaces()) )
        {
            return false;
        }
        //copy data. We'll use dangerous "memcpy" here because we really NEED speed.
        memcpy ( reinterpret_cast<void*>(pmsh.GetUVFacesPtr()), reinterpret_cast<const void*>(this->GetUVFacesPtr()), sizeof(Face)*static_cast<size_t>(this->GetNumUVFaces()) ); 
        return true;
    }

public:
    //note: There are no Get/SetVColFace, because we support only one vertex
    //color per vertex position.... so the indices will match with the point
    //ones.

public:
    /**
    * Test is this mesh contains vertex/face positions
    * @return true if this mehs contains almost one vertex and one face,
    *   false on the contrary.
    */
    bool IsEmpty() const XNORMAL_NOTHROW 
    {
        return ( 0U==this->GetNumPoints() || 0U==this->GetNumPointFaces() );
    }

public:
    
    /**
    * Cage data for maximum ray distances in the raycaster
    * There is a CageFace for each mesh face
    */
    class CageFace
    {
    public:
        xnUInt32 GetNVerts() const XNORMAL_NOTHROW 
        {
            XNORMAL_ASSERT ( 3U==nVerts || 4U==nVerts ); // tri or quad faces only!
            return nVerts;
        }

        void SetNVerts ( const xnUInt32 nv ) XNORMAL_NOTHROW 
        {
            XNORMAL_ASSERT ( 3U==nv || 4U==nv );
            nVerts = nv;
        }

        /**
        * Vertex normal to use for the cage extrussion
        * @remarks If the corresponding face is a triangle the 4th component 
        * will be ignored
        */
        Vector normal[4];

        /**
        * Distance from the original vertex position to the cage position
        * Althought could be possitive or negative(or zero), if the cage is 
        * well set the distances will be always greater or equal to zero.
        * @remarks If the corresponding face is a triangle the 4th component
        * will be ignored
        */
        xnSingle distance[4];

        /**
        * Final extruded point of the cage ( calculated as meshVertex + 
        * cage.normal*distance )
        * @remarks If the corresponding face is a triangle the 4th component 
        * will be ignored
        */
        Vector extrudedPoint[4];

    protected:
        /**
        * Number of face vertices. 3 for triangular faces or 4 for quad faces. 
        * Ngons are not supported
        */
        xnUInt32 nVerts;
    };
    
    /**
    * Get the number of cage faces
    */
    virtual xnUInt32 GetNumCageFaces () const XNORMAL_NOTHROW = 0;
    
    const CageFace& GetCageFace ( const xnUInt32 ith ) const XNORMAL_NOTHROW 
    {
        XNORMAL_ASSERT ( ith<this->GetNumCageFaces() );
        return this->GetCageFaceP(ith);
    }

    virtual const CageFace* GetCageFacesPtrConst() const XNORMAL_NOTHROW = 0;
    virtual CageFace* GetCageFacesPtr() XNORMAL_NOTHROW = 0;

    void SetCageFace ( const xnUInt32 ith, const CageFace& f ) XNORMAL_NOTHROW 
    {
        XNORMAL_ASSERT ( ith<this->GetNumCageFaces() );
        this->SetCageFaceP ( ith, f );
    }

    virtual bool AddCageFace ( const CageFace& f ) XNORMAL_NOTHROW = 0;
    
    virtual bool ReserveCageFaces ( const xnUInt32 nFaces ) XNORMAL_NOTHROW = 0;
    
    virtual bool ResizeCageFaces ( const xnUInt32 nFaces ) XNORMAL_NOTHROW = 0;
    
    bool CopyCageFacesTo ( IPolyMesh& pmsh ) XNORMAL_NOTHROW 
    {
        if ( 0U==this->GetNumCageFaces() )
        {
            pmsh.ClearCage();
            return true;
        }

        if ( false==pmsh.ResizeCageFaces(this->GetNumCageFaces()) )
        {
            return false;
        }
        //copy data. We'll use dangerous "memcpy" here because we really NEED speed.
        memcpy ( reinterpret_cast<void*>(pmsh.GetCageFacesPtr()), reinterpret_cast<const void*>(this->GetCageFacesPtr()), sizeof(CageFace)*static_cast<size_t>(this->GetNumCageFaces()) ); 
        return true;
    }

    /**
    * Erase the cage data
    */
    virtual void ClearCage () XNORMAL_NOTHROW = 0;
    
    /**
    * Get the maximum cage extrussion distance for all the mesh's faces
    * @return The maximum cage distance, is world units
    */
    virtual xnSingle GetMaxCageDistance () const XNORMAL_NOTHROW = 0;

public:
    /**
    * Release all the mesh data ( vertices, UVs, triangles... all ) from memory.
    */
    void Clear () XNORMAL_NOTHROW
    {
        this->ClearPoints();
        this->ClearPointFaces();

        this->ClearNormals();
        this->ClearNormalFaces();

        this->ClearBiNormals();
        this->ClearBiNormalFaces();

        this->ClearTangents();
        this->ClearTangentFaces();

        this->ClearUVs();
        this->ClearUVFaces();

        this->ClearVColors();
        
        this->ClearCage();
    }

    /**
    * Calculate the approximate memory used for this mesh
    * @return The RAM used, in megabytes
    */
    virtual xnSingle CalculateUsedRAM() const XNORMAL_NOTHROW = 0;

public:
    /**
    * Scale the mesh
    * @param scl [in] The scale to apply
    */
    virtual void Scale ( const xnSingle scl ) XNORMAL_NOTHROW = 0;

public:
    /**
    * Calculate the radius and AABB of this mesh
    */
    virtual void CalculateAABBAndRadius ( AABB &aabb, xnSingle &rad ) const XNORMAL_NOTHROW = 0;

public:
    virtual void AverageVertexNormals ( IString &err, IProgressCallback* pCall ) XNORMAL_NOTHROW = 0;
    virtual void HardenVertexNormals ( IString &err, IProgressCallback* pCall ) XNORMAL_NOTHROW = 0;
    virtual xnSingle CalculateTangentBasis ( const bool bIsDerivMapBake, IString &err, IProgressCallback* pCall ) XNORMAL_NOTHROW = 0;

public:
    /**
    * Reserved for IGraphicsDrv::LoadMesh
    * This property will be filled with the loaded mesh in VRAM for the
    * graphics driver as is only for internal use.
    */
    virtual const IGraphicsDrvMesh*GetGDMeshConst() const XNORMAL_NOTHROW = 0;
    virtual IGraphicsDrvMesh* GetGDMesh() XNORMAL_NOTHROW = 0;
    virtual void SetGDMesh ( IGraphicsDrvMesh* ) XNORMAL_NOTHROW = 0;

protected:
    virtual ~IPolyMesh()
    {
    }

protected:
    virtual const Vector& GetPointP ( const xnUInt32 ith ) const XNORMAL_NOTHROW = 0;
    virtual void SetPointP ( const xnUInt32 ith, const Vector& v ) XNORMAL_NOTHROW = 0;

    virtual const Vector& GetNormalP ( const xnUInt32 ith ) const XNORMAL_NOTHROW = 0;
    virtual void SetNormalP ( const xnUInt32 ith, const Vector& v ) XNORMAL_NOTHROW = 0;

    virtual const Vector& GetBiNormalP ( const xnUInt32 ith ) const XNORMAL_NOTHROW = 0;
    virtual void SetBiNormalP ( const xnUInt32 ith, const Vector& v ) XNORMAL_NOTHROW = 0;

    virtual const Vector& GetTangentP ( const xnUInt32 ith ) const XNORMAL_NOTHROW = 0;
    virtual void SetTangentP ( const xnUInt32 ith, const Vector& v ) XNORMAL_NOTHROW = 0;

    virtual const Vector2& GetUVP ( const xnUInt32 ith ) const XNORMAL_NOTHROW = 0;
    virtual void SetUVP ( const xnUInt32 ith, const Vector2& v ) XNORMAL_NOTHROW = 0;

    virtual xnUInt32 GetVColorP ( const xnUInt32 ith ) const XNORMAL_NOTHROW = 0;
    virtual void SetVColorP ( const xnUInt32 ith, const xnUInt32 c ) XNORMAL_NOTHROW = 0;
    
protected:
    virtual const Face& GetPointFaceP ( const xnUInt32 f ) const XNORMAL_NOTHROW = 0;
    virtual void SetPointFaceP ( const xnUInt32 f, const Face& ) XNORMAL_NOTHROW = 0;

    virtual const Face& GetNormalFaceP ( const xnUInt32 f ) const XNORMAL_NOTHROW = 0;
    virtual void SetNormalFaceP ( const xnUInt32 f, const Face& ) XNORMAL_NOTHROW = 0;

    virtual const Face& GetBiNormalFaceP ( const xnUInt32 f ) const XNORMAL_NOTHROW = 0;
    virtual void SetBiNormalFaceP ( const xnUInt32 f, const Face& ) XNORMAL_NOTHROW = 0;

    virtual const Face& GetTangentFaceP ( const xnUInt32 f ) const XNORMAL_NOTHROW = 0;
    virtual void SetTangentFaceP ( const xnUInt32 f, const Face& ) XNORMAL_NOTHROW = 0;

    virtual const Face& GetUVFaceP ( const xnUInt32 f ) const XNORMAL_NOTHROW = 0;
    virtual void SetUVFaceP ( const xnUInt32 f, const Face& ) XNORMAL_NOTHROW = 0;

protected:
    virtual const CageFace& GetCageFaceP ( const xnUInt32 ith ) const XNORMAL_NOTHROW = 0;
    virtual void SetCageFaceP ( const xnUInt32 ith, const CageFace& ) XNORMAL_NOTHROW = 0;
};



