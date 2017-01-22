#if 0
    This file is part of the xNormal 3 SDK, formatted for Doxygen/JavaDoc 
    documentation system. 
    (c) 2005-2015, S.Orgaz.
    Contact: granthill76 'at' yahoo.com
    Visit http://www.xnormal.net for more info
 
    FOR MORE INFO ABOUT THE LICENSE, CONSULT THE XNORMAL PDF DOCUMENTATION
#endif //0 comment


/**
* An interface to triangulate a quad-faced mesh.
*/
class ITriangulator : public IPlugin
{
public:
    /**
    * Default constructor
    * @param xnh [in] The xNormal host pointer ( will be passed automatically
    * by the xNormal plug-in manager )
    */
    ITriangulator ( IHost &xnh ) : IPlugin(xnh)
    {
    }

    ePLUGIN_TYPE GetPluginType() const XNORMAL_NOTHROWOVR
    {
        return PLUGIN_TYPE_TRIANGULATOR;
    }

public:
    /**
    * Triangulate a poly mesh
    *
    * @param src [in] The SOURCE polygonal mesh to convert
    * @param dst [in] The DESTINATION polygonal mesh to convert
    * @param err [inout] A string where you can place error info or keep it empty to indicate all was ok.
    * @param pCall [in] An optional progress callback pointer. Can be NULL.
    *
    * @remarks The implementation can "transfer" src vertices to dst mesh
    * and to erase the src mesh faces after aren't used anymore to speed up
    * the process ( that why we don't mark the src as "const" ). 
    * In xNormal there is only one time this is called... and the source
    * mesh gonna be destroyed after calling it... so the implementation can
    * use the PolyMesh::TransferVertices() inside this method to avoid an 
    * unnecesary data copy.
    */
    virtual void Triangulate ( IPolyMesh& src, IPolyMesh& dst, IString &err, IProgressCallback* pCall ) XNORMAL_NOTHROW = 0;

protected:
    virtual ~ITriangulator ()
    {
    }
};

