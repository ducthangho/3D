#if 0
    This file is part of the xNormal 3 SDK, formatted for Doxygen/JavaDoc 
    documentation system. 
    (c) 2005-2015, S.Orgaz.
    Contact: granthill76 'at' yahoo.com
    Visit http://www.xnormal.net for more info
 
    FOR MORE INFO ABOUT THE LICENSE, CONSULT THE XNORMAL PDF DOCUMENTATION
#endif //0 comment


/**
* Basic trait(syntax sugar) for some templates like the basic_vector (
* only for internal use )
*/
template <typename T>
struct basic_XN_trait
{
    /**
    * Value type of the template
    */
    typedef T value_type;

    /**
     * Value pointer of the template
    */
    typedef T* value_pointer;

    /**
     * Value reference of the template
    */
    typedef T& value_ref;
};


