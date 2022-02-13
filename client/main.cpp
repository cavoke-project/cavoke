#include "cavokeclientcontroller.h"
#include "startcontroller.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    CavokeClientController c(&a);
    StartController c(&a);
    return a.exec();
}
