#include "cavokeclientcontroller.h"

CavokeClientController::CavokeClientController(QObject *parent)
    : QObject{parent},
      model{parent},
      networkManager{parent},
      testWindowView{},
      startView{},
      joinGameView{},
      settingsView{} {
    connect(this, SIGNAL(loadGamesList()), &networkManager,
            SLOT(getGamesList()));

    connect(&testWindowView, SIGNAL(startGame(QString)), &model,
            SLOT(loadQmlGame(QString)));
    connect(&model, SIGNAL(startQmlApplication(CavokeQmlGameModel *)), this,
            SLOT(startQmlApplication(CavokeQmlGameModel *)));

    connect(&testWindowView, SIGNAL(shownStartView()), this,
            SLOT(showStartView()));
    connect(&joinGameView, SIGNAL(shownStartView()), this,
            SLOT(showStartView()));
    connect(&createGameView, SIGNAL(shownStartView()), this,
            SLOT(showStartView()));
    connect(&settingsView, SIGNAL(shownStartView()), this,
            SLOT(showStartView()));

    connect(&startView, SIGNAL(shownTestWindowView()), this,
            SLOT(showTestWindowView()));
    connect(&startView, SIGNAL(shownJoinGameView()), this,
            SLOT(showJoinGameView()));
    connect(&startView, SIGNAL(shownCreateGameView()), this,
            SLOT(showCreateGameView()));
    connect(&startView, SIGNAL(shownSettingsView()), this,
            SLOT(showSettingsView()));

    connect(&startView, SIGNAL(clickedExitButton()), this,
            SLOT(exitApplication()), Qt::QueuedConnection);

    connect(&testWindowView, SIGNAL(testHealthConnectionButton()),
            &networkManager, SLOT(doTestHealthCheck()));

    connect(&networkManager, SIGNAL(finalizedGamesList(QJsonArray)), &model,
            SLOT(updateGamesList(QJsonArray)));
    connect(&model, SIGNAL(gamesListUpdated(std::vector<GameInfo>)),
            &createGameView, SLOT(gotGamesListUpdate(std::vector<GameInfo>)));
    connect(&createGameView, SIGNAL(currentIndexChanged(int)), &model,
            SLOT(receivedGameIndexChange(int)));
    connect(&model, SIGNAL(updateSelectedGame(GameInfo)), &createGameView,
            SLOT(gotNewSelectedGame(GameInfo)));

    connect(&joinGameView, SIGNAL(joinedTicTacToe()), this,
            SLOT(startTicTacToe()));

    startView.show();

    emit loadGamesList();
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

void CavokeClientController::showCreateGameView() {
    createGameView.show();
}

void CavokeClientController::showSettingsView() {
    settingsView.show();
}

void CavokeClientController::startQmlApplication(
    CavokeQmlGameModel *gameModel) {
    auto *qmlView = new QQuickView();
    connect(qmlView, SIGNAL(closing(QQuickCloseEvent *)), gameModel,
            SLOT(getClosingFromQml(QQuickCloseEvent *)));
    qmlView->show();
    qmlView->rootContext()->setContextProperty("cavoke", gameModel);
    qmlView->setSource(gameModel->qmlPath);
    if (!qmlView->errors().empty()) {
        qDebug() << "An error(s) occurred while opening the QML app:" << '\n';
        for (const auto &error : qmlView->errors()) {
            qDebug() << error;
        }
        qmlView->destroy();
        return;
    }

    // Test
    emit gameModel->receiveUpdate("c++ -> qml working!");
}

void CavokeClientController::exitApplication() {
    testWindowView.close();
    joinGameView.close();
    createGameView.close();
    startView.close();
}
void CavokeClientController::startTicTacToe() {
    currentQmlGameModel =
        new CavokeQmlGameModel(QUrl("../../client/"
                                    "tictactoe-files/tic-tac-toe.qml"));
    startQmlApplication(currentQmlGameModel);
    connect(currentQmlGameModel, SIGNAL(sendMoveToNetwork(QString)),
            &networkManager, SLOT(sendMove(QString)));
    connect(&networkManager, SIGNAL(gotGameUpdate(QString)),
            currentQmlGameModel, SLOT(getUpdateFromNetwork(QString)));
    connect(currentQmlGameModel, SIGNAL(closingQml()), this, SLOT(stopTicTacToe()));
    joinGameView.close();
    networkManager.startPolling();
}

void CavokeClientController::stopTicTacToe() {
    startView.show();
    currentQmlGameModel->deleteLater();
    networkManager.stopPolling();
}
