#if 0
    This file is part of the xNormal 3 SDK, formatted for Doxygen/JavaDoc 
    documentation system. 
    (c) 2005-2015, S.Orgaz.
    Contact: granthill76 'at' yahoo.com
    Visit http://www.xnormal.net for more info
 
    FOR MORE INFO ABOUT THE LICENSE, CONSULT THE XNORMAL PDF DOCUMENTATION
#endif //0 comment


#ifndef __XNORMAL__PLANE__HEADER__19D8303D__1362__4a4d__82A7__34D4F03E97B8__
    #define __XNORMAL__PLANE__HEADER__19D8303D__1362__4a4d__82A7__34D4F03E97B8__

    /**
    * A plane class
    */
    class Plane XNORMAL_FINALCLASS
    {
    public:
        /**
        * Default constructor (empy atm)
        */
        Plane () XNORMAL_NOTHROW {}

        /**
        * Construct a plane from a normal and the D term
        * @param normal [in] The normal(A,B,C) in the equation Ax+By+Cz+D==0
        * @param d [in] The D term in equation Ax+By+Cz+D==0
        */
        explicit Plane ( const Vector& normal, const xnSingle d ) XNORMAL_NOTHROW : Normal(normal), D(d) {}

        /**
        * Construct a plane from a normal and a contained point
        * @param normal [in] The normal(A,B,C) in the equation Ax+By+Cz+D==0
        * @param p [in] A point contained in the plane
        */
        explicit Plane ( const Vector& normal, const Vector& p ) XNORMAL_NOTHROW : Normal(normal), D(-normal.Dot(p)) {}

        /**
        * Construct a plane from tree coplanar points
        * @param p1 [in] The first point
        * @param p2 [in] The second point
        * @param p3 [in] The third point
        */
        explicit Plane ( const Vector& p1, const Vector& p2, const Vector& p3 ) XNORMAL_NOTHROW
        {
            const Vector v21 ( (p2-p1).Normalize() );
            const Vector v31 ( (p3-p1).Normalize() );
            Vector::Cross(v21,v31,&this->Normal);
            this->D = -this->Normal.Normalize().Dot(p1);
        }

        /**
        * Copy constructor 
        * @param p [in] The plane to copy the data from
        */
        Plane ( const Plane& p ) XNORMAL_NOTHROW : Normal(p.Normal), D(p.D) {}


    public:
        /**
        * Make a plane from a normal "n" and a point "p"
        * @param n [in] The normal
        * @param p [in] A point contained in the plane
        * @return The plane as Ax+By+Cz+D==0, where the (A,B,C) is the plane normal specified by "n"
        */
        static Plane FromNormalAndPoint ( const Vector& n, const Vector& p ) XNORMAL_NOTHROW
        {
            return Plane ( n, -n.Dot(p) );
        }

        /**
        * Construct a plane from three coplanar points(in counter-clockwise order)
        * @param p1 [in] The first point
        * @param p2 [in] The second point
        * @param p3 [in] The third point
        * @return The plane which includes the three specidied points
        * @remarks The specified points must be given in counter-clockwise order or the resulting plane
        *   normal will be flipped.
        */
        static Plane FromThreePoints ( const Vector& p1, const Vector& p2, const Vector& p3 ) XNORMAL_NOTHROW
        {
            return Plane(p1,p2,p3);
        }

    public:
        /**
        * Calculate the point distance to this plane
        * @param p [in] The point to measure the distance to this plane
        * @return The distance of the point "p" to this plane ( in generic units )
        */
        xnSingle GetPointDistance ( const Vector& p ) const XNORMAL_NOTHROW
        {
            return this->Normal.Dot(p) + this->D;
        }

    public:
        /**
        * The normal of this plane. Remember that the normal corresponds to the (A,B,C) triplet
        * in the plane equation Ax+By+Cz+D==0
        */
        Vector Normal;

        /**
        * The D term in the plane equation Ax+By+Cz+D==0
        */
        xnSingle D;
    };

#endif // __XNORMAL__PLANE__HEADER__19D8303D__1362__4a4d__82A7__34D4F03E97B8__

