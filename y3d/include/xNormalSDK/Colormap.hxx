#if 0
    This file is part of the xNormal 3 SDK, formatted for Doxygen/JavaDoc 
    documentation system. 
    (c) 2005-2015, S.Orgaz.
    Contact: granthill76 'at' yahoo.com
    Visit http://www.xnormal.net for more info
 
    FOR MORE INFO ABOUT THE LICENSE, CONSULT THE XNORMAL PDF DOCUMENTATION
#endif //0 comment

#ifdef XNORMAL_COMPILER_VSTUDIO
    #pragma pack(4)
#endif

/**
* Internal colormap pixel structure. To save space in memory the pixels in the image are
* packed into this structure. Is an internal structure, is NOT for general use.
*/
struct ColormapPixel XNORMAL_FINALCLASS
{
    ColormapPixel() : col(0)
    {
    }

    ColormapPixel( const xnUInt32 c ) : col(c)
    {
    }

    ColormapPixel( const ColormapPixel &p ) : col(p.col)
    {
    }

    bool IsWritten () const
    {
        return ((col&0xFF000000)!=0);
    }

    operator Color() const
    {
        return Color(col);
    }

    ColormapPixel& operator = ( const xnUInt32 c )
    {
        col = c;
    }

    ColormapPixel& operator = ( const Color& c )
    {
        col = c.ToARGB();
    }

    /**
    * 0xAARRGGBB 8-bit color
    */
    xnUInt32 col;

};

#ifdef XNORMAL_COMPILER_VSTUDIO
    #pragma pack()
#endif

/**
* A color map is an image with 0xAARRGGBB 8-bit components
* Range of the pixels is [0,255].
*/
class Colormap XNORMAL_FINALCLASS : public basic_XN_image<ColormapPixel>
{
public:
    /**
    * Default constructor from with and height
    * @param width [in] The width of the map in pixels
    * @param height [in] The height of the map in pixels
    * @remarks Initially all the pixels will be marked with zero flags(and all its values will be
    *   set to zero too) so all the pixel will be invalid until you do a SetPixel() over them with
    *   the correct pixel flags assigned
    */
    Colormap ( const xnUInt32 width, const xnUInt32 height ) :
        basic_XN_image<ColormapPixel>(width,height)
    {
    }
};
