#if 0
    This file is part of the xNormal 3 SDK, formatted for Doxygen/JavaDoc 
    documentation system. 
    (c) 2005-2015, S.Orgaz.
    Contact: granthill76 'at' yahoo.com
    Visit http://www.xnormal.net for more info
 
    FOR MORE INFO ABOUT THE LICENSE, CONSULT THE XNORMAL PDF DOCUMENTATION
#endif //0 comment


/**
* This class contains some basic information about the xNormal host for plugins
*/
class IHost
{
public:
    /**
    * Get the code of the culture locale.
    * @return An ASCII string with the culture ID ISO3166-1 code  ( en_US, es_ES, es_AR, etc... )
    */
    virtual const char* GetCultureID() const XNORMAL_NOTHROW = 0;

    /**
    * Get the xNormal version
    */
    virtual sVERSION GetVersion() const XNORMAL_NOTHROW = 0;

    /**
    * Get the xNormal SDK version
    */
    virtual xnUInt32 GetSDKVersion() const XNORMAL_NOTHROW = 0;

public:
    virtual const char* GetTBCGuid() const XNORMAL_NOTHROW = 0;
    virtual const char* GetTBC_OS2TS_Script() const XNORMAL_NOTHROW = 0;
    virtual const char* GetTBC_TS2OS_Script() const XNORMAL_NOTHROW = 0;

public:
    /**
    * Get the output image's render size ( in pixels )
    * @param width [out] A reference to an xnUInt32 variable where to output the width.
    * @param height [out] A reference to an xnUInt32 variable where to output the height.
    **/
    virtual void GetOutputRenderSize ( xnUInt32 &width, xnUInt32 &height ) const XNORMAL_NOTHROW = 0;

public:
    virtual bool IsUnattended () const XNORMAL_NOTHROW = 0;

public:
    /*
    * Get the xNormal's installation directory in UTF8 format with the directory
    * sepparator at the end. For example, "c:/ArtTools/xNormal3/"
    */
    virtual const char* GetStartupPath () const XNORMAL_NOTHROW = 0;

    /*
    * Get the xNormal's installation directory in UNICODE format with the directory
    * sepparator at the end. For example, "c:/ArtTools/xNormal3/"
    */
    virtual const wchar_t* GetStartupPathW () const XNORMAL_NOTHROW = 0;

    /*
    * Get the previous folder to the executable path of xNormal.
    */
    virtual const char* GetPrevStartupPath () const XNORMAL_NOTHROW = 0;

    /*
    * Get the previous folder to the executable path of xNormal.
    */
    virtual const wchar_t* GetPrevStartupPathW () const XNORMAL_NOTHROW = 0;

    /**
    * Get the xNormal's example complete-path folder.
    */
    virtual const char* GetExamplesPath () const XNORMAL_NOTHROW = 0;

    /**
    * Get the xNormal's example complete-path folder.
    */
    virtual const wchar_t* GetExamplesPathW () const XNORMAL_NOTHROW = 0;

    /**
    * Get the xNormal's UI complete-path folder
    */
    virtual const char* GetUIPath () const XNORMAL_NOTHROW = 0;

    /**
    * Get the xNormal's UI complete-path folder
    */
    virtual const wchar_t* GetUIPathW () const XNORMAL_NOTHROW = 0;

    /**
    * Get the xNormal's settings complete-path folder
    */
    virtual const char* GetSettingsPath () const XNORMAL_NOTHROW = 0;

    /**
    * Get the xNormal's settings complete-path folder
    */
    virtual const wchar_t* GetSettingsPathW () const XNORMAL_NOTHROW = 0;

    /**
    * Get the xNormal's screenshots complete-path folder
    */
    virtual const char* GetScreenshotsPath () const XNORMAL_NOTHROW = 0;

    /**
    * Get the xNormal's screenshots complete-path folder
    */
    virtual const wchar_t* GetScreenshotsPathW () const XNORMAL_NOTHROW = 0;

