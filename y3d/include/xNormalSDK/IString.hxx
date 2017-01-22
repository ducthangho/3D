#if 0
    This file is part of the xNormal 3 SDK, formatted for Doxygen/JavaDoc 
    documentation system. 
    (c) 2005-2015, S.Orgaz.
    Contact: granthill76 'at' yahoo.com
    Visit http://www.xnormal.net for more info
 
    FOR MORE INFO ABOUT THE LICENSE, CONSULT THE XNORMAL PDF DOCUMENTATION
#endif //0 comment

/**
* An string class helper to convert between/to UTF8.
* 
*/
class IString
{
public:
    virtual void Release() XNORMAL_NOTHROW = 0;

    /**
    * Test if the basic_string is empty
    */
    virtual bool IsEmpty() const XNORMAL_NOTHROW = 0;

    /**
    * Return the number of CHARACTERS in the basic_string(NOT counting the zero terminator)
    */
    virtual xnUInt32 GetLength ( void ) const XNORMAL_NOTHROW = 0; 
    
    /**
    * Clear the string contents
    */
    virtual void Clear () XNORMAL_NOTHROW = 0;

public:
    /**
    * Get the raw unicode char pointer for reading.
    **/
    virtual const wchar_t* GetPtr () const XNORMAL_NOTHROW = 0;

    /**
    * Get the raw UTF8 char pointer for reading.
    **/
    virtual const char* GetPtrUTF8 () const XNORMAL_NOTHROW = 0;

    /**
    * Convert the string to an UTF8 byte array
    * @param utf8 [inout] The array where to save the data ( it will be automatically resized )
    * @return true if successful or false if error ( in that case the utf8 array should be cleared and empty ).
    **/
    virtual bool ToUTF8 ( ByteArray &utf8 ) const XNORMAL_NOTHROW = 0;

public:
    /**
    * Assing this string using a wide-char unicode string
    * @param str [in] The unicode string.
    * @return TRUE if assignation was ok or FALSE if failed ( usually due to an out of memory )
    */
    virtual bool SetW ( const wchar_t *str ) XNORMAL_NOTHROW = 0;

    /**
    * Assign the string from UTF8 or ASCII characters.
    * @param str [in] The UTF8 or ASCII data.
    * @return TRUE if successful or FALSE it not ( usually due to an out of memory or UTF8 string malformation ).
    */
    virtual bool Set ( const char *str ) XNORMAL_NOTHROW = 0;

public:
    virtual IString& ToUpper() XNORMAL_NOTHROW = 0;
    virtual IString& ToLower() XNORMAL_NOTHROW = 0;

protected:
    virtual ~IString ()
    {
    }
};

