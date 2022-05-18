#include "cavokeclientcontroller.h"
#include <QOAuthHttpServerReplyHandler>

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
    connect(this, SIGNAL(clearScreens()), &gamesListView, SLOT(displayEmpty()));

    // both Join and Create gameView actions
    connect(&networkManager, SIGNAL(gotSessionInfo(SessionInfo)), this,
            SLOT(gotSessionInfo(SessionInfo)));
    connect(this, SIGNAL(setGameName(QString)), &protoRoomView,
            SLOT(updateGameName(QString)));

    // joinGameView workflow
    connect(&joinGameView, SIGNAL(joinedGame(QString)), this,
            SLOT(joinGameStart(QString)));
    connect(&networkManager, SIGNAL(gotGameInfo(GameInfo)), this,
            SLOT(gotCurrentGameInfo(GameInfo)));

    // gamesListView actions
    connect(&gamesListView, SIGNAL(currentIndexChanged(int)), &model,
            SLOT(receivedGameIndexChangeInList(int)));
    connect(&model, SIGNAL(updateSelectedGameInList(GameInfo)), &gamesListView,
            SLOT(gotNewSelectedGame(GameInfo)));
    connect(&gamesListView, SIGNAL(requestedDownloadGame(int)), &model,
            SLOT(gotIndexToDownload(int)));
    connect(this, SIGNAL(clearScreens()), &gamesListView, SLOT(displayEmpty()));

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
    connect(&protoRoomView, SIGNAL(leftRoom()), this, SLOT(leftSession()));
    connect(&protoRoomView, SIGNAL(leftRoom()), &networkManager,
            SLOT(leaveSession()));
    connect(this, SIGNAL(createdAvailableRolesList(std::vector<Role>)),
            &protoRoomView, SLOT(gotRolesListUpdate(std::vector<Role>)));
    connect(&protoRoomView, SIGNAL(newRoleChosen(int)), &networkManager,
            SLOT(changeRoleInSession(int)));

    // settingsView actions
    connect(this, SIGNAL(initSettingsValues(QString, QString)), &settingsView,
            SLOT(initStartValues(QString, QString)));
    connect(&settingsView, SIGNAL(updatedSettings(QString, QString)), this,
            SLOT(updateSettings(QString, QString)));

    defaultSettingsInitialization();

    // oauth reply handler
    auto replyHandler = new QOAuthHttpServerReplyHandler(1337, this);
    cavoke::auth::AuthenticationManager::getInstance().oauth2.setReplyHandler(
        replyHandler);

    startView.show();
}

void CavokeClientController::defaultSettingsInitialization() {
    settings.setValue(PLAYER_NICKNAME,
                      settings.value(PLAYER_NICKNAME, DEFAULT_NICKNAME));
    settings.setValue(
        NETWORK_HOST,
        settings.value(NETWORK_HOST, NetworkManager::DEFAULT_HOST));

    networkManager.changeHost(
        QUrl::fromUserInput(settings.value(NETWORK_HOST).toString()));

    emit initSettingsValues(settings.value(PLAYER_NICKNAME).toString(),
                            settings.value(NETWORK_HOST).toString());
}

void CavokeClientController::showTestWindowView() {
    testWindowView.show();
}

void CavokeClientController::leftSession() {
    networkManager.stopGamePolling();
    networkManager.stopSessionPolling();
    networkManager.stopValidationPolling();
    qmlDownloadStatus = QMLDownloadStatus::NOT_STARTED;
    hostGuestStatus = HostGuestStatus::NOT_IN;
    currentGameInfo = GameInfo();
    currentSessionInfo = SessionInfo();
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
    startQmlByGameId(currentGameInfo.id);
}

void CavokeClientController::stopQml() {
    networkManager.stopGamePolling();
    leftSession();
    startView.show();
    currentQmlGameModel->deleteLater();
}

