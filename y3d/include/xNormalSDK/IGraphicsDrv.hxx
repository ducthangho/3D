#if 0
    This file is part of the xNormal 3 SDK, formatted for Doxygen/JavaDoc 
    documentation system. 
    (c) 2005-2015, S.Orgaz.
    Contact: granthill76 'at' yahoo.com
    Visit http://www.xnormal.net for more info
 
    FOR MORE INFO ABOUT THE LICENSE, CONSULT THE XNORMAL PDF DOCUMENTATION
#endif //0 comment


class IGraphicsDrv;

/* Normal map type forIgraphicsDrv::DrawMesh() method */
enum eNORMAL_MAP_TYPE
{
    /* Object-space normal map */
    NORMAL_MAP_TYPE_OBJECT_SPACE = 0,

    /* Tangent-space normal map */
    NORMAL_MAP_TYPE_TANGENT_SPACE,

    /* Derivative map ( see Morten Mikkelsen's thesis at http://jbit.net/~sparky/sfgrad_bump/mm_sfgrad_bump.pdf ) */
    NORMAL_MAP_TYPE_DERIVATIVE
};

/**
* Transparency mode for IGraphics::DrawMesh() method
*/
enum eTRANSPARENCY_MODE
{
    /**
    * No alpha blending at all. Object will be full opaque
    */
    TRANSPARENCY_MODE_NONE = 0,

    /**
    * 1bit alpha test. If the base texture texel alpha value is less than 
    * a factor the pixel will be discarded
    */
    TRANSPARENCY_MODE_1BIT,

    /**
    * Additive-glow transparency (one,one)
    */
    TRANSPARENCY_MODE_ADDITIVE,

    /**
    * Screen-glass transparency (srcAlpha,one)
    * @remarks This mode requires triangle sorting
    */
    TRANSPARENCY_MODE_GLASS,

    /**
    * Alpha blending transparency (srcAlpha,invSrcAlpha)
    * @remarks This mode requires triangle sorting
    */
    TRANSPARENCY_MODE_BLEND
};

/**
* Texture type
*/
enum eTEXTURE_TYPE
{
    /**
    * Default texture type ( nothing special )
    */
    TEXTURE_TYPE_DEFAULT,

    /**
    * The texture is a cube map.
    * Cubemaps are defined using 6 faces vertically as
    * 
    * \code
    *       Z-
    *       Z+
    *       Y-
    *       Y+
    *       X-
    *       X+
    * \endcode
    *
    * So a 256x256 face cubemap will occupy 256x1536\n
    * Cubemaps have the data origin at bottom-left
    */
    TEXTURE_TYPE_CUBEMAP,

    /**
    * The texture is a normalmap ( you can apply then some kind of special 
    * texture compression like ATI 3Dc and keep the texture always normalized )
    */
    TEXTURE_TYPE_NORMALMAP
};

/**
* Video mode interface
*/
class IVideoMode
{
public:
    virtual void Release() XNORMAL_NOTHROW = 0;

public:
    /**
    * Get the total screen horizontal width in pixels
    */
    virtual xnUInt32 GetWidth() const XNORMAL_NOTHROW = 0;

    /**
    * Get the total screen vertical width in pixels
    */
    virtual xnUInt32 GetHeight () const XNORMAL_NOTHROW = 0;

    /**
    * Get the bits per pixel (bpp) for this video mode
    * For example, if the video mode uses a pixel format A2R10G10B10 then this 
    * must return 32bpp(2+10+10+10)
    */
    virtual xnUInt32 GetBPP () const XNORMAL_NOTHROW  = 0;

    /**
    * Get the vertical refresh requency of this video mode in Hertzs
    */
    virtual xnUInt32 GetHz() const XNORMAL_NOTHROW = 0;

    /**
    * Get the number of fullscreen antialiasing samples for this mode ( 
    * for example, 2x, 4x, 6x...etc )
    */
    virtual xnUInt32 GetAA() const XNORMAL_NOTHROW = 0;

public:
    /**
    * Pass video mode to a string formated as : 
    * width x height x bpp x FSAA samples
    */
    virtual void ToString ( IString& outRes ) const XNORMAL_NOTHROW = 0;

protected:
    virtual ~IVideoMode()
    { 
    }
};

/**
* An interface for any loaded-and-compiled-by-the graphics drv resources
*/
class IGraphicsDrvResource
{
public:
    virtual void Release() XNORMAL_NOTHROW = 0;

    /**
    * Get the graphics driver that owns this resource
    */
    virtual IGraphicsDrv* GetGraphicsDrv() const XNORMAL_NOTHROW = 0;

