#include "browserlibtest.h"

#include "qteditorfactory.h"
#include "qtpropertymanager.h"
#include "qttreepropertybrowser.h"

browserlibTest::browserlibTest(QWidget *parent): QMainWindow(parent)
{
    QtTreePropertyBrowser* pBrowser = new QtTreePropertyBrowser(this);
    setCentralWidget(pBrowser);
    QtSizePropertyManager* pManager = new QtSizePropertyManager(pBrowser);
    QtProperty* pProperty = pManager->addProperty("Size");

    QtSpinBoxFactory* pFactory = new QtSpinBoxFactory(pBrowser);
    pBrowser->setFactoryForManager(pManager->subIntPropertyManager(), pFactory);

    pBrowser->addProperty(pProperty);

    QtColorPropertyManager* pColorManager = new QtColorPropertyManager(pBrowser);
    QtProperty* pColorProperty = pColorManager->addProperty("Color");
    QtColorEditorFactory* pColorFactory = new QtColorEditorFactory(pBrowser);
    pBrowser->setFactoryForManager(pColorManager, pColorFactory);
    pBrowser->addProperty(pColorProperty);
}
