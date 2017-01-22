#if 0
    This file is part of the xNormal 3 SDK, formatted for Doxygen/JavaDoc 
    documentation system. 
    (c) 2005-2015, S.Orgaz.
    Contact: granthill76 'at' yahoo.com
    Visit http://www.xnormal.net for more info
 
    FOR MORE INFO ABOUT THE LICENSE, CONSULT THE XNORMAL PDF DOCUMENTATION
#endif //0 comment


/**
* An interface for the bump mapping tangent basis calculator and vertex normal
* calculation too
*/
class ITangentBasisCalculator : public IPlugin 
{
public:
    /**
    * Default constructor
    * @param xnh [in] The xNormal host pointer ( will be passed automatically
    * by the xNormal plug-in manager )
    */
    ITangentBasisCalculator ( IHost &xnh ) : IPlugin(xnh)
    {
    }

    ePLUGIN_TYPE GetPluginType() const XNORMAL_NOTHROWOVR
    {
        return PLUGIN_TYPE_TANGENT_BASIS_CALCULATOR;
    }

public:
    /**
    * Average the vertex normals for the specified polygonal mesh
    *
    * @param polyMsh [in] A reference to the polygonal mesh.
    *
    * @param err [inout] A string where you can place error info or keep it empty to indicate all was ok.
    *
    * @param pCall [in] An optional callback pointer to notify the task progress. Can be NULL.
    *
    * @remarks After this, the tangent basis will be invalid and you will need 
    * to call the CalculateTangentBasis() method to calculate it.
    *
    */
    virtual void AverageVertexNormals ( IPolyMesh& polyMsh, IString &err, IProgressCallback* pCall ) const XNORMAL_NOTHROW
    {
        polyMsh.AverageVertexNormals ( err, pCall );
    }

    /**
    * Harden the vertex normals for the specified polygonal mesh
    *
    * @param polyMsh [in] A reference to the polygonal mesh.
    *
    * @param err [inout] A string where you can place error info or keep it empty to indicate all was ok.
    *
    * @param pCall [in] An optional callback pointer to notify the task progress. Can be NULL.
    *
    * @remarks After this, the tangent basis will be invalid and you will 
    * need to call the CalculateTangentBasis() method to calculate it.
    */
    virtual void HardenVertexNormals ( IPolyMesh& polyMsh, IString &err, IProgressCallback* pCall ) const XNORMAL_NOTHROW
    {
        polyMsh.HardenVertexNormals ( err, pCall );
    }

    /**
    * Calculate the per-vertex tangent basis for the specified polygonal mesh.
    *
    * @param msh [in] The input mesh to calculate the tangent basis.
    *
    * @param err [inout] A string where you can place error info or keep it empty to indicate all was ok.
    *
    * @param bIsDerivMapBake [in] "true" if xNormal is asking the plugin to compute 
    * the tangent basis for a derivative height map or "false" if it's asking 
    * to compute the tangent basis for a tangent-space normal map.
    *
    * @param pCall [in] An optional callback pointer to notify the task progress. Can be NULL.
    *
    * @returns If the bIsDerivMapBake is true then youo must return a suggested
    * initial scale for derivative maps. If bIsDerivMapBake is false ( so xNormal
    * is asking you just to compute the tangent basis for normal maps ) then the
    * return value will be ignored.
    */
    virtual xnSingle CalculateTangentBasis ( IPolyMesh& msh, const bool bIsDerivMapBake, IString &err, IProgressCallback* pCall ) const XNORMAL_NOTHROW
    {
        return msh.CalculateTangentBasis ( bIsDerivMapBake, err, pCall );
    }

public:
    /**
    * The inerpolated tangent basis structure used by some methods later.
    */
    struct InterpolatedTB XNORMAL_FINALCLASS
    {
        /**
        * UN-normalized interpolated normal
        **/
        Vector norm;

        /**
        * UN-normalized interpolated biNormal
        **/
        Vector biNorm;

        /**
        * UN-normalized interpolated tangent
        **/
        Vector tan;
    };

    /**
    * Convert object-space normal to tangent-space normal using a custom
    * transformation at texel-level for a tangent-space normal map.
    *
    * @param itb [in] The Gouraud-shaded-linear-interpolated tangent basis,
	* unnormalized, as is directly computed by the rasterizer's interpolator.
    *
    * @param objSpaceNorm [in] The highpoly's object-space normal to be 
    * transformed. It may be normalized or not ( it's passed directly from 
    * the mesh importer or from a normal map ).
    *
    * @return The resulting tangent space normal. It won't be touched by 
    * xNormal. No normalization or orthogonalization will be performed... 
    * will be used as is returned, untouched ( but will be unbiased to map
    * components into [0,1] to save it in, for example, a 32bpp 
    * (4x unsigned char) TGA file )
    * 
    */
    virtual Vector OS2TS ( const InterpolatedTB& itb, const Vector& objSpaceNorm ) const XNORMAL_NOTHROW
    {
        //Invert the tangent basis. Like the normal/biNormal/tangent should be orthogonal we can just transpose it to save some cycles..
        Matrix3x3 l_tm;
        l_tm.e11 = itb.tan.x;
        l_tm.e21 = itb.tan.y;
        l_tm.e31 = itb.tan.z;

        l_tm.e12 = itb.biNorm.x;
        l_tm.e22 = itb.biNorm.y;
        l_tm.e32 = itb.biNorm.z;

        l_tm.e13 = itb.norm.x;
        l_tm.e23 = itb.norm.y;
        l_tm.e33 = itb.norm.z;

        //Convert the object-space normal multiplying it by the transposed (tangent basis)
        Vector l_vRes;
        Matrix3x3::TransformVector ( objSpaceNorm, l_tm, &l_vRes );
        return l_vRes;
    }

