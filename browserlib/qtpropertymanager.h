/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the tools applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QTPROPERTYMANAGER_H
#define QTPROPERTYMANAGER_H

#include "qtpropertybrowser.h"
#include "qpoint3d.h"
#include "qpoint3df.h"
#include "qrotation3D.h"
#include "qtpropertybrowserutils_p.h"
#include <QtCore/QDateTime>
#include <QtCore/QLocale>
#include <QtCore/QMap>
#include <QtCore/QTimer>
#include <QtCore/QRegularExpression>
#include <QtGui/QIcon>
#include <QtCore/QMetaEnum>
#include <QtGui/QFontDatabase>
#include <QtWidgets/QStyleOption>
#include <QtWidgets/QStyle>
#include <QtWidgets/QApplication>
#include <QtGui/QPainter>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE
class QPoint3D;
class QPoint3DF;
class QRotation3D;
class QDate;
class QTime;
class QDateTime;
class QLocale;
class QRegularExpression;

//template functions
template <class PrivateData, class Value>
static void setSimpleMinimumData(PrivateData* data, const Value& minVal)
{
    data->minVal = minVal;
    if (data->maxVal < data->minVal)
        data->maxVal = data->minVal;

    if (data->val < data->minVal)
        data->val = data->minVal;
}

template <class PrivateData, class Value>
static void setSimpleMaximumData(PrivateData* data, const Value& maxVal)
{
    data->maxVal = maxVal;
    if (data->minVal > data->maxVal)
        data->minVal = data->maxVal;

    if (data->val > data->maxVal)
        data->val = data->maxVal;
}

template <class PrivateData, class Value>
static void setSizeMinimumData(PrivateData* data, const Value& newMinVal)
{
    data->minVal = newMinVal;
    if (data->maxVal.width() < data->minVal.width())
        data->maxVal.setWidth(data->minVal.width());
    if (data->maxVal.height() < data->minVal.height())
        data->maxVal.setHeight(data->minVal.height());

    if (data->val.width() < data->minVal.width())
        data->val.setWidth(data->minVal.width());
    if (data->val.height() < data->minVal.height())
        data->val.setHeight(data->minVal.height());
}

template <class PrivateData, class Value>
static void setSizeMaximumData(PrivateData* data, const Value& newMaxVal)
{
    data->maxVal = newMaxVal;
    if (data->minVal.width() > data->maxVal.width())
        data->minVal.setWidth(data->maxVal.width());
    if (data->minVal.height() > data->maxVal.height())
        data->minVal.setHeight(data->maxVal.height());

    if (data->val.width() > data->maxVal.width())
        data->val.setWidth(data->maxVal.width());
    if (data->val.height() > data->maxVal.height())
        data->val.setHeight(data->maxVal.height());
}

template <class SizeValue>
static SizeValue qBoundSize(const SizeValue& minVal, const SizeValue& val, const SizeValue& maxVal)
{
    SizeValue croppedVal = val;
    if (minVal.width() > val.width())
        croppedVal.setWidth(minVal.width());
    else if (maxVal.width() < val.width())
        croppedVal.setWidth(maxVal.width());

    if (minVal.height() > val.height())
        croppedVal.setHeight(minVal.height());
    else if (maxVal.height() < val.height())
        croppedVal.setHeight(maxVal.height());

    return croppedVal;
}

class QtGroupPropertyManager : public QtAbstractPropertyManager
{
    Q_OBJECT
public:
    QtGroupPropertyManager(QObject *parent = 0);
    ~QtGroupPropertyManager();

protected:
    bool hasValue(const QtProperty *property) const override;

    void initializeProperty(QtProperty *property) override;
    void uninitializeProperty(QtProperty *property) override;
};

class QtIntPropertyManager;
class QtIntPropertyManagerPrivate
{
    QtIntPropertyManager* q_ptr;
    Q_DECLARE_PUBLIC(QtIntPropertyManager)
public:

    struct Data
    {
        int val{ 0 };
        int minVal{ -INT_MAX };
        int maxVal{ INT_MAX };
        int singleStep{ 1 };
        int minimumValue() const { return minVal; }
        int maximumValue() const { return maxVal; }
        void setMinimumValue(int newMinVal) { setSimpleMinimumData(this, newMinVal); }
        void setMaximumValue(int newMaxVal) { setSimpleMaximumData(this, newMaxVal); }
    };

    typedef QMap<const QtProperty*, Data> PropertyValueMap;
    PropertyValueMap m_values;
};
class QtIntPropertyManager : public QtAbstractPropertyManager
{
    Q_OBJECT
public:
    QtIntPropertyManager(QObject *parent = 0);
    ~QtIntPropertyManager();

    int value(const QtProperty *property) const;
    int minimum(const QtProperty *property) const;
    int maximum(const QtProperty *property) const;
    int singleStep(const QtProperty *property) const;

public Q_SLOTS:
    void setValue(QtProperty *property, int val);
    void setMinimum(QtProperty *property, int minVal);
    void setMaximum(QtProperty *property, int maxVal);
    void setRange(QtProperty *property, int minVal, int maxVal);
    void setSingleStep(QtProperty *property, int step);
Q_SIGNALS:
    void valueChanged(QtProperty *property, int val);
    void rangeChanged(QtProperty *property, int minVal, int maxVal);
    void singleStepChanged(QtProperty *property, int step);
protected:
    QString valueText(const QtProperty *property) const override;
    void initializeProperty(QtProperty *property) override;
    void uninitializeProperty(QtProperty *property) override;
private:
    QScopedPointer<QtIntPropertyManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtIntPropertyManager)
    Q_DISABLE_COPY_MOVE(QtIntPropertyManager)
};

class QtBoolPropertyManager;
class QtBoolPropertyManagerPrivate
{
    QtBoolPropertyManager* q_ptr;
    Q_DECLARE_PUBLIC(QtBoolPropertyManager)
public:
    QtBoolPropertyManagerPrivate();

    QMap<const QtProperty*, bool> m_values;
    const QIcon m_checkedIcon;
    const QIcon m_uncheckedIcon;
};
class QtBoolPropertyManager : public QtAbstractPropertyManager
{
    Q_OBJECT
public:
    QtBoolPropertyManager(QObject *parent = 0);
    ~QtBoolPropertyManager();

    bool value(const QtProperty *property) const;

public Q_SLOTS:
    void setValue(QtProperty *property, bool val);
Q_SIGNALS:
    void valueChanged(QtProperty *property, bool val);
protected:
    QString valueText(const QtProperty *property) const override;
    QIcon valueIcon(const QtProperty *property) const override;
    void initializeProperty(QtProperty *property) override;
    void uninitializeProperty(QtProperty *property) override;
private:
    QScopedPointer<QtBoolPropertyManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtBoolPropertyManager)
    Q_DISABLE_COPY_MOVE(QtBoolPropertyManager)
};

