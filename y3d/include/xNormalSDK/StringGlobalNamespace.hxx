#if 0
    This file is part of the xNormal 3 SDK, formatted for Doxygen/JavaDoc 
    documentation system. 
    (c) 2005-2015, S.Orgaz.
    Contact: granthill76 'at' yahoo.com
    Visit http://www.xnormal.net for more info
 
    FOR MORE INFO ABOUT THE LICENSE, CONSULT THE XNORMAL PDF DOCUMENTATION
#endif //0 comment


/**
* Left compare for the String class to make things like this:\n
* \code
* String l_str(L"dog");
* if ( "hello"==l_str )
* {
*   ...
* }
* \endcode
*/
XNORMAL_INLINE bool operator == ( const char* c, const XNORMAL_SDK_NAMESPACE::String& str )
{
    return (str==XNORMAL_SDK_NAMESPACE::String(c));
}

/**
* Left compare for the String class to make things like this:\n
* \code
* String l_str(L"dog");
* if ( L"hello"==l_str )
* {
*   ...
* }
* \endcode
*/
XNORMAL_INLINE bool operator == ( const wchar_t* c, 
    const XNORMAL_SDK_NAMESPACE::String& str )
{
    return (str==XNORMAL_SDK_NAMESPACE::String(c));
}

XNORMAL_INLINE bool operator != ( const char* c, 
    const XNORMAL_SDK_NAMESPACE::String& str )
{
    return (XNORMAL_SDK_NAMESPACE::String(c)!=str);
}

XNORMAL_INLINE bool operator != ( const wchar_t* c, 
    const XNORMAL_SDK_NAMESPACE::String& str )
{
    return (XNORMAL_SDK_NAMESPACE::String(c)!=str);
}

XNORMAL_INLINE bool operator < ( const wchar_t* c, 
    const XNORMAL_SDK_NAMESPACE::String& str )
{
    return (XNORMAL_SDK_NAMESPACE::String(c)<str);
}

XNORMAL_INLINE bool operator <= ( const wchar_t* c, 
    const XNORMAL_SDK_NAMESPACE::String& str )
{
    return (XNORMAL_SDK_NAMESPACE::String(c)<=str);
}

XNORMAL_INLINE bool operator > ( const wchar_t* c, 
    const XNORMAL_SDK_NAMESPACE::String& str )
{
    return (XNORMAL_SDK_NAMESPACE::String(c)>str);
}

XNORMAL_INLINE bool operator >= ( const wchar_t* c, 
    const XNORMAL_SDK_NAMESPACE::String& str )
{
    return (XNORMAL_SDK_NAMESPACE::String(c)>=str);
}

XNORMAL_INLINE XNORMAL_SDK_NAMESPACE::String operator + ( const char* c, 
    const XNORMAL_SDK_NAMESPACE::String& str )
{
    return (XNORMAL_SDK_NAMESPACE::String(c)+str);
}

XNORMAL_INLINE XNORMAL_SDK_NAMESPACE::String operator + ( const wchar_t* c, 
    const XNORMAL_SDK_NAMESPACE::String& str )
{
    return (XNORMAL_SDK_NAMESPACE::String(c)+str);
}