    /**
    * Get the user VRAM in BYTES
    */
    virtual xnUInt64 GetUsedVRAM () const XNORMAL_NOTHROW = 0;

protected:
    virtual ~IGraphicsDrvResource ()
    {
    }
};

/**
* An interface for loaded-and-compiled-into-vram graphics drv meshes
*/
class IGraphicsDrvMesh : public IGraphicsDrvResource
{
public:
    /**
    * Get the number of vertices of the mesh
    */
    virtual xnUInt32 GetNVerts () const XNORMAL_NOTHROW = 0;

    /**
    * Get the number of triangles of the mesh
    */
    virtual xnUInt32 GetNTris () const XNORMAL_NOTHROW = 0;

protected:
    virtual ~IGraphicsDrvMesh ()
    {
    }
};

/**
* An interface for a loaded-and-compiled-into-vram graphics drv texture
*/
class IGraphicsDrvTexture : public IGraphicsDrvResource
{
public:
    /**
    * Get the width of the texture in pixels
    */
    virtual xnUInt32 GetWidth() const XNORMAL_NOTHROW = 0;

    /**
    * Get the height of the texture in pixels
    */
    virtual xnUInt32 GetHeight() const XNORMAL_NOTHROW = 0;

    /**
    * Get if the texture has alpha channel or not
    * @return "true" is the texture contains an alpha channel or "false"
    * if only RGB channels are present
    */
    virtual bool HasAlpha() const XNORMAL_NOTHROW  = 0;

protected:
    virtual ~IGraphicsDrvTexture ()
    {
    }
};

/**
* An interface for the graphics driver
* The graphics driver mission is to paint 3D things in the screen
*/
class IGraphicsDrv : public IPlugin
{
public:
    /**
    * Default constructor
    * @param h [in] The xNormal host pointer ( will be passed automatically by the xNormal plugin manager )
    */
    IGraphicsDrv ( IHost &h ) : IPlugin(h)
    {
    }

    ePLUGIN_TYPE GetPluginType() const XNORMAL_NOTHROWOVR
    {
        return PLUGIN_TYPE_GRAPHICS_DRIVER;
    }

public:
    /**
    * Initialize the driver and create a rendering window
    * @param vm [in] A video mode from a previous GetAvailableVideoModes() 
    * call or NULL for windowed mode.
    *
    * @param dualView [in] TRUE to use all the available monitors in the system
    * or FALSE to use only the default monitor.
    * 
    * @param err [inout] A string where you can place any error info or keep it empty if all was ok.
    *
    * @remarks The implementation must create a window and process its messages
    * in the BeginPaint() and EndPaint() call. If the passed 'vm' param was 
    * NULL, then the created window must have at least 800x600 pixels.
    * 
    */
    virtual void Init ( IVideoMode* vm, const bool dualView, IString &err ) XNORMAL_NOTHROW = 0;

    /**
    * Deallocate and release any buffer created in the initialization
    **/
    virtual void Uninitialize() XNORMAL_NOTHROW = 0;

public:
    /**
    * Get available fllscreen video modes sorted by quality.0=max quality.
    * @param dualView [in] TRUE to use multimonitor, FALSE to use only one 
    * monitor. When you specify TRUE the list will contain a list of shared 
    * video modes ( videomodes supported by ALL the  monitors... ignoring 
    * the ones not compatible with ALL the monitors ).
    *
    * @param m [out] An array of video models. You NEED to delete them
    * MANUALLY when you finish with the list or a memory-leak will be 
    * incoming...
    */
    virtual void GetVideoModes ( const bool dualView, IArray<IVideoMode*> &m ) const XNORMAL_NOTHROW = 0;

public:
    /**
    * Get a pointer to the created platform-specific window ( HWND in Windows,
    * X11 Window in linux )
    */
    virtual void* GetHWND () const XNORMAL_NOTHROW = 0;

    /**
    * Get the screen width ( in full screen video mode ) or the client rect
    * width ( in windowed mode ) 
    */
    virtual xnUInt32 GetScreenWidth () const XNORMAL_NOTHROW = 0;

    /**
    * Get the screen height ( in full screen video mode ) or the client rect
    * height ( in windowed mode ) 
    */
    virtual xnUInt32 GetScreenHeight () const XNORMAL_NOTHROW = 0;

public:
    enum eTEXTURE_HINT
    {
        TEXTURE_HINT_DEFAULT = 0,
        TEXTURE_HINT_UI_TEXTURE,
        TEXTURE_HINT_NORMAL_MAP,
        TEXTURE_HINT_VECTOR_DISPLACEMENT_MAP
    };

