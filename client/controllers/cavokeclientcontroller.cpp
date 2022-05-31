#include "cavokeclientcontroller.h"
#include <QOAuthHttpServerReplyHandler>

CavokeClientController::CavokeClientController(QObject *parent)
    : QObject{parent},
      model{parent},
      networkManager{parent},
      testWindowView{},
      startView{},
      joinGameView{},
      statisticsView{},
      settingsView{},
      roomView{},
      sessionView{},
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
    connect(&statisticsView, SIGNAL(shownStartView()), this,
            SLOT(showStartView()));
    connect(&roomView, SIGNAL(shownStartView()), this, SLOT(showStartView()));

    // Main navigation buttons from startView
    connect(&startView, SIGNAL(shownTestWindowView()), this,
            SLOT(showTestWindowView()));    // Not displayed
    connect(&startView, SIGNAL(shownJoinGameView()), this,
            SLOT(showJoinGameView()));
    connect(&startView, SIGNAL(shownCreateGameView()), this,
            SLOT(showCreateGameView()));
    connect(&startView, SIGNAL(shownGamesListView()), this,
            SLOT(showGamesListView()));
    connect(&startView, SIGNAL(shownSettingsView()), this,
            SLOT(showSettingsView()));
    connect(&startView, SIGNAL(shownStatisticsView()), this,
            SLOT(showStatisticsView()));

    // startView Exit button
    connect(&startView, SIGNAL(clickedExitButton()), this,
            SLOT(exitApplication()), Qt::QueuedConnection);

    // testWindowView actions
    connect(&testWindowView, SIGNAL(startGame(QString)), &model,
            SLOT(loadQmlGame(QString)));
    connect(&testWindowView, SIGNAL(testHealthConnectionButton()),
            &networkManager, SLOT(getHealth()));

    // createGameView actions
    connect(&createGameView, SIGNAL(startedCreateGameRoutine(QString)), this,
            SLOT(createGameStart(QString)));

    // joinGameView actions
    connect(&joinGameView, SIGNAL(joinedGame(QString)), this,
            SLOT(joinGameStart(QString)));

    // gamesListView actions
    connect(&gamesListView, SIGNAL(currentIndexChanged(int)), &model,
            SLOT(receivedGameIndexChangeInList(int)));
    connect(&gamesListView, SIGNAL(requestGameStatistics(QString)),
            &networkManager, SLOT(getGameStatistics(QString)));
    connect(&model, SIGNAL(updateSelectedGameInList(GameInfo)), &gamesListView,
            SLOT(gotNewSelectedGame(GameInfo)));
    connect(&networkManager, SIGNAL(gotGameStatistics(GameStatistics)),
            &gamesListView, SLOT(gotNewGameStatistics(GameStatistics)));
    connect(&gamesListView, SIGNAL(requestedDownloadGame(int)), &model,
            SLOT(gotIndexToDownload(int)));
    connect(this, SIGNAL(clearScreens()), &gamesListView, SLOT(displayEmpty()));

    // session/room/game-Info processing actions
    connect(&networkManager, SIGNAL(gotSessionInfo(SessionInfo)), this,
            SLOT(gotSessionInfo(SessionInfo)));
    connect(&networkManager, SIGNAL(gotGameInfo(GameInfo)), this,
            SLOT(gotCurrentGameInfo(GameInfo)));
    connect(this, SIGNAL(setGameName(QString)), &sessionView,
            SLOT(updateGameName(QString)));
    connect(&networkManager, SIGNAL(gotRoomInfo(RoomInfo)), this,
            SLOT(gotRoomInfo(RoomInfo)));

    // Download games list from server workflow
    connect(this, SIGNAL(loadGamesList()), &networkManager,
            SLOT(getGamesList()));
    connect(&networkManager, SIGNAL(finalizedGamesList(QJsonArray)), &model,
            SLOT(updateGamesList(QJsonArray)));
    connect(&model, SIGNAL(gamesListUpdated(std::vector<GameInfo>)),
            &gamesListView, SLOT(gotGamesListUpdate(std::vector<GameInfo>)));
    connect(&model, SIGNAL(gamesListUpdated(std::vector<GameInfo>)),
            &statisticsView, SLOT(gotGamesListUpdate(std::vector<GameInfo>)));
    connect(&model, SIGNAL(gamesListUpdated(std::vector<GameInfo>)), &roomView,
            SLOT(gotGamesListUpdate(std::vector<GameInfo>)));

    // Download and unpack game workflow
    connect(&model, SIGNAL(downloadGame(QString)), &networkManager,
            SLOT(getGamesClient(QString)));
    connect(&networkManager, SIGNAL(downloadedGameFile(QFile *, QString)), this,
            SLOT(unpackDownloadedQml(QFile *, QString)));

    // roomView actions
    connect(&roomView, SIGNAL(leftRoom()), this, SLOT(leftRoom()));
    connect(&roomView, SIGNAL(createdSession(QString)), this,
            SLOT(createSessionStart(QString)));
    connect(&roomView, SIGNAL(joinedSession(QString)), this,
            SLOT(joinSessionStart(QString)));
    connect(&roomView, SIGNAL(requestedSessionUpdate(QString)), &networkManager,
            SLOT(getSessionInfo(QString)));

    // sessionView actions
    connect(&sessionView, SIGNAL(joinedCreatedGame()), this,
            SLOT(startLoadedQml()));
    connect(&networkManager, SIGNAL(gotValidationResult(ValidationResult)),
            &sessionView, SLOT(updateValidationResult(ValidationResult)));
    connect(&sessionView, SIGNAL(createdGame()), &networkManager,
            SLOT(startSession()));
    connect(this, SIGNAL(createdAvailableRolesList(std::vector<Role>)),
            &sessionView, SLOT(gotRolesListUpdate(std::vector<Role>)));
    connect(&sessionView, SIGNAL(newRoleChosen(int)), &networkManager,
            SLOT(changeRoleInSession(int)));
    connect(&sessionView, SIGNAL(leftSession()), this, SLOT(leftSession()));
    connect(&sessionView, SIGNAL(shownRoomView()), this, SLOT(showRoomView()));

    // settingsView actions
    connect(this, SIGNAL(initSettingsValues(QString, QString)), &settingsView,
            SLOT(initStartValues(QString, QString)));
    connect(&settingsView, SIGNAL(updatedSettings(QString, QString)), this,
            SLOT(updateSettings(QString, QString)));
    connect(&networkManager, SIGNAL(gotDisplayName(QString)), &settingsView,
            SLOT(updateDisplayName(QString)));

    // statisticsView actions
    connect(&networkManager, SIGNAL(gotUserStatistics(UserStatistics)),
            &statisticsView, SLOT(gotUserStatisticsUpdate(UserStatistics)));
    connect(&statisticsView, SIGNAL(requestedRefresh()), &networkManager,
            SLOT(getMyUserStatistics()));
    connect(&networkManager, SIGNAL(gotUserGameStatistics(UserGameStatistics)),
            &statisticsView,
            SLOT(gotUserGameStatisticsUpdate(UserGameStatistics)));
    connect(&statisticsView, SIGNAL(statisticsGameChanged(QString)),
            &networkManager, SLOT(getMyUserGameStatistics(QString)));

    defaultSettingsInitialization();

    // oauth reply handler
    auto replyHandler = new QOAuthHttpServerReplyHandler(1337, this);
    auto &auth = cavoke::auth::AuthenticationManager::getInstance();
    auth.oauth2.setReplyHandler(replyHandler);

    // on new authentication update my user id and display name
    connect(&auth, SIGNAL(authenticated()), &networkManager, SLOT(getMe()));
    connect(&auth, SIGNAL(authenticated()), &networkManager,
            SLOT(getMyUserStatistics()));
    // initialize auth in a separate thread
    QTimer::singleShot(0, [&]() { auth.init(); });

    showStartView();
}