class QtDoublePropertyManager;
class QtDoublePropertyManagerPrivate
{
    QtDoublePropertyManager* q_ptr;
    Q_DECLARE_PUBLIC(QtDoublePropertyManager)
public:

    struct Data
    {
        double val{ 0 };
        double minVal{ -DBL_MAX };
        double maxVal{ DBL_MAX };
        double singleStep{ 1 };
        int decimals{ 2 };
        double minimumValue() const { return minVal; }
        double maximumValue() const { return maxVal; }
        void setMinimumValue(double newMinVal) { setSimpleMinimumData(this, newMinVal); }
        void setMaximumValue(double newMaxVal) { setSimpleMaximumData(this, newMaxVal); }
    };

    typedef QMap<const QtProperty*, Data> PropertyValueMap;
    PropertyValueMap m_values;
};
class QtDoublePropertyManager : public QtAbstractPropertyManager
{
    Q_OBJECT
public:
    QtDoublePropertyManager(QObject *parent = 0);
    ~QtDoublePropertyManager();

    double value(const QtProperty *property) const;
    double minimum(const QtProperty *property) const;
    double maximum(const QtProperty *property) const;
    double singleStep(const QtProperty *property) const;
    int decimals(const QtProperty *property) const;

public Q_SLOTS:
    void setValue(QtProperty *property, double val);
    void setMinimum(QtProperty *property, double minVal);
    void setMaximum(QtProperty *property, double maxVal);
    void setRange(QtProperty *property, double minVal, double maxVal);
    void setSingleStep(QtProperty *property, double step);
    void setDecimals(QtProperty *property, int prec);
Q_SIGNALS:
    void valueChanged(QtProperty *property, double val);
    void rangeChanged(QtProperty *property, double minVal, double maxVal);
    void singleStepChanged(QtProperty *property, double step);
    void decimalsChanged(QtProperty *property, int prec);
protected:
    QString valueText(const QtProperty *property) const override;
    void initializeProperty(QtProperty *property) override;
    void uninitializeProperty(QtProperty *property) override;
private:
    QScopedPointer<QtDoublePropertyManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtDoublePropertyManager)
    Q_DISABLE_COPY_MOVE(QtDoublePropertyManager)
};

class QtStringPropertyManager;
class QtStringPropertyManagerPrivate
{
    QtStringPropertyManager* q_ptr;
    Q_DECLARE_PUBLIC(QtStringPropertyManager)
public:

    struct Data
    {
        QString val;
        QRegularExpression regExp;
    };

    typedef QMap<const QtProperty*, Data> PropertyValueMap;
    QMap<const QtProperty*, Data> m_values;
};
class QtStringPropertyManager : public QtAbstractPropertyManager
{
    Q_OBJECT
public:
    QtStringPropertyManager(QObject *parent = 0);
    ~QtStringPropertyManager();

    QString value(const QtProperty *property) const;
    QRegularExpression regExp(const QtProperty *property) const;

public Q_SLOTS:
    void setValue(QtProperty *property, const QString &val);
    void setRegExp(QtProperty *property, const QRegularExpression &regExp);
Q_SIGNALS:
    void valueChanged(QtProperty *property, const QString &val);
    void regExpChanged(QtProperty *property, const QRegularExpression &regExp);
protected:
    QString valueText(const QtProperty *property) const override;
    void initializeProperty(QtProperty *property) override;
    void uninitializeProperty(QtProperty *property) override;
private:
    QScopedPointer<QtStringPropertyManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtStringPropertyManager)
    Q_DISABLE_COPY_MOVE(QtStringPropertyManager)
};

class QtDatePropertyManager;
class QtDatePropertyManagerPrivate
{
    QtDatePropertyManager* q_ptr;
    Q_DECLARE_PUBLIC(QtDatePropertyManager)
public:
    explicit QtDatePropertyManagerPrivate(QtDatePropertyManager* q);

    struct Data
    {
        QDate val{ QDate::currentDate() };
        QDate minVal{ QDate(1752, 9, 14) };
        QDate maxVal{ QDate(9999, 12, 31) };
        QDate minimumValue() const { return minVal; }
        QDate maximumValue() const { return maxVal; }
        void setMinimumValue(QDate newMinVal) { setSimpleMinimumData(this, newMinVal); }
        void setMaximumValue(QDate newMaxVal) { setSimpleMaximumData(this, newMaxVal); }
    };

    QString m_format;

    typedef QMap<const QtProperty*, Data> PropertyValueMap;
    QMap<const QtProperty*, Data> m_values;
};
class QtDatePropertyManager : public QtAbstractPropertyManager
{
    Q_OBJECT
public:
    QtDatePropertyManager(QObject *parent = 0);
    ~QtDatePropertyManager();

    QDate value(const QtProperty *property) const;
    QDate minimum(const QtProperty *property) const;
    QDate maximum(const QtProperty *property) const;

public Q_SLOTS:
    void setValue(QtProperty *property, QDate val);
    void setMinimum(QtProperty *property, QDate minVal);
    void setMaximum(QtProperty *property, QDate maxVal);
    void setRange(QtProperty *property, QDate minVal, QDate maxVal);
Q_SIGNALS:
    void valueChanged(QtProperty *property, QDate val);
    void rangeChanged(QtProperty *property, QDate minVal, QDate maxVal);
protected:
    QString valueText(const QtProperty *property) const override;
    void initializeProperty(QtProperty *property) override;
    void uninitializeProperty(QtProperty *property) override;
private:
    QScopedPointer<QtDatePropertyManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtDatePropertyManager)
    Q_DISABLE_COPY_MOVE(QtDatePropertyManager)
};

class QtTimePropertyManager;
class QtTimePropertyManagerPrivate
{
    QtTimePropertyManager* q_ptr;
    Q_DECLARE_PUBLIC(QtTimePropertyManager)
public:
    explicit QtTimePropertyManagerPrivate(QtTimePropertyManager* q);

    const QString m_format;

    typedef QMap<const QtProperty*, QTime> PropertyValueMap;
    PropertyValueMap m_values;
};
class QtTimePropertyManager : public QtAbstractPropertyManager
{
    Q_OBJECT
public:
    QtTimePropertyManager(QObject *parent = 0);
    ~QtTimePropertyManager();