    /**
    * Get the pixel-shader osbject-space to tangent-space routine for OpenGL/OpenRL, which must be equal to the OS2TS ( const InterpolatedTB& itb, const Vector& objSpaceNorm )
    **/
    virtual const char* OS2TSScripted () const XNORMAL_NOTHROW
    {
        return "vec3 OS2TS ( vec3 osNorm, vec3 tan, vec3 biNorm, vec3 norm ) {"
               "   return osNorm * mat3(tan,biNorm,norm); "
               "}";
    }

    /**
    * Convert object-space normal to tangent-space normal using a custom
    * transformation at texel-level for a derivative height map.
    *
    * @param itb [in] The Gouraud-shaded-linear-interpolated tangent basis,
	* unnormalized, as is directly computed by the rasterizer's interpolator.
    *
    * @param objSpaceNorm [in] The highpoly's object-space normal to be 
    * transformed. It may be normalized or not ( it's passed directly from 
    * the mesh importer or from a normal map ).
    *
    * @return The resulting tangent space normal. It won't be touched by 
    * xNormal. No normalization or orthogonalization will be performed... 
    * will be used as is returned, untouched.
    */
    virtual Vector OS2TSDerivative ( const InterpolatedTB& itb, const Vector& objSpaceNorm ) const XNORMAL_NOTHROW
    {
        return Vector(0.0f); // we don't support derivative maps by default.
    }

public:
    /**
    * Convert a tangent-space normal to object-space using a custom 
    * transformation at texel-level.
    *
    * @param itb [in] The Gouraud-shaded-linear-interpolated tangent basis, 
	* unnormalized, as is directly computed by the rasterizer's interpolator.
    *
    * @param tanSpaceNorm [in] The tangent-space normal to be transformed... 
    * untouched, fetched directly from the normal map image ( that means
    * it can be normalized or not! ).
    *
    * @return The resulting tangent space normal. It won't be touched by
    * xNormal... No normalization or orthogonalization will be performed... 
    * will be used as is returned, untouched ( but will be unbiased to map 
    * components into [0,1] to save it in, for example, a 32bpp 
    * (4x unsigned char) TGA file )
    * 
    */
    virtual Vector TS2OS ( const InterpolatedTB& itb, const Vector& tanSpaceNorm ) const XNORMAL_NOTHROW
    {
        Matrix3x3 l_tm ( itb.tan, itb.biNorm, itb.norm );
        Vector l_vRes;
        Matrix3x3::TransformVector ( tanSpaceNorm, l_tm, &l_vRes);
        return l_vRes;
    }

    /**
    * Get the pixel-shader tangent-space to object-space routine for OpenGL/OpenRL, which must be equal to the TS2OS ( const InterpolatedTB& itb, const Vector& tsSpaceNorm )
    **/
    virtual const char* TS2OSScripted () const XNORMAL_NOTHROW
    {
        return "vec3 TS2OS ( vec3 tsNorm, vec3 tan, vec3 biNorm, vec3 norm ) {"
                "   return tsNorm * mat3 ( tan.x,biNorm.x,norm.x, tan.y,biNorm.y,norm.y,  tan.z,biNorm.z,norm.z);" 
               "}";
    }

    /**
    * Convert a tangent-space normal to object-space using a custom 
    * transformation at texel-level for a derivative height map.
    *
    * @param itb [in] The Gouraud-shaded-linear-interpolated tangent basis, 
	* unnormalized, as is directly computed by the rasterizer's interpolator.
    *
    * @param tanSpaceNorm [in] The tangent-space normal to be transformed... 
    * untouched, fetched directly from the normal map image ( that means
    * it can be normalized or not! ).
    *
    * @return The resulting tangent space normal. It won't be touched by
    * xNormal... No normalization or orthogonalization will be performed...
    * 
    */
    virtual Vector TS2OSDerivative ( const InterpolatedTB& itb, const Vector& tanSpaceNorm ) const XNORMAL_NOTHROW
    {
        return Vector ( 0.0f ); // we don't support derivative maps by default.
    }

public:
    /**
    * Method to indicate to xNormal if this calculator supports 
    * derivative normal maps or not.
    * @return "true" if supports derivative normal maps or "false" if not.
    **/
    virtual bool SupportsDerivativeNormalMaps () const XNORMAL_NOTHROW
    {
        return false; // we don't support derivative normal maps by default
    }

    /**
    * Scale the derivative map's pixel
    * @param tangSpaceNorm [in] The derivative map's pixel.
    * @param bump_scale [in] The scale the user wants to apply.
    **/
    virtual Vector ScaleBumpDerivative ( const Vector& tangSpaceNorm, const float bump_scale ) const XNORMAL_NOTHROW
    {
        return Vector ( 0.0f ); // we don't support derivative maps by default.
    }

    /**
    * Suggest the initial bump scale for rendering.
    *
    * @param polyMsh [in] The mesh the implemention can use to compute an initial
    * bump scale value.
    * 
    * @param dmWidth [in] The derivative map's width, in pixels.
    *
    * @param dmHeight [in] The derivative map's height, in pixels.
    *
    * @returns The initial bump scale value. If the implemention does not support derivative maps then you should return 0.0f.
    */
    virtual xnSingle SuggestInitialBumpScaleValueForRendering ( const IPolyMesh &polyMsh, const xnUInt32 dmWidth, const xnUInt32 dmHeight ) const XNORMAL_NOTHROW
    {
        return 0.0f;
    }

protected:
    virtual ~ITangentBasisCalculator ()
    {
    }
};

