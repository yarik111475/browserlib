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

#ifndef QTTREEPROPERTYBROWSER_H
#define QTTREEPROPERTYBROWSER_H

#include "qtpropertybrowser.h"
#include <QtCore/QSet>
#include <QtGui/QIcon>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QItemDelegate>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtGui/QPainter>
#include <QtWidgets/QApplication>
#include <QtGui/QFocusEvent>
#include <QtWidgets/QStyle>
#include <QtGui/QPalette>

QT_BEGIN_NAMESPACE

class QTreeWidgetItem;
class QtTreePropertyBrowser;
class QtTreePropertyBrowserPrivate;

// ------------ QtPropertyEditorView
class QtPropertyEditorView : public QTreeWidget
{
    Q_OBJECT
public:
    QtPropertyEditorView(QWidget* parent = 0);

    void setEditorPrivate(QtTreePropertyBrowserPrivate* editorPrivate)
    {
        m_editorPrivate = editorPrivate;
    }

    QTreeWidgetItem* indexToItem(const QModelIndex& index) const
    {
        return itemFromIndex(index);
    }

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void drawRow(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    QtTreePropertyBrowserPrivate* m_editorPrivate;
};

// ------------ QtPropertyEditorDelegate
class QtPropertyEditorDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    QtPropertyEditorDelegate(QObject* parent = 0)
        : QItemDelegate(parent), m_editorPrivate(0), m_editedItem(0), m_editedWidget(0)
    {}

    void setEditorPrivate(QtTreePropertyBrowserPrivate* editorPrivate)
    {
        m_editorPrivate = editorPrivate;
    }

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option,
        const QModelIndex& index) const override;

    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option,
        const QModelIndex& index) const override;

    void paint(QPainter* painter, const QStyleOptionViewItem& option,
        const QModelIndex& index) const override;

    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

    void setModelData(QWidget*, QAbstractItemModel*,
        const QModelIndex&) const  override {}

    void setEditorData(QWidget*, const QModelIndex&) const override {}

    bool eventFilter(QObject* object, QEvent* event) override;
    void closeEditor(QtProperty* property);

    QTreeWidgetItem* editedItem() const { return m_editedItem; }

private slots:
    void slotEditorDestroyed(QObject* object);

private:
    int indentation(const QModelIndex& index) const;

    typedef QMap<QWidget*, QtProperty*> EditorToPropertyMap;
    mutable EditorToPropertyMap m_editorToProperty;

    typedef QMap<QtProperty*, QWidget*> PropertyToEditorMap;
    mutable PropertyToEditorMap m_propertyToEditor;
    QtTreePropertyBrowserPrivate* m_editorPrivate;
    mutable QTreeWidgetItem* m_editedItem;
    mutable QWidget* m_editedWidget;
};


enum class ResizeMode
{
    Interactive,
    Stretch,
    Fixed,
    ResizeToContents
};

class QtTreePropertyBrowserPrivate
{
    QtTreePropertyBrowser* q_ptr;
    Q_DECLARE_PUBLIC(QtTreePropertyBrowser)

public:
    QtTreePropertyBrowserPrivate();
    void init(QWidget* parent);

    void propertyInserted(QtBrowserItem* index, QtBrowserItem* afterIndex);
    void propertyRemoved(QtBrowserItem* index);
    void propertyChanged(QtBrowserItem* index);
    QWidget* createEditor(QtProperty* property, QWidget* parent) const;
 
    QtProperty* indexToProperty(const QModelIndex& index) const;
    QTreeWidgetItem* indexToItem(const QModelIndex& index) const;
    QtBrowserItem* indexToBrowserItem(const QModelIndex& index) const;
    bool lastColumn(int column) const;
    void disableItem(QTreeWidgetItem* item) const;
    void enableItem(QTreeWidgetItem* item) const;
    bool hasValue(QTreeWidgetItem* item) const;

    void slotCollapsed(const QModelIndex& index);
    void slotExpanded(const QModelIndex& index);

    QColor calculatedBackgroundColor(QtBrowserItem* item) const;

    QtPropertyEditorView* treeWidget() const;
    bool markPropertiesWithoutValue() const;

