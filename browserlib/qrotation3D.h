#ifndef QROTATION3D_H
#define QROTATION3D_H
#include "QtCore"
#include "qdatastream.h"
#include <QtCore/qnamespace.h>


class QRotation3D
{
public:
    QRotation3D();

    QRotation3D(int xpos, int ypos, int zpos);

    inline bool isNull() const;

    inline int x() const;

    inline int y() const;

    inline int z() const;

    inline void setX(int x);

    inline void setY(int y);

    inline void setZ(int zpos);

    inline int manhattanLength() const;

    inline int &rx();

    inline int &ry();

    inline int &rz();

    inline QRotation3D &operator+=(const QRotation3D &p);

    inline QRotation3D &operator-=(const QRotation3D &p);

    inline QRotation3D &operator*=(float factor);

    inline QRotation3D &operator*=(double factor);

    inline QRotation3D &operator*=(int factor);

    inline QRotation3D &operator/=(qreal divisor);

    static inline int dotProduct(const QRotation3D &p1, const QRotation3D &p2)
    {
        return p1.xp * p2.xp + p1.yp * p2.yp;
    }

    friend inline bool operator==(const QRotation3D &, const QRotation3D &);

    friend inline bool operator!=(const QRotation3D &, const QRotation3D &);

    friend inline const QRotation3D operator+(const QRotation3D &, const QRotation3D &);

    friend inline const QRotation3D operator-(const QRotation3D &, const QRotation3D &);

    friend inline const QRotation3D operator*(const QRotation3D &, float);

    friend inline const QRotation3D operator*(float, const QRotation3D &);

    friend inline const QRotation3D operator*(const QRotation3D &, double);

    friend inline const QRotation3D operator*(double, const QRotation3D &);

    friend inline const QRotation3D operator*(const QRotation3D &, int);

    friend inline const QRotation3D operator*(int, const QRotation3D &);

    friend inline const QRotation3D operator+(const QRotation3D &);

    friend inline const QRotation3D operator-(const QRotation3D &);

    friend inline const QRotation3D operator/(const QRotation3D &, qreal);

private:
    friend class QTransform;
    int xp;
    int yp;
    int zp;
};
Q_DECLARE_TYPEINFO(QRotation3D, Q_MOVABLE_TYPE);

/*****************************************************************************
  QRotation3D stream functions
 *****************************************************************************/
//QDataStream &operator<<(QDataStream &s, const QRotation3D &p){
//    s << p.x() << p.y() << p.z();
//    return s;
//}

//QDataStream &operator>>(QDataStream &s, QRotation3D &p){
//    int x,y,z;
//    s >> x;
//    p.rx()=x;

//    s >> y;
//    p.ry()=y;

//    s >> z;
//    p.rz()=z;
//    return s;
//}

/*****************************************************************************
  QRotation3D debug functions
 *****************************************************************************/
//QDebug operator<<(QDebug dbg, QRotation3D &p)
//{
//    dbg.nospace();
//    dbg << "QPoint3D" << '(';
//    dbg<<p.x()<<"; "<<p.y()<<"; "<<p.z();
//    dbg << ')';
//    return dbg;
//}
//QDebug operator<<(QDebug dbg, const QRotation3D &p)
//{
//    dbg.nospace();
//    dbg << "QPoint3D" << '(';
//    dbg<<p.x()<<"; "<<p.y()<<"; "<<p.z();
//    dbg << ')';
//    return dbg;
//}


/*****************************************************************************
  QRotation3D inline functions
 *****************************************************************************/
inline QRotation3D::QRotation3D()
    : xp(0), yp(0),zp(0) {}

inline QRotation3D::QRotation3D(int xpos, int ypos, int zpos)
    : xp(xpos), yp(ypos), zp(zpos) {}

inline bool QRotation3D::isNull() const
{
    return xp == 0 && yp == 0 && zp==0;
}

inline int QRotation3D::x() const
{
    return xp;
}

inline int QRotation3D::y() const
{
    return yp;
}

inline int QRotation3D::z() const
{
    return zp;
}

 inline void QRotation3D::setX(int xpos)
{
     xp = xpos;
 }

