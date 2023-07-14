#include "GoBangServe.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GoBangServe w;
    w.show();
    return a.exec();
}
