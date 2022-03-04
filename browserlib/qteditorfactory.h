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
#ifndef QTEDITORFACTORY_H
#define QTEDITORFACTORY_H

#include "qtpropertymanager.h"
#include "qtpropertybrowserutils_p.h"
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QAbstractItemView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMenu>
#include <QtGui/QKeyEvent>
#include <QtGui/QRegularExpressionValidator>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QColorDialog>
#include <QtWidgets/QFontDialog>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QKeySequenceEdit>
#include <QtCore/QMap>
#include <QtCore/QRegularExpression>
#include <QtWidgets/QKeySequenceEdit>

QT_BEGIN_NAMESPACE

// QtCharEdit
class QtCharEdit : public QWidget
{
    Q_OBJECT
public:
    QtCharEdit(QWidget* parent = 0);

    QChar value() const;
    bool eventFilter(QObject* o, QEvent* e) override;
public Q_SLOTS:
    void setValue(const QChar& value);
Q_SIGNALS:
    void valueChanged(const QChar& value);
protected:
    void focusInEvent(QFocusEvent* e) override;
    void focusOutEvent(QFocusEvent* e) override;
    void keyPressEvent(QKeyEvent* e) override;
    void keyReleaseEvent(QKeyEvent* e) override;
    bool event(QEvent* e) override;
private slots:
    void slotClearChar();
private:
    void handleKeyEvent(QKeyEvent* e);

    QChar m_value;
    QLineEdit* m_lineEdit;
};

// QtColorEditWidget
class QtColorEditWidget : public QWidget {
    Q_OBJECT

public:
    QtColorEditWidget(QWidget* parent);

    bool eventFilter(QObject* obj, QEvent* ev) override;

public Q_SLOTS:
    void setValue(const QColor& value);

private Q_SLOTS:
    void buttonClicked();

Q_SIGNALS:
    void valueChanged(const QColor& value);

private:
    QColor m_color;
    QLabel* m_pixmapLabel;
    QLabel* m_label;
    QToolButton* m_button;
};

// QtFontEditWidget
class QtFontEditWidget : public QWidget {
    Q_OBJECT

public:
    QtFontEditWidget(QWidget* parent);

    bool eventFilter(QObject* obj, QEvent* ev) override;

public Q_SLOTS:
    void setValue(const QFont& value);

private Q_SLOTS:
    void buttonClicked();

Q_SIGNALS:
    void valueChanged(const QFont& value);

private:
    QFont m_font;
    QLabel* m_pixmapLabel;
    QLabel* m_label;
    QToolButton* m_button;
};

// Set a hard coded left margin to account for the indentation
// of the tree view icon when switching to an editor

static inline void setupTreeViewEditorMargin(QLayout * lt)
{
    enum { DecorationMargin = 4 };
    if (QApplication::layoutDirection() == Qt::LeftToRight)
        lt->setContentsMargins(DecorationMargin, 0, 0, 0);
    else
        lt->setContentsMargins(0, 0, DecorationMargin, 0);
}

// ---------- EditorFactoryPrivate :
// Base class for editor factory private classes. Manages mapping of properties to editors and vice versa.

template <class Editor>
class EditorFactoryPrivate
{
public:

    typedef QList<Editor*> EditorList;
    typedef QMap<QtProperty*, EditorList> PropertyToEditorListMap;
    typedef QMap<Editor*, QtProperty*> EditorToPropertyMap;

    Editor* createEditor(QtProperty* property, QWidget* parent);
    void initializeEditor(QtProperty* property, Editor* e);
    void slotEditorDestroyed(QObject* object);

    PropertyToEditorListMap  m_createdEditors;
    EditorToPropertyMap m_editorToProperty;
};

template <class Editor>
Editor* EditorFactoryPrivate<Editor>::createEditor(QtProperty* property, QWidget* parent)
{
    Editor* editor = new Editor(parent);
    initializeEditor(property, editor);
    return editor;
}

