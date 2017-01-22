#if 0
    This file is part of the xNormal 3 SDK, formatted for Doxygen/JavaDoc 
    documentation system. 
    (c) 2005-2015, S.Orgaz.
    Contact: granthill76 'at' yahoo.com
    Visit http://www.xnormal.net for more info
 
    FOR MORE INFO ABOUT THE LICENSE, CONSULT THE XNORMAL PDF DOCUMENTATION
#endif //0 comment

/**
* A geometry class is an object which contains meshes.
*/
class IGeometry
{
public:
    virtual void Release() XNORMAL_NOTHROW = 0;

    /**
    * Get the name of the geometry
    */
    virtual const wchar_t* GetName() const XNORMAL_NOTHROW = 0;

    /**
    * Get the Geometry's identifier ( actually, the index from the 
    * lowpoly/highpoly list in xNormal )
    */
    virtual xnUInt32 GetID() const XNORMAL_NOTHROW = 0;

public:
    /**
    * Sure the geometry has UVs, normals, tangent basis calculated, etc... 
    * so it will be valid as lowpoly model and xNormal could use it well.
    * @param err [inout] A string where you can write any error info or keep it empty to indicate all was ok.
    */
    virtual void ValidateAsLowModel ( IString &err ) const XNORMAL_NOTHROW = 0;

public:
    /**
    * Scale the vertex positions/cage of all the contained meshes.
    * @param scl [in] The scale to use (in world units). Usually any vlaue in
    * range [-infinite,+infinite] will be accepted but if you pass 0.0f all the
    * vertices will be set to zero!
    *
    * @remarks This is very useful to convert the scale proportion when you
    * import a mesh (without having to re-export the models). The operation 
    * is destructive because all the vertices are modified directly by this
    * value so there is no way to recover the previous ones without copying
    * the data.
    */
    virtual void Scale ( const xnSingle scl ) XNORMAL_NOTHROW = 0;

public:
    /**
    * Calculates how much RAM ( in MegaBytes ) are occupying the meshes inside
    * this geometry for statistical control(used in the final preview window)
    */
    virtual xnSingle CalculateUsedRAM () const XNORMAL_NOTHROW = 0;

public:
    /**
    * Calculate the total AABB and radius of the geometry, putting the result
    * in the Geometry::TotalAABB and Geometry::TotalRadius properties.
    * @remarks xNormal will call this automatically. You don't need to call
    * this manually for nothing...
    */
    virtual void CalculateAABBAndRadius ( AABB &aabb, xnSingle &rad ) const XNORMAL_NOTHROW = 0;

public:
    /**
    * Get the maximum extrussion cage distance for all the contained meshes
    * @return The maximum extrussion cage distance for all the contained meshes
    */
    virtual xnSingle GetMaxCageDistance () const XNORMAL_NOTHROW = 0;

public:
    /**
    * Create a polygonal mesh and add it to the geometry
    * @return The mesh pointer or NULL if out of memory
    */
    virtual IPolyMesh* AddPolyMesh () XNORMAL_NOTHROW = 0;

    /**
    * Get the number of polymesh the geometry contains.
    * @return The number of poly meshes.
    **/
    virtual xnUInt32 GetNumPolyMeshes() const XNORMAL_NOTHROW = 0;

    /**
    * Test if the geometry contains meshes or it's empty
    * @return true if the geometry is empty or false if the geometry contains meshes.
    **/
    bool IsEmpty () const XNORMAL_NOTHROW
    {
        return ( 0U==this->GetNumPolyMeshes() );
    }

    /**
    * Remove and deallocate all the meshes, making the geometry empty
    **/
    virtual void ReleaseMeshes() XNORMAL_NOTHROW = 0;

    /**
    * Get the polygonal mesh list of this geometry
    * @return A const reference to the mesh list ( you cannot change it, just const-iterate it )
    */
    const IPolyMesh& GetPolyMeshConst ( const xnUInt32 ith ) const XNORMAL_NOTHROW
    { 
        XNORMAL_ASSERT ( ith<this->GetNumPolyMeshes() );
        return this->GetPolyMeshConstP ( ith );
    }

    /**
    * Get the polygonal mesh list of this geometry
    * @return A mutable reference to the mesh list.
    */
    IPolyMesh& GetPolyMesh ( const xnUInt32 ith ) XNORMAL_NOTHROW
    { 
        XNORMAL_ASSERT ( ith<this->GetNumPolyMeshes() );
        return this->GetPolyMeshP ( ith );
    }

    /**
    * Triangulate all the polygonal meshes ( force all the polygonal meshes's face to use three vertices )
    *
    * @param tri [in] A pointer to an ITriangulator ( cannot be NULL )
    * @param err [inout] A string where you can place any error info or keep it empty to indicate all was ok.
    * @param pCall [in] An optional progress callback pointer. Can be NULL.
    *
    */
    virtual void Triangulate ( ITriangulator* tri, IString &err, IProgressCallback *pCall ) XNORMAL_NOTHROW = 0;

protected:
    virtual ~IGeometry ()
    {
    }

    virtual const IPolyMesh& GetPolyMeshConstP ( const xnUInt32 ith ) const XNORMAL_NOTHROW = 0;
    virtual IPolyMesh& GetPolyMeshP ( const xnUInt32 ith ) XNORMAL_NOTHROW = 0;
};

