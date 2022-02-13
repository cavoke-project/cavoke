#include "startcontroller.h"

StartController::StartController(QObject *parent) : QObject{parent}, view{} {
    view.show();
    cavokeWindow = new CavokeClientController(parent);
    connect(&view, SIGNAL(shownCavokeWindow()), &cavokeWindow, SLOT(showCavokeWindow()));
    
}

void StartController::showStartView() {
    view.show();
}

void StartController::closeStartView() {
    view.close();
}
