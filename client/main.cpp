#include "cavokeclientcontroller.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CavokeClientController c(&a);
    return a.exec();
}
