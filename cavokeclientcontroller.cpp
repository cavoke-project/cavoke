#include "cavokeclientcontroller.h"


CavokeClientController::CavokeClientController(QObject *parent)
    : QObject{parent}, model{parent}, view{} {
    connect(&view, SIGNAL(startGame(QString)), &model, SLOT(loadQmlGame(QString)));
    connect(&model, SIGNAL(startQmlApplication(CavokeQmlGameModel*)), this, SLOT(startQmlApplication(CavokeQmlGameModel*)));

    view.show();
}

void CavokeClientController::startQmlApplication(CavokeQmlGameModel *gameModel) {
    auto *qmlView = new QQuickView();
    qmlView->rootContext()->setContextProperty("cavoke", gameModel);
    qmlView->setSource(gameModel->qmlPath);
    qmlView->show();

    // Test
    emit gameModel->receiveUpdate("c++ -> qml working!");
}