    QtBrowserItem* currentItem() const;
    void setCurrentItem(QtBrowserItem* browserItem, bool block);
    void editItem(QtBrowserItem* browserItem);

    void slotCurrentBrowserItemChanged(QtBrowserItem* item);
    void slotCurrentTreeItemChanged(QTreeWidgetItem* newItem, QTreeWidgetItem*);

    QTreeWidgetItem* editedItem() const;

private:
    void updateItem(QTreeWidgetItem* item);

    QMap<QtBrowserItem*, QTreeWidgetItem*> m_indexToItem;
    QMap<QTreeWidgetItem*, QtBrowserItem*> m_itemToIndex;

    QMap<QtBrowserItem*, QColor> m_indexToBackgroundColor;

    QtPropertyEditorView* m_treeWidget;

    bool m_headerVisible;
    ResizeMode m_resizeMode;
    class QtPropertyEditorDelegate* m_delegate;
    bool m_markPropertiesWithoutValue;
    bool m_browserChangedBlocked;
    QIcon m_expandIcon;
};
class QtTreePropertyBrowser : public QtAbstractPropertyBrowser
{
    Q_OBJECT
    Q_PROPERTY(int indentation READ indentation WRITE setIndentation)
    Q_PROPERTY(bool rootIsDecorated READ rootIsDecorated WRITE setRootIsDecorated)
    Q_PROPERTY(bool alternatingRowColors READ alternatingRowColors WRITE setAlternatingRowColors)
    Q_PROPERTY(bool headerVisible READ isHeaderVisible WRITE setHeaderVisible)
    Q_PROPERTY(ResizeMode resizeMode READ resizeMode WRITE setResizeMode)
    Q_PROPERTY(int splitterPosition READ splitterPosition WRITE setSplitterPosition)
    Q_PROPERTY(bool propertiesWithoutValueMarked READ propertiesWithoutValueMarked WRITE setPropertiesWithoutValueMarked)

public:
    Q_ENUM(ResizeMode)

    QtTreePropertyBrowser(QWidget *parent = 0);
    ~QtTreePropertyBrowser();

    int indentation() const;
    void setIndentation(int i);

    bool rootIsDecorated() const;
    void setRootIsDecorated(bool show);

    bool alternatingRowColors() const;
    void setAlternatingRowColors(bool enable);

    bool isHeaderVisible() const;
    void setHeaderVisible(bool visible);

    ResizeMode resizeMode() const;
    void setResizeMode(ResizeMode mode);

    int splitterPosition() const;
    void setSplitterPosition(int position);

    void setExpanded(QtBrowserItem *item, bool expanded);
    bool isExpanded(QtBrowserItem *item) const;

    bool isItemVisible(QtBrowserItem *item) const;
    void setItemVisible(QtBrowserItem *item, bool visible);

    void setBackgroundColor(QtBrowserItem *item, const QColor &color);
    QColor backgroundColor(QtBrowserItem *item) const;
    QColor calculatedBackgroundColor(QtBrowserItem *item) const;

    void setPropertiesWithoutValueMarked(bool mark);
    bool propertiesWithoutValueMarked() const;

    void editItem(QtBrowserItem *item);

Q_SIGNALS:

    void collapsed(QtBrowserItem *item);
    void expanded(QtBrowserItem *item);

protected:
    void itemInserted(QtBrowserItem *item, QtBrowserItem *afterItem) override;
    void itemRemoved(QtBrowserItem *item) override;
    void itemChanged(QtBrowserItem *item) override;

private:

    QScopedPointer<QtTreePropertyBrowserPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtTreePropertyBrowser)
    Q_DISABLE_COPY_MOVE(QtTreePropertyBrowser)

    Q_PRIVATE_SLOT(d_func(), void slotCollapsed(const QModelIndex &))
    Q_PRIVATE_SLOT(d_func(), void slotExpanded(const QModelIndex &))
    Q_PRIVATE_SLOT(d_func(), void slotCurrentBrowserItemChanged(QtBrowserItem *))
    Q_PRIVATE_SLOT(d_func(), void slotCurrentTreeItemChanged(QTreeWidgetItem *, QTreeWidgetItem *))

};

QT_END_NAMESPACE

#endif
