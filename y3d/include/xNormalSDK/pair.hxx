#if 0
    This file is part of the xNormal 3 SDK, formatted for Doxygen/JavaDoc 
    documentation system. 
    (c) 2005-2015, S.Orgaz.
    Contact: granthill76 'at' yahoo.com
    Visit http://www.xnormal.net for more info
 
    FOR MORE INFO ABOUT THE LICENSE, CONSULT THE XNORMAL PDF DOCUMENTATION
#endif //0 comment


#ifndef __XNORMAL__PAIR__HEADER__43952D42__C6DD__4887__88BD__E2BE00CF3C0E
    #define __XNORMAL__PAIR__HEADER__43952D42__C6DD__4887__88BD__E2BE00CF3C0E

    template <typename F,typename S>
    struct pair XNORMAL_FINALCLASS
    {
        pair() {}
        
        pair ( const F& f, const S& s ) : first(f), second(s) {}

        F first;
        S second;
    };

    template <typename F, typename S>
    XNORMAL_INLINE pair<F,S> make_pair ( const F& first, const S& second )
    {
        return pair<F,S>(first,second);
    }

#endif //__XNORMAL__PAIR__HEADER__43952D42__C6DD__4887__88BD__E2BE00CF3C0E

