#if 0
    This file is part of the xNormal 3 SDK, formatted for Doxygen/JavaDoc 
    documentation system. 
    (c) 2005-2015, S.Orgaz.
    Contact: granthill76 'at' yahoo.com
    Visit http://www.xnormal.net for more info
 
    FOR MORE INFO ABOUT THE LICENSE, CONSULT THE XNORMAL PDF DOCUMENTATION
#endif //0 comment


/**
* A simplified std::auto_ptr version which just calls the object release method
* instead of deleting it. Notice it CAN'T share pointers, only one can own the 
* pointer at one time
*/
template <typename T>
class auto_ptr_release XNORMAL_FINALCLASS
{
public:
    auto_ptr_release(T* ptr = xnNULL) XNORMAL_NOTHROW : m_pPtr(ptr) {}

    ~auto_ptr_release () XNORMAL_NOTHROW
    {
        this->reset();
    }

public:
    T* get() const XNORMAL_NOTHROW
    {
        return m_pPtr;
    }

    bool IsValid () const XNORMAL_NOTHROW
    {
        return (m_pPtr!=0);
    }

public:
    /**
    * Return the pointer to the class object
    * @return The pointer to class object
    */
    T* operator -> () const XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( m_pPtr!=xnNULL );
        return m_pPtr;
    }

    /**
    * Dereference pointer
    * @return A reference to the assigned pointer
    */
    T& operator *() const XNORMAL_NOTHROW
    {
        XNORMAL_ASSERT ( m_pPtr!=xnNULL );
        return *m_pPtr;
    }

    /**
    * Get the adress of the internal pointer
    * @return The pointer to the assigned pointer
    */
    T** operator & () XNORMAL_NOTHROW
    {
        return &m_pPtr;
    }

public:
    auto_ptr_release<T>& operator = ( auto_ptr_release<T>& p ) XNORMAL_NOTHROW
    {
        this->reset(p.release());
        return *this;
    }

    auto_ptr_release<T>& operator = ( T* newp ) XNORMAL_NOTHROW
    {
        this->reset(newp);
        return *this;
    }

public:
    bool operator == ( T* ptr ) const XNORMAL_NOTHROW
    {
        return (m_pPtr==ptr);
    }

public:
    operator T* () const XNORMAL_NOTHROW
    {
        return m_pPtr;
    }

public:
    /**
    * Return wrapped pointer and give up ownership
    * @return A pointer to the wrapped object
    */
    T* release() XNORMAL_NOTHROW
    {
        T *tmp = m_pPtr;
        m_pPtr = xnNULL;
        return tmp;
    }

    /**
    * Destroy designated object and store new pointer
    * @param ptr [in] The new pointer to assign to this smart pointer
    */
    void reset ( T* ptr=xnNULL ) XNORMAL_NOTHROW
    {
        if ( ptr!=m_pPtr )
        {
            if ( m_pPtr!=xnNULL )
            {
                m_pPtr->Release();
            }
            m_pPtr = ptr;
        }
    }

protected:
    T *m_pPtr;
};

template <typename Ta,typename T>
XNORMAL_INLINE bool operator == ( Ta* ptr, const auto_ptr_release<T>& autoPtr ) XNORMAL_NOTHROW
{
    return ( ((T*)ptr)==autoPtr.get() );
}

