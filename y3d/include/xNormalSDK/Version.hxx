#if 0
    This file is part of the xNormal 3 SDK, formatted for Doxygen/JavaDoc 
    documentation system. 
    (c) 2005-2015, S.Orgaz.
    Contact: granthill76 'at' yahoo.com
    Visit http://www.xnormal.net for more info
 
    FOR MORE INFO ABOUT THE LICENSE, CONSULT THE XNORMAL PDF DOCUMENTATION
#endif //0 comment


/**
 * Version struct contains a major and a minor fields and is used in plugins
 * and some internal classes
*/
struct sVERSION XNORMAL_FINALCLASS
{
    /**
    * Default constructor (empty atm)
    */
    sVERSION () XNORMAL_NOTHROW {}

    /**
    * Construct from the major, minor and revision numbers
    * @param vMajor [in] The version major ( for example: 2 )
    * @param vMinor [in] The version major ( for example: 37 )
    * @param rev [in] OPTIONAL parameter(will be set to ZERO if not passed)
    * with the build  number ( for example: 2903 )
    */
    explicit sVERSION ( const xnUInt8 vMajor, const xnUInt8 vMinor, const xnUInt8 rev ) XNORMAL_NOTHROW : VersionMajor(vMajor), VersionMinor(vMinor), Revision(rev) {}

    sVERSION ( const sVERSION &v ) XNORMAL_NOTHROW : VersionMajor(v.VersionMajor), VersionMinor(v.VersionMinor), Revision(v.Revision) {}

    sVERSION& operator = ( const sVERSION &v ) XNORMAL_NOTHROW
    {
        this->VersionMajor = v.VersionMajor;
        this->VersionMinor = v.VersionMinor;
        this->Revision = v.Revision;

        return *this;
    }

    /**
    * Test if a version is equal to this version
    * @param ver [in] The version to test against this version
    * @return TRUE if the specified version is equal to this version or 
    * FALSE if not
    */
    bool operator == ( const sVERSION& ver ) const XNORMAL_NOTHROW
    {
    	return ( this->VersionMajor==ver.VersionMajor && this->VersionMinor==ver.VersionMinor && this->Revision==ver.Revision );
    }

    /**
    * Test if a version is different to this version
    * @param ver [in] The version to test against this version
    * @return TRUE if the specified version is different to this version or
    * FALSE if not
    */
    bool operator != ( const sVERSION& ver ) const XNORMAL_NOTHROW
    {
    	return ( this->VersionMajor!=ver.VersionMajor || this->VersionMinor!=ver.VersionMinor || this->Revision!=ver.Revision );
    }

    /**
    * Test if a version is greater to this version
    * @param ver [in] The version to test against this version
    * @return TRUE if the specified version is greater than this version or
    * FALSE if not
    */
    bool operator > ( const sVERSION& ver ) const XNORMAL_NOTHROW
    {
        return ( this->ToUInt32() > ver.ToUInt32() );
    }


    /**
    * Test if a version is greater or equal to this version
    * @param ver [in] The version to test against this version
    * @return TRUE if the specified version is greater or equal than this 
    * version or FALSE if not
    */
    bool operator >= ( const sVERSION& ver ) const XNORMAL_NOTHROW
    {
        return ( this->ToUInt32() >= ver.ToUInt32() );
    }

    /**
    * Test if a version is less to this version
    * @param ver [in] The version to test against this version
    * @return TRUE if the specified version is less than this version or 
    * FALSE if not
    */  
    bool operator < ( const sVERSION& ver ) const XNORMAL_NOTHROW
    {
        return ( this->ToUInt32() < ver.ToUInt32() );
    }

    /**
    * Test if a version is less to this version
    * @param ver [in] The version to test against this version
    * @return TRUE if the specified version is less or equal than this version
    * or FALSE if not
    */
    bool operator <= ( const sVERSION& ver ) const XNORMAL_NOTHROW
    {
        return ( this->ToUInt32() <= ver.ToUInt32() );
    }

    template <typename T>
    void ToStringStd ( T& outRes ) const XNORMAL_NOTHROW
    {
        char l_str[255];
        
        const int nArg = sprintf ( l_str, "%u.%u.%u", this->VersionMajor, this->VersionMinor, this->Revision );
        if ( nArg>2 )
        {
            outRes = l_str;
        }
        else
        {
            XNORMAL_ASSERT ( false );
            outRes.clear();
        }
    }

    /**
    * Convert the version to a wide string
    * @param outRes [out] A reference to a string to be filled as Major.Minor.Revision
    */
    XNORMAL_INLINE void ToString ( IString& outRes ) const XNORMAL_NOTHROW
    {
        std::string l_str;
        this->ToStringStd ( l_str );
        outRes.Set ( l_str.c_str() );
    }

    xnUInt32 ToUInt32() const XNORMAL_NOTHROW
    {
        return static_cast<xnUInt32>(this->VersionMajor<<16U) | static_cast<xnUInt32>(this->VersionMinor<<8U) | static_cast<xnUInt32>(this->Revision);
    }

    /**
    * The major component of the version
    */
    xnUInt8 VersionMajor;

    /**
    * The minor component of the version
    */
    xnUInt8 VersionMinor;

    /**
    * The revision or build number component of the version
    */
    xnUInt8 Revision;

    xnUInt8 padding; //padding to 32bits.
};

