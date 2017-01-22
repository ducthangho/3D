#if 0
    This file is part of the xNormal 3 SDK, formatted for Doxygen/JavaDoc 
    documentation system. 
    (c) 2005-2015, S.Orgaz.
    Contact: granthill76 'at' yahoo.com
    Visit http://www.xnormal.net for more info
 
    FOR MORE INFO ABOUT THE LICENSE, CONSULT THE XNORMAL PDF DOCUMENTATION
#endif //0 comment


/**
* Global Unique Identifier (GUID) structure represents an unique number hash.
*/
struct sGUID XNORMAL_FINALCLASS
{
    /**
    * Default constructor ( set all to zero )
    */
    sGUID () XNORMAL_NOTHROW
    { 
        this->SetToZero();
    }

    /**
    * Construct from parameters.
    */
    explicit sGUID ( const xnUInt32 d1, const xnUInt16 d2, const xnUInt16 d3,
                     const xnUInt8 d4_1, const xnUInt8 d4_2, const xnUInt8 d4_3, 
                     const xnUInt8 d4_4, const xnUInt8 d4_5, const xnUInt8 d4_6, 
                     const xnUInt8 d4_7, const xnUInt8 d4_8 ) XNORMAL_NOTHROW : Data1(d1), Data2(d2), Data3(d3)
    {
	    this->Data4[0] = d4_1;
	    this->Data4[1] = d4_2;
	    this->Data4[2] = d4_3;
	    this->Data4[3] = d4_4;
	    this->Data4[4] = d4_5;
	    this->Data4[5] = d4_6;
	    this->Data4[6] = d4_7;
	    this->Data4[7] = d4_8;
    }

    sGUID ( const sGUID& g ) XNORMAL_NOTHROW : Data1(g.Data1), Data2(g.Data2), Data3(g.Data3)
    {
        for ( size_t i=0U; i<sizeof(Data4)/sizeof(Data4[0]); ++i )
        {
            this->Data4[i] = g.Data4[i];
        }
    }

    sGUID& operator = ( const sGUID &g ) XNORMAL_NOTHROW
    {
        this->Data1 = g.Data1;
        this->Data2 = g.Data2;
        this->Data3 = g.Data3;

        for ( xnInt32 i=0; i<8; ++i )
        {
	        this->Data4[i] = g.Data4[i];
        }

        return *this;
    }

    void SetToZero () XNORMAL_NOTHROW
    {
	    this->Data1 = 0U;
	    this->Data2 = 0U;
	    this->Data3 = 0U;

        for ( xnInt32 i=0; i<8; ++i )
        {
	        this->Data4[i] = 0U;
        }
    }

    bool IsZero () const XNORMAL_NOTHROW
    {
        return ( 0U==this->Data1 && 0U==this->Data2 && 0U==this->Data3 && 
            0U==this->Data4[0] && 0U==this->Data4[1] && 0U==this->Data4[2] && 0U==this->Data4[3] && 
            0U==this->Data4[4] &&0U==this->Data4[5] && 0U==this->Data4[6] && 0U==this->Data4[7] );
    }

    template <typename T>
    void ToStringStd ( T& str ) const XNORMAL_NOTHROW
    {
        char c[255];

    	//const int res = sprintf ( c, "{%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX}", 
        const int res = sprintf ( c, "{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
            static_cast<unsigned int>(this->Data1), static_cast<unsigned int>(this->Data2), static_cast<unsigned int>(this->Data3), 
            static_cast<unsigned int>(this->Data4[0]), static_cast<unsigned int>(this->Data4[1]), static_cast<unsigned int>(this->Data4[2]), static_cast<unsigned int>(this->Data4[3]), 
            static_cast<unsigned int>(this->Data4[4]), static_cast<unsigned int>(this->Data4[5]), static_cast<unsigned int>(this->Data4[6]), static_cast<unsigned int>(this->Data4[7]) );
    
        if ( 38==res )
        {
            str = c;
        }
        else
        {
            XNORMAL_ASSERT ( false );
            str.clear();
        }
    }

    /**
    * Convert to a human-readable string.
    * @param str [out] A reference to a string to be filled with the GUID
    */
    XNORMAL_INLINE void ToString ( IString& str ) const XNORMAL_NOTHROW
    {
        std::string l_str;
        this->ToStringStd ( l_str );
        str.Set ( l_str.c_str() );
    }
    
