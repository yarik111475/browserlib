#ifndef QPOINT3DF_H
#define QPOINT3DF_H
#include "QtCore"
#include "qdatastream.h"
#include <QtCore/qnamespace.h>


class QPoint3DF
{
public:
    QPoint3DF();

    QPoint3DF(double xpos, double ypos, double zpos);

    inline bool isNull() const;

    inline double x() const;

    inline double y() const;

    inline double z() const;

    inline void setX(double x);

    inline void setY(double y);

    inline void setZ(double zpos);

    inline double manhattanLength() const;

    inline double &rx();

    inline double &ry();

    inline double &rz();

    inline QPoint3DF &operator+=(const QPoint3DF &p);

    inline QPoint3DF &operator-=(const QPoint3DF &p);

    inline QPoint3DF &operator*=(float factor);

    inline QPoint3DF &operator*=(double factor);

    inline QPoint3DF &operator*=(int factor);

    inline QPoint3DF &operator/=(qreal divisor);

    static inline double dotProduct(const QPoint3DF &p1, const QPoint3DF &p2)
    {
        return p1.xp * p2.xp + p1.yp * p2.yp;
    }

    friend inline bool operator==(const QPoint3DF &, const QPoint3DF &);

    friend inline bool operator!=(const QPoint3DF &, const QPoint3DF &);

    friend inline const QPoint3DF operator+(const QPoint3DF &, const QPoint3DF &);

    friend inline const QPoint3DF operator-(const QPoint3DF &, const QPoint3DF &);

    friend inline const QPoint3DF operator*(const QPoint3DF &, float);

    friend inline const QPoint3DF operator*(float, const QPoint3DF &);

    friend inline const QPoint3DF operator*(const QPoint3DF &, double);

    friend inline const QPoint3DF operator*(double, const QPoint3DF &);

    friend inline const QPoint3DF operator*(const QPoint3DF &, int);

    friend inline const QPoint3DF operator*(int, const QPoint3DF &);

    friend inline const QPoint3DF operator+(const QPoint3DF &);

    friend inline const QPoint3DF operator-(const QPoint3DF &);

    friend inline const QPoint3DF operator/(const QPoint3DF &, qreal);

private:
    friend class QTransform;
    double xp;
    double yp;
    double zp;
};
Q_DECLARE_TYPEINFO(QPoint3DF, Q_MOVABLE_TYPE);

/*****************************************************************************
  QPoint3DF stream functions
 *****************************************************************************/
//QDataStream &operator<<(QDataStream &s, const QPoint3DF &p){
//    s << p.x() << p.y() << p.z();
//    return s;
//}

//QDataStream &operator>>(QDataStream &s, QPoint3DF &p){
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
  QPoint3DF debug functions
 *****************************************************************************/
//QDebug operator<<(QDebug dbg, QPoint3DF &p)
//{
//    dbg.nospace();
//    dbg << "QPoint3DF" << '(';
//    dbg<<p.x()<<"; "<<p.y()<<"; "<<p.z();
//    dbg << ')';
//    return dbg;
//}
//QDebug operator<<(QDebug dbg, const QPoint3DF &p)
//{
//    dbg.nospace();
//    dbg << "QPoint3DF" << '(';
//    dbg<<p.x()<<"; "<<p.y()<<"; "<<p.z();
//    dbg << ')';
//    return dbg;
//}


/*****************************************************************************
  QPoint3DF inline functions
 *****************************************************************************/
inline QPoint3DF::QPoint3DF()
    : xp(0), yp(0),zp(0) {}

inline QPoint3DF::QPoint3DF(double xpos, double ypos, double zpos)
    : xp(xpos), yp(ypos), zp(zpos) {}

inline bool QPoint3DF::isNull() const
{
    return xp == 0 && yp == 0 && zp==0;
}

inline double QPoint3DF::x() const
{
    return xp;
}

inline double QPoint3DF::y() const
{
    return yp;
}

inline double QPoint3DF::z() const
{
    return zp;
}

 inline void QPoint3DF::setX(double xpos)
{
     xp = xpos;
 }

