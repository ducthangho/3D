#if 0
    This file is part of the xNormal 3 SDK, formatted for Doxygen/JavaDoc 
    documentation system. 
    (c) 2005-2015, S.Orgaz.
    Contact: granthill76 'at' yahoo.com
    Visit http://www.xnormal.net for more info
 
    FOR MORE INFO ABOUT THE LICENSE, CONSULT THE XNORMAL PDF DOCUMENTATION
#endif //0 comment


/**
* Mesh importer interface class for xNormal.
*
* The mission of the IMeshImporter is to read and parse a file and to emit a
* Geometry object containing meshes with the elements filled ( Mesh::Positions,
* Mesh::Normals, Mesh::UVs and Mesh::Triangles ). Optionally, the 
* implementation CAN fill the Mesh::Tangents and Mesh::BiNormals implementation
* or keep it void and then xNormal will calculate the tangent basis calculation
* for you automatically.
* 
* One of the most common problems with the normal mappers is the tangent basis 
* calculation.
* The program uses a propietary method and your 3D engine exporters other
* COMPLETELY DIFFERENT so you can't use that normal mapper well in your 3D
* engine! Also there can be other problem... Your engine doesn't include a 
* tangent basis calculation function, so you have to use external libraries
* to perform this task ( like NVMeshMender ) and the results are far from 
* good...
*
* The xNormal SDK offers you various options:\n
*
*       1) Implement your own IMeshImporter and emit a Geometry object. 
*          Use YOUR OWN TANGENT BASIS CALCULATION to output the 
*          Mesh::Tangents and Mesh::BiNormals elements.\n\n
*
*       2) Implement your own IMeshImporter and emit a Geometry object.
*          Keep the Mesh::Tangents and Mesh::BiNormals unfilled(void) so
*          xNormal will calculate automatically the tangent basis for you.\n\n
*
*       3) Use the included xNormal SDK TangentBasisCalculator_Len class 
*          in your 3D engine exporters to calculate the tangent basis.\n\n
*
*       4) Use the provided standard D3DX and NVMEshMender tangent basis 
*          calculator implementations.
*/
class IMeshImporter : public IPlugin
{
public:
    /**
    * Default constructor
    * @param xnh [in] The xNormal host pointer ( will be passed automatically 
    * by the xNormal plugin manager )
    */
    IMeshImporter ( IHost &xnh ) : IPlugin(xnh)
    {
    }

    ePLUGIN_TYPE GetPluginType() const XNORMAL_NOTHROWOVR
    {
        return PLUGIN_TYPE_MESH_IMPORTER;
    }

public:
    /**
    * Test if this importer/exporter can process the specified file
    * @param fileName [in] The file name with complete path and extension ( guaranteed to be non-NULL and non-empty ).
    * @return TRUE if the implementation can process the specified file or FALSE if not
    * @remarks For example, if the implementation is a BMP image importer and the fileName specified is "c:/test/chair.tga" you should return FALSE.
    */
    virtual bool CanProcessFile ( const IString &fileName ) const XNORMAL_NOTHROW = 0;

    /**
    * Import meshes from a specified file
    *
    * @param file [in] Complete path to a file in UTF8 format.
    * If you are in Windows can be something like "c:/test/chair.obj". 
    *
    * @param ignoreNormals [in] TRUE to ignore normals because the used 
    * checked the "Smooth normals" so the normals will be averaged or 
    * FALSE to consider them.
    *
    * @param ignoreUVs [in] TRUE to ignore the texture coordinates or FALSE to consider them.
    *
    * @param ignoreVertexColors [in] TRUE to ignore the per-vertex colors or FALSE to consider them.
    *
    * @param ignoreTS [in] TRUE to ignore the tangent basis ( tangents
    * and biNormals ) or FALSE to load it.
    *
    * @param geom [inout] A reference to a Geometry where to add the meshes
    *
    * @param err [inout] A string where you can put any extra error info or keep it empty to indicate all was ok.
    *
    * @param pCall A callback where to reflect the task progress or NULL to ignore it.
    *
    * @remarks The imported geometry MUST use the typical OpenGL 
    * coordinate system, with X+ to the right, Y+ to up and Z+ from far to
    * near and UVs with origin at bottom-left and triangles defined in CCW=front order
    *
    * @remarks YOU must sure the imported mesh doesn't have degenerated triangles, duplicated vertices or other anomalies... 
    *
    */
    virtual void Import ( const IString &file, const bool ignoreNormals, const bool ignoreUVs, const bool ignoreVertexColors, const bool ignoreTS, IGeometry& geom, IString &err, IProgressCallback* pCall ) XNORMAL_NOTHROW = 0;

protected:
    virtual ~IMeshImporter ()
    {
    }
};

