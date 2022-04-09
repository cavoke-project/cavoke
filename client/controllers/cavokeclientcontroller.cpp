#include "cavokeclientcontroller.h"

CavokeClientController::CavokeClientController(QObject *parent)
    : QObject{parent},
      model{parent},
      networkManager{parent},
      testWindowView{},
      startView{},
      joinGameView{},
      settingsView{},
      protoRoomView{},
      settings{} {
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
    connect(&protoRoomView, SIGNAL(shownStartView()), this,
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
            SLOT(createGameStart(int)));
    connect(this, SIGNAL(createGameDownloaded()), this,
            SLOT(createGameSendRequest()));

    // both Join and Create gameView actions
    connect(&networkManager, SIGNAL(gotSessionInfo(SessionInfo)), this,
            SLOT(gotSessionInfo(SessionInfo)));

    // joinGameView workflow
    connect(&joinGameView, SIGNAL(joinedGame(QString)), this,
            SLOT(joinGameStart(QString)));
    connect(this, SIGNAL(joinGameDownloaded()), this,
            SLOT(creatingJoiningGameDone()));

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
    connect(&model, SIGNAL(gamesListUpdated(std::vector<GameInfo>)),
            &gamesListView, SLOT(gotGamesListUpdate(std::vector<GameInfo>)));

    // Download and unpack game workflow
    connect(&model, SIGNAL(downloadGame(QString)), &networkManager,
            SLOT(getGamesClient(QString)));
    connect(&networkManager, SIGNAL(downloadedGameFile(QFile *, QString)), this,
            SLOT(unpackDownloadedQml(QFile *, QString)));

    // protoRoomView actions
    connect(&protoRoomView, SIGNAL(joinedCreatedGame()), this,
            SLOT(startLoadedQml()));
    connect(&networkManager, SIGNAL(gotValidationResult(ValidationResult)),
            &protoRoomView, SLOT(updateValidationResult(ValidationResult)));
    connect(&protoRoomView, SIGNAL(createdGame()), &networkManager,
            SLOT(startSession()));

    // settingsView actions
    connect(this, SIGNAL(initSettingsValues(QString, QString)), &settingsView,
            SLOT(initStartValues(QString, QString)));
    connect(&settingsView, SIGNAL(updatedSettings(QString, QString)), this,
            SLOT(updateSettings(QString, QString)));

    // Default settings initialization
    settings.setValue("player/nickname",
                      settings.value("player/nickname", "Guest"));
    settings.setValue(
        "network/host",
        settings.value("network/host", NetworkManager::BASE_HOST));
    
    networkManager.changeHost(
        QUrl::fromUserInput(settings.value("network/host").toString()));

    emit initSettingsValues(settings.value("player/nickname").toString(),
                            settings.value("network/host").toString());

    startView.show();

//    emit loadGamesList(); Now it loads games list inside of changeHost
}

void CavokeClientController::showTestWindowView() {
    testWindowView.show();
}

void CavokeClientController::showStartView() {
    startView.show();
    status = CreateJoinControllerStatus::NOTHING;
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
    settingsView.close();
    startView.close();
}

void CavokeClientController::startQmlByGameId(const QString &gameId) {
    currentQmlGameModel =
        new CavokeQmlGameModel(cache_manager::get_cached_app_path(gameId));
    startQmlApplication(currentQmlGameModel);
    connect(currentQmlGameModel, SIGNAL(sendMoveToNetwork(QString)),
            &networkManager, SLOT(sendMove(QString)));
    connect(&networkManager, SIGNAL(gotGameUpdate(QString)),
            currentQmlGameModel, SLOT(getUpdateFromNetwork(QString)));
    connect(currentQmlGameModel, SIGNAL(closingQml()), this, SLOT(stopQml()));
    networkManager.startGamePolling();
}

void CavokeClientController::startLoadedQml() {
    networkManager.stopSessionPolling();
    networkManager.stopValidationPolling();
    startQmlByGameId(currentGameId);
}

void CavokeClientController::stopQml() {
    startView.show();
    currentQmlGameModel->deleteLater();
    networkManager.stopGamePolling();
}

void CavokeClientController::unpackDownloadedQml(QFile *file,
                                                 const QString &gameId) {
    cache_manager::save_zip_to_cache(file, gameId);
    qDebug() << "UnpackDownloadedQml Finished";
    file->deleteLater();
    if (status == CreateJoinControllerStatus::CREATING) {
        emit createGameDownloaded();
    } else if (status == CreateJoinControllerStatus::JOINING) {
        emit joinGameDownloaded();
    }
}

void CavokeClientController::createGameStart(int gameIndex) {
    qDebug() << "Now we are creating game with index: " << gameIndex;

    status = CreateJoinControllerStatus::CREATING;
    protoRoomView.prepareJoinCreate(false);

    currentGameId = model.getGameIdByIndex(gameIndex);

    createGameView.close();
    protoRoomView.show();

    downloadCurrentGame();
}

void CavokeClientController::joinGameStart(const QString &inviteCode) {
    qDebug() << "Now we are joinGameStart with inviteCode: " << inviteCode;

    status = CreateJoinControllerStatus::JOINING;
    protoRoomView.prepareJoinCreate(true);

    protoRoomView.updateStatus(ProtoRoomView::CreatingGameStatus::REQUESTED);

    joinGameView.close();
    protoRoomView.show();

    networkManager.joinSession(inviteCode);
}

void CavokeClientController::downloadCurrentGame() {
    qDebug() << "Now we are downloading game: " << currentGameId;

    protoRoomView.updateStatus(ProtoRoomView::CreatingGameStatus::DOWNLOAD);
    model.gotGameIdToDownload(currentGameId);
}

void CavokeClientController::createGameSendRequest() {
    qDebug() << "Now we are createGameSendRequest with id: " << currentGameId;

    networkManager.createSession(currentGameId);
    protoRoomView.updateStatus(ProtoRoomView::CreatingGameStatus::REQUESTED);
}

void CavokeClientController::gotSessionInfo(const SessionInfo &sessionInfo) {
    qDebug() << "Now we got session info";

    protoRoomView.updateSessionInfo(sessionInfo);

    if (status == CreateJoinControllerStatus::CREATING) {
        creatingJoiningGameDone();
    } else if (status == CreateJoinControllerStatus::JOINING) {
        currentGameId = sessionInfo.game_id;
        downloadCurrentGame();
    }
}

void CavokeClientController::creatingJoiningGameDone() {
    qDebug() << "Now creating/joining game preparations are done";

    protoRoomView.updateStatus(ProtoRoomView::CreatingGameStatus::DONE);
    networkManager.startSessionPolling();
    if (status == CreateJoinControllerStatus::CREATING) {
        networkManager.startValidationPolling();
        status = CreateJoinControllerStatus::POLLING_CREATE;
    } else if (status == CreateJoinControllerStatus::JOINING) {
        status = CreateJoinControllerStatus::POLLING_JOIN;
    }
}
void CavokeClientController::updateSettings(const QString &nickname,
                                            const QString &host) {
    settings.setValue("player/nickname", nickname);
    settings.setValue("network/host", host);
    networkManager.changeHost(QUrl::fromUserInput(host));
}
