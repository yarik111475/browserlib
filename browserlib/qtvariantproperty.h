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

#ifndef QTVARIANTPROPERTY_H
#define QTVARIANTPROPERTY_H
#include "qpoint3d.h"
#include "qpoint3df.h"
#include "qrotation3D.h"
#include "qtpropertymanager.h"
#include "qteditorfactory.h"
#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtCore/QDate>
#include <QtCore/QLocale>
#include <QtCore/QRegularExpression>

QT_BEGIN_NAMESPACE

class QRegularExpression;

typedef QMap<int, QIcon> QtIconMap;

class QtVariantProperty;
class QtVariantEditorFactory;
class QtVariantPropertyManager;

class QtVariantPropertyPrivate
{
public:
    QtVariantPropertyPrivate(QtVariantPropertyManager* m) : manager(m) {}

    QtVariantPropertyManager* manager;
};
class QtVariantProperty : public QtProperty
{
public:
    ~QtVariantProperty();
    QVariant value() const;
    QVariant attributeValue(const QString &attribute) const;
    int valueType() const;
    int propertyType() const;

    void setValue(const QVariant &value);
    void setAttribute(const QString &attribute, const QVariant &value);
protected:
    QtVariantProperty(QtVariantPropertyManager *manager);
private:
    friend class QtVariantPropertyManager;
    QScopedPointer<class QtVariantPropertyPrivate> d_ptr;
};

class QtVariantPropertyManagerPrivate
{
    QtVariantPropertyManager* q_ptr;
    Q_DECLARE_PUBLIC(QtVariantPropertyManager)
public:
    QtVariantPropertyManagerPrivate();

    bool m_creatingProperty;
    bool m_creatingSubProperties;
    bool m_destroyingSubProperties;
    int m_propertyType;

    void slotValueChanged(QtProperty* property, int val);
    void slotRangeChanged(QtProperty* property, int min, int max);
    void slotSingleStepChanged(QtProperty* property, int step);
    void slotValueChanged(QtProperty* property, double val);
    void slotRangeChanged(QtProperty* property, double min, double max);
    void slotSingleStepChanged(QtProperty* property, double step);
    void slotDecimalsChanged(QtProperty* property, int prec);
    void slotValueChanged(QtProperty* property, bool val);
    void slotValueChanged(QtProperty* property, const QString& val);
    void slotRegExpChanged(QtProperty* property, const QRegularExpression& regExp);
    void slotValueChanged(QtProperty* property, QDate val);
    void slotRangeChanged(QtProperty* property, QDate min, QDate max);
    void slotValueChanged(QtProperty* property, QTime val);
    void slotValueChanged(QtProperty* property, const QDateTime& val);
    void slotValueChanged(QtProperty* property, const QKeySequence& val);
    void slotValueChanged(QtProperty* property, const QChar& val);
    void slotValueChanged(QtProperty* property, const QLocale& val);
    //##########
    void slotValueChanged(QtProperty* property, const QPoint3D& val);
    //##########
    void slotValueChanged(QtProperty* property, const QRotation3D& val);
    //##########
    void slotValueChanged(QtProperty* property, const QPoint3DF& val);
    //##########
    void slotValueChanged(QtProperty* property, const QPoint& val);
    void slotValueChanged(QtProperty* property, const QPointF& val);
    void slotValueChanged(QtProperty* property, const QSize& val);
    void slotRangeChanged(QtProperty* property, const QSize& min, const QSize& max);
    void slotValueChanged(QtProperty* property, const QSizeF& val);
    void slotRangeChanged(QtProperty* property, const QSizeF& min, const QSizeF& max);
    void slotValueChanged(QtProperty* property, const QRect& val);
    void slotConstraintChanged(QtProperty* property, const QRect& val);
    void slotValueChanged(QtProperty* property, const QRectF& val);
    void slotConstraintChanged(QtProperty* property, const QRectF& val);
    void slotValueChanged(QtProperty* property, const QColor& val);
    void slotEnumChanged(QtProperty* property, int val);
    void slotEnumNamesChanged(QtProperty* property, const QStringList& enumNames);
    void slotEnumIconsChanged(QtProperty* property, const QMap<int, QIcon>& enumIcons);
    void slotValueChanged(QtProperty* property, const QSizePolicy& val);
    void slotValueChanged(QtProperty* property, const QFont& val);
    void slotValueChanged(QtProperty* property, const QCursor& val);
    void slotFlagChanged(QtProperty* property, int val);
    void slotFlagNamesChanged(QtProperty* property, const QStringList& flagNames);
    void slotPropertyInserted(QtProperty* property, QtProperty* parent, QtProperty* after);
    void slotPropertyRemoved(QtProperty* property, QtProperty* parent);

