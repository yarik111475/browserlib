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

#ifndef QTGROUPBOXPROPERTYBROWSER_H
#define QTGROUPBOXPROPERTYBROWSER_H

#include "qtpropertybrowser.h"
#include <QtCore/QSet>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QGroupBox>
#include <QtCore/QTimer>
#include <QtCore/QMap>


QT_BEGIN_NAMESPACE

class QtGroupBoxPropertyBrowser;

class QtGroupBoxPropertyBrowserPrivate
{
    QtGroupBoxPropertyBrowser* q_ptr;
    Q_DECLARE_PUBLIC(QtGroupBoxPropertyBrowser)
public:

    void init(QWidget* parent);

    void propertyInserted(QtBrowserItem* index, QtBrowserItem* afterIndex);
    void propertyRemoved(QtBrowserItem* index);
    void propertyChanged(QtBrowserItem* index);
    QWidget* createEditor(QtProperty* property, QWidget* parent) const;
    
    void slotEditorDestroyed();
    void slotUpdate();

    struct WidgetItem
    {
        QWidget* widget{ nullptr }; // can be null
        QLabel* label{ nullptr };
        QLabel* widgetLabel{ nullptr };
        QGroupBox* groupBox{ nullptr };
        QGridLayout* layout{ nullptr };
        QFrame* line{ nullptr };
        WidgetItem* parent{ nullptr };
        QList<WidgetItem*> children;
    };
private:
    void updateLater();
    void updateItem(WidgetItem* item);
    void insertRow(QGridLayout* layout, int row) const;
    void removeRow(QGridLayout* layout, int row) const;

    bool hasHeader(WidgetItem* item) const;

    QMap<QtBrowserItem*, WidgetItem*> m_indexToItem;
    QMap<WidgetItem*, QtBrowserItem*> m_itemToIndex;
    QMap<QWidget*, WidgetItem*> m_widgetToItem;
    QGridLayout* m_mainLayout;
    QList<WidgetItem*> m_children;
    QList<WidgetItem*> m_recreateQueue;
};
class QtGroupBoxPropertyBrowser : public QtAbstractPropertyBrowser
{
    Q_OBJECT
public:

    QtGroupBoxPropertyBrowser(QWidget *parent = 0);
    ~QtGroupBoxPropertyBrowser();

protected:
    void itemInserted(QtBrowserItem *item, QtBrowserItem *afterItem) override;
    void itemRemoved(QtBrowserItem *item) override;
    void itemChanged(QtBrowserItem *item) override;

private:

    QScopedPointer<QtGroupBoxPropertyBrowserPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtGroupBoxPropertyBrowser)
    Q_DISABLE_COPY_MOVE(QtGroupBoxPropertyBrowser)
    Q_PRIVATE_SLOT(d_func(), void slotUpdate())
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed())

};

QT_END_NAMESPACE

#endif
