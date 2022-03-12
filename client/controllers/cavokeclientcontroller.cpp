#include "cavokeclientcontroller.h"

CavokeClientController::CavokeClientController(QObject *parent)
    : QObject{parent},
      model{parent},
      networkManager{parent},
      testWindowView{},
      startView{},
      joinGameView{},
      settingsView{},
      protoRoomView{} {
    // startQml connection
    connect(&model, SIGNAL(startQmlApplication(CavokeQmlGameModel *)), this,
            SLOT(startQmlApplication(CavokeQmlGameModel *)));

    // All back buttons
    connect(&testWindowView, SIGNAL(shownStartView()), this,
            SLOT(showStartView()));
    connect(&joinGameView, SIGNAL(shownStartView()), this,
            SLOT(showStartView()));
    connect(&createGameView, SIGNAL(shownStartView()), this,
            SLOT(showStartView()));
    connect(&gamesListView, SIGNAL(shownStartView()), this,
            SLOT(showStartView()));
    connect(&settingsView, SIGNAL(shownStartView()), this,
            SLOT(showStartView()));

    // Main navigation buttons from startView
    connect(&startView, SIGNAL(shownTestWindowView()), this,
            SLOT(showTestWindowView()));
    connect(&startView, SIGNAL(shownJoinGameView()), this,
            SLOT(showJoinGameView()));
    connect(&startView, SIGNAL(shownCreateGameView()), this,
            SLOT(showCreateGameView()));
    connect(&startView, SIGNAL(shownGamesListView()), this,
            SLOT(showGamesListView()));
    connect(&startView, SIGNAL(shownSettingsView()), this,
            SLOT(showSettingsView()));

    // startView Exit button
    connect(&startView, SIGNAL(clickedExitButton()), this,
            SLOT(exitApplication()), Qt::QueuedConnection);

    // testWindowView actions
    connect(&testWindowView, SIGNAL(startGame(QString)), &model,
            SLOT(loadQmlGame(QString)));
    connect(&testWindowView, SIGNAL(testHealthConnectionButton()),
            &networkManager, SLOT(getHealth()));

    // createGameView actions
    connect(&createGameView, SIGNAL(currentIndexChanged(int)), &model,
            SLOT(receivedGameIndexChange(int)));
    connect(&model, SIGNAL(updateSelectedGame(GameInfo)), &createGameView,
            SLOT(gotNewSelectedGame(GameInfo)));
    connect(&createGameView, SIGNAL(startedCreateGameRoutine(int)), this,
            SLOT(createGameDownload(int)));
    connect(this, SIGNAL(createGameDownloaded()), this,
            SLOT(createGameSendRequest()));
    connect(&networkManager, SIGNAL(gotSessionInfo(QString)), this,
            SLOT(createGameShowProtoRoomView(QString)));

    // joinGameView workflow
    connect(&joinGameView, SIGNAL(joinedGame(QString)), this,
            SLOT(joinGameRequest(QString)));
    connect(&networkManager, SIGNAL(gotJoinedSessionInfo(QString)), this,
            SLOT(joinGameDownload(QString)));
    connect(this, SIGNAL(joinGameDownloaded()), this,
            SLOT(joinGameShowProtoRoomView()));

    // gamesListView actions
    connect(&gamesListView, SIGNAL(currentIndexChanged(int)), &model,
            SLOT(receivedGameIndexChangeInList(int)));
    connect(&model, SIGNAL(updateSelectedGameInList(GameInfo)), &gamesListView,
            SLOT(gotNewSelectedGame(GameInfo)));
    connect(&gamesListView, SIGNAL(requestedDownloadGame(int)), &model,
            SLOT(gotIndexToDownload(int)));

    // Download games list from server workflow
    connect(this, SIGNAL(loadGamesList()), &networkManager,
            SLOT(getGamesList()));
    connect(&networkManager, SIGNAL(finalizedGamesList(QJsonArray)), &model,
            SLOT(updateGamesList(QJsonArray)));
    connect(&model, SIGNAL(gamesListUpdated(std::vector<GameInfo>)),
            &createGameView, SLOT(gotGamesListUpdate(std::vector<GameInfo>)));

    // Download and unpack game workflow
    connect(&model, SIGNAL(downloadGame(QString)), &networkManager,
            SLOT(getGamesClient(QString)));
    connect(&networkManager, SIGNAL(downloadedGameFile(QFile *, QString)), this,
            SLOT(unpackDownloadedQml(QFile *, QString)));

    // protoRoomView actions
    connect(&protoRoomView, SIGNAL(joinedCreatedGame(QString)), this,
            SLOT(startQmlByGameId(QString)));

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

void CavokeClientController::showGamesListView() {
    gamesListView.show();
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
    qmlView->show();
}

void CavokeClientController::exitApplication() {
    testWindowView.close();
    joinGameView.close();
    gamesListView.close();
    createGameView.close();
    startView.close();
}

void CavokeClientController::startQmlByGameId(const QString &gameId) {
    currentQmlGameModel = new CavokeQmlGameModel(
        QUrl(cache_manager::get_cached_app_path(gameId)));
    startQmlApplication(currentQmlGameModel);
    connect(currentQmlGameModel, SIGNAL(sendMoveToNetwork(QString)),
            &networkManager, SLOT(sendMove(QString)));
    connect(&networkManager, SIGNAL(gotGameUpdate(QString)),
            currentQmlGameModel, SLOT(getUpdateFromNetwork(QString)));
    connect(currentQmlGameModel, SIGNAL(closingQml()), this, SLOT(stopQml()));
    networkManager.startPolling();
}

void CavokeClientController::stopQml() {
    startView.show();
    currentQmlGameModel->deleteLater();
    networkManager.stopPolling();
}

void CavokeClientController::unpackDownloadedQml(QFile *file,
                                                 const QString &gameId) {
    cache_manager::save_zip_to_cache(file, gameId);
    qDebug() << "UnpackDownloadedQml Finished";
    file->deleteLater();
    if (isCreatingSession) {
        emit createGameDownloaded();
    } else if (isJoiningSession) {
        emit joinGameDownloaded();
    }
}

void CavokeClientController::createGameDownload(int gameIndex) {
    qDebug() << "Now we are createGameDownload with index: " << gameIndex;
    isCreatingSession = true;
    isJoiningSession = false;
    creatingGameId = model.getGameIdByIndex(gameIndex);
    model.gotIndexToDownload(gameIndex);

    protoRoomView.prepareJoinCreate(false);
    protoRoomView.updateInviteCode("");
    protoRoomView.updateStatus(CreatingGameStatus::DOWNLOAD);
    protoRoomView.updateGameName(creatingGameId);

    createGameView.close();
    protoRoomView.show();
}

void CavokeClientController::createGameSendRequest() {
    qDebug() << "Now we are createGameSendRequest with id: " << creatingGameId;

    networkManager.createSession(creatingGameId);
    protoRoomView.updateStatus(CreatingGameStatus::REQUESTED);
}
void CavokeClientController::createGameShowProtoRoomView(
    const QString &inviteCode) {
    qDebug() << "Now we are createGameShowProtoRoomView with inviteCode: "
             << inviteCode;

    protoRoomView.updateStatus(CreatingGameStatus::DONE);
    protoRoomView.updateInviteCode(inviteCode);
}
void CavokeClientController::joinGameRequest(const QString &inviteCode) {
    qDebug() << "Now we are joinGameRequest with inviteCode: " << inviteCode;
    isCreatingSession = false;
    isJoiningSession = true;

    protoRoomView.prepareJoinCreate(false);
    protoRoomView.updateInviteCode("");
    protoRoomView.updateStatus(CreatingGameStatus::REQUESTED);

    networkManager.joinSession(inviteCode);
    joinGameView.close();
    protoRoomView.show();
}
void CavokeClientController::joinGameDownload(const QString &gameId) {
    qDebug() << "Now we are joinGameDownload with gameId: " << gameId;

    protoRoomView.updateGameName(gameId);
    protoRoomView.updateStatus(CreatingGameStatus::DOWNLOAD);

    networkManager.getGamesClient(gameId);
}
void CavokeClientController::joinGameShowProtoRoomView() {
    qDebug() << "Now we are joinGameShowProtoRoomView";

    protoRoomView.updateStatus(CreatingGameStatus::DONE);
}
