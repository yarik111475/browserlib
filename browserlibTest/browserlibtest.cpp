#include "browserlibtest.h"
#include "qttreepropertybrowser.h"

browserlibTest::browserlibTest(QWidget *parent): QMainWindow(parent)
{
	QtTreePropertyBrowser* pTreeBrowser = new QtTreePropertyBrowser(this);
	setCentralWidget(pTreeBrowser);
}
