#include "cavokeclientcontroller.h"


CavokeClientController::CavokeClientController(QObject *parent)
        : QObject{parent}, model{parent}, testWindowView{}, startView{}, joinGameView{} {
    
    connect(&testWindowView, SIGNAL(startGame(QString)), &model, SLOT(loadQmlGame(QString)));
    connect(&model, SIGNAL(startQmlApplication(CavokeQmlGameModel * )), this,
            SLOT(startQmlApplication(CavokeQmlGameModel * )));
    
    connect(&testWindowView, SIGNAL(shownStartView()), this, SLOT(showStartView()));
    connect(&joinGameView, SIGNAL(shownStartView()), this, SLOT(showStartView()));
    
    connect(&startView, SIGNAL(shownTestWindowView()), this, SLOT(showTestWindowView()));
    connect(&startView, SIGNAL(shownJoinGameView()), this, SLOT(showJoinGameView()));
    
    connect(&startView, SIGNAL(clickedExitButton()), this, SLOT(exitApplication()), Qt::QueuedConnection);
    
    startView.show();
}

void CavokeClientController::showTestWindowView() {
    testWindowView.show();
}

void CavokeClientController::showStartView() {
    startView.show();
}

void CavokeClientController::showJoinGameView() {
    joinGameView.show();
}


void CavokeClientController::startQmlApplication(CavokeQmlGameModel *gameModel) {
    auto *qmlView = new QQuickView();
    qmlView->rootContext()->setContextProperty("cavoke", gameModel);
    qmlView->setSource(gameModel->qmlPath);
    if (!qmlView->errors().empty()) {
        qDebug() << "An error(s) occurred while opening the QML app:" << '\n';
        for (const auto &error: qmlView->errors()) {
            qDebug() << error;
        }
        qmlView->destroy();
        return;
    }
    qmlView->show();

    // Test
    emit gameModel->receiveUpdate("c++ -> qml working!");
}

void CavokeClientController::exitApplication() {
    // FIXME: not the best way, probably
    testWindowView.close();
    joinGameView.close();
    startView.close();
}