    QTime value(const QtProperty *property) const;

public Q_SLOTS:
    void setValue(QtProperty *property, QTime val);
Q_SIGNALS:
    void valueChanged(QtProperty *property, QTime val);
protected:
    QString valueText(const QtProperty *property) const override;
    void initializeProperty(QtProperty *property) override;
    void uninitializeProperty(QtProperty *property) override;
private:
    QScopedPointer<QtTimePropertyManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtTimePropertyManager)
    Q_DISABLE_COPY_MOVE(QtTimePropertyManager)
};

class QtDateTimePropertyManager;
class QtDateTimePropertyManagerPrivate
{
    QtDateTimePropertyManager* q_ptr;
    Q_DECLARE_PUBLIC(QtDateTimePropertyManager)
public:
    explicit QtDateTimePropertyManagerPrivate(QtDateTimePropertyManager* q);

    const QString m_format;

    typedef QMap<const QtProperty*, QDateTime> PropertyValueMap;
    PropertyValueMap m_values;
};
class QtDateTimePropertyManager : public QtAbstractPropertyManager
{
    Q_OBJECT
public:
    QtDateTimePropertyManager(QObject *parent = 0);
    ~QtDateTimePropertyManager();

    QDateTime value(const QtProperty *property) const;

public Q_SLOTS:
    void setValue(QtProperty *property, const QDateTime &val);
Q_SIGNALS:
    void valueChanged(QtProperty *property, const QDateTime &val);
protected:
    QString valueText(const QtProperty *property) const override;
    void initializeProperty(QtProperty *property) override;
    void uninitializeProperty(QtProperty *property) override;
private:
    QScopedPointer<QtDateTimePropertyManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtDateTimePropertyManager)
    Q_DISABLE_COPY_MOVE(QtDateTimePropertyManager)
};

class QtKeySequencePropertyManager;
class QtKeySequencePropertyManagerPrivate
{
    QtKeySequencePropertyManager* q_ptr;
    Q_DECLARE_PUBLIC(QtKeySequencePropertyManager)
public:

    QString m_format;

    typedef QMap<const QtProperty*, QKeySequence> PropertyValueMap;
    PropertyValueMap m_values;
};
class QtKeySequencePropertyManager : public QtAbstractPropertyManager
{
    Q_OBJECT
public:
    QtKeySequencePropertyManager(QObject *parent = 0);
    ~QtKeySequencePropertyManager();

    QKeySequence value(const QtProperty *property) const;

public Q_SLOTS:
    void setValue(QtProperty *property, const QKeySequence &val);
Q_SIGNALS:
    void valueChanged(QtProperty *property, const QKeySequence &val);
protected:
    QString valueText(const QtProperty *property) const override;
    void initializeProperty(QtProperty *property) override;
    void uninitializeProperty(QtProperty *property) override;
private:
    QScopedPointer<QtKeySequencePropertyManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtKeySequencePropertyManager)
    Q_DISABLE_COPY_MOVE(QtKeySequencePropertyManager)
};

class QtCharPropertyManager;
class QtCharPropertyManagerPrivate
{
    QtCharPropertyManager* q_ptr;
    Q_DECLARE_PUBLIC(QtCharPropertyManager)
public:

    typedef QMap<const QtProperty*, QChar> PropertyValueMap;
    PropertyValueMap m_values;
};
class QtCharPropertyManager : public QtAbstractPropertyManager
{
    Q_OBJECT
public:
    QtCharPropertyManager(QObject *parent = 0);
    ~QtCharPropertyManager();

    QChar value(const QtProperty *property) const;

public Q_SLOTS:
    void setValue(QtProperty *property, const QChar &val);
Q_SIGNALS:
    void valueChanged(QtProperty *property, const QChar &val);
protected:
    QString valueText(const QtProperty *property) const override;
    void initializeProperty(QtProperty *property) override;
    void uninitializeProperty(QtProperty *property) override;
private:
    QScopedPointer<QtCharPropertyManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtCharPropertyManager)
    Q_DISABLE_COPY_MOVE(QtCharPropertyManager)
};

class QtEnumPropertyManager;

class QtLocalePropertyManager;
class QtLocalePropertyManagerPrivate
{
    QtLocalePropertyManager* q_ptr;
    Q_DECLARE_PUBLIC(QtLocalePropertyManager)
public:

    QtLocalePropertyManagerPrivate();

    void slotEnumChanged(QtProperty* property, int value);
    void slotPropertyDestroyed(QtProperty* property);

    typedef QMap<const QtProperty*, QLocale> PropertyValueMap;
    PropertyValueMap m_values;

    QtEnumPropertyManager* m_enumPropertyManager;

    QMap<const QtProperty*, QtProperty*> m_propertyToLanguage;
    QMap<const QtProperty*, QtProperty*> m_propertyToCountry;

    QMap<const QtProperty*, QtProperty*> m_languageToProperty;
    QMap<const QtProperty*, QtProperty*> m_countryToProperty;
};
class QtLocalePropertyManager : public QtAbstractPropertyManager
{
    Q_OBJECT
public:
    QtLocalePropertyManager(QObject *parent = 0);
    ~QtLocalePropertyManager();

    QtEnumPropertyManager *subEnumPropertyManager() const;

    QLocale value(const QtProperty *property) const;

public Q_SLOTS:
    void setValue(QtProperty *property, const QLocale &val);
Q_SIGNALS:
    void valueChanged(QtProperty *property, const QLocale &val);
protected:
    QString valueText(const QtProperty *property) const override;
    void initializeProperty(QtProperty *property) override;
    void uninitializeProperty(QtProperty *property) override;
private:
    QScopedPointer<QtLocalePropertyManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtLocalePropertyManager)
    Q_DISABLE_COPY_MOVE(QtLocalePropertyManager)
    Q_PRIVATE_SLOT(d_func(), void slotEnumChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotPropertyDestroyed(QtProperty *))
};

class QtPointPropertyManager;
class QtPointPropertyManagerPrivate
{
    QtPointPropertyManager* q_ptr;
    Q_DECLARE_PUBLIC(QtPointPropertyManager)
public:

    void slotIntChanged(QtProperty* property, int value);
    void slotPropertyDestroyed(QtProperty* property);

    typedef QMap<const QtProperty*, QPoint> PropertyValueMap;
    PropertyValueMap m_values;

    QtIntPropertyManager* m_intPropertyManager;

    QMap<const QtProperty*, QtProperty*> m_propertyToX;
    QMap<const QtProperty*, QtProperty*> m_propertyToY;

    QMap<const QtProperty*, QtProperty*> m_xToProperty;
    QMap<const QtProperty*, QtProperty*> m_yToProperty;
};
class QtPointPropertyManager : public QtAbstractPropertyManager
{
    Q_OBJECT
public:
    QtPointPropertyManager(QObject *parent = 0);
    ~QtPointPropertyManager();

    QtIntPropertyManager *subIntPropertyManager() const;

