#if 0
    This file is part of the xNormal 3 SDK, formatted for Doxygen/JavaDoc 
    documentation system. 
    (c) 2005-2015, S.Orgaz.
    Contact: granthill76 'at' yahoo.com
    Visit http://www.xnormal.net for more info
 
    FOR MORE INFO ABOUT THE LICENSE, CONSULT THE XNORMAL PDF DOCUMENTATION
#endif //0 comment

/**
* An interface to make videos
*/
class IVideoMaker : public IPlugin
{
public:
    /**
    * Default constructor ( empty atm )
    */
    IVideoMaker ( IHost &xnh ) : IPlugin(xnh)
    {
    }

    ePLUGIN_TYPE GetPluginType() const XNORMAL_NOTHROWOVR
    {
        return PLUGIN_TYPE_VIDEO_MAKER;
    }

public:
    /**
    * Start video capturing the screen
    * @param width [in] The width(in pixels) of the video screen. Must be divisible by 16 ( to ensure codec compatiblity )
    * @param height [in] The height(in pixels) of the video screen. Must be divisible by 16 ( to ensure codec compatiblity )
    * @param file [in] The file name where to store the video.
    * @param err [inout] An error where you can place error info or keep it empty to indicate all was ok.
    */
    virtual void Start ( const xnUInt32 width, const xnUInt32 height, const IString &file, IString &err ) XNORMAL_NOTHROW = 0;

    /**
    * End video and flush file to disk
    * @param err [inout] An error where you can place error info or keep it empty to indicate all was ok.
    */
    virtual void End ( IString &err ) XNORMAL_NOTHROW = 0;

public:
    /**
    * Get the approximate desired codec framerate ( in FPS )
    * @return The framerate in frames per second ( for example 25.0f for PAL codecs )
    */
    virtual xnSingle GetDesiredFPS () const XNORMAL_NOTHROW = 0;

public:
    struct sBGRData
    {
        xnByte b, g, r;
    };
    
    /**
    * Add a frame to the video
    * @param fb [in] A sBGRData pointer with the pixels of the image frame ( origin: BOTTOM-LEFT )
    * @param err [inout] An error where you can place error info or keep it empty to indicate all was ok.
    * @remarks The framebuffer size must be width*height and must match the 
    * ones you specified in the Start() method above ( and must be divisible by 16 )
    */
    virtual void AddFrame ( const sBGRData *fb, IString &err ) XNORMAL_NOTHROW = 0;

protected:
    virtual ~IVideoMaker()
    {
    }
};

