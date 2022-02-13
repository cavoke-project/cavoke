#include "cavokeclientcontroller.h"


CavokeClientController::CavokeClientController(QObject *parent)
        : QObject{parent}, model{parent}, view{} {
    connect(&view, SIGNAL(startGame(QString)), &model, SLOT(loadQmlGame(QString)));
    connect(&model, SIGNAL(startQmlApplication(CavokeQmlGameModel * )), this,
            SLOT(startQmlApplication(CavokeQmlGameModel * )));
    view.show();
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

void CavokeClientController::closeCavokeView() {
    view.close();
}

void CavokeClientController::showCavokeView() {
    view.show();
}
