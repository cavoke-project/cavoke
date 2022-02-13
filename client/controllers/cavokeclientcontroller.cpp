#include "cavokeclientcontroller.h"


CavokeClientController::CavokeClientController(QObject *parent)
        : QObject{parent}, model{parent}, clientView{}, startView{} {
    connect(&clientView, SIGNAL(startGame(QString)), &model, SLOT(loadQmlGame(QString)));
    connect(&model, SIGNAL(startQmlApplication(CavokeQmlGameModel * )), this,
            SLOT(startQmlApplication(CavokeQmlGameModel * )));
    connect(&clientView, SIGNAL(shownStartView()), this, SLOT(showStartView()));
    connect(&startView, SIGNAL(shownClientView()), this, SLOT(showCavokeView()));
    startView.show();
}

void CavokeClientController::startQmlApplication(CavokeQmlGameModel *gameModel) {
    auto *qmlView = new QQuickView();
    qmlView->rootContext()->setContextProperty("cavoke", gameModel);
    qmlView->setSource(gameModel->qmlPath);
    if (!qmlView->errors().empty()) {
        qDebug() << "An error(s) occurred while opening the QML app:" << '\n';
        for (const auto& error: qmlView->errors()) {
            qDebug() << error;
        }
        qmlView->destroy();
        return;
    }
    qmlView->show();

    // Test
    emit gameModel->receiveUpdate("c++ -> qml working!");
}

void CavokeClientController::showCavokeView() {
    clientView.show();
}

void CavokeClientController::showStartView() {
    startView.show();
}
