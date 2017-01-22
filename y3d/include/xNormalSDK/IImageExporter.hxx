#if 0
    This file is part of the xNormal 3 SDK, formatted for Doxygen/JavaDoc 
    documentation system. 
    (c) 2005-2015, S.Orgaz.
    Contact: granthill76 'at' yahoo.com
    Visit http://www.xnormal.net for more info
 
    FOR MORE INFO ABOUT THE LICENSE, CONSULT THE XNORMAL PDF DOCUMENTATION
#endif //0 comment


/**
* An interface to export imnages
*/
class IImageExporter : public IPlugin
{
public:
    /**
    * Default constructor
    * @param xnh [in] The xNormal host pointer
    */
    IImageExporter ( IHost &xnh ) : IPlugin(xnh)
    {
    }

    ePLUGIN_TYPE GetPluginType() const XNORMAL_NOTHROWOVR
    {
        return PLUGIN_TYPE_IMAGE_EXPORTER;
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
    * Method to indicate if the importer can write alpha channels
    * @return The number of bits for the alpha channel that this exporter supports
    */
    virtual xnUInt32 AlphaBitSupport() const XNORMAL_NOTHROW
    {
        return 0U;
    }

    /**
    * Export the specified image mipmaps to a disk file .
    *
    * @param file [in] The file name to be written with the specified image. Cannot be NULL nor empty.
    * @param img [in] The image to export.
    *
    * @param err [in] A string where you can place any error info or keep it empty is all was ok.
    *
    * @param pCall [in] An optional callback pointer where to notify the task progress.
    *
    * @remarks The implementation should try to store all the mipmaps 
    * inside the specified file.
    * The DDS and the RAW are two formats those allow to do this... 
    * But other formats like the BMP or the TGA can't store mipmaps... 
    * In that case you could write the images as file_#mipmap.. 
    * For example... If we pass as "file" parameter "house" and we have 3 
    * mipmaps then you should write "house_0.bmp"(with the top-mipmap),
    * "house_1.bmp"(with the medium mipmap) and "house_2.bmp"(with the 
    * lowest mipmap 1x1)... The implemenetation decides really... 
    * You can store them like you want! As recommendation use the 
    * Utils::GetMipmapFileName() method to get a mipmap file name.
    *
    * @remarks An exception must be thrown if something fails
    * @remarks If you use non-squared mipmaps, you must pass the mipmaps
    * down to 1x1. For example, this is a good mipmap list: 128x64, 
    * 64x32, 32x16, 16x8, 8x4, 4x2, 2x1, 1x1
    */
    virtual void Export ( const IString& file, const IImage& img, IString &err, IProgressCallback* pCall ) XNORMAL_NOTHROW = 0;

    /**
    * Get if the exporter supports HDR floating-point values ( color values greater than 1.0f or less than 0.0f )
    * @return true if the exporter supports HDR values or false if not(the default)
    */
    virtual bool SupportsHDR() const XNORMAL_NOTHROW
    {
        return false;
    }

protected:
    virtual ~IImageExporter()
    {
    }
};

