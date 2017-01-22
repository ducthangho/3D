#if 0
    This file is part of the xNormal 3 SDK, formatted for Doxygen/JavaDoc 
    documentation system. 
    (c) 2005-2015, S.Orgaz.
    Contact: granthill76 'at' yahoo.com
    Visit http://www.xnormal.net for more info
 
    FOR MORE INFO ABOUT THE LICENSE, CONSULT THE XNORMAL PDF DOCUMENTATION
#endif //0 comment

/**
* An image is a 2D array of colors
*/
template <typename T>
class basic_XN_image
{
public:
    virtual void Release() XNORMAL_NOTHROW = 0;

public:
    /**
    * Get the width (in pixels) of this image
    */
    virtual xnUInt32 GetWidth () const XNORMAL_NOTHROW = 0;
    
    /**
    * Get the height (in pixels) of this image
    */
    virtual xnUInt32 GetHeight () const XNORMAL_NOTHROW = 0;

    bool IsEmpty() const XNORMAL_NOTHROW
    {
        return ( 0U==this->GetWidth() || 0U==this->GetHeight() );
    }

    /**
    * Resize the image
    * @param width [in] The new width, in pixels, for the image. Can be zero to release the pixels.
    * @param height [in] The new height, in pixels, for the image. Can be zero to release the pixels.
    * @remarks The image will be initialized to transparent black color and all the previous pixels will be invalidated.
    */
    bool Resize ( const xnUInt32 width, const xnUInt32 height ) XNORMAL_NOTHROW
    {
        return this->ResizeP ( width, height );
    }

    /*
    * Get the RAM memory occupation of this image for statistical control.
    * @return The number of bytes in RAM used by this image
    */
    xnUInt64 GetMemoryOccupationInBytes () const XNORMAL_NOTHROW
    {
        return static_cast<xnUInt64>(this->GetWidth()*this->GetHeight()*sizeof(T));
    }

public:
    /**
    * Get a pixel from the image using a (X,Y) coordinate
    *
    * @param x [in] The horizontal coordinate ( in pixels ) of the pixel
    * to retrieve. Remember that the image data origin is specified at 
    * BOTTOM-LEFT, so (x=0,y=0) is the bottom left and the
    * (x=img.width-1,y=img.height-1) is the TOP-RIGHT.
    *
    * @param y [in] The vertical coordinate ( in pixels ) of the pixel
    * to retrieve. Remember that the image data origin is specified at 
    * BOTTOM-LEFT, so (x=0,y=0) is the bottom left and the
    * (x=img.width-1,y=img.height-1) is the TOP-RIGHT.
    *
    * @return A pixel struct reference
    *
    * @remarks An assert will be fired if you specify (X,Y) outbounds
    */
    const T& GetPixel ( const xnUInt32 x, const xnUInt32 y ) const XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( x<this->GetWidth() );
        XNORMAL_ASSERT ( y<this->GetHeight() );
        return this->GetPixelP ( x, y );
    }

    /**
    * Get a pixel in the image using UVs instead of XY pair 
    * ( useful for texture fetch )
    *
    * @param uv [in] UV pair(OpenGL st) texture map coordinates
    * in range [0.0f,1.0f].
    * Values are automatically clamped in range [0.0f,1.0f] if you pass,
    * for example, an UV of [90.43f,-100.0f]
    *
    * @return A reference to the pixel
    */
    const Color& FetchUV ( const Vector2& uv ) const XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( false==m_stlLines.empty() );

        const xnUInt32 x ( static_cast<xnUInt32>(Utils::Clamp(uv.x,1.0f,0.0f)*static_cast<xnSingle>(this->GetWidth()-1U)) );
        const xnUInt32 y ( static_cast<xnUInt32>(Utils::Clamp(uv.y,1.0f,0.0f)*static_cast<xnSingle>(this->GetHeight()-1U)) );
        return this->GetPixel(x,y);
    }

    /**
    * Set a pixel in the image
    *
    * @param x [in] The horizontal coordinate ( in pixels ) of the pixel
    * to retrieve. Remember that the image data origin is specified at
    * BOTTOM-LEFT, so (x=0,y=0) is the bottom left and the
    * (x=img.width-1,y=img.height-1) is the TOP-RIGHT.
    *
    * @param y [in] The vertical coordinate ( in pixels ) of the pixel
    * to retrieve. Remember that the image data origin is specified at
    * BOTTOM-LEFT, so (x=0,y=0) is the bottom left and the
    * (x=img.width-1,y=img.height-1) is the TOP-RIGHT.
    *
    * @param pix [in] A pixel struct reference where to copy the data FROM
    */
    void SetPixel ( const xnUInt32 x, const xnUInt32 y, const T& pix ) XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( x<this->GetWidth() );
        XNORMAL_ASSERT ( y<this->GetHeight() );
        this->SetPixelP ( x, y, pix );
    }

