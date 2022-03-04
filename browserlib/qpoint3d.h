#ifndef QPOINT3D_H
#define QPOINT3D_H
#include "QtCore"
#include "qdatastream.h"
#include <QtCore/qnamespace.h>


class QPoint3D
{
public:
    QPoint3D();

    QPoint3D(int xpos, int ypos, int zpos);

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

    inline QPoint3D &operator+=(const QPoint3D &p);

    inline QPoint3D &operator-=(const QPoint3D &p);

    inline QPoint3D &operator*=(float factor);

    inline QPoint3D &operator*=(double factor);

    inline QPoint3D &operator*=(int factor);

    inline QPoint3D &operator/=(qreal divisor);

    static inline int dotProduct(const QPoint3D &p1, const QPoint3D &p2)
    {
        return p1.xp * p2.xp + p1.yp * p2.yp;
    }

    friend inline bool operator==(const QPoint3D &, const QPoint3D &);

    friend inline bool operator!=(const QPoint3D &, const QPoint3D &);

    friend inline const QPoint3D operator+(const QPoint3D &, const QPoint3D &);

    friend inline const QPoint3D operator-(const QPoint3D &, const QPoint3D &);

    friend inline const QPoint3D operator*(const QPoint3D &, float);

    friend inline const QPoint3D operator*(float, const QPoint3D &);

    friend inline const QPoint3D operator*(const QPoint3D &, double);

    friend inline const QPoint3D operator*(double, const QPoint3D &);

    friend inline const QPoint3D operator*(const QPoint3D &, int);

    friend inline const QPoint3D operator*(int, const QPoint3D &);

    friend inline const QPoint3D operator+(const QPoint3D &);

    friend inline const QPoint3D operator-(const QPoint3D &);

    friend inline const QPoint3D operator/(const QPoint3D &, qreal);

private:
    friend class QTransform;
    int xp;
    int yp;
    int zp;
};
Q_DECLARE_TYPEINFO(QPoint3D, Q_MOVABLE_TYPE);

/*****************************************************************************
  QPoint stream functions
 *****************************************************************************/
//QDataStream &operator<<(QDataStream &s, const QPoint3D &p){
//    s << p.x() << p.y() << p.z();
//    return s;
//}

//QDataStream &operator>>(QDataStream &s, QPoint3D &p){
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
  QPoint3D debug functions
 *****************************************************************************/
//QDebug operator<<(QDebug dbg, QPoint3D &p)
//{
//    dbg.nospace();
//    dbg << "QPoint3D" << '(';
//    dbg<<p.x()<<"; "<<p.y()<<"; "<<p.z();
//    dbg << ')';
//    return dbg;
//}
//QDebug operator<<(QDebug dbg, const QPoint3D &p)
//{
//    dbg.nospace();
//    dbg << "QPoint3D" << '(';
//    dbg<<p.x()<<"; "<<p.y()<<"; "<<p.z();
//    dbg << ')';
//    return dbg;
//}


/*****************************************************************************
  QPoint inline functions
 *****************************************************************************/
inline QPoint3D::QPoint3D()
    : xp(0), yp(0),zp(0) {}

inline QPoint3D::QPoint3D(int xpos, int ypos, int zpos)
    : xp(xpos), yp(ypos), zp(zpos) {}

inline bool QPoint3D::isNull() const
{
    return xp == 0 && yp == 0 && zp==0;
}

inline int QPoint3D::x() const
{
    return xp;
}

inline int QPoint3D::y() const
{
    return yp;
}

inline int QPoint3D::z() const
{
    return zp;
}

 inline void QPoint3D::setX(int xpos)
{
     xp = xpos;
 }

inline void QPoint3D::setY(int ypos)
{
    yp = ypos;
}

inline void QPoint3D::setZ(int zpos)
{
    zp = zpos;
}

inline int QPoint3D::manhattanLength() const
{
    return qAbs(x())+qAbs(y())+qAbs(z());
}

inline int &QPoint3D::rx()
{
    return xp;
}

inline int &QPoint3D::ry()
{
    return yp;
}

inline int &QPoint3D::rz()
{
    return zp;
}

inline QPoint3D &QPoint3D::operator+=(const QPoint3D &p)
{
    xp+=p.xp;
    yp+=p.yp;
    zp+=p.zp;
    return *this;
}

inline QPoint3D &QPoint3D::operator-=(const QPoint3D &p)
{
    xp-=p.xp;
    yp-=p.yp;
    zp-=p.zp;
    return *this;
}

inline QPoint3D &QPoint3D::operator*=(float factor)
{
    xp = qRound(xp*factor);
    yp = qRound(yp*factor);
    zp = qRound(zp*factor);
    return *this;
}

inline QPoint3D &QPoint3D::operator*=(double factor)
{
    xp = qRound(xp*factor);
    yp = qRound(yp*factor);
    zp = qRound(zp*factor);
    return *this;
}

inline QPoint3D &QPoint3D::operator*=(int factor)
{
    xp = xp*factor;
    yp = yp*factor;
    zp = zp*factor;
    return *this;
}

inline bool operator==(const QPoint3D &p1, const QPoint3D &p2)
{
    return p1.xp == p2.xp && p1.yp == p2.yp && p1.zp==p2.zp;
}

inline bool operator!=(const QPoint3D &p1, const QPoint3D &p2)
{
    return p1.xp != p2.xp || p1.yp != p2.yp || p1.zp!=p2.zp;
}

inline const QPoint3D operator+(const QPoint3D &p1, const QPoint3D &p2)
{
    return QPoint3D(p1.xp+p2.xp, p1.yp+p2.yp, p1.zp+p2.zp);
}

inline const QPoint3D operator-(const QPoint3D &p1, const QPoint3D &p2)
{
    return QPoint3D(p1.xp-p2.xp, p1.yp-p2.yp, p1.zp-p2.zp);
}

inline const QPoint3D operator*(const QPoint3D &p, float factor)
{
    return QPoint3D(qRound(p.xp*factor), qRound(p.yp*factor), qRound(p.zp*factor));
}

inline const QPoint3D operator*(const QPoint3D &p, double factor)
{
    return QPoint3D(qRound(p.xp*factor), qRound(p.yp*factor), qRound(p.zp*factor));
}

inline const QPoint3D operator*(const QPoint3D &p, int factor)
{
    return QPoint3D(p.xp*factor, p.yp*factor, p.zp*factor);
}

inline const QPoint3D operator*(float factor, const QPoint3D &p)
{
    return QPoint3D(qRound(p.xp*factor), qRound(p.yp*factor), qRound(p.zp*factor));
}

inline const QPoint3D operator*(double factor, const QPoint3D &p)
{
    return QPoint3D(qRound(p.xp*factor), qRound(p.yp*factor), qRound(p.zp*factor));
}

inline const QPoint3D operator*(int factor, const QPoint3D &p)
{
    return QPoint3D(p.xp*factor, p.yp*factor, p.zp*factor);
}

inline const QPoint3D operator+(const QPoint3D &p)
{
    return p;
}

inline const QPoint3D operator-(const QPoint3D &p)
{
    return QPoint3D(-p.xp, -p.yp, -p.zp);
}

inline QPoint3D &QPoint3D::operator/=(double c)
{
    xp = qRound(xp/c);
    yp = qRound(yp/c);
    zp = qRound(zp/c);
    return *this;
}

inline const QPoint3D operator/(const QPoint3D &p, double c)
{
    return QPoint3D(qRound(p.xp/c), qRound(p.yp/c), qRound(p.zp/c));
}

#endif // QPOINT3D_H
