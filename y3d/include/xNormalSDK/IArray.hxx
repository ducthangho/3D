#if 0
    This file is part of the xNormal 3 SDK, formatted for Doxygen/JavaDoc 
    documentation system. 
    (c) 2005-2015, S.Orgaz.
    Contact: granthill76 'at' yahoo.com
    Visit http://www.xnormal.net for more info
 
    FOR MORE INFO ABOUT THE LICENSE, CONSULT THE XNORMAL PDF DOCUMENTATION
#endif //0 comment


/**
 * A resizeable array class based on STL's std::vector but 
 * simplified and specialized to be used in xNormal.
 *
 * @remarks I need to use this custom container class to avoid 
 * multiple STL implementions ( for example, xNormal could be 
 * written using the default VS's STL while a person making a
 * xNormal plugin could use STLPort... 
*/
template <typename T>
class IArray : public basic_XN_trait<T>
{
public:
    virtual void Release() XNORMAL_NOTHROW = 0;

public:
    /**
    * Test is the array is empty
    */
    virtual bool IsEmpty() const XNORMAL_NOTHROW = 0;

    /**
    * Get the total number of elements
    */
    virtual xnUInt32 GetLength() const XNORMAL_NOTHROW = 0;

    /**
    * Resize the array
    * @param nElements [in] The new array's count. If you specify zero,
    *   the array wont free the reserved memory(like the std::vector)
    */
    virtual bool Resize ( const xnUInt32 nElements ) XNORMAL_NOTHROW = 0;

    /**
    * Empty the array and deallocate all the memory owned by the array
    */
    virtual void Clear () XNORMAL_NOTHROW = 0;

    virtual bool Add ( const T& val ) XNORMAL_NOTHROW = 0;

public:
    const T& GetConst ( const xnUInt32 i ) const XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( i<this->GetLength() );
        return this->GetP ( i );
    }

    T& Get ( const xnUInt32 i ) XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( i<this->GetLength() );
        return this->GetP ( i );
    }

    void Set ( const xnUInt32 i, const T &b ) XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( i<this->GetLength() );
        return this->SetP ( i, b );
    }

    virtual const T* GetPtrConst() const XNORMAL_NOTHROW = 0;
    virtual T* GetPtr() XNORMAL_NOTHROW = 0;

protected:
    virtual ~IArray()
    {
    }

    virtual const T& GetConstP ( const xnUInt32 i ) const XNORMAL_NOTHROW = 0;
    virtual T& GetP ( const xnUInt32 i ) XNORMAL_NOTHROW = 0;
    virtual void SetP ( const xnUInt32 i, const T& b ) XNORMAL_NOTHROW = 0;
};

typedef IArray<xnByte> ByteArray;