    /**
    * Parse GUID from a string
    * @param str [in] A string in format {XXXXXXXX-YYYY-ZZZZ-AABB-CCDDEEFFGGHH} if success or a 
    *   ZERO guid if fails
    */
    static sGUID Parse ( const IString& str ) XNORMAL_NOTHROW
    {
        sGUID g;
        unsigned int d1;
        unsigned int d2, d3;
        unsigned int d4[8];

        //const int n = sscanf ( str.GetPtrUTF8(), "{%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX}", &d1, &d2, &d3, &d4[0], &d4[1], &d4[2], &d4[3], &d4[4], &d4[5], &d4[6], &d4[7] );
        const int n = sscanf ( str.GetPtrUTF8(), "{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}", &d1, &d2, &d3, &d4[0], &d4[1], &d4[2], &d4[3], &d4[4], &d4[5], &d4[6], &d4[7] );        
                
        if ( n!=11 )
        {
            XNORMAL_ASSERT ( false );
            g.SetToZero();
        }
        else
        {
            g.Data1 = static_cast<xnUInt32>(d1);
            g.Data2 = static_cast<xnUInt16>(d2);
            g.Data3 = static_cast<xnUInt16>(d3);
            g.Data4[0] = static_cast<xnUInt8>(d4[0]);
            g.Data4[1] = static_cast<xnUInt8>(d4[1]);
            g.Data4[2] = static_cast<xnUInt8>(d4[2]);
            g.Data4[3] = static_cast<xnUInt8>(d4[3]);
            g.Data4[4] = static_cast<xnUInt8>(d4[4]);
            g.Data4[5] = static_cast<xnUInt8>(d4[5]);
            g.Data4[6] = static_cast<xnUInt8>(d4[6]);
            g.Data4[7] = static_cast<xnUInt8>(d4[7]);
        }
        return g;
    }

    /**
    * Create a new GUID
    */
    static sGUID CreateNewGUID () XNORMAL_NOTHROW
    {
        sGUID res;
        GUID l_sGuid;
        ::CoCreateGuid(&l_sGuid);

        res.Data1 = static_cast<xnUInt32>(l_sGuid.Data1);
        res.Data2 = static_cast<xnUInt16>(l_sGuid.Data2);
        res.Data3 = static_cast<xnUInt16>(l_sGuid.Data3);

        for ( size_t i=0U; i<8U; ++i )
        {
            res.Data4[i] = static_cast<xnUInt8>(l_sGuid.Data4[i]);
        }

        return res;
    }

    /**
    * Test if the specified GUID is equal to this GUID
    */
    bool operator == ( const sGUID& g ) const XNORMAL_NOTHROW
    {
        return ( g.Data1==this->Data1 && 
                 g.Data2==this->Data2 && 
                 g.Data3==this->Data3 &&
                 g.Data4[0]==this->Data4[0] && 
                 g.Data4[1]==this->Data4[1] && 
                 g.Data4[2]==this->Data4[2] &&
                 g.Data4[3]==this->Data4[3] && 
                 g.Data4[4]==this->Data4[4] && 
                 g.Data4[5]==this->Data4[5] &&
                 g.Data4[6]==this->Data4[6] && 
                 g.Data4[7]==this->Data4[7] );
    }

    /**
    * Test if the specified GUID is different to this GUID
    */
    bool operator != ( const sGUID& g ) const XNORMAL_NOTHROW
    {
        return ( g.Data1!=this->Data1 ||
                 g.Data2!=this->Data2 ||
                 g.Data3!=this->Data3 ||
                 g.Data4[0]!=this->Data4[0] || 
                 g.Data4[1]!=this->Data4[1] || 
                 g.Data4[2]!=this->Data4[2] ||
                 g.Data4[3]!=this->Data4[3] || 
                 g.Data4[4]!=this->Data4[4] || 
                 g.Data4[5]!=this->Data4[5] ||
                 g.Data4[6]!=this->Data4[6] || 
                 g.Data4[7]!=this->Data4[7] );
    }

    /**
    * Data1 property.
    */
    xnUInt32 Data1;

    /**
    * Data2 property.
    */
    xnUInt16 Data2;

    /**
    * Data3 property
    */
    xnUInt16 Data3;

    /**
    * Data4 property.
    */
    xnUInt8 Data4[8];
};


