#if 0
    This file is part of the xNormal 3 SDK, formatted for Doxygen/JavaDoc 
    documentation system. 
    (c) 2005-2015, S.Orgaz.
    Contact: granthill76 'at' yahoo.com
    Visit http://www.xnormal.net for more info
 
    FOR MORE INFO ABOUT THE LICENSE, CONSULT THE XNORMAL PDF DOCUMENTATION
#endif //0 comment


/**
* An interface to notify the user about an asyncronous task with abort/stop
* possibilities
*/
class IProgressCallback
{
public:
    /**
    * Notify to the callback about the task progress
    *
    * @param info A text containing the current task information.
    *
    * @param perc A value in range [0,100] with the percentage of progression. Values outside that range will be clipped to [0,100].
    *
    * @return TRUE to continue or FALSE if the progress was aborted
    */
    virtual bool Notify ( const wchar_t *info, const xnUInt8 perc ) XNORMAL_NOTHROW = 0;

protected:
    virtual ~IProgressCallback()
    {
    }
};

/**
* A small class helper to manage the progress callback messages ( with 
* C++ RAII, autoclean message on destruction, automatic pCall!=0 test, 
* pct autolimit, ... )
*/
class ProgressCallbackHelper
{
public:
    ProgressCallbackHelper ( IProgressCallback* pCall, const wchar_t *finalTxt=L"" ) : m_pCall(pCall), mc_strFinal((xnNULL==finalTxt)?L"":finalTxt)
    {
    }

    ~ProgressCallbackHelper() XNORMAL_NOTHROW
    {
        XNORMAL_TRY
        {
            if ( m_pCall!=xnNULL )
            {
                m_pCall->Notify ( mc_strFinal.c_str(), 0U );
            }
        } XNORMAL_CATCH_ALL_EMPTY;
    }

public:
    bool Notify ( const wchar_t *message, const xnUInt8 pct ) XNORMAL_NOTHROW
    {
        bool res ( true );
        if ( m_pCall!=xnNULL )
        {
            xnUInt8 p ( pct );
            if ( p>100U )
            {
                p = 100U;
            }
            res = m_pCall->Notify(message,p);
        }
        return res;
    }

    // Helper for std::wstring, etc... 
    template <typename T>
    bool Notify ( const T &t, const xnUInt8 pct ) XNORMAL_NOTHROW
    {
        return this->Notify ( t.c_str(), pct );
    }

private:
    IProgressCallback *m_pCall;
    const std::wstring mc_strFinal;
};