inline void QPoint3DF::setY(double ypos)
{
    yp = ypos;
}

inline void QPoint3DF::setZ(double zpos)
{
    zp = zpos;
}

inline double QPoint3DF::manhattanLength() const
{
    return qAbs(x())+qAbs(y())+qAbs(z());
}

inline double &QPoint3DF::rx()
{
    return xp;
}

inline double &QPoint3DF::ry()
{
    return yp;
}

inline double &QPoint3DF::rz()
{
    return zp;
}

inline QPoint3DF &QPoint3DF::operator+=(const QPoint3DF &p)
{
    xp+=p.xp;
    yp+=p.yp;
    zp+=p.zp;
    return *this;
}

inline QPoint3DF &QPoint3DF::operator-=(const QPoint3DF &p)
{
    xp-=p.xp;
    yp-=p.yp;
    zp-=p.zp;
    return *this;
}

inline QPoint3DF &QPoint3DF::operator*=(float factor)
{
    xp = qRound(xp*factor);
    yp = qRound(yp*factor);
    zp = qRound(zp*factor);
    return *this;
}

inline QPoint3DF &QPoint3DF::operator*=(double factor)
{
    xp = qRound(xp*factor);
    yp = qRound(yp*factor);
    zp = qRound(zp*factor);
    return *this;
}

inline QPoint3DF &QPoint3DF::operator*=(int factor)
{
    xp = xp*factor;
    yp = yp*factor;
    zp = zp*factor;
    return *this;
}

inline bool operator==(const QPoint3DF &p1, const QPoint3DF &p2)
{
    return p1.xp == p2.xp && p1.yp == p2.yp && p1.zp==p2.zp;
}

inline bool operator!=(const QPoint3DF &p1, const QPoint3DF &p2)
{
    return p1.xp != p2.xp || p1.yp != p2.yp || p1.zp!=p2.zp;
}

inline const QPoint3DF operator+(const QPoint3DF &p1, const QPoint3DF &p2)
{
    return QPoint3DF(p1.xp+p2.xp, p1.yp+p2.yp, p1.zp+p2.zp);
}

inline const QPoint3DF operator-(const QPoint3DF &p1, const QPoint3DF &p2)
{
    return QPoint3DF(p1.xp-p2.xp, p1.yp-p2.yp, p1.zp-p2.zp);
}

inline const QPoint3DF operator*(const QPoint3DF &p, float factor)
{
    return QPoint3DF(qRound(p.xp*factor), qRound(p.yp*factor), qRound(p.zp*factor));
}

inline const QPoint3DF operator*(const QPoint3DF &p, double factor)
{
    return QPoint3DF(qRound(p.xp*factor), qRound(p.yp*factor), qRound(p.zp*factor));
}

inline const QPoint3DF operator*(const QPoint3DF &p, int factor)
{
    return QPoint3DF(p.xp*factor, p.yp*factor, p.zp*factor);
}

inline const QPoint3DF operator*(float factor, const QPoint3DF &p)
{
    return QPoint3DF(qRound(p.xp*factor), qRound(p.yp*factor), qRound(p.zp*factor));
}

inline const QPoint3DF operator*(double factor, const QPoint3DF &p)
{
    return QPoint3DF(qRound(p.xp*factor), qRound(p.yp*factor), qRound(p.zp*factor));
}

inline const QPoint3DF operator*(int factor, const QPoint3DF &p)
{
    return QPoint3DF(p.xp*factor, p.yp*factor, p.zp*factor);
}

inline const QPoint3DF operator+(const QPoint3DF &p)
{
    return p;
}

inline const QPoint3DF operator-(const QPoint3DF &p)
{
    return QPoint3DF(-p.xp, -p.yp, -p.zp);
}

inline QPoint3DF &QPoint3DF::operator/=(double c)
{
    xp = qRound(xp/c);
    yp = qRound(yp/c);
    zp = qRound(zp/c);
    return *this;
}

inline const QPoint3DF operator/(const QPoint3DF &p, double c)
{
    return QPoint3DF(qRound(p.xp/c), qRound(p.yp/c), qRound(p.zp/c));
}

#endif // QPOINT3DF_H