    /**
    * Load a texture
    * 
    * @param img [in] The image.
    * 
    * @param th [in] A hint with the use you want to assign to that texture. It's used to decide compression type and mipmapping :D
    * 
    * @param err [in] A string where to write the error info or keep it empty if all was ok.
    *
    * @return The texture pointer. NULL if fails.
    */
    virtual IGraphicsDrvTexture* LoadTexture ( const IImage& img, const eTEXTURE_HINT th, IString &err ) XNORMAL_NOTHROW = 0;

    /**
    * Load a cube ( 6-faces ) texture for reflection mapping, etc...
    *
    * The specified image must be in VERTICAL order as:
    * \code
    *       Z-
    *       Z+
    *       Y-
    *       Y+
    *       X-
    *       X+
    * \endcode
    *
    * so a 256x256-per-face cube texture will occupy a 1536x256 image.
    *
    * @param img [in] The image.
    * @param err [in] A string where to write the error info or keep it empty if all was ok.
    * @return The texture pointer. NULL if fails.
    */
    virtual IGraphicsDrvTexture* LoadCubeTexture ( const IImage& img, IString &err ) XNORMAL_NOTHROW = 0;

    /**
    * Unload all the textures previously loaded with LoadTexture() from VRAM
    */
    virtual void UnloadAllTextures() XNORMAL_NOTHROW = 0;

public:
    /**
    * Load a mesh into VRAM
    *
    * @param msh [in] The mesh.
    *
    * @param highpolyMesh [in] TRUE if is a highpolygon model, so you have only to consider positions and normals.
    * @param err [in] A string where to write the error info or keep it empty if all was ok.
    * @param pCall [in] An optional pointer to a progress callback or NULL to ignore it
    *
    * @return A valid IGraphicsDrvMesh pointer if no error or NULL if error.
    *
    * @remarks Cage won't be loaded because cqges are paint in realtime from system-memory to video-memory because are too "dynamic".
    */
    virtual IGraphicsDrvMesh* LoadMesh ( IPolyMesh &msh, const bool highpolyMesh, IString &err, IProgressCallback* pCall ) XNORMAL_NOTHROW = 0;

    /**
    * Unload all the meshes previously loaded with LoadMesh() from VRAM
    */
    virtual void UnloadAllMeshes() XNORMAL_NOTHROW = 0;