void CavokeClientController::defaultSettingsInitialization() {
    settings.setValue(PLAYER_NICKNAME,
                      settings.value(PLAYER_NICKNAME, DEFAULT_NICKNAME));
    settings.setValue(
        NETWORK_HOST,
        settings.value(NETWORK_HOST, NetworkManager::DEFAULT_HOST));

    networkManager.changeHost(
        QUrl::fromUserInput(settings.value(NETWORK_HOST).toString()));

    if (cavoke::auth::AuthenticationManager::getInstance().checkAuthStatus()) {
        networkManager.getMe();
    }

    emit initSettingsValues(settings.value(PLAYER_NICKNAME).toString(),
                            settings.value(NETWORK_HOST).toString());
}

void CavokeClientController::showTestWindowView() {
    testWindowView.show();
}

void CavokeClientController::leftSession(bool real_leave) {
    if (real_leave) {
        networkManager.leaveSession();
    }
    networkManager.stopGamePolling();
    networkManager.stopSessionPolling();
    networkManager.stopValidationPolling();
    qmlDownloadStatus = QMLDownloadStatus::NOT_STARTED;
    hostGuestStatus = HostGuestStatus::NOT_IN;
    currentGameInfo = GameInfo();
    currentSessionInfo = SessionInfo();
}

