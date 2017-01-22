#if 0
    This file is part of the xNormal 3 SDK, formatted for Doxygen/JavaDoc 
    documentation system. 
    (c) 2005-2015, S.Orgaz.
    Contact: granthill76 'at' yahoo.com
    Visit http://www.xnormal.net for more info
 
    FOR MORE INFO ABOUT THE LICENSE, CONSULT THE XNORMAL PDF DOCUMENTATION
#endif //0 comment

/**
* Plugin type
*/
enum ePLUGIN_TYPE
{
    /**
    * The plugin is a mesh importer that implements an IMeshImporter 
    * interface.
    */
    PLUGIN_TYPE_MESH_IMPORTER = 0,

    /**
    * The plugin is a mesh exporer that implements an IMeshExporter 
    * interface.
    */
    PLUGIN_TYPE_MESH_EXPORTER,

    /**
    * The plugin is an image importer that implements the IImageImporter 
    * interface.
    */
    PLUGIN_TYPE_IMAGE_IMPORTER,

    /**
    * The plugin is an image importer that implements the IImageExporter 
    * interface.
    */
    PLUGIN_TYPE_IMAGE_EXPORTER,

    /**
    * The plugin is a vertex-tangent-basis-calculator that implements 
    * the ITangentBasisCalculator interface.
    */
    PLUGIN_TYPE_TANGENT_BASIS_CALCULATOR,

    /**
    * The plugin is a graphics driver that implements the IGraphicsDrv
    * interface.
    */
    PLUGIN_TYPE_GRAPHICS_DRIVER,

    /**
    * The plugin is a map renderer that implements the IMapRenderer interface(non public, sorry)
    */
    PLUGIN_TYPE_MAP_RENDERER,

    /**
    * The plugin is a map renderer that implements the IVideoMaker interface
    */
    PLUGIN_TYPE_VIDEO_MAKER,

    /**
    * The plugin is a triangulator that implements the ITriangulator interface
    */
    PLUGIN_TYPE_TRIANGULATOR
};

/**
* Basic interface for xNormal plugins.
* To create a plugin for xNormal implement this IPlugin class, then export
* three dynamic-linked-library functions like these:\n
*
* \code
* XNORMAL_DLLEXPORT xnUInt32 xNormalSDKVersion () XNORMAL_NOTHROW
* {
*     return XNORMAL_SDK_VERSION;
* }

* XNORMAL_DLLEXPORT void xNormalInitPlugins ( void* xnh ) XNORMAL_NOTHROW
* {
*     XNORMAL_TRY
*     {
*         XNORMAL_ASSERT ( xnh!=xnNULL );
*         IHost &host ( *reinterpret_cast<IHost*>(xnh) );
*         auto_ptr_release<MyPlugij1> plug ( new MyPlugij1(host) );
*         if ( true==host.RegisterImageImporter(*plug) )
*         {
*             plug.release();
*         }
*     } XNORMAL_CATCH_ALL_EMPTY;
* }
*
* XNORMAL_DLLEXPORT void xNormalShutdown () XNORMAL_NOTHROW
* {
*     // This will be called when xNormal is closing, so you could free some resources here if needed
* }

* \endcode
*
* Remember that xNormal.exe uses a C++ Multithreaded DLL model. Any model different than that will cause problems... 
*/
class IPlugin
{
public:
    /**
    * Default constructor
    * @param xnh [in] The xNormal host pointer ( will be passed automatically
    * by the xNormal plugin manager )
    */
    IPlugin ( IHost &xnh ) : m_host(xnh)
    {
    }

    /**
    * Free the plugin. xNormal will call this method to release the plugin to
    * avoid C CRT problems with the DLL. The implementation can free any 
    * resource and call a "delete this" here.
    */
    virtual void Release() XNORMAL_NOTHROW = 0;

public:
    /**
    * Name of the plugin
    */
    virtual const wchar_t* GetPluginName() const XNORMAL_NOTHROW = 0;
    
    /**
    * Description of the plugin
    */
    virtual const wchar_t* GetPluginDescription() const XNORMAL_NOTHROW = 0;

    /**
    * Author of the plugin
    */
    virtual const wchar_t* GetPluginAuthor () const XNORMAL_NOTHROW = 0;

    /**
    * Version of the plugin ( for example 3.4 )
    */
    virtual void GetPluginVersion ( sVERSION& ) const XNORMAL_NOTHROW = 0;

    /**
    * Plugin type
    */
    virtual ePLUGIN_TYPE GetPluginType() const XNORMAL_NOTHROW = 0;

    /**
    * If the plugin uses files(like mesh/image importers and exporters), put here the filter description and extension.
    * @returns The description and extension, "|" separated. For example "BMP files|*.bmp;*.BMP|PNG Files|*.png"
    */
    virtual const wchar_t* GetPluginFileFilter () const XNORMAL_NOTHROW
    {
        return xnNULL;
    }

    /**
    * Global Unique Identifier(GUID) of the plugin
    */
    virtual void GetPluginGUID ( sGUID& )  const XNORMAL_NOTHROW = 0;

public:
    /**
    * TRUE if the plugin implements the IPlugin::Configure() method which
    * can be called from the xNormal Plugin Manager manually by the user.
    */
    virtual bool CanBeConfigured() const XNORMAL_NOTHROW
    {
        return false;
    }

    /**
    * Launch a configuration wizard dialog (platform dependent, you can use 
    * MFC, .NET winforms, wxWidgets or whatever you want ).
    *
    * @remarks The plugin must decide internally how to store its settings 
    * after calling this, for example, using the Window's Registry or a 
    * .dat/.xml file.
    */
    virtual void Configure () XNORMAL_NOTHROW
    {
    }

public:
    enum ePLUGIN_PROGRAMMING_LANGUAGE
    {
        PLUGIN_PROGRAMMING_LANGUAGE_CPP = 0, //C++
        PLUGIN_PROGRAMMING_LANGUAGE_JAVA
    };

    virtual IPlugin::ePLUGIN_PROGRAMMING_LANGUAGE GetPluginProgrammingLanguage() const XNORMAL_NOTHROW
    {
        return IPlugin::PLUGIN_PROGRAMMING_LANGUAGE_CPP;
    }

protected:
    /**
    * A reference to the xNormal host passed in the default constructor
    */
    IHost &m_host;

    virtual ~IPlugin()
    {
    }
};