    /**
    * Force all the meshes added by the IGraphicsDrv::LoadMesh() to be send to
    * the graphics card and loaded into VRAM.
    *
    * @param pCall [in] An optional pointer to a progress callback or NULL to
    * ignore it
    *
    * @remarks This is usually called by the xNormal 3D viewer once all the
    * models have been loaded. The implementation can ignore it if needed and 
    * load the meshes into VRAM on each IGraphicsDrv::LoadMesh().
    * However, some implementation could benefit from this flush approach so
    * we included this method.
    */
    virtual void FlushMeshes( IProgressCallback *pCall ) XNORMAL_NOTHROW = 0;

public:
    /**
    * Draw a mesh
    *
    * @param mesh [in] The mesh to draw. Pass here an IGraphicsDrvMesh pointer
    * from any IGraphicsDrv::LoadMesh() call
    *
    * @param baseTex [in] The base texture ( .rgb=color, .a=opacity )
    * @param aoLightmapTex [in] The ambient occlusion/lightmap texture ( 
    * modulates the base texture, alpha channel is ignored )
    *
    * @param emissiveTex [in] The emissive texture ( alpha channel is ignored )
    * @param normalAndDisplaceTex [in] The normal map ( in .rgb ) and height
    *       map ( bump/signed height/dispacement map in .a )
    *
    * @param nmType [in] The type of the normal map to use. It can be an 
    * object-space normal map, a tangent-space one or a derivative map.
    * This parameter will be ignored if normalAndDisplaceTex is NULL.
    * 
    * @param mshDerivMapBumpScl [in] A scalar value to give the 
    * derivative map more abrupt aspect. This is ignored unless the
    * nmType previous paramenter is set to NORMAL_MAP_TYPE_DERIVATIVE.
    *
    * @param specTex [in] The specular texture ( .rgb=spec color, 
    * .a=spec power )
    * @param reflectTex [in] The cube environment map reflection texture ( 
    * alpha is ignored )
    * @param glossTex [in] The gloss texture modulates the environment map (
    * .rgb=color, .a=blend mix factor )
    * 
    * @param diffuseGITex [in] The diffuse cubemap lighting probe ( 
    * alpha is ignored )
    * @param diffuseGIIntensity [in] The diffuse cubemap lighting intensity ( 
    * range [0.0f,1.0f] means the blend factor between conventional llighting
    * and the GI lighting )
    *
    * @param viewNormalsShader [in] TRUE to see the per-pixel normals applied
    * to the model
    *
    * @param viewEdges [in] TRUE to see the model wireframe
    *
    * @param parallaxStrength [in] Parallax "strength" factor ( 
    * usually in range [0.0f,8.0f] ).
	* If you set it greater than 0.017f, the implementation should enable 
    * parallax mapping.
	* If it's set to 0.017f or less then the implementation can disable 
    * parallax mapping.
    *
    * @param useFresnel [in] TRUE to use fresnel reflections
    * @param fresnelRefractiveIndex [in] The Schilick reflection index factor (1.0=air,1.33=water, 2.61=diamond ).
    * @param hdrReflectMult [in] An HDR multiplier factor for the texture ( because xNormal can only store images in range [0,1] ).
    *
    * @param castShadows [in] TRUE to cast shadows, FALSE to not
    * @param receiveShadows [in] TRUE if the mesh will receive shadows from other objects.
    * @param backfaceCull [in] TRUE to use back-face culling or FALSE to paint the mesh two-sided.
    * 
    * @param transparencyMode [in] The transparency mode to use for this mesh
    * @param alphaTestValue [in] When the transparency mode is set to 1bit, 
    * any pixel with an alpha texel value less than that will be discarded 
    * from drawing. If you don't use 1bit transparency this value just will 
    * be ignored.
    *
    * @param matte [in] TRUE to ignore the xNormal default dynamic point light.
    * @param showPerVertexAO [in] TRUE to show only the AO matte.
    * 
    * @param vectorAndDisplaceTex [in] A texture with the vector direction in
    * RGB channels and the displacement length premultiplied.
    * @param vdmTS [in] TRUE if the vector displacement map is in tangent space
    * or FALSE if is in object/world space.
    * @param showVDMSeams [in] TRUE to show the vector displacment mapping UV
    * seams or FALSE if not.
    * @param rotTM [in] Transformation rotation matrix.
    * @param pos [in] The position pivot where to draw the mesh
    *
    * @remarks The driver can, internally, sort the transparencies but 
    * initially the objects should be painted in the specified order
    */
    virtual void DrawMesh ( const IGraphicsDrvMesh& mesh, 
        const IGraphicsDrvTexture* baseTex,
        const IGraphicsDrvTexture* aoLightmapTex, 
        const IGraphicsDrvTexture* emissiveTex,
        const IGraphicsDrvTexture* normalAndDisplaceTex, 
        const eNORMAL_MAP_TYPE nmType, const xnSingle mshDerivMapBumpScl,
        const IGraphicsDrvTexture* specTex,
        const IGraphicsDrvTexture* reflectTex, 
        const IGraphicsDrvTexture* glossTex,
        const IGraphicsDrvTexture* diffuseGITex, 
        const xnSingle diffuseGIIntensity,
        const bool viewNormalsShader, const bool viewEdges,
        const xnSingle parallaxStrength,
        const bool useFresnel, const xnSingle fresnelRefractiveIndex, const xnSingle hdrReflectMult,
        const bool castShadows, const bool receiveShadows, 
        const bool backfaceCull,
        const eTRANSPARENCY_MODE transparencyMode, 
        const xnUInt8 alphaTestValue,
        const bool matte, const bool showPerVertexAO,
        const IGraphicsDrvTexture *vectorAndDisplaceTex, const bool vdmTS, 
        const bool showVDMSeams,
        const Matrix3x3 &rotTM,
        const Vector &pos ) XNORMAL_NOTHROW = 0;

    /**
    * Draw the vertex extrussion cage for a mesh
    * @param msh [in] The mesh with the cage to draw
    * @param col [in] The color and opacity of the cage
    */
    virtual void DrawCage ( const IPolyMesh& msh, const Color& col ) XNORMAL_NOTHROW = 0;

