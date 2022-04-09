#include <QApplication>
#include "cavokeclientcontroller.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Cavoke");
    QCoreApplication::setOrganizationDomain("cavoke.alexkovrigin.me");
    QCoreApplication::setApplicationName("Cavoke");
    CavokeClientController c(&a);
    return a.exec();
}