    QPoint value(const QtProperty *property) const;

public Q_SLOTS:
    void setValue(QtProperty *property, const QPoint &val);
Q_SIGNALS:
    void valueChanged(QtProperty *property, const QPoint &val);
protected:
    QString valueText(const QtProperty *property) const override;
    void initializeProperty(QtProperty *property) override;
    void uninitializeProperty(QtProperty *property) override;
private:
    QScopedPointer<QtPointPropertyManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtPointPropertyManager)
    Q_DISABLE_COPY_MOVE(QtPointPropertyManager)
    Q_PRIVATE_SLOT(d_func(), void slotIntChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotPropertyDestroyed(QtProperty *))
};

//########### start of QtPoint3DPropertyManager declaration
class QtPoint3DPropertyManager;
class QtPoint3DPropertyManagerPrivate
{
    QtPoint3DPropertyManager* q_ptr;
    Q_DECLARE_PUBLIC(QtPoint3DPropertyManager)
public:

    void slotIntChanged(QtProperty* property, int value);
    void slotPropertyDestroyed(QtProperty* property);

    typedef QMap<const QtProperty*, QPoint3D> PropertyValueMap;
    PropertyValueMap m_values;

    QtIntPropertyManager* m_intPropertyManager;

    QMap<const QtProperty*, QtProperty*> m_propertyToX;
    QMap<const QtProperty*, QtProperty*> m_propertyToY;
    QMap<const QtProperty*, QtProperty*> m_propertyToZ;

    QMap<const QtProperty*, QtProperty*> m_xToProperty;
    QMap<const QtProperty*, QtProperty*> m_yToProperty;
    QMap<const QtProperty*, QtProperty*> m_zToProperty;
};
class QtPoint3DPropertyManager : public QtAbstractPropertyManager
{
    Q_OBJECT
public:
    QtPoint3DPropertyManager(QObject *parent = 0);
    ~QtPoint3DPropertyManager();

    QtIntPropertyManager *subIntPropertyManager() const;

    QPoint3D value(const QtProperty *property) const;

public Q_SLOTS:
    void setValue(QtProperty *property, const QPoint3D &val);
Q_SIGNALS:
    void valueChanged(QtProperty *property, const QPoint3D &val);
protected:
    QString valueText(const QtProperty *property) const override;
    void initializeProperty(QtProperty *property) override;
    void uninitializeProperty(QtProperty *property) override;
private:
    QScopedPointer<QtPoint3DPropertyManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtPoint3DPropertyManager)
    Q_DISABLE_COPY_MOVE(QtPoint3DPropertyManager)
    Q_PRIVATE_SLOT(d_func(), void slotIntChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotPropertyDestroyed(QtProperty *))
};
//########## end of QtPoint3DPropertyManager declaration

//########## start of QtPoint3DFPropertyManager declaration
class QtPoint3DFPropertyManager;
class QtPoint3DFPropertyManagerPrivate
{
    QtPoint3DFPropertyManager* q_ptr;
    Q_DECLARE_PUBLIC(QtPoint3DFPropertyManager)
public:

    struct Data
    {
        QPoint3DF val;
        int decimals{ 2 };
    };

    void slotDoubleChanged(QtProperty* property, double value);
    void slotPropertyDestroyed(QtProperty* property);

    typedef QMap<const QtProperty*, Data> PropertyValueMap;
    PropertyValueMap m_values;

    QtDoublePropertyManager* m_doublePropertyManager;

    QMap<const QtProperty*, QtProperty*> m_propertyToX;
    QMap<const QtProperty*, QtProperty*> m_propertyToY;
    QMap<const QtProperty*, QtProperty*> m_propertyToZ;

    QMap<const QtProperty*, QtProperty*> m_xToProperty;
    QMap<const QtProperty*, QtProperty*> m_yToProperty;
    QMap<const QtProperty*, QtProperty*> m_zToProperty;
};
class QtPoint3DFPropertyManager : public QtAbstractPropertyManager
{
    Q_OBJECT
public:
    QtPoint3DFPropertyManager(QObject *parent = 0);
    ~QtPoint3DFPropertyManager();

    QtDoublePropertyManager *subDoublePropertyManager() const;

    QPoint3DF value(const QtProperty *property) const;
    int decimals(const QtProperty *property) const;

public Q_SLOTS:
    void setValue(QtProperty *property, const QPoint3DF &val);
    void setDecimals(QtProperty *property, int prec);
Q_SIGNALS:
    void valueChanged(QtProperty *property, const QPoint3DF &val);
    void decimalsChanged(QtProperty *property, int prec);
protected:
    QString valueText(const QtProperty *property) const override;
    void initializeProperty(QtProperty *property) override;
    void uninitializeProperty(QtProperty *property) override;
private:
    QScopedPointer<QtPoint3DFPropertyManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtPoint3DFPropertyManager)
    Q_DISABLE_COPY_MOVE(QtPoint3DFPropertyManager)
    Q_PRIVATE_SLOT(d_func(), void slotDoubleChanged(QtProperty *, double))
    Q_PRIVATE_SLOT(d_func(), void slotPropertyDestroyed(QtProperty *))
};
//########## end of QtPoint3DFPropertyManager declaration

//########### start of QtRotation3DPropertyManager declaration
class QtRotation3DPropertyManager;
class QtRotation3DPropertyManagerPrivate
{
    QtRotation3DPropertyManager* q_ptr;
    Q_DECLARE_PUBLIC(QtRotation3DPropertyManager)
public:

    void slotIntChanged(QtProperty* property, int value);
    void slotPropertyDestroyed(QtProperty* property);

    typedef QMap<const QtProperty*, QRotation3D> PropertyValueMap;
    PropertyValueMap m_values;

    QtIntPropertyManager* m_intPropertyManager;

    QMap<const QtProperty*, QtProperty*> m_propertyToX;
    QMap<const QtProperty*, QtProperty*> m_propertyToY;
    QMap<const QtProperty*, QtProperty*> m_propertyToZ;

    QMap<const QtProperty*, QtProperty*> m_xToProperty;
    QMap<const QtProperty*, QtProperty*> m_yToProperty;
    QMap<const QtProperty*, QtProperty*> m_zToProperty;
};
class QtRotation3DPropertyManager : public QtAbstractPropertyManager
{
    Q_OBJECT
public:
    QtRotation3DPropertyManager(QObject *parent = 0);
    ~QtRotation3DPropertyManager();

    QtIntPropertyManager *subIntPropertyManager()const;

