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

#ifndef QTBUTTONPROPERTYBROWSER_H
#define QTBUTTONPROPERTYBROWSER_H

#include "qtpropertybrowser.h"
#include <QtCore/QSet>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtCore/QTimer>
#include <QtCore/QMap>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QStyle>

QT_BEGIN_NAMESPACE

class QtButtonPropertyBrowser;

class QtButtonPropertyBrowserPrivate
{
    QtButtonPropertyBrowser* q_ptr;
    Q_DECLARE_PUBLIC(QtButtonPropertyBrowser)
public:

    void init(QWidget* parent);

    void propertyInserted(QtBrowserItem* index, QtBrowserItem* afterIndex);
    void propertyRemoved(QtBrowserItem* index);
    void propertyChanged(QtBrowserItem* index);
    QWidget* createEditor(QtProperty* property, QWidget* parent) const;

    void slotEditorDestroyed();
    void slotUpdate();
    void slotToggled(bool checked);

    struct WidgetItem
    {
        QWidget* widget{ nullptr }; // can be null
        QLabel* label{ nullptr }; // main label with property name
        QLabel* widgetLabel{ nullptr }; // label substitute showing the current value if there is no widget
        QToolButton* button{ nullptr }; // expandable button for items with children
        QWidget* container{ nullptr }; // container which is expanded when the button is clicked
        QGridLayout* layout{ nullptr }; // layout in container
        WidgetItem* parent{ nullptr };
        QList<WidgetItem*> children;
        bool expanded{ false };
    };
private:
    void updateLater();
    void updateItem(WidgetItem* item);
    void insertRow(QGridLayout* layout, int row) const;
    void removeRow(QGridLayout* layout, int row) const;
    int gridRow(WidgetItem* item) const;
    int gridSpan(WidgetItem* item) const;
    void setExpanded(WidgetItem* item, bool expanded);
    QToolButton* createButton(QWidget* panret = 0) const;

    QMap<QtBrowserItem*, WidgetItem*> m_indexToItem;
    QMap<WidgetItem*, QtBrowserItem*> m_itemToIndex;
    QMap<QWidget*, WidgetItem*> m_widgetToItem;
    QMap<QObject*, WidgetItem*> m_buttonToItem;
    QGridLayout* m_mainLayout;
    QList<WidgetItem*> m_children;
    QList<WidgetItem*> m_recreateQueue;
};
class QtButtonPropertyBrowser : public QtAbstractPropertyBrowser
{
    Q_OBJECT
public:

    QtButtonPropertyBrowser(QWidget *parent = 0);
    ~QtButtonPropertyBrowser();

    void setExpanded(QtBrowserItem *item, bool expanded);
    bool isExpanded(QtBrowserItem *item) const;

Q_SIGNALS:

    void collapsed(QtBrowserItem *item);
    void expanded(QtBrowserItem *item);

protected:
    void itemInserted(QtBrowserItem *item, QtBrowserItem *afterItem) override;
    void itemRemoved(QtBrowserItem *item) override;
    void itemChanged(QtBrowserItem *item) override;

private:

    QScopedPointer<QtButtonPropertyBrowserPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtButtonPropertyBrowser)
    Q_DISABLE_COPY_MOVE(QtButtonPropertyBrowser)
    Q_PRIVATE_SLOT(d_func(), void slotUpdate())
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed())
    Q_PRIVATE_SLOT(d_func(), void slotToggled(bool))

};

QT_END_NAMESPACE

#endif
