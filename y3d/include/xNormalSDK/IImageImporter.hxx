#if 0
    This file is part of the xNormal 3 SDK, formatted for Doxygen/JavaDoc 
    documentation system. 
    (c) 2005-2015, S.Orgaz.
    Contact: granthill76 'at' yahoo.com
    Visit http://www.xnormal.net for more info
 
    FOR MORE INFO ABOUT THE LICENSE, CONSULT THE XNORMAL PDF DOCUMENTATION
#endif //0 comment


/**
* An interface to import images
*/
class IImageImporter : public IPlugin
{
public:
    /**
    * Default constructor
    * @param xnh [in] The xNormal host pointer ( will be passed automatically
    * by the xNormal plugin manager )
    */
    IImageImporter ( IHost &xnh ) : IPlugin(xnh)
    {
    }

    ePLUGIN_TYPE GetPluginType() const XNORMAL_NOTHROWOVR
    {
        return PLUGIN_TYPE_IMAGE_IMPORTER;
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
    * Method to indicate if the importer can read alpha channels
    * @return The number of bits for the alpha channel that this importer supports
    */
    virtual xnUInt32 AlphaBitSupport() const XNORMAL_NOTHROW
    {
        return 0U;
    }

    /**
    * Import the specified image file 
    * @param file [in] Filename of the image to import.
    * @param img [in] An image where to write the data to.
    * @param err [in] A string where you can output error info. Keep it empty to indicate the import process was ok.
    * @param pCall [in] A progress callback to monitorize the import task status or NULL to ignore it.
    *    
    */
    virtual void Import ( const IString &file, IImage& img, IString &err, IProgressCallback* pCall ) XNORMAL_NOTHROW = 0;

    /**
    * Get if the exporter supports HDR floating-point values ( color values greater than 1.0f or less than 0.0f )
    * @return true if the exporter supports HDR values or false if not(the default)
    */
    virtual bool SupportsHDR() const XNORMAL_NOTHROW
    {
        return false;
    }

protected:
    virtual ~IImageImporter()
    {
    }
};

