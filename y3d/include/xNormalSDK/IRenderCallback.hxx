#if 0
    This file is part of the xNormal 3 SDK, formatted for Doxygen/JavaDoc 
    documentation system. 
    (c) 2005-2015, S.Orgaz.
    Contact: granthill76 'at' yahoo.com
    Visit http://www.xnormal.net for more info
 
    FOR MORE INFO ABOUT THE LICENSE, CONSULT THE XNORMAL PDF DOCUMENTATION
#endif //0 comment


/**
* An interface to notify a portion of the renderer was completed to show it 
* into the preview window or to save it to the disk
*/
class IRenderCallback
{
public:
    /**
    * Mandatory virtual destructor
    */
    virtual ~IRenderCallback() {}

public:
    /**
    * Prepare the implementation to render tiles
    * @param width [in] Width (in pixels) of the framebuffer
    * @param height [in] Height (in pixels) of the framebuffer
    * @param tileSize [in] Tile size (in pixels, squared)
    * @param tilesHoriz [in] Total number of horizontal tiles
    * @param tilesVert [in] Total number of vertical tiles
    * @param backColARGB [in] The background color, in ARGB 32bpp format
    *
    * @remarks This will be called BEFORE the renderer starts to send us 
    * the tiles rendered
    * @remarks The total number of tiles to be rendered will be 
    * tilesHoriz * tilesVert
    */
    virtual void OnBeginRenderTiles ( const xnUInt32 width, 
        const xnUInt32 height,
        const xnUInt32 tileSize, const xnUInt32 tilesHoriz, 
        const xnUInt32 tilesVert,
        const xnUInt32 backColARGB ) = 0; 

    /**
    * This will be called just before the renderer is going to write a tile
    * @param tileX [in] Framebuffer horizontal coordinate(in pixels) of the
    * tile rendered. Remember the origin (0,0) corresponds to the 
    * BOTTOM-LEFT corner.
    * @param tileY [in] Framebuffer horizontal coordinate(in pixels) of the
    * tile rendered. Remember the origin (0,0) corresponds to the 
    * BOTTOM-LEFT corner.
    * @param threadID [in] ThreadID of the renderer ( to paint a rectangle
    * of a color, etc )
    * @remarks A preview window could use this to paint bucket's rectangle
    * to indicate the
    *   user that the renderer is going to render this portion of the image
    */
    virtual void OnTileActivate ( const xnUInt32 tileX, const xnUInt32 tileY,
        const xnUInt32 threadID ) = 0;

    /**
    * This will be called once the renderer completed a tile rendered
    * @param tileX [in] Framebuffer horizontal coordinate(in pixels) of the
    * tile rendered. Remember the origin (0,0) corresponds to the 
    * BOTTOM-LEFT corner.
    * @param tileY [in] Framebuffer horizontal coordinate(in pixels) of the
    * tile rendered. 
    *   Remember the origin (0,0) corresponds to the BOTTOM-LEFT corner.
    * @param threadID [in] ThreadID of the renderer ( to paint a rectangle of 
    * a color, etc )
    * @param tileImg [in] An image with the RGBA pixels of the tile.
    */
    virtual void OnTileRendered ( const xnUInt32 tileX, const xnUInt32 tileY,
        const xnUInt32 threadID,
        const Image* tileImg ) = 0;

    /**
    * Finish rendered occured. Now you can flush and write any pending data.
    */
    virtual void OnEndRenderTiles( Image* fb=0 ) = 0;
};