    /**
    * Get the xNormal's plugins complete-path folder
    */
    virtual const char* GetPluginsPath () const XNORMAL_NOTHROW = 0;

    /**
    * Get the xNormal's plugins complete-path folder
    */
    virtual const wchar_t* GetPluginsPathW () const XNORMAL_NOTHROW = 0;

    /**
    * Get the xNormal's SDK complete-path folder
    */
    virtual const char* GetSDKPath () const XNORMAL_NOTHROW = 0;

    /**
    * Get the xNormal's SDK complete-path folder
    */
    virtual const wchar_t* GetSDKPathW () const XNORMAL_NOTHROW = 0;

    /**
    * Get the OS temporally files directory. Usually this is something like 
    * "C:/Documents and Settings/Administrator/Temp" in Windows
    */
    virtual void GetTempDir ( IString &s ) const XNORMAL_NOTHROW = 0;

    /**
    * Get a temporary file name.
    * @param s [inout] A IString where to place the file name.
    * @param ext [in] An optional file extension ( without dot. For instance "tmp", or ".dat", etc... )
    */
    virtual void GetTempFile ( IString &s, const wchar_t *ext ) const XNORMAL_NOTHROW = 0;

public:
    /**
    * Write a text to the xNormal debug-log text file
    * @param text [in] The UNICODE text to be saved to the log ( as UTF8 )
    */
    virtual void LogW ( const wchar_t *text ) XNORMAL_NOTHROW = 0;

    /**
    * Write a text to the xNormal debug-log text file
    * @param text [in] The UTF8 text to be saved to the log
    */
    virtual void Log ( const char *text ) XNORMAL_NOTHROW = 0;

    // helper to log texture using a std::wstring, etc...
    XNORMAL_INLINE void LogStd ( const std::string &t ) XNORMAL_NOTHROW
    {
        this->Log ( t.c_str() );
    }

    XNORMAL_INLINE void LogStdW ( const std::wstring &t ) XNORMAL_NOTHROW
    {
        this->LogW ( t.c_str() );
    }

    /**
    * Clear the debug-log file
    */
    virtual void ClearLog () XNORMAL_NOTHROW = 0;

public:
    virtual IString* CreateString() const XNORMAL_NOTHROW = 0;
    virtual ByteArray* CreateByteArray() const XNORMAL_NOTHROW = 0;
    virtual IImage* CreateImage() const XNORMAL_NOTHROW = 0;

public:
    /**
    * Register an image importer plugin
    * @param ii [in] the image importer.
    * @return true if ok, false if it couldn't be registered ( because it was already registered, due to an out of memory or because the xNormal SDK version mismatches )
    * @remarks xNormal will call the IImageImporter::Release() method when the program closes.
    **/
    virtual bool RegisterImageImporter ( IImageImporter &ii ) XNORMAL_NOTHROW = 0;
    virtual bool RegisterImageExporter ( IImageExporter &ie ) XNORMAL_NOTHROW = 0;

    virtual bool RegisterMeshImporter ( IMeshImporter &mi ) XNORMAL_NOTHROW = 0;
    virtual bool RegisterMeshExporter ( IMeshExporter &me ) XNORMAL_NOTHROW = 0;

    virtual bool RegisterGraphicsDrv ( IGraphicsDrv &gDrv ) XNORMAL_NOTHROW = 0;

    virtual bool RegisterTangentBasisCalculator ( ITangentBasisCalculator &tbc ) XNORMAL_NOTHROW = 0;

    virtual bool RegisterVideoMaker ( IVideoMaker &vm ) XNORMAL_NOTHROW = 0;

    virtual bool RegisterTriangulator ( ITriangulator &t ) XNORMAL_NOTHROW = 0;

    virtual bool RegisterMapRenderer ( IPlugin &p ) XNORMAL_NOTHROW = 0;

protected:
    virtual ~IHost()
    {
    }
};