template <class Editor>
void EditorFactoryPrivate<Editor>::initializeEditor(QtProperty* property, Editor* editor)
{
    typename PropertyToEditorListMap::iterator it = m_createdEditors.find(property);
    if (it == m_createdEditors.end())
        it = m_createdEditors.insert(property, EditorList());
    it.value().append(editor);
    m_editorToProperty.insert(editor, property);
}

template <class Editor>
void EditorFactoryPrivate<Editor>::slotEditorDestroyed(QObject* object)
{
    const typename EditorToPropertyMap::iterator ecend = m_editorToProperty.end();
    for (typename EditorToPropertyMap::iterator itEditor = m_editorToProperty.begin(); itEditor != ecend; ++itEditor) {
        if (itEditor.key() == object) {
            Editor* editor = itEditor.key();
            QtProperty* property = itEditor.value();
            const typename PropertyToEditorListMap::iterator pit = m_createdEditors.find(property);
            if (pit != m_createdEditors.end()) {
                pit.value().removeAll(editor);
                if (pit.value().isEmpty())
                    m_createdEditors.erase(pit);
            }
            m_editorToProperty.erase(itEditor);
            return;
        }
    }
}


class QtSpinBoxFactory;
class QtSpinBoxFactoryPrivate : public EditorFactoryPrivate<QSpinBox>
{
    QtSpinBoxFactory* q_ptr;
    Q_DECLARE_PUBLIC(QtSpinBoxFactory)
public:

    void slotPropertyChanged(QtProperty* property, int value);
    void slotRangeChanged(QtProperty* property, int min, int max);
    void slotSingleStepChanged(QtProperty* property, int step);
    void slotSetValue(int value);
};
class QtSpinBoxFactory : public QtAbstractEditorFactory<QtIntPropertyManager>
{
    Q_OBJECT
public:
    QtSpinBoxFactory(QObject *parent = 0);
    ~QtSpinBoxFactory();
protected:
    void connectPropertyManager(QtIntPropertyManager *manager) override;
    QWidget *createEditor(QtIntPropertyManager *manager, QtProperty *property,
                QWidget *parent) override;
    void disconnectPropertyManager(QtIntPropertyManager *manager) override;
private:
    QScopedPointer<QtSpinBoxFactoryPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtSpinBoxFactory)
    Q_DISABLE_COPY_MOVE(QtSpinBoxFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotRangeChanged(QtProperty *, int, int))
    Q_PRIVATE_SLOT(d_func(), void slotSingleStepChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(int))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

class QtSliderFactory;
class QtSliderFactoryPrivate : public EditorFactoryPrivate<QSlider>
{
    QtSliderFactory* q_ptr;
    Q_DECLARE_PUBLIC(QtSliderFactory)
public:
    void slotPropertyChanged(QtProperty* property, int value);
    void slotRangeChanged(QtProperty* property, int min, int max);
    void slotSingleStepChanged(QtProperty* property, int step);
    void slotSetValue(int value);
};
class QtSliderFactory : public QtAbstractEditorFactory<QtIntPropertyManager>
{
    Q_OBJECT
public:
    QtSliderFactory(QObject *parent = 0);
    ~QtSliderFactory();
protected:
    void connectPropertyManager(QtIntPropertyManager *manager) override;
    QWidget *createEditor(QtIntPropertyManager *manager, QtProperty *property,
                QWidget *parent) override;
    void disconnectPropertyManager(QtIntPropertyManager *manager) override;
private:
    QScopedPointer<QtSliderFactoryPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtSliderFactory)
    Q_DISABLE_COPY_MOVE(QtSliderFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotRangeChanged(QtProperty *, int, int))
    Q_PRIVATE_SLOT(d_func(), void slotSingleStepChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(int))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