    /**
    * Draw a simple point using the specified color
    * @param pos [in] The 3d position of the point in world space
    * @param col [in] The color to use. Implementation can ignore opacity if
    * required.
    *
    * @remarks The implementation must choose the point size ( for example, 
    * based on camera far range or fixed screen size )
    *
    * @remarks This is intended for only a few points( for example, to mark a
    * few selected ones ). If you pass here tons of triangles performance 
    * WILL suffer!
    */
    virtual void DrawPoint ( const Vector& pos, const Color& col ) XNORMAL_NOTHROW = 0;

public:
    /**
    * Draw 2D text in the screen
    *
    * @param x [in] X(horizontal) screen coordinate in pixels where the 
    * text must start. The xNormal screen starts with [x=0,y=0] at TOP-LEFT.
    * Text alignment is TOP-LEFT too.
    *
    * @param y [in] Y(vertical) screen coordinate in pixels where the text
    * must start. The xNormal screen starts with [x=0,y=0] at TOP-LEFT. 
    * Text alignment is TOP-LEFT too.
    *
    * @param text [in] The text to draw
    * @param color [in] The text color and opacity to use
    *
    * @remarks Usually text should be rendered with 11x11 glyphs
    */
    virtual void DrawTextW ( const xnUInt32 x, const xnUInt32 y, const IString& text, const Color& color ) XNORMAL_NOTHROW = 0;

    /**
    * Measure a text width and height in pixels
    *
    * @param text [in] The text to measure
    * @param width [out] A reference to a variable where to put the 
    * width(in pixels) of the specified text
    * @param height [out] A reference to a variable where to put the 
    * height(in pixels) of the specified text
    */
    virtual void MeasureText ( const IString& text, xnUInt32& width, xnUInt32 &height ) XNORMAL_NOTHROW = 0;

    /**
    * Draw an overlay in the screen ( a textured or non-textured 2D rectangle )
    *
    * @param x [in] The X(horizontal) screen coordinate(in pixels) where the 
    * overlay starts. Notice xNormal uses [x=0,y=0] as origin in the TOP-LEFT.
    *
    * @param y [in] The Y(vertical) screen coordinate(in pixels) where the 
    * overlay starts. Notice xNormal uses [x=0,y=0] as origin in the TOP-LEFT.
    *
    * @param width [in] The width(in pixels) of the overlay.
    * @param height [in] The height(in pixels) of the overlay.
    *
    * @param tex [in] The texture to use. Pass NULL if you don't want a textured overlay.
    *
    * @param color [in] The modulate color and opacity of the overlay
    *
    * @param startU [in] The start U(openGL s) texture coordinate of the overlay in range [0,1].
    * @param startV [in] The start V(openGL t) texture coordinate of the overlay in range [0,1].
    * @param endU [in] The end U(openGL s) texture coordinate of the overlay in range [0,1].
    * @param endV [in] The end V(openGL t) texture coordinate of the overlay in range [0,1].
    *
    * \code
    * Overlays are texture mapped as OpenGL, with the origin at BOTTOM-LEFT. 
    * U grows to right, V grows to up:
    *
    *       (u=0.0f,v=1.0f)-----------(u=1.0f,v=1.0f)
    *              |                         |
    *              |                         |
    *              |                         |
    *              |                         |
    *              |                         |
    *              |                         |
    *       (u=0.0f,v=0.0f)-----------(u=1.0f,v=0.0f)
    * \endcode
    */
    virtual void DrawOverlay ( const xnUInt32 x, const xnUInt32 y, 
        const xnUInt32 width,
        const xnUInt32 height, const IGraphicsDrvTexture* tex, 
        const Color& color,
        const xnSingle startU, const xnSingle startV, 
        const xnSingle endU,
        const xnSingle endV ) XNORMAL_NOTHROW = 0;

    /**
    * Draw a cursor in the screen ( a textured or non-textured 2D rectangle )
    *
    * @param x [in] The X(horizontal) screen coordinate(in pixels) where the 
    * overlay starts. Notice xNormal uses [x=0,y=0] as origin in the TOP-LEFT.
    * @param y [in] The Y(vertical) screen coordinate(in pixels) where the 
    * overlay starts. Notice xNormal uses [x=0,y=0] as origin in the TOP-LEFT.
    *
    * @param width [in] The width(in pixels) of the overlay.
    * @param height [in] The height(in pixels) of the overlay.
    *
    * @param tex [in] The texture to use. Pass NULL if you don't want a textured overlay.
    *
    * @param color [in] The modulate color and opacity of the overlay
    *
    * @param startU [in] The start U(openGL s) texture coordinate of the overlay in range [0,1].
    * @param startV [in] The start V(openGL t) texture coordinate of the overlay in range [0,1].
    * @param endU [in] The end U(openGL s) texture coordinate of the overlay in range [0,1].
    * @param endV [in] The end V(openGL t) texture coordinate of the overlay in range [0,1].
    * 
    * \code
    * Overlays are texture mapped as OpenGL, with the origin at BOTTOM-LEFT.
    * U grows to right, V grows to up:
    *
    *       (u=0.0f,v=1.0f)-----------(u=1.0f,v=1.0f)
    *              |                         |
    *              |                         |
    *              |                         |
    *              |                         |
    *              |                         |
    *              |                         |
    *       (u=0.0f,v=0.0f)-----------(u=1.0f,v=0.0f)
    * \endcode
    * 
    * @remarks The difference with the DrawOverlay() method is that the 
    * DrawCursor() one will paint the cursor in the nearest Z plane, 
    * overwritting any pixel
    */
    virtual void DrawCursor ( const xnUInt32 x, const xnUInt32 y, 
        const xnUInt32 width,
        const xnUInt32 height, const IGraphicsDrvTexture* tex, 
        const Color& color,
        const xnSingle startU, const xnSingle startV, 
        const xnSingle endU,
        const xnSingle endV ) XNORMAL_NOTHROW = 0;