    QRotation3D value(const QtProperty *property) const;

public Q_SLOTS:
    void setValue(QtProperty *property, const QRotation3D &val);
Q_SIGNALS:
    void valueChanged(QtProperty *property, const QRotation3D &val);
protected:
    QString valueText(const QtProperty *property) const override;
    void initializeProperty(QtProperty *property) override;
    void uninitializeProperty(QtProperty *property) override;
private:
    QScopedPointer<QtRotation3DPropertyManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtRotation3DPropertyManager)
    Q_DISABLE_COPY_MOVE(QtRotation3DPropertyManager)
    Q_PRIVATE_SLOT(d_func(), void slotIntChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotPropertyDestroyed(QtProperty *))
};
//########## end of QtRotation3DPropertyManager declaration

class QtPointFPropertyManager;
class QtPointFPropertyManagerPrivate
{
    QtPointFPropertyManager* q_ptr;
    Q_DECLARE_PUBLIC(QtPointFPropertyManager)
public:

    struct Data
    {
        QPointF val;
        int decimals{ 2 };
    };

    void slotDoubleChanged(QtProperty* property, double value);
    void slotPropertyDestroyed(QtProperty* property);

    typedef QMap<const QtProperty*, Data> PropertyValueMap;
    PropertyValueMap m_values;

    QtDoublePropertyManager* m_doublePropertyManager;

    QMap<const QtProperty*, QtProperty*> m_propertyToX;
    QMap<const QtProperty*, QtProperty*> m_propertyToY;

    QMap<const QtProperty*, QtProperty*> m_xToProperty;
    QMap<const QtProperty*, QtProperty*> m_yToProperty;
};
class QtPointFPropertyManager : public QtAbstractPropertyManager
{
    Q_OBJECT
public:
    QtPointFPropertyManager(QObject *parent = 0);
    ~QtPointFPropertyManager();

    QtDoublePropertyManager *subDoublePropertyManager() const;

    QPointF value(const QtProperty *property) const;
    int decimals(const QtProperty *property) const;

public Q_SLOTS:
    void setValue(QtProperty *property, const QPointF &val);
    void setDecimals(QtProperty *property, int prec);
Q_SIGNALS:
    void valueChanged(QtProperty *property, const QPointF &val);
    void decimalsChanged(QtProperty *property, int prec);
protected:
    QString valueText(const QtProperty *property) const override;
    void initializeProperty(QtProperty *property) override;
    void uninitializeProperty(QtProperty *property) override;
private:
    QScopedPointer<QtPointFPropertyManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtPointFPropertyManager)
    Q_DISABLE_COPY_MOVE(QtPointFPropertyManager)
    Q_PRIVATE_SLOT(d_func(), void slotDoubleChanged(QtProperty *, double))
    Q_PRIVATE_SLOT(d_func(), void slotPropertyDestroyed(QtProperty *))
};

//##########

class QtSizePropertyManager;
class QtSizePropertyManagerPrivate
{
    QtSizePropertyManager* q_ptr;
    Q_DECLARE_PUBLIC(QtSizePropertyManager)
public:

    void slotIntChanged(QtProperty* property, int value);
    void slotPropertyDestroyed(QtProperty* property);
    void setValue(QtProperty* property, const QSize& val);
    void setRange(QtProperty* property,
        const QSize& minVal, const QSize& maxVal, const QSize& val);

    struct Data
    {
        QSize val{ 0, 0 };
        QSize minVal{ 0, 0 };
        QSize maxVal{ INT_MAX, INT_MAX };
        QSize minimumValue() const { return minVal; }
        QSize maximumValue() const { return maxVal; }
        void setMinimumValue(const QSize& newMinVal) { setSizeMinimumData(this, newMinVal); }
        void setMaximumValue(const QSize& newMaxVal) { setSizeMaximumData(this, newMaxVal); }
    };

    typedef QMap<const QtProperty*, Data> PropertyValueMap;
    PropertyValueMap m_values;

    QtIntPropertyManager* m_intPropertyManager;

    QMap<const QtProperty*, QtProperty*> m_propertyToW;
    QMap<const QtProperty*, QtProperty*> m_propertyToH;

    QMap<const QtProperty*, QtProperty*> m_wToProperty;
    QMap<const QtProperty*, QtProperty*> m_hToProperty;
};
class QtSizePropertyManager : public QtAbstractPropertyManager
{
    Q_OBJECT
public:
    QtSizePropertyManager(QObject *parent = 0);
    ~QtSizePropertyManager();

    QtIntPropertyManager *subIntPropertyManager() const;

    QSize value(const QtProperty *property) const;
    QSize minimum(const QtProperty *property) const;
    QSize maximum(const QtProperty *property) const;

public Q_SLOTS:
    void setValue(QtProperty *property, const QSize &val);
    void setMinimum(QtProperty *property, const QSize &minVal);
    void setMaximum(QtProperty *property, const QSize &maxVal);
    void setRange(QtProperty *property, const QSize &minVal, const QSize &maxVal);
Q_SIGNALS:
    void valueChanged(QtProperty *property, const QSize &val);
    void rangeChanged(QtProperty *property, const QSize &minVal, const QSize &maxVal);
protected:
    QString valueText(const QtProperty *property) const override;
    void initializeProperty(QtProperty *property) override;
    void uninitializeProperty(QtProperty *property) override;
private:
    QScopedPointer<QtSizePropertyManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtSizePropertyManager)
    Q_DISABLE_COPY_MOVE(QtSizePropertyManager)
    Q_PRIVATE_SLOT(d_func(), void slotIntChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotPropertyDestroyed(QtProperty *))
};

class QtSizeFPropertyManager;
class QtSizeFPropertyManagerPrivate
{
    QtSizeFPropertyManager* q_ptr;
    Q_DECLARE_PUBLIC(QtSizeFPropertyManager)
public:

    void slotDoubleChanged(QtProperty* property, double value);
    void slotPropertyDestroyed(QtProperty* property);
    void setValue(QtProperty* property, const QSizeF& val);
    void setRange(QtProperty* property,
        const QSizeF& minVal, const QSizeF& maxVal, const QSizeF& val);

    struct Data
    {
        QSizeF val{ 0, 0 };
        QSizeF minVal{ 0, 0 };
        QSizeF maxVal{ std::numeric_limits<qreal>::max(), std::numeric_limits<qreal>::max() };
        int decimals{ 2 };
        QSizeF minimumValue() const { return minVal; }
        QSizeF maximumValue() const { return maxVal; }
        void setMinimumValue(const QSizeF& newMinVal) { setSizeMinimumData(this, newMinVal); }
        void setMaximumValue(const QSizeF& newMaxVal) { setSizeMaximumData(this, newMaxVal); }
    };
    typedef QMap<const QtProperty*, Data> PropertyValueMap;
    PropertyValueMap m_values;