public:
    /**
    * Calculate the average color of the image
    * @return The average image color
    */
    virtual T GetAverageColor () const XNORMAL_NOTHROW = 0;

public:
    /**
    * Tries to adjust all the components to range [MAXV,MINV]
    * (1.0f,0.0f usually) using the maximum LUMINANCE found
    * @param maxv [in] Maximum value ( maxmin is 0.0f, maxmax is 1.0f )
    * @param minv [in] Minimum value ( minmin is 0.0f, minmax is 1.0f )
    * @param pCall [in] An optional callback to receive task info or NULL
    * if not needed
    * @return Max luminance found
    * @remarks An exception will be raised on error
    */
    virtual xnSingle Normalize ( const xnSingle maxv, const xnSingle minv, IProgressCallback* pCall ) XNORMAL_NOTHROW = 0;

    /**
    * Normalize the image using the maximum R, G, B value... and, optionally,
    * unbias the image(pass range from [-1,1] to [1,0])
    * 
    * @param unbias [in] TRUE to pass the image from range [-1,1] to [0,1]
    * after the normalization occurs.
    * 
    * @param pCall [in] An optional callback to receive task info or NULL
    * if not needed
    */
    virtual void RGBMaxNormalize ( const bool unbias, IProgressCallback* pCall ) XNORMAL_NOTHROW = 0;

public:
    /**
    * Clear the image using a color
    */
    virtual void Clear ( const T& c ) XNORMAL_NOTHROW = 0;

public:
    /**
    * Test if the image has any alpha set to a value greater than zero and less than one
    * @return true is there is a pixel with alpha greater than zero and less than one
    * @remarks This method can be computationally expensive because each pixel is tested... so if you have to call several times this method we encourage you to store the first call's result in a variable
    **/
    virtual bool HasTrueAlpha () const XNORMAL_NOTHROW = 0;

    /**
    * Test if the colors in the image are in LDR(range [0.0f,1.0f]) or HDR(values can be greater than 1.0f or even negative!)
    * @param includeAlpha [in] A boolean indicating if the alpha channel should be tested or excluded from the test
    * @return true if any pixel has an element outside the range [0.0f,1.0f], false if all the pixels in the image have their color elements strictly in range [-0.0f,1.0f]
    * @remarks This method can be computationally expensive because each pixel is tested... so if you have to call several times this method we encourage you to store the first call's result in a variable
    **/
    virtual bool IsHDR ( const bool includeAlpha ) const XNORMAL_NOTHROW = 0;

    /**
    * Sure all the pixels have their color elements in range [minv,maxv]
    * @param minv [in] The minimum value. If any pixel's color element has a value less than this its new value will be the minimum value. Example: if ( c.r<minv ) c.r = minv
    * @param maxv [in] The maximum value. If any pixel's color element has a value greater than this its new value will be the maximum value. Example: if ( c.r>maxv ) c.r = maxv
    * @param alphaToo [in] A boolean indicating if you want the alpha value clamped also or excluded
    **/
    virtual void Clamp ( const xnSingle minv, const xnSingle maxv, const bool alphaToo ) XNORMAL_NOTHROW = 0;

public:
    virtual bool CopyTo ( basic_XN_image<T> &img ) const XNORMAL_NOTHROW = 0;

    static void CopyRect ( const basic_XN_image<T> &src, const xnUInt32 srcX, const xnUInt32 srcY, const xnUInt32 width, const xnUInt32 height, basic_XN_image<T> &dst, const xnUInt32 dstX, const xnUInt32 dstY ) XNORMAL_NOTHROW
    {
        for ( xnUInt32 yy=srcY; yy<srcY+height; ++yy )
        {
            for ( xnUInt32 xx=srcX; xx<srcX+width; ++xx )
            {
                const Color &col ( src.GetPixel ( xx, yy ) );
                dst.SetPixel ( dstX  + (xx-srcX), dstY + (yy-srcY), col );
            }
        }
    }


protected:
    virtual ~basic_XN_image ()
    {
    }

    virtual const T& GetPixelP ( const xnUInt32 x, const xnUInt32 y ) const XNORMAL_NOTHROW = 0;
    virtual void SetPixelP ( const xnUInt32 x, const xnUInt32 y, const T& pix ) XNORMAL_NOTHROW = 0;

    virtual bool ResizeP ( const xnUInt32 width, const xnUInt32 height ) XNORMAL_NOTHROW = 0;
};

typedef basic_XN_image<Color> IImage;