void CavokeClientController::unpackDownloadedQml(QFile *file,
                                                 const QString &gameId) {
    cache_manager::save_zip_to_cache(file, gameId);
    qDebug() << "UnpackDownloadedQml Finished";
    file->deleteLater();

    qmlDownloadStatus = QMLDownloadStatus::DOWNLOADED;
}

void CavokeClientController::createGameStart(int gameIndex) {
    qDebug() << "Now we are creating game with index: " << gameIndex;

    currentGameInfo = model.getGameByIndex(gameIndex);

    protoRoomView.clear();
    protoRoomView.updateStatus(ProtoRoomView::CreatingGameStatus::REQUESTED);
    createGameView.close();
    protoRoomView.show();

    networkManager.createSession(currentGameInfo.id);
}

void CavokeClientController::joinGameStart(const QString &inviteCode) {
    qDebug() << "Now we are joinGameStart with inviteCode: " << inviteCode;

    protoRoomView.clear();
    protoRoomView.updateStatus(ProtoRoomView::CreatingGameStatus::REQUESTED);
    joinGameView.close();
    protoRoomView.show();

    networkManager.joinSession(inviteCode);
}

void CavokeClientController::gotSessionInfo(const SessionInfo &sessionInfo) {
    qDebug() << "Now we got session info";

    currentSessionInfo = sessionInfo;

    if (hostGuestStatus == HostGuestStatus::NOT_IN) {
        networkManager.startSessionPolling();
    }

    if (currentSessionInfo.isHost && hostGuestStatus != HostGuestStatus::HOST) {
        becomeHost();
    } else if (!currentSessionInfo.isHost &&
               hostGuestStatus != HostGuestStatus::GUEST) {
        becomeGuest();
    }

    if (qmlDownloadStatus == QMLDownloadStatus::NOT_STARTED) {
        networkManager.getGamesClient(currentSessionInfo.game_id);
        qmlDownloadStatus = QMLDownloadStatus::DOWNLOADING;
        protoRoomView.updateStatus(ProtoRoomView::CreatingGameStatus::DOWNLOAD);
    }

    protoRoomView.updateSessionInfo(currentSessionInfo);

    if (currentGameInfo.players_num == 0) {
        networkManager.getGamesConfig(currentSessionInfo.game_id);
    } else if (qmlDownloadStatus == QMLDownloadStatus::DOWNLOADED) {
        protoRoomView.updateStatus(ProtoRoomView::CreatingGameStatus::DONE);
        collectListOfAvailableRoles();
    }
}

void CavokeClientController::gotCurrentGameInfo(const GameInfo &gameInfo) {
    currentGameInfo = gameInfo;
    emit setGameName(currentGameInfo.display_name);
}

void CavokeClientController::updateSettings(const QString &nickname,
                                            const QString &host) {
    settings.setValue(PLAYER_NICKNAME, nickname);
    settings.setValue(NETWORK_HOST, host);
    emit clearScreens();
    networkManager.changeHost(QUrl::fromUserInput(host));
}
void CavokeClientController::collectListOfAvailableRoles() {
    std::vector<bool> isFree(currentGameInfo.players_num, true);
    int ourRole = -1;
    QString userId = networkManager.getUserId();
    for (const auto &player : currentSessionInfo.players) {
        isFree[player.player_id] = false;
        if (player.user_id == userId) {
            ourRole = player.player_id;
        }
    }
    std::vector<Role> availableRoles;
    availableRoles.emplace_back(currentGameInfo.role_names[ourRole],
                                ourRole);  // Now first.
    for (int i = 0; i < currentGameInfo.players_num; ++i) {
        if (isFree[i]) {
            availableRoles.emplace_back(currentGameInfo.role_names[i], i);
        }
    }
    emit createdAvailableRolesList(availableRoles);
}

void CavokeClientController::becomeHost() {
    hostGuestStatus = HostGuestStatus::HOST;
    networkManager.startValidationPolling();
}

void CavokeClientController::becomeGuest() {
    hostGuestStatus = HostGuestStatus::GUEST;
    networkManager.stopValidationPolling();
}
