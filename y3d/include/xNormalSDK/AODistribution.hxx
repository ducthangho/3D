#if 0
    This file is part of the xNormal 3 SDK, formatted for Doxygen/JavaDoc 
    documentation system. 
    (c) 2005-2015, S.Orgaz.
    Contact: granthill76 'at' yahoo.com
    Visit http://www.xnormal.net for more info
 
    FOR MORE INFO ABOUT THE LICENSE, CONSULT THE XNORMAL PDF DOCUMENTATION
#endif //0 comment

/**
* Kind of ambient occlusion distribution
* To see what is ambient occlusion see 
* http://en.wikipedia.org/wiki/Ambient_occlusion
*/
enum eAO_DISTRIBUTION
{
    /**
    * Uniform distribution gives you a very constant and rough ambient 
    * occlusion
    */
    AO_DISTRIBUTION_UNIFORM = 0,

    /**
    * Cosine distribution makes the ambient occlusion to be less pronounced 
    * because it will assign more weight to the rays those hit perpendicular
    * to the surface normal.
    */
    AO_DISTRIBUTION_COSINE, 

    /**
    * Like the cosine distribution, but preserves better detail of wrinkles 
    * pores and roughness.
    */
    AO_DISTRIBUTION_COSINE_SQ
};