class QtScrollBarFactory;
class QtScrollBarFactoryPrivate : public  EditorFactoryPrivate<QScrollBar>
{
    QtScrollBarFactory* q_ptr;
    Q_DECLARE_PUBLIC(QtScrollBarFactory)
public:
    void slotPropertyChanged(QtProperty* property, int value);
    void slotRangeChanged(QtProperty* property, int min, int max);
    void slotSingleStepChanged(QtProperty* property, int step);
    void slotSetValue(int value);
};
class QtScrollBarFactory : public QtAbstractEditorFactory<QtIntPropertyManager>
{
    Q_OBJECT
public:
    QtScrollBarFactory(QObject *parent = 0);
    ~QtScrollBarFactory();
protected:
    void connectPropertyManager(QtIntPropertyManager *manager) override;
    QWidget *createEditor(QtIntPropertyManager *manager, QtProperty *property,
                QWidget *parent) override;
    void disconnectPropertyManager(QtIntPropertyManager *manager) override;
private:
    QScopedPointer<QtScrollBarFactoryPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtScrollBarFactory)
    Q_DISABLE_COPY_MOVE(QtScrollBarFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotRangeChanged(QtProperty *, int, int))
    Q_PRIVATE_SLOT(d_func(), void slotSingleStepChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(int))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

class QtCheckBoxFactory;
class QtCheckBoxFactoryPrivate : public EditorFactoryPrivate<QtBoolEdit>
{
    QtCheckBoxFactory* q_ptr;
    Q_DECLARE_PUBLIC(QtCheckBoxFactory)
public:
    void slotPropertyChanged(QtProperty* property, bool value);
    void slotSetValue(bool value);
};
class QtCheckBoxFactory : public QtAbstractEditorFactory<QtBoolPropertyManager>
{
    Q_OBJECT
public:
    QtCheckBoxFactory(QObject *parent = 0);
    ~QtCheckBoxFactory();
protected:
    void connectPropertyManager(QtBoolPropertyManager *manager) override;
    QWidget *createEditor(QtBoolPropertyManager *manager, QtProperty *property,
                QWidget *parent) override;
    void disconnectPropertyManager(QtBoolPropertyManager *manager) override;
private:
    QScopedPointer<QtCheckBoxFactoryPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtCheckBoxFactory)
    Q_DISABLE_COPY_MOVE(QtCheckBoxFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, bool))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(bool))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

class QtDoubleSpinBoxFactory;
class QtDoubleSpinBoxFactoryPrivate : public EditorFactoryPrivate<QDoubleSpinBox>
{
    QtDoubleSpinBoxFactory* q_ptr;
    Q_DECLARE_PUBLIC(QtDoubleSpinBoxFactory)
public:

    void slotPropertyChanged(QtProperty* property, double value);
    void slotRangeChanged(QtProperty* property, double min, double max);
    void slotSingleStepChanged(QtProperty* property, double step);
    void slotDecimalsChanged(QtProperty* property, int prec);
    void slotSetValue(double value);
};
class QtDoubleSpinBoxFactory : public QtAbstractEditorFactory<QtDoublePropertyManager>
{
    Q_OBJECT
public:
    QtDoubleSpinBoxFactory(QObject *parent = 0);
    ~QtDoubleSpinBoxFactory();
protected:
    void connectPropertyManager(QtDoublePropertyManager *manager) override;
    QWidget *createEditor(QtDoublePropertyManager *manager, QtProperty *property,
                QWidget *parent) override;
    void disconnectPropertyManager(QtDoublePropertyManager *manager) override;
private:
    QScopedPointer<QtDoubleSpinBoxFactoryPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtDoubleSpinBoxFactory)
    Q_DISABLE_COPY_MOVE(QtDoubleSpinBoxFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, double))
    Q_PRIVATE_SLOT(d_func(), void slotRangeChanged(QtProperty *, double, double))
    Q_PRIVATE_SLOT(d_func(), void slotSingleStepChanged(QtProperty *, double))
    Q_PRIVATE_SLOT(d_func(), void slotDecimalsChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(double))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

class QtLineEditFactory;
class QtLineEditFactoryPrivate : public EditorFactoryPrivate<QLineEdit>
{
    QtLineEditFactory* q_ptr;
    Q_DECLARE_PUBLIC(QtLineEditFactory)
public:

