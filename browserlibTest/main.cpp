#include "browserlibtest.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    browserlibTest w;
    w.show();
    return a.exec();
}