    QtDoublePropertyManager* m_doublePropertyManager;

    QMap<const QtProperty*, QtProperty*> m_propertyToW;
    QMap<const QtProperty*, QtProperty*> m_propertyToH;

    QMap<const QtProperty*, QtProperty*> m_wToProperty;
    QMap<const QtProperty*, QtProperty*> m_hToProperty;
};
class QtSizeFPropertyManager : public QtAbstractPropertyManager
{
    Q_OBJECT
public:
    QtSizeFPropertyManager(QObject *parent = 0);
    ~QtSizeFPropertyManager();

    QtDoublePropertyManager *subDoublePropertyManager() const;

    QSizeF value(const QtProperty *property) const;
    QSizeF minimum(const QtProperty *property) const;
    QSizeF maximum(const QtProperty *property) const;
    int decimals(const QtProperty *property) const;

public Q_SLOTS:
    void setValue(QtProperty *property, const QSizeF &val);
    void setMinimum(QtProperty *property, const QSizeF &minVal);
    void setMaximum(QtProperty *property, const QSizeF &maxVal);
    void setRange(QtProperty *property, const QSizeF &minVal, const QSizeF &maxVal);
    void setDecimals(QtProperty *property, int prec);
Q_SIGNALS:
    void valueChanged(QtProperty *property, const QSizeF &val);
    void rangeChanged(QtProperty *property, const QSizeF &minVal, const QSizeF &maxVal);
    void decimalsChanged(QtProperty *property, int prec);
protected:
    QString valueText(const QtProperty *property) const override;
    void initializeProperty(QtProperty *property) override;
    void uninitializeProperty(QtProperty *property) override;
private:
    QScopedPointer<QtSizeFPropertyManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtSizeFPropertyManager)
    Q_DISABLE_COPY_MOVE(QtSizeFPropertyManager)
    Q_PRIVATE_SLOT(d_func(), void slotDoubleChanged(QtProperty *, double))
    Q_PRIVATE_SLOT(d_func(), void slotPropertyDestroyed(QtProperty *))
};

class QtRectPropertyManager;
class QtRectPropertyManagerPrivate
{
    QtRectPropertyManager* q_ptr;
    Q_DECLARE_PUBLIC(QtRectPropertyManager)
public:

    void slotIntChanged(QtProperty* property, int value);
    void slotPropertyDestroyed(QtProperty* property);
    void setConstraint(QtProperty* property, const QRect& constraint, const QRect& val);

    struct Data
    {
        QRect val{ 0, 0, 0, 0 };
        QRect constraint;
    };

    typedef QMap<const QtProperty*, Data> PropertyValueMap;
    PropertyValueMap m_values;

    QtIntPropertyManager* m_intPropertyManager;

    QMap<const QtProperty*, QtProperty*> m_propertyToX;
    QMap<const QtProperty*, QtProperty*> m_propertyToY;
    QMap<const QtProperty*, QtProperty*> m_propertyToW;
    QMap<const QtProperty*, QtProperty*> m_propertyToH;

    QMap<const QtProperty*, QtProperty*> m_xToProperty;
    QMap<const QtProperty*, QtProperty*> m_yToProperty;
    QMap<const QtProperty*, QtProperty*> m_wToProperty;
    QMap<const QtProperty*, QtProperty*> m_hToProperty;
};
class QtRectPropertyManager : public QtAbstractPropertyManager
{
    Q_OBJECT
public:
    QtRectPropertyManager(QObject *parent = 0);
    ~QtRectPropertyManager();

    QtIntPropertyManager *subIntPropertyManager() const;

    QRect value(const QtProperty *property) const;
    QRect constraint(const QtProperty *property) const;

public Q_SLOTS:
    void setValue(QtProperty *property, const QRect &val);
    void setConstraint(QtProperty *property, const QRect &constraint);
Q_SIGNALS:
    void valueChanged(QtProperty *property, const QRect &val);
    void constraintChanged(QtProperty *property, const QRect &constraint);
protected:
    QString valueText(const QtProperty *property) const override;
    void initializeProperty(QtProperty *property) override;
    void uninitializeProperty(QtProperty *property) override;
private:
    QScopedPointer<QtRectPropertyManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtRectPropertyManager)
    Q_DISABLE_COPY_MOVE(QtRectPropertyManager)
    Q_PRIVATE_SLOT(d_func(), void slotIntChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotPropertyDestroyed(QtProperty *))
};

class QtRectFPropertyManager;
class QtRectFPropertyManagerPrivate
{
    QtRectFPropertyManager* q_ptr;
    Q_DECLARE_PUBLIC(QtRectFPropertyManager)
public:

    void slotDoubleChanged(QtProperty* property, double value);
    void slotPropertyDestroyed(QtProperty* property);
    void setConstraint(QtProperty* property, const QRectF& constraint, const QRectF& val);

    struct Data
    {
        QRectF val{ 0, 0, 0, 0 };
        QRectF constraint;
        int decimals{ 2 };
    };

    typedef QMap<const QtProperty*, Data> PropertyValueMap;
    PropertyValueMap m_values;

    QtDoublePropertyManager* m_doublePropertyManager;

    QMap<const QtProperty*, QtProperty*> m_propertyToX;
    QMap<const QtProperty*, QtProperty*> m_propertyToY;
    QMap<const QtProperty*, QtProperty*> m_propertyToW;
    QMap<const QtProperty*, QtProperty*> m_propertyToH;

    QMap<const QtProperty*, QtProperty*> m_xToProperty;
    QMap<const QtProperty*, QtProperty*> m_yToProperty;
    QMap<const QtProperty*, QtProperty*> m_wToProperty;
    QMap<const QtProperty*, QtProperty*> m_hToProperty;
};
class QtRectFPropertyManager : public QtAbstractPropertyManager
{
    Q_OBJECT
public:
    QtRectFPropertyManager(QObject *parent = 0);
    ~QtRectFPropertyManager();

    QtDoublePropertyManager *subDoublePropertyManager() const;

    QRectF value(const QtProperty *property) const;
    QRectF constraint(const QtProperty *property) const;
    int decimals(const QtProperty *property) const;

public Q_SLOTS:
    void setValue(QtProperty *property, const QRectF &val);
    void setConstraint(QtProperty *property, const QRectF &constraint);
    void setDecimals(QtProperty *property, int prec);
Q_SIGNALS:
    void valueChanged(QtProperty *property, const QRectF &val);
    void constraintChanged(QtProperty *property, const QRectF &constraint);
    void decimalsChanged(QtProperty *property, int prec);
protected:
    QString valueText(const QtProperty *property) const override;
    void initializeProperty(QtProperty *property) override;
    void uninitializeProperty(QtProperty *property) override;
private:
    QScopedPointer<QtRectFPropertyManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtRectFPropertyManager)
    Q_DISABLE_COPY_MOVE(QtRectFPropertyManager)
    Q_PRIVATE_SLOT(d_func(), void slotDoubleChanged(QtProperty *, double))
    Q_PRIVATE_SLOT(d_func(), void slotPropertyDestroyed(QtProperty *))
};