    void valueChanged(QtProperty* property, const QVariant& val);

    int internalPropertyToType(QtProperty* property) const;
    QtVariantProperty* createSubProperty(QtVariantProperty* parent, QtVariantProperty* after,
        QtProperty* internal);
    void removeSubProperty(QtVariantProperty* property);

    QMap<int, QtAbstractPropertyManager*> m_typeToPropertyManager;
    QMap<int, QMap<QString, int> > m_typeToAttributeToAttributeType;

    QMap<const QtProperty*, QPair<QtVariantProperty*, int> > m_propertyToType;

    QMap<int, int> m_typeToValueType;


    QMap<QtProperty*, QtVariantProperty*> m_internalToProperty;

    const QString m_constraintAttribute;
    const QString m_singleStepAttribute;
    const QString m_decimalsAttribute;
    const QString m_enumIconsAttribute;
    const QString m_enumNamesAttribute;
    const QString m_flagNamesAttribute;
    const QString m_maximumAttribute;
    const QString m_minimumAttribute;
    const QString m_regExpAttribute;
};
class QtVariantPropertyManager : public QtAbstractPropertyManager
{
    Q_OBJECT
public:
    QtVariantPropertyManager(QObject *parent = 0);
    ~QtVariantPropertyManager();

    virtual QtVariantProperty *addProperty(int propertyType, const QString &name = QString());

    int propertyType(const QtProperty *property) const;
    int valueType(const QtProperty *property) const;
    QtVariantProperty *variantProperty(const QtProperty *property) const;

    virtual bool isPropertyTypeSupported(int propertyType) const;
    virtual int valueType(int propertyType) const;
    virtual QStringList attributes(int propertyType) const;
    virtual int attributeType(int propertyType, const QString &attribute) const;

    virtual QVariant value(const QtProperty *property) const;
    virtual QVariant attributeValue(const QtProperty *property, const QString &attribute) const;