inline void QRotation3D::setY(int ypos)
{
    yp = ypos;
}

inline void QRotation3D::setZ(int zpos)
{
    zp = zpos;
}

inline int QRotation3D::manhattanLength() const
{
    return qAbs(x())+qAbs(y())+qAbs(z());
}

inline int &QRotation3D::rx()
{
    return xp;
}

inline int &QRotation3D::ry()
{
    return yp;
}

inline int &QRotation3D::rz()
{
    return zp;
}

inline QRotation3D &QRotation3D::operator+=(const QRotation3D &p)
{
    xp+=p.xp;
    yp+=p.yp;
    zp+=p.zp;
    return *this;
}

inline QRotation3D &QRotation3D::operator-=(const QRotation3D &p)
{
    xp-=p.xp;
    yp-=p.yp;
    zp-=p.zp;
    return *this;
}

inline QRotation3D &QRotation3D::operator*=(float factor)
{
    xp = qRound(xp*factor);
    yp = qRound(yp*factor);
    zp = qRound(zp*factor);
    return *this;
}

inline QRotation3D &QRotation3D::operator*=(double factor)
{
    xp = qRound(xp*factor);
    yp = qRound(yp*factor);
    zp = qRound(zp*factor);
    return *this;
}

inline QRotation3D &QRotation3D::operator*=(int factor)
{
    xp = xp*factor;
    yp = yp*factor;
    zp = zp*factor;
    return *this;
}

inline bool operator==(const QRotation3D &p1, const QRotation3D &p2)
{
    return p1.xp == p2.xp && p1.yp == p2.yp && p1.zp==p2.zp;
}

inline bool operator!=(const QRotation3D &p1, const QRotation3D &p2)
{
    return p1.xp != p2.xp || p1.yp != p2.yp || p1.zp!=p2.zp;
}

inline const QRotation3D operator+(const QRotation3D &p1, const QRotation3D &p2)
{
    return QRotation3D(p1.xp+p2.xp, p1.yp+p2.yp, p1.zp+p2.zp);
}

inline const QRotation3D operator-(const QRotation3D &p1, const QRotation3D &p2)
{
    return QRotation3D(p1.xp-p2.xp, p1.yp-p2.yp, p1.zp-p2.zp);
}

inline const QRotation3D operator*(const QRotation3D &p, float factor)
{
    return QRotation3D(qRound(p.xp*factor), qRound(p.yp*factor), qRound(p.zp*factor));
}

inline const QRotation3D operator*(const QRotation3D &p, double factor)
{
    return QRotation3D(qRound(p.xp*factor), qRound(p.yp*factor), qRound(p.zp*factor));
}

inline const QRotation3D operator*(const QRotation3D &p, int factor)
{
    return QRotation3D(p.xp*factor, p.yp*factor, p.zp*factor);
}

inline const QRotation3D operator*(float factor, const QRotation3D &p)
{
    return QRotation3D(qRound(p.xp*factor), qRound(p.yp*factor), qRound(p.zp*factor));
}

inline const QRotation3D operator*(double factor, const QRotation3D &p)
{
    return QRotation3D(qRound(p.xp*factor), qRound(p.yp*factor), qRound(p.zp*factor));
}

inline const QRotation3D operator*(int factor, const QRotation3D &p)
{
    return QRotation3D(p.xp*factor, p.yp*factor, p.zp*factor);
}

inline const QRotation3D operator+(const QRotation3D &p)
{
    return p;
}

inline const QRotation3D operator-(const QRotation3D &p)
{
    return QRotation3D(-p.xp, -p.yp, -p.zp);
}

inline QRotation3D &QRotation3D::operator/=(double c)
{
    xp = qRound(xp/c);
    yp = qRound(yp/c);
    zp = qRound(zp/c);
    return *this;
}

inline const QRotation3D operator/(const QRotation3D &p, double c)
{
    return QRotation3D(qRound(p.xp/c), qRound(p.yp/c), qRound(p.zp/c));
}

#endif // QROTATION3D_H