class QtEnumPropertyManager;
class QtEnumPropertyManagerPrivate
{
    QtEnumPropertyManager* q_ptr;
    Q_DECLARE_PUBLIC(QtEnumPropertyManager)
public:

    struct Data
    {
        int val{ -1 };
        QStringList enumNames;
        QMap<int, QIcon> enumIcons;
    };

    typedef QMap<const QtProperty*, Data> PropertyValueMap;
    PropertyValueMap m_values;
};
class QtEnumPropertyManager : public QtAbstractPropertyManager
{
    Q_OBJECT
public:
    QtEnumPropertyManager(QObject *parent = 0);
    ~QtEnumPropertyManager();

    int value(const QtProperty *property) const;
    QStringList enumNames(const QtProperty *property) const;
    QMap<int, QIcon> enumIcons(const QtProperty *property) const;

public Q_SLOTS:
    void setValue(QtProperty *property, int val);
    void setEnumNames(QtProperty *property, const QStringList &names);
    void setEnumIcons(QtProperty *property, const QMap<int, QIcon> &icons);
Q_SIGNALS:
    void valueChanged(QtProperty *property, int val);
    void enumNamesChanged(QtProperty *property, const QStringList &names);
    void enumIconsChanged(QtProperty *property, const QMap<int, QIcon> &icons);
protected:
    QString valueText(const QtProperty *property) const override;
    QIcon valueIcon(const QtProperty *property) const override;
    void initializeProperty(QtProperty *property) override;
    void uninitializeProperty(QtProperty *property) override;
private:
    QScopedPointer<QtEnumPropertyManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtEnumPropertyManager)
    Q_DISABLE_COPY_MOVE(QtEnumPropertyManager)
};

class QtFlagPropertyManager;
class QtFlagPropertyManagerPrivate
{
    QtFlagPropertyManager* q_ptr;
    Q_DECLARE_PUBLIC(QtFlagPropertyManager)
public:

    void slotBoolChanged(QtProperty* property, bool value);
    void slotPropertyDestroyed(QtProperty* property);

    struct Data
    {
        int val{ -1 };
        QStringList flagNames;
    };

    typedef QMap<const QtProperty*, Data> PropertyValueMap;
    PropertyValueMap m_values;

    QtBoolPropertyManager* m_boolPropertyManager;

    QMap<const QtProperty*, QList<QtProperty*> > m_propertyToFlags;

    QMap<const QtProperty*, QtProperty*> m_flagToProperty;
};
class QtFlagPropertyManager : public QtAbstractPropertyManager
{
    Q_OBJECT
public:
    QtFlagPropertyManager(QObject *parent = 0);
    ~QtFlagPropertyManager();

    QtBoolPropertyManager *subBoolPropertyManager() const;

    int value(const QtProperty *property) const;
    QStringList flagNames(const QtProperty *property) const;

public Q_SLOTS:
    void setValue(QtProperty *property, int val);
    void setFlagNames(QtProperty *property, const QStringList &names);
Q_SIGNALS:
    void valueChanged(QtProperty *property, int val);
    void flagNamesChanged(QtProperty *property, const QStringList &names);
protected:
    QString valueText(const QtProperty *property) const override;
    void initializeProperty(QtProperty *property) override;
    void uninitializeProperty(QtProperty *property) override;
private:
    QScopedPointer<QtFlagPropertyManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtFlagPropertyManager)
    Q_DISABLE_COPY_MOVE(QtFlagPropertyManager)
    Q_PRIVATE_SLOT(d_func(), void slotBoolChanged(QtProperty *, bool))
    Q_PRIVATE_SLOT(d_func(), void slotPropertyDestroyed(QtProperty *))
};

class QtSizePolicyPropertyManager;
class QtSizePolicyPropertyManagerPrivate
{
    QtSizePolicyPropertyManager* q_ptr;
    Q_DECLARE_PUBLIC(QtSizePolicyPropertyManager)
public:

    QtSizePolicyPropertyManagerPrivate();

    void slotIntChanged(QtProperty* property, int value);
    void slotEnumChanged(QtProperty* property, int value);
    void slotPropertyDestroyed(QtProperty* property);

    typedef QMap<const QtProperty*, QSizePolicy> PropertyValueMap;
    PropertyValueMap m_values;

    QtIntPropertyManager* m_intPropertyManager;
    QtEnumPropertyManager* m_enumPropertyManager;

    QMap<const QtProperty*, QtProperty*> m_propertyToHPolicy;
    QMap<const QtProperty*, QtProperty*> m_propertyToVPolicy;
    QMap<const QtProperty*, QtProperty*> m_propertyToHStretch;
    QMap<const QtProperty*, QtProperty*> m_propertyToVStretch;

    QMap<const QtProperty*, QtProperty*> m_hPolicyToProperty;
    QMap<const QtProperty*, QtProperty*> m_vPolicyToProperty;
    QMap<const QtProperty*, QtProperty*> m_hStretchToProperty;
    QMap<const QtProperty*, QtProperty*> m_vStretchToProperty;
};
class QtSizePolicyPropertyManager : public QtAbstractPropertyManager
{
    Q_OBJECT
public:
    QtSizePolicyPropertyManager(QObject *parent = 0);
    ~QtSizePolicyPropertyManager();

    QtIntPropertyManager *subIntPropertyManager() const;
    QtEnumPropertyManager *subEnumPropertyManager() const;

    QSizePolicy value(const QtProperty *property) const;

public Q_SLOTS:
    void setValue(QtProperty *property, const QSizePolicy &val);
Q_SIGNALS:
    void valueChanged(QtProperty *property, const QSizePolicy &val);
protected:
    QString valueText(const QtProperty *property) const override;
    void initializeProperty(QtProperty *property) override;
    void uninitializeProperty(QtProperty *property) override;
private:
    QScopedPointer<QtSizePolicyPropertyManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtSizePolicyPropertyManager)
    Q_DISABLE_COPY_MOVE(QtSizePolicyPropertyManager)
    Q_PRIVATE_SLOT(d_func(), void slotIntChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotEnumChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotPropertyDestroyed(QtProperty *))
};

class QtFontPropertyManager;
class QtFontPropertyManagerPrivate
{
    QtFontPropertyManager* q_ptr;
    Q_DECLARE_PUBLIC(QtFontPropertyManager)
public:

