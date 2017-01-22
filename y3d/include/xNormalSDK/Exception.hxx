#if 0
    This file is part of the xNormal 3 SDK, formatted for Doxygen/JavaDoc 
    documentation system. 
    (c) 2005-2015, S.Orgaz.
    Contact: granthill76 'at' yahoo.com
    Visit http://www.xnormal.net for more info
 
    FOR MORE INFO ABOUT THE LICENSE, CONSULT THE XNORMAL PDF DOCUMENTATION
#endif //0 comment

class Exception : public std::exception
{
public:
    explicit Exception ( const std::wstring &txt ) : mc_str(Utils::toUTF8(txt.c_str())) {}
    explicit Exception ( const std::string &txt ) : mc_str(txt) {}
    explicit Exception ( const wchar_t *txt ) : mc_str(Utils::toUTF8(txt)) {}
    explicit Exception ( const char *txt ) : mc_str(txt) {}
    Exception ( const Exception &e ) : mc_str(e.mc_str) {}
    ~Exception () {}
    
public:
    const char* what() const XNORMAL_NOTHROW
    { 
        return mc_str.c_str();
    }

protected:
    const std::string mc_str;
};