    void slotPropertyChanged(QtProperty* property, const QString& value);
    void slotRegExpChanged(QtProperty* property, const QRegularExpression& regExp);
    void slotSetValue(const QString& value);
};
class QtLineEditFactory : public QtAbstractEditorFactory<QtStringPropertyManager>
{
    Q_OBJECT
public:
    QtLineEditFactory(QObject *parent = 0);
    ~QtLineEditFactory();
protected:
    void connectPropertyManager(QtStringPropertyManager *manager) override;
    QWidget *createEditor(QtStringPropertyManager *manager, QtProperty *property,
                QWidget *parent) override;
    void disconnectPropertyManager(QtStringPropertyManager *manager) override;
private:
    QScopedPointer<QtLineEditFactoryPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtLineEditFactory)
    Q_DISABLE_COPY_MOVE(QtLineEditFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, const QString &))
    Q_PRIVATE_SLOT(d_func(), void slotRegExpChanged(QtProperty *, const QRegularExpression &))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(const QString &))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

class QtDateEditFactory;
class QtDateEditFactoryPrivate : public EditorFactoryPrivate<QDateEdit>
{
    QtDateEditFactory* q_ptr;
    Q_DECLARE_PUBLIC(QtDateEditFactory)
public:

    void slotPropertyChanged(QtProperty* property, QDate value);
    void slotRangeChanged(QtProperty* property, QDate min, QDate max);
    void slotSetValue(QDate value);
};
class QtDateEditFactory : public QtAbstractEditorFactory<QtDatePropertyManager>
{
    Q_OBJECT
public:
    QtDateEditFactory(QObject *parent = 0);
    ~QtDateEditFactory();
protected:
    void connectPropertyManager(QtDatePropertyManager *manager) override;
    QWidget *createEditor(QtDatePropertyManager *manager, QtProperty *property,
                QWidget *parent) override;
    void disconnectPropertyManager(QtDatePropertyManager *manager) override;
private:
    QScopedPointer<QtDateEditFactoryPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtDateEditFactory)
    Q_DISABLE_COPY_MOVE(QtDateEditFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, QDate))
    Q_PRIVATE_SLOT(d_func(), void slotRangeChanged(QtProperty *, QDate, QDate))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(QDate))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

class QtTimeEditFactory;
class QtTimeEditFactoryPrivate : public EditorFactoryPrivate<QTimeEdit>
{
    QtTimeEditFactory* q_ptr;
    Q_DECLARE_PUBLIC(QtTimeEditFactory)
public:

    void slotPropertyChanged(QtProperty* property, QTime value);
    void slotSetValue(QTime value);
};
class QtTimeEditFactory : public QtAbstractEditorFactory<QtTimePropertyManager>
{
    Q_OBJECT
public:
    QtTimeEditFactory(QObject *parent = 0);
    ~QtTimeEditFactory();
protected:
    void connectPropertyManager(QtTimePropertyManager *manager) override;
    QWidget *createEditor(QtTimePropertyManager *manager, QtProperty *property,
                QWidget *parent) override;
    void disconnectPropertyManager(QtTimePropertyManager *manager) override;
private:
    QScopedPointer<QtTimeEditFactoryPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtTimeEditFactory)
    Q_DISABLE_COPY_MOVE(QtTimeEditFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, QTime))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(QTime))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

class QtDateTimeEditFactory;
class QtDateTimeEditFactoryPrivate : public EditorFactoryPrivate<QDateTimeEdit>
{
    QtDateTimeEditFactory* q_ptr;
    Q_DECLARE_PUBLIC(QtDateTimeEditFactory)
public:

    void slotPropertyChanged(QtProperty* property, const QDateTime& value);
    void slotSetValue(const QDateTime& value);

};
class QtDateTimeEditFactory : public QtAbstractEditorFactory<QtDateTimePropertyManager>
{
    Q_OBJECT
public:
    QtDateTimeEditFactory(QObject *parent = 0);
    ~QtDateTimeEditFactory();
protected:
    void connectPropertyManager(QtDateTimePropertyManager *manager) override;
    QWidget *createEditor(QtDateTimePropertyManager *manager, QtProperty *property,
                QWidget *parent) override;
    void disconnectPropertyManager(QtDateTimePropertyManager *manager) override;
private:
    QScopedPointer<QtDateTimeEditFactoryPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtDateTimeEditFactory)
    Q_DISABLE_COPY_MOVE(QtDateTimeEditFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, const QDateTime &))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(const QDateTime &))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