    QtFontPropertyManagerPrivate();

    void slotIntChanged(QtProperty* property, int value);
    void slotEnumChanged(QtProperty* property, int value);
    void slotBoolChanged(QtProperty* property, bool value);
    void slotPropertyDestroyed(QtProperty* property);
    void slotFontDatabaseChanged();
    void slotFontDatabaseDelayedChange();

    QStringList m_familyNames;

    typedef QMap<const QtProperty*, QFont> PropertyValueMap;
    PropertyValueMap m_values;

    QtIntPropertyManager* m_intPropertyManager;
    QtEnumPropertyManager* m_enumPropertyManager;
    QtBoolPropertyManager* m_boolPropertyManager;

    QMap<const QtProperty*, QtProperty*> m_propertyToFamily;
    QMap<const QtProperty*, QtProperty*> m_propertyToPointSize;
    QMap<const QtProperty*, QtProperty*> m_propertyToBold;
    QMap<const QtProperty*, QtProperty*> m_propertyToItalic;
    QMap<const QtProperty*, QtProperty*> m_propertyToUnderline;
    QMap<const QtProperty*, QtProperty*> m_propertyToStrikeOut;
    QMap<const QtProperty*, QtProperty*> m_propertyToKerning;

    QMap<const QtProperty*, QtProperty*> m_familyToProperty;
    QMap<const QtProperty*, QtProperty*> m_pointSizeToProperty;
    QMap<const QtProperty*, QtProperty*> m_boldToProperty;
    QMap<const QtProperty*, QtProperty*> m_italicToProperty;
    QMap<const QtProperty*, QtProperty*> m_underlineToProperty;
    QMap<const QtProperty*, QtProperty*> m_strikeOutToProperty;
    QMap<const QtProperty*, QtProperty*> m_kerningToProperty;

    bool m_settingValue;
    QTimer* m_fontDatabaseChangeTimer;
};
class QtFontPropertyManager : public QtAbstractPropertyManager
{
    Q_OBJECT
public:
    QtFontPropertyManager(QObject *parent = 0);
    ~QtFontPropertyManager();

    QtIntPropertyManager *subIntPropertyManager() const;
    QtEnumPropertyManager *subEnumPropertyManager() const;
    QtBoolPropertyManager *subBoolPropertyManager() const;

    QFont value(const QtProperty *property) const;

public Q_SLOTS:
    void setValue(QtProperty *property, const QFont &val);
Q_SIGNALS:
    void valueChanged(QtProperty *property, const QFont &val);
protected:
    QString valueText(const QtProperty *property) const override;
    QIcon valueIcon(const QtProperty *property) const override;
    void initializeProperty(QtProperty *property) override;
    void uninitializeProperty(QtProperty *property) override;
private:
    QScopedPointer<QtFontPropertyManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtFontPropertyManager)
    Q_DISABLE_COPY_MOVE(QtFontPropertyManager)
    Q_PRIVATE_SLOT(d_func(), void slotIntChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotEnumChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotBoolChanged(QtProperty *, bool))
    Q_PRIVATE_SLOT(d_func(), void slotPropertyDestroyed(QtProperty *))
    Q_PRIVATE_SLOT(d_func(), void slotFontDatabaseChanged())
    Q_PRIVATE_SLOT(d_func(), void slotFontDatabaseDelayedChange())
};

class QtColorPropertyManager;
class QtColorPropertyManagerPrivate
{
    QtColorPropertyManager* q_ptr;
    Q_DECLARE_PUBLIC(QtColorPropertyManager)
public:

    void slotIntChanged(QtProperty* property, int value);
    void slotPropertyDestroyed(QtProperty* property);

    typedef QMap<const QtProperty*, QColor> PropertyValueMap;
    PropertyValueMap m_values;

    QtIntPropertyManager* m_intPropertyManager;

    QMap<const QtProperty*, QtProperty*> m_propertyToR;
    QMap<const QtProperty*, QtProperty*> m_propertyToG;
    QMap<const QtProperty*, QtProperty*> m_propertyToB;
    QMap<const QtProperty*, QtProperty*> m_propertyToA;

    QMap<const QtProperty*, QtProperty*> m_rToProperty;
    QMap<const QtProperty*, QtProperty*> m_gToProperty;
    QMap<const QtProperty*, QtProperty*> m_bToProperty;
    QMap<const QtProperty*, QtProperty*> m_aToProperty;
};
class QtColorPropertyManager : public QtAbstractPropertyManager
{
    Q_OBJECT
public:
    QtColorPropertyManager(QObject *parent = 0);
    ~QtColorPropertyManager();

    QtIntPropertyManager *subIntPropertyManager() const;

    QColor value(const QtProperty *property) const;

public Q_SLOTS:
    void setValue(QtProperty *property, const QColor &val);
Q_SIGNALS:
    void valueChanged(QtProperty *property, const QColor &val);
protected:
    QString valueText(const QtProperty *property) const override;
    QIcon valueIcon(const QtProperty *property) const override;
    void initializeProperty(QtProperty *property) override;
    void uninitializeProperty(QtProperty *property) override;
private:
    QScopedPointer<QtColorPropertyManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtColorPropertyManager)
    Q_DISABLE_COPY_MOVE(QtColorPropertyManager)
    Q_PRIVATE_SLOT(d_func(), void slotIntChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotPropertyDestroyed(QtProperty *))
};

class QtCursorPropertyManager;
class QtCursorPropertyManagerPrivate
{
    QtCursorPropertyManager* q_ptr;
    Q_DECLARE_PUBLIC(QtCursorPropertyManager)
public:
    typedef QMap<const QtProperty*, QCursor> PropertyValueMap;
    PropertyValueMap m_values;
};
class QtCursorPropertyManager : public QtAbstractPropertyManager
{
    Q_OBJECT
public:
    QtCursorPropertyManager(QObject *parent = 0);
    ~QtCursorPropertyManager();

#ifndef QT_NO_CURSOR
    QCursor value(const QtProperty *property) const;
#endif

public Q_SLOTS:
    void setValue(QtProperty *property, const QCursor &val);
Q_SIGNALS:
    void valueChanged(QtProperty *property, const QCursor &val);
protected:
    QString valueText(const QtProperty *property) const override;
    QIcon valueIcon(const QtProperty *property) const override;
    void initializeProperty(QtProperty *property) override;
    void uninitializeProperty(QtProperty *property) override;
private:
    QScopedPointer<QtCursorPropertyManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtCursorPropertyManager)
    Q_DISABLE_COPY_MOVE(QtCursorPropertyManager)
};

QT_END_NAMESPACE

#endif