    static int enumTypeId();
    static int flagTypeId();
    static int groupTypeId();
    static int iconMapTypeId();
public Q_SLOTS:
    virtual void setValue(QtProperty *property, const QVariant &val);
    virtual void setAttribute(QtProperty *property,
                const QString &attribute, const QVariant &value);
Q_SIGNALS:
    void valueChanged(QtProperty *property, const QVariant &val);
    void attributeChanged(QtProperty *property,
                const QString &attribute, const QVariant &val);
protected:
    bool hasValue(const QtProperty *property) const override;
    QString valueText(const QtProperty *property) const override;
    QIcon valueIcon(const QtProperty *property) const override;
    void initializeProperty(QtProperty *property) override;
    void uninitializeProperty(QtProperty *property) override;
    QtProperty *createProperty() override;
private:
    QScopedPointer<class QtVariantPropertyManagerPrivate> d_ptr;
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotRangeChanged(QtProperty *, int, int))
    Q_PRIVATE_SLOT(d_func(), void slotSingleStepChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, double))
    Q_PRIVATE_SLOT(d_func(), void slotRangeChanged(QtProperty *, double, double))
    Q_PRIVATE_SLOT(d_func(), void slotSingleStepChanged(QtProperty *, double))
    Q_PRIVATE_SLOT(d_func(), void slotDecimalsChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, bool))
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, const QString &))
    Q_PRIVATE_SLOT(d_func(), void slotRegExpChanged(QtProperty *, const QRegularExpression &))
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, QDate))
    Q_PRIVATE_SLOT(d_func(), void slotRangeChanged(QtProperty *, QDate, QDate))
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, QTime))
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, const QDateTime &))
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, const QKeySequence &))
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, const QChar &))
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, const QLocale &))
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, const QPoint &))
    //##########
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, const QPoint3D &))
    //##########
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, const QRotation3D &))
    //##########
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, const QPoint3DF &))
    //#########
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, const QPointF &))
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, const QSize &))
    Q_PRIVATE_SLOT(d_func(), void slotRangeChanged(QtProperty *, const QSize &, const QSize &))
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, const QSizeF &))
    Q_PRIVATE_SLOT(d_func(), void slotRangeChanged(QtProperty *, const QSizeF &, const QSizeF &))
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, const QRect &))
    Q_PRIVATE_SLOT(d_func(), void slotConstraintChanged(QtProperty *, const QRect &))
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, const QRectF &))
    Q_PRIVATE_SLOT(d_func(), void slotConstraintChanged(QtProperty *, const QRectF &))
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, const QColor &))
    Q_PRIVATE_SLOT(d_func(), void slotEnumNamesChanged(QtProperty *, const QStringList &))
    Q_PRIVATE_SLOT(d_func(), void slotEnumIconsChanged(QtProperty *, const QMap<int, QIcon> &))
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, const QSizePolicy &))
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, const QFont &))
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, const QCursor &))
    Q_PRIVATE_SLOT(d_func(), void slotFlagNamesChanged(QtProperty *, const QStringList &))

    Q_PRIVATE_SLOT(d_func(), void slotPropertyInserted(QtProperty *, QtProperty *, QtProperty *))
    Q_PRIVATE_SLOT(d_func(), void slotPropertyRemoved(QtProperty *, QtProperty *))
    Q_DECLARE_PRIVATE(QtVariantPropertyManager)
    Q_DISABLE_COPY_MOVE(QtVariantPropertyManager)
};

class QtVariantEditorFactoryPrivate
{
    QtVariantEditorFactory* q_ptr;
    Q_DECLARE_PUBLIC(QtVariantEditorFactory)
public:

    QtSpinBoxFactory* m_spinBoxFactory;
    QtDoubleSpinBoxFactory* m_doubleSpinBoxFactory;
    QtCheckBoxFactory* m_checkBoxFactory;
    QtLineEditFactory* m_lineEditFactory;
    QtDateEditFactory* m_dateEditFactory;
    QtTimeEditFactory* m_timeEditFactory;
    QtDateTimeEditFactory* m_dateTimeEditFactory;
    QtKeySequenceEditorFactory* m_keySequenceEditorFactory;
    QtCharEditorFactory* m_charEditorFactory;
    QtEnumEditorFactory* m_comboBoxFactory;
    QtCursorEditorFactory* m_cursorEditorFactory;
    QtColorEditorFactory* m_colorEditorFactory;
    QtFontEditorFactory* m_fontEditorFactory;

    QMap<QtAbstractEditorFactoryBase*, int> m_factoryToType;
    QMap<int, QtAbstractEditorFactoryBase*> m_typeToFactory;
};
class QtVariantEditorFactory : public QtAbstractEditorFactory<QtVariantPropertyManager>
{
    Q_OBJECT
public:
    QtVariantEditorFactory(QObject *parent = 0);
    ~QtVariantEditorFactory();
protected:
    void connectPropertyManager(QtVariantPropertyManager *manager) override;
    QWidget *createEditor(QtVariantPropertyManager *manager, QtProperty *property,
                QWidget *parent) override;
    void disconnectPropertyManager(QtVariantPropertyManager *manager) override;
private:
    QScopedPointer<class QtVariantEditorFactoryPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtVariantEditorFactory)
    Q_DISABLE_COPY_MOVE(QtVariantEditorFactory)
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QIcon)
Q_DECLARE_METATYPE(QtIconMap)
#endif