    /**
    * Draw a matrix axix in the screen ( X=red, Y=green, Z=blue )
    *
    * @param pos [in] Position of the matrix in world space
    * @param tm [in] The matrix with the three axis. Axis can be scaled to
    * control the painting size of each axe.
    *
    * @remarks You should draw matrices with depth test
    */
    virtual void DrawMatrix ( const Vector& pos, const Matrix3x3& tm ) XNORMAL_NOTHROW = 0;

    /**
    * Draw a dotted line in the screen
    *
    * @param posOrig [in] Origin of the line in world space
    * @param posTo [in] End point of the line in world space
    * @param col [in] The color of the line. The implementation could ignore
    * the opacity.
    *
    * @remarks You should draw with depth test enabled
    */
    virtual void DrawDottedLine ( const Vector& posOrig, const Vector& posTo, const Color& col ) XNORMAL_NOTHROW = 0;

public:
    /**
    * Draw a 2d selection rectable ( a 2d screen rect with dotted lines border)
    *
    * @param x [in] The X(horizontal) screen coordinate(in pixels) where the 2D
    * rect starts. Notice xNormal uses [x=0,y=0] as origin in the TOP-LEFT
    * @param y [in] The Y(vertical) screen coordinate(in pixels) where the 2D
    * rect starts. Notice xNormal uses [x=0,y=0] as origin in the TOP-LEFT
    *
    * @param width [in] The width(in pixels) of the rect
    * @param height [in] The height(in pixels) of the rect
    *
    * @param lineCol [in] The outer stippled-border color
    * @param fillCol [in] The fill color and opacity
    */
    virtual void Draw2DSelectionRect ( const xnUInt32 x, const xnUInt32 y, 
        const xnUInt32 width, const xnUInt32 height, const Color& lineCol,
        const Color& fillCol ) XNORMAL_NOTHROW = 0;

public:
    /**
    * Create a 3D grid centered at origin (0,0,0) which extends +-width and 
    * +-height units.
    *
    * @param width [in] Radius(width) of the grid in world units
    * @param height [in] Radius(height) of the grid in world units
    *
    * @param gridRows [in] Number of rows to paint. Implementation can ignore 
    * this if needed.
    * @param gridColumns [in] Number of columns to paint. Implementation can
    * ignore this if needed.
    *
    * @param originPoint [in] The grid origin offset ( a value that will added 
    * to all the grid vertices ).
    *
    * @remarks The implementation can alter a few the color between rows and
    * columns to resalt the row and columns intervals.
    *
    * @remarks If the user called this method before, you must release the 
    * previous grid before creating the new one
    */
    virtual void CreateGrid ( const xnSingle width, const xnSingle height,
        const xnUInt32 gridRows, const xnUInt32 gridColumns, 
        const Vector& originPoint ) XNORMAL_NOTHROW = 0;

    /**
    * Draw the grid created with CreateGrid() before
    * @param col [in] Color and opacity of the grid
    */ 
    virtual void DrawGrid ( const Color& col ) XNORMAL_NOTHROW = 0;

public:
    /**
    * Create an spherical starfield
    * @param startRadius [in] The radius of the sphere where to start 
    * creating points
    * @param endRadius [in] The radius of the sphere where to end 
    * creating points
    */
    virtual void CreateStarfield ( const xnSingle startRadius, const xnSingle endRadius ) XNORMAL_NOTHROW = 0;