class QtKeySequenceEditorFactory;
class QtKeySequenceEditorFactoryPrivate : public EditorFactoryPrivate<QKeySequenceEdit>
{
    QtKeySequenceEditorFactory* q_ptr;
    Q_DECLARE_PUBLIC(QtKeySequenceEditorFactory)
public:

    void slotPropertyChanged(QtProperty* property, const QKeySequence& value);
    void slotSetValue(const QKeySequence& value);
};
class QtKeySequenceEditorFactory : public QtAbstractEditorFactory<QtKeySequencePropertyManager>
{
    Q_OBJECT
public:
    QtKeySequenceEditorFactory(QObject *parent = 0);
    ~QtKeySequenceEditorFactory();
protected:
    void connectPropertyManager(QtKeySequencePropertyManager *manager) override;
    QWidget *createEditor(QtKeySequencePropertyManager *manager, QtProperty *property,
                QWidget *parent) override;
    void disconnectPropertyManager(QtKeySequencePropertyManager *manager) override;
private:
    QScopedPointer<QtKeySequenceEditorFactoryPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtKeySequenceEditorFactory)
    Q_DISABLE_COPY_MOVE(QtKeySequenceEditorFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, const QKeySequence &))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(const QKeySequence &))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

class QtCharEditorFactory;
class QtCharEditorFactoryPrivate : public EditorFactoryPrivate<QtCharEdit>
{
    QtCharEditorFactory* q_ptr;
    Q_DECLARE_PUBLIC(QtCharEditorFactory)
public:

    void slotPropertyChanged(QtProperty* property, const QChar& value);
    void slotSetValue(const QChar& value);

};
class QtCharEditorFactory : public QtAbstractEditorFactory<QtCharPropertyManager>
{
    Q_OBJECT
public:
    QtCharEditorFactory(QObject *parent = 0);
    ~QtCharEditorFactory();
protected:
    void connectPropertyManager(QtCharPropertyManager *manager) override;
    QWidget *createEditor(QtCharPropertyManager *manager, QtProperty *property,
                QWidget *parent) override;
    void disconnectPropertyManager(QtCharPropertyManager *manager) override;
private:
    QScopedPointer<QtCharEditorFactoryPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtCharEditorFactory)
    Q_DISABLE_COPY_MOVE(QtCharEditorFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, const QChar &))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(const QChar &))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

class QtEnumEditorFactory;
class QtEnumEditorFactoryPrivate : public EditorFactoryPrivate<QComboBox>
{
    QtEnumEditorFactory* q_ptr;
    Q_DECLARE_PUBLIC(QtEnumEditorFactory)
public:

    void slotPropertyChanged(QtProperty* property, int value);
    void slotEnumNamesChanged(QtProperty* property, const QStringList&);
    void slotEnumIconsChanged(QtProperty* property, const QMap<int, QIcon>&);
    void slotSetValue(int value);
};
class QtEnumEditorFactory : public QtAbstractEditorFactory<QtEnumPropertyManager>
{
    Q_OBJECT
public:
    QtEnumEditorFactory(QObject *parent = 0);
    ~QtEnumEditorFactory();
protected:
    void connectPropertyManager(QtEnumPropertyManager *manager) override;
    QWidget *createEditor(QtEnumPropertyManager *manager, QtProperty *property,
                QWidget *parent) override;
    void disconnectPropertyManager(QtEnumPropertyManager *manager) override;
private:
    QScopedPointer<QtEnumEditorFactoryPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtEnumEditorFactory)
    Q_DISABLE_COPY_MOVE(QtEnumEditorFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotEnumNamesChanged(QtProperty *,
                        const QStringList &))
    Q_PRIVATE_SLOT(d_func(), void slotEnumIconsChanged(QtProperty *,
                        const QMap<int, QIcon> &))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(int))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

