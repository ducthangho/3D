#if 0
    This file is part of the xNormal 3 SDK, formatted for Doxygen/JavaDoc 
    documentation system. 
    (c) 2005-2015, S.Orgaz.
    Contact: granthill76 'at' yahoo.com
    Visit http://www.xnormal.net for more info
 
    FOR MORE INFO ABOUT THE LICENSE, CONSULT THE XNORMAL PDF DOCUMENTATION
#endif //0 comment


class IMeshExporter : public IPlugin
{
public:
    /**
    * Default constructor
    * @param xnh [in] The xNormal host pointer ( will be passed automatically
    * by the xNormal plugin manager )
    */
    IMeshExporter ( IHost &xnh ) : IPlugin(xnh)
    {
    }

    ePLUGIN_TYPE GetPluginType() const XNORMAL_NOTHROWOVR
    {
        return PLUGIN_TYPE_MESH_EXPORTER;
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
    * Export the specified geometry to a specified file
    *
    * @param file [in] Complete path to a file. For example, "c:/test/chair.obj". 
    *
    * @param geom [in] A reference to the geometry to export.
    *
    * @param err [inout] A string where you can place error info or keep it empty to indicate all was ok.
    *
    * @param pCall [in] A callback where to reflect the task progress or NULL to ignore it.
    */
    virtual void Export ( const IString &file, const IGeometry& geom, IString &err, IProgressCallback* pCall ) XNORMAL_NOTHROW = 0;

protected:
    virtual ~IMeshExporter()
    {
    }
};