    /**
    * Draw the starfield created with a previous call to CreateStartFiled()
    */
    virtual void DrawStarfield() XNORMAL_NOTHROW = 0;

public:
    /**
    * Add a dynamic light
    *
    * @param pos [in] Position in world space of the light billboard
    *
    * @param ambientCol [in] Ambient color of the light. Alpha component can be ignored.
    * @param diffCol [in] Diffuse color of the light. Alpha component can be ignored.
    * @param specCol [in] Specular color of the light. Alpha component can be ignored.
    *
    * @param intensity [in] Intensity of the light ( usually in range [0.0f,2.0f] ).
    *
    * @param castShadow [in] TRUE if the light casts shadows or FALSE if not.
    * @param shadowBias [in] Shadow bias ( if the implementation doesn't use shadow maps you can ignore this value ).
    *
    * @param range [in] Light range ( to perform attenuation or  unit-shadowmaps ).
    *
    * @param billboardRadius [in] The billboard radio in world units. 
    * Some implementation can put a billboard where the light is located with, for instance, a bulb icon...
    * Well this param controls that billboard's radius hint.
    *
    * @param areaLightRadius [in] The radius of the area light for computing soft penumbra shadows.
	* @param secCol [in] Trilight's secondary color.
	* @param tertCol [in] Trilight's tertiary color.
	* 
	* @param indirectIntensity [in] Light indirect intensity in range [0.0f,1.0f]. 
    * 0.0f means no indirect lighting will be computed.
    */
    virtual void AddDynamicLight ( const Vector& pos, const Color& ambientCol, 
        const Color& diffCol,
        const Color& specCol, const xnSingle intensity, const bool castShadow, 
        const xnSingle shadowBias, const xnSingle range, 
        const xnSingle billboardRadius, const xnSingle areaLightRadius, 
        const Color& secCol, const Color& tertCol, 
        const xnSingle indirectIntensity ) XNORMAL_NOTHROW = 0;

public:
    /**
    * Begin rendering
    *
    * @param camRot [in] The camera rotation matrix in row-major order 
    * like Direct3D as:\n
    *   X axe = (e11, e12, e13)\n
    *   Y axe = (e21, e22, e23)\n
    *   Z axe = (e31, e32, e33)\n
    * where e31 means 3rd row, 1st colum.
    *
    * @param camPos [in] Camera position in world space
    *
    * @param nearR [in] Camera near range ( put always a greater-than-zero
    * value )
    * @param farR [in] Camera far range ( put it always greater-than the
    * nearR parameter )
    *
    * @param vFov [in] Camera vertical filed of view(FoV) in RADIANS 
    *
    * @param clearCol [in] Background clear color
    *
    * @param useGlow [in] TRUE to use a composited glow effect for the 
    * emissive textures
    * @param emissiveGlowIntensity [in] The intensity of the glow usually 
    * in range [0.0f,8.0f]
    *
    * @param exposure [in] The HDR exposure ( usually in range [0.0f,2.0f] ) 
    * factor.
    * @param hdrThreshold [in] The HDR bright pass threshold ( usually in 
    * range [0.0f,1.0f] ) factor.
    * Any pixel in the framebuffer with .r>=threshold || .g>=threshold || 
    * .b>=threshold will emit a "glow".
    *
    * @param ssaoEnable [in] TRUE to enable ssao, FALSE to disable it.
    * 
    * @param ssaoBlurSize [in] Blur size radius, in pixels
    *
    * @param ssaoBright [in] The factor to control the ssao bright.
    *   Usually in range [0.0f,2.0f]. More bright will make the
    *   resulting image more "white"
    *
    * @param ssaoContrast [in] The factor to control the ssao contrast.
    *   Usually in range [0.0f,2.0f]. More contrast will make the
    *   ao shadow to appear more "black".
    *
    * @param ssaoAtten [in] The quadratic attenuation factor for the SSAO.
    * Usually in range [0.0f,2.0f]. The implementation is free to multiply 
    * it by an adjustment value if required.
    *
    * @param ssaoRadius [in] The trace radius in world space in world space
    * (usually will be passed as the model set radius * factor = ssaoRadius)
    *
    * @param maxTessellationLevel [in] The maximum tessellation level to use
    * for displacement mapping. Usually is in range [0,6], where 0 means
    * "don't use displacement mapping"... and 6 means to subdivide each 
    * triangle six times.
    */
    virtual void BeginPaint ( const Matrix3x3& camRot, const Vector& camPos, 
        const xnSingle nearR,
        const xnSingle farR, const xnSingle vFov, const Color& clearCol,
        const bool useGlow, const xnSingle emissiveGlowIntensity, 
        const xnSingle exposure, const xnSingle hdrThreshold, 
        const bool ssaoEnable, const xnSingle ssaoBlurSize,
        const xnSingle ssaoBright, const xnSingle ssaoContrast,
        const xnSingle ssaoAtten, const xnSingle ssaoRadius,
        const xnUInt32 maxTessellationLevel ) XNORMAL_NOTHROW = 0;

    /**
    * End rendering
    */
    virtual void EndPaint() XNORMAL_NOTHROW = 0;