class QtCursorEditorFactory;
class QtCursorEditorFactoryPrivate
{
    QtCursorEditorFactory* q_ptr;
    Q_DECLARE_PUBLIC(QtCursorEditorFactory)
public:
    QtCursorEditorFactoryPrivate();

    void slotPropertyChanged(QtProperty* property, const QCursor& cursor);
    void slotEnumChanged(QtProperty* property, int value);
    void slotEditorDestroyed(QObject* object);

    QtEnumEditorFactory* m_enumEditorFactory;
    QtEnumPropertyManager* m_enumPropertyManager;

    QMap<QtProperty*, QtProperty*> m_propertyToEnum;
    QMap<QtProperty*, QtProperty*> m_enumToProperty;
    QMap<QtProperty*, QWidgetList > m_enumToEditors;
    QMap<QWidget*, QtProperty*> m_editorToEnum;
    bool m_updatingEnum;
};
class QtCursorEditorFactory : public QtAbstractEditorFactory<QtCursorPropertyManager>
{
    Q_OBJECT
public:
    QtCursorEditorFactory(QObject *parent = 0);
    ~QtCursorEditorFactory();
protected:
    void connectPropertyManager(QtCursorPropertyManager *manager) override;
    QWidget *createEditor(QtCursorPropertyManager *manager, QtProperty *property,
                QWidget *parent) override;
    void disconnectPropertyManager(QtCursorPropertyManager *manager) override;
private:
    QScopedPointer<QtCursorEditorFactoryPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtCursorEditorFactory)
    Q_DISABLE_COPY_MOVE(QtCursorEditorFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, const QCursor &))
    Q_PRIVATE_SLOT(d_func(), void slotEnumChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

class QtColorEditorFactory;
class QtColorEditorFactoryPrivate : public EditorFactoryPrivate<QtColorEditWidget>
{
    QtColorEditorFactory* q_ptr;
    Q_DECLARE_PUBLIC(QtColorEditorFactory)
public:

    void slotPropertyChanged(QtProperty* property, const QColor& value);
    void slotSetValue(const QColor& value);
};
class QtColorEditorFactory : public QtAbstractEditorFactory<QtColorPropertyManager>
{
    Q_OBJECT
public:
    QtColorEditorFactory(QObject *parent = 0);
    ~QtColorEditorFactory();
protected:
    void connectPropertyManager(QtColorPropertyManager *manager) override;
    QWidget *createEditor(QtColorPropertyManager *manager, QtProperty *property,
                QWidget *parent) override;
    void disconnectPropertyManager(QtColorPropertyManager *manager) override;
private:
    QScopedPointer<QtColorEditorFactoryPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtColorEditorFactory)
    Q_DISABLE_COPY_MOVE(QtColorEditorFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, const QColor &))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(const QColor &))
};

class QtFontEditorFactory;
class QtFontEditorFactoryPrivate : public EditorFactoryPrivate<QtFontEditWidget>
{
    QtFontEditorFactory* q_ptr;
    Q_DECLARE_PUBLIC(QtFontEditorFactory)
public:

    void slotPropertyChanged(QtProperty* property, const QFont& value);
    void slotSetValue(const QFont& value);
};
class QtFontEditorFactory : public QtAbstractEditorFactory<QtFontPropertyManager>
{
    Q_OBJECT
public:
    QtFontEditorFactory(QObject *parent = 0);
    ~QtFontEditorFactory();
protected:
    void connectPropertyManager(QtFontPropertyManager *manager) override;
    QWidget *createEditor(QtFontPropertyManager *manager, QtProperty *property,
                QWidget *parent) override;
    void disconnectPropertyManager(QtFontPropertyManager *manager) override;
private:
    QScopedPointer<QtFontEditorFactoryPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtFontEditorFactory)
    Q_DISABLE_COPY_MOVE(QtFontEditorFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, const QFont &))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(const QFont &))
};

QT_END_NAMESPACE

#endif