void CavokeClientController::leftRoom() {
    networkManager.stopRoomPolling();
    networkManager.leaveRoom();
    roomHostGuestStatus = HostGuestStatus::NOT_IN;
    currentRoomInfo = RoomInfo();
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

void CavokeClientController::showRoomView() {
    displacement = UserDisplacement::ROOM;
    roomView.show();
}

void CavokeClientController::showSessionView() {
    displacement = UserDisplacement::SESSION;
    sessionView.show();
}

void CavokeClientController::showStatisticsView() {
    statisticsView.requestUpdates();
    statisticsView.show();
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
    roomView.close();
    sessionView.close();
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
    displacement = UserDisplacement::GAME;
    networkManager.stopSessionPolling();
    networkManager.stopValidationPolling();
    networkManager.stopRoomPolling();
    startQmlByGameId(currentGameInfo.id);
}

void CavokeClientController::stopQml() {
    networkManager.stopGamePolling();
    networkManager.startRoomPolling();
    leftSession(false); // Without actual `leave` request, due to issue #171
    showRoomView();
    currentQmlGameModel->deleteLater();
}

void CavokeClientController::unpackDownloadedQml(QFile *file,
                                                 const QString &gameId) {
    cache_manager::save_zip_to_cache(file, gameId);
    qDebug() << "UnpackDownloadedQml Finished";
    file->deleteLater();

    qmlDownloadStatus = QMLDownloadStatus::DOWNLOADED;
}

void CavokeClientController::createGameStart(const QString &roomName) {
    qDebug() << "Now we are creating room with name:" << roomName;

    createGameView.close();
    showRoomView();
    networkManager.createRoom(roomName);
}

void CavokeClientController::joinGameStart(const QString &inviteCode) {
    qDebug() << "Now we are joining room with inviteCode:" << inviteCode;

    joinGameView.close();
    showRoomView();
    networkManager.joinRoom(inviteCode);
}

void CavokeClientController::createSessionStart(const QString &gameId) {
    qDebug() << "Now we are creating session with gameId:" << gameId;

    roomView.close();
    showSessionView();
    networkManager.roomCreateSession(gameId);
}

void CavokeClientController::joinSessionStart(const QString &sessionId) {
    qDebug() << "Now we are joining session with sessionId:" << sessionId;

    roomView.close();
    showSessionView();
    networkManager.joinSession(sessionId);
}

void CavokeClientController::gotRoomInfo(const RoomInfo &roomInfo) {
    qDebug() << "Got room info";

    if (displacement != UserDisplacement::ROOM) {
        return;  // We are not in a room, actually
    }

    currentRoomInfo = roomInfo;

    if (roomHostGuestStatus == HostGuestStatus::NOT_IN) {
        networkManager.startRoomPolling();
    }

    if (currentRoomInfo.isHost &&
        roomHostGuestStatus != HostGuestStatus::HOST) {
        becomeRoomHost();
    } else if (!currentRoomInfo.isHost &&
               roomHostGuestStatus != HostGuestStatus::GUEST) {
        becomeRoomGuest();
    }

    roomView.updateRoomInfo(currentRoomInfo);
}

void CavokeClientController::gotSessionInfo(const SessionInfo &sessionInfo) {
    qDebug() << "Now we got session info";

    if (displacement == UserDisplacement::ROOM) {
        roomView.updateSessionInfo(sessionInfo);
        return;
    }

    if (displacement != UserDisplacement::SESSION) {
        return;  // We are not in a session, actually
    }

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
        sessionView.updateStatus(SessionView::CreatingGameStatus::DOWNLOAD);
    }

    sessionView.updateSessionInfo(currentSessionInfo);

    if (currentGameInfo.players_num == 0) {
        networkManager.getGamesConfig(currentSessionInfo.game_id);
    } else if (qmlDownloadStatus == QMLDownloadStatus::DOWNLOADED) {
        sessionView.updateStatus(SessionView::CreatingGameStatus::DONE);
        collectListOfAvailableRoles();
    }
}

void CavokeClientController::gotCurrentGameInfo(const GameInfo &gameInfo) {
    currentGameInfo = gameInfo;
    emit setGameName(currentGameInfo.display_name);
}

void CavokeClientController::updateSettings(const QString &displayName,
                                            const QString &host) {
    settings.setValue(PLAYER_NICKNAME, displayName);
    settings.setValue(NETWORK_HOST, host);
    emit clearScreens();
    networkManager.changeHost(QUrl::fromUserInput(host));
    networkManager.changeName(displayName);
}
void CavokeClientController::collectListOfAvailableRoles() {
    std::vector<bool> isFree(currentGameInfo.players_num, true);
    int ourRole = -1;
    QString userId = networkManager.getUserId();
    for (const auto &player : currentSessionInfo.players) {
        isFree[player.player_id] = false;
        if (player.user.user_id == userId) {
            ourRole = player.player_id;
        }
    }
    if (ourRole == -1) {
        return;
        // I guess it is the bug when we have already left the session but
        //            still
        // somehow made a request to get info about session
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

void CavokeClientController::becomeRoomHost() {
    roomHostGuestStatus = HostGuestStatus::HOST;
}

void CavokeClientController::becomeRoomGuest() {
    roomHostGuestStatus = HostGuestStatus::GUEST;
}