    /**
    * Flip backbuffer, wait for vsync and present to the window.
    */
    virtual void SwapBuffers () XNORMAL_NOTHROW = 0;

public:
    /**
    * Prepare this graphics driver to turn itself off one second to paint
    * some kind of system common dialog ( OpenFileDialog, SaveFileDialog,
    * ColorDialog... )
    * 
    * @param allow [in] TRUE if xNormal is going to show a system common 
    * dialog or FALSE if xNormal finished to use system common dialogs.
    *
    * @remarks This is for when the user clicks over the "Save meshes" button
    * inside the xNormal 3D viewer or other uses... Some implementation can 
    * just ignore this... but if the implementation uses Direct3D you can 
    * use this to call the method IDirect3DDevice9::SetDialogBoxMode() or to 
    * reset the device or whatever...
    */
    virtual void AllowRenderSystemDialog( const bool allow ) XNORMAL_NOTHROW = 0;

public:
    /**
    * Get the approximated VRAM size used (meshes, textures, framebuffer, etc...)
    */
    virtual xnUInt64 GetUsedVideoMemoryBytes () const XNORMAL_NOTHROW = 0;

public:
    /**
    * Take a screenshot of the current framebuffer ( in any image format, 
    * you choose )
    */
    virtual void TakeScreenshot () XNORMAL_NOTHROW = 0;

    /**
    * Get the framebuffer pixels
    * @return [in] A reference to a sBGRFramebuffer with the BGRA pixels. 
    * The framebuffer returned will be cropped to a divisible-by-16 size 
    * to guarantee the maximum video codec compatibility.
    */
    virtual const IVideoMaker::sBGRData* GetFramebufferForVideoCapture() XNORMAL_NOTHROW = 0;

public:
    /**
    * Get the screen pixel coordinates(in pixels, left-top origin) of a 
    * world-space 3d point.
    *
    * @param wsPos [in] The world space position
    * @param outScreenPos [out] The screen position in pixels, with origin
    * [x=0,y=0] at TOP-LEFT
    * @param zNeeded [in] TRUE to write the .z component on the outScreenPos or FALSE to ignore it.
    */
    virtual void Project ( const Vector& wsPos, Vector& outScreenPos, const bool zNeeded ) const XNORMAL_NOTHROW = 0;

public:
    /**
    * Begin picking mode
    *
    * @param x [in] The X screen coordinate of the 2D-rect picking rectangle
    * in pixels (left-top origin)
    * @param y [in] The Y screen coordinate of the 2D-rect picking rectangle
    * in pixels (left-top origin)
    * @param width [in] The 2D-rect picking rectangle width in pixels
    * @param height [in] The 2D-rect picking rectangle width in pixels
    *
    * @remarks You must call BeginPicking(), EndPicking() and PickVertices()
    * AFTER the BeginPaint()/EndPaint() call
    */
    virtual void BeginPicking ( const xnUInt32 x, const xnUInt32 y, const xnUInt32 width, const xnUInt32 height ) XNORMAL_NOTHROW = 0;

    /**
    * Retrieve the "picked" vertices of the mesh cage selected by the 2d-
    * rectangle specified by a previous BeginPicking() call.
    *
    * @param mesh [in] The mesh to test.
    * @param ignoreOccluded [in] TRUE to ignore the z-buffer occluded vertices
    * or FALSE to test all the vertices including the occluded ones.
    *
    * @param picked [inout] A reference to a pair[faceIdx,vIdx] list to fill 
    * with the picked vertices the user selected with the 2D rect region. 
    * The implementation must clear internally this list before to add any 
    * index.
    * The faceIdx "first" pair member indicates the number of face that owns 
    * that vertex. The vIdx "second" pair member indicates the indices of that
    * vertex in the face and goes in range [0,3]
    * 
    * @remarks If you call this without calling BeginPicking() first or if
    * you call this NOT between BeginPicking() and EndPicking(), an exception 
    * must be thrown.
    *
    * @remarks You must call BeginPicking(), EndPicking() and PickVertices()
    * AFTER the BeginPaint()/EndPaint() call
    */
    virtual void PickCage ( const IPolyMesh &mesh, const bool ignoreOccluded, IArray<pair<xnUInt32,xnUInt32> >& picked ) XNORMAL_NOTHROW = 0;

    /**
    * End picking mode
    *
    * @remarks You must call BeginPicking(), EndPicking() and PickVertices()
    * AFTER the BeginPaint()/EndPaint() call
    */
    virtual void EndPicking () XNORMAL_NOTHROW = 0;

protected:
    virtual ~IGraphicsDrv ()
    {
    }
};

