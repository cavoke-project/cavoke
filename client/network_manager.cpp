#include "network_manager.h"

NetworkManager::NetworkManager(QObject *parent)
    : manager{this},
      oauth2{&cavoke::auth::AuthenticationManager::getInstance().oauth2} {
    gamePollingTimer = new QTimer(this);
    gamePollingTimer->setInterval(500);
    gamePollingTimer->callOnTimeout([this]() { getPlayState(); });
    sessionPollingTimer = new QTimer(this);
    sessionPollingTimer->setInterval(500);
    sessionPollingTimer->callOnTimeout([this]() { getSessionInfo(); });
    validationPollingTimer = new QTimer(this);
    validationPollingTimer->setInterval(500);
    validationPollingTimer->callOnTimeout([this]() { validateSession(); });
    // generate randomly for local server mode
    queryUserId = QUuid::createUuid().toString(QUuid::WithoutBraces);

    oauth2->setNetworkAccessManager(&manager);
}

void NetworkManager::getHealth() {
    auto reply = oauth2->get(HOST.resolved(HEALTH));
    connect(reply, &QNetworkReply::finished, this,
            [reply, this]() { gotHealth(reply); });
}

void NetworkManager::gotHealth(QNetworkReply *reply) {
    QString answer = reply->readAll();
    reply->close();
    reply->deleteLater();
    qDebug() << answer;
}

void NetworkManager::getGamesList() {
    auto reply = oauth2->get(HOST.resolved(GAMES_LIST));
    connect(reply, &QNetworkReply::finished, this,
            [reply, this]() { gotGamesList(reply); });
}

void NetworkManager::gotGamesList(QNetworkReply *reply) {
    QByteArray answer = reply->readAll();
    reply->close();
    reply->deleteLater();
    qDebug() << answer;
    QJsonDocument response_wrapper = QJsonDocument::fromJson(answer);
    emit finalizedGamesList(response_wrapper.array());  // What is it?
}

void NetworkManager::getGamesConfig(const QString &gameId) {
    QUrl route =
        HOST.resolved(GAMES).resolved(gameId + "/").resolved(GET_CONFIG);
    route.setQuery({{"user_id", getUserId()}});
    auto reply = oauth2->get(route);
    connect(reply, &QNetworkReply::finished, this,
            [reply, this]() { gotGamesConfig(reply); });
}

void NetworkManager::gotGamesConfig(QNetworkReply *reply) {
    if (reply->error()) {
        qDebug() << reply->errorString();
        return;
    }
    QByteArray answer = reply->readAll();
    reply->close();
    reply->deleteLater();
    qDebug() << answer;
    GameInfo gameInfo;
    gameInfo.read(QJsonDocument::fromJson(answer).object());
    emit gotGameInfo(gameInfo);
}

void NetworkManager::getGamesClient(const QString &gameId) {
    QUrl route =
        HOST.resolved(GAMES).resolved(gameId + "/").resolved(GET_CLIENT);
    route.setQuery({{"user_id", getUserId()}});
    qDebug() << route.toString();
    auto reply = oauth2->get(route);
    connect(reply, &QNetworkReply::finished, this,
            [reply, gameId, this]() { gotGamesClient(reply, gameId); });
}

void NetworkManager::gotGamesClient(QNetworkReply *reply,
                                    const QString &gameId) {
    if (reply->error()) {
        qDebug() << reply->errorString();
        return;
    }
    // Probably next part should be in some other place
    QFile *file = new QTemporaryFile();
    if (file->open(QFile::WriteOnly)) {
        file->write(reply->readAll());
        file->flush();
        file->close();
    }
    emit downloadedGameFile(file, gameId);
    // End of the part
    reply->close();
    reply->deleteLater();
}

void NetworkManager::createSession(const QString &gameId) {
    QUrl route = HOST.resolved(SESSIONS_CREATE);
    route.setQuery({{"game_id", gameId}, {"user_id", getUserId()}});
    qDebug() << route.toString();
    auto reply = oauth2->post(route, "{}");
    connect(reply, &QNetworkReply::finished, this,
            [reply, this]() { gotSession(reply); });
}

void NetworkManager::joinSession(const QString &inviteCode) {
    QUrl route = HOST.resolved(SESSIONS_JOIN);
    route.setQuery({{"user_id", getUserId()}, {"invite_code", inviteCode}});
    qDebug() << route.toString();
    auto reply = oauth2->post(route, "{}");
    connect(reply, &QNetworkReply::finished, this,
            [reply, this]() { gotSession(reply); });
}

void NetworkManager::gotSession(QNetworkReply *reply) {
    QByteArray answer = reply->readAll();
    reply->close();
    reply->deleteLater();
    qDebug() << "Got session: " << answer;

    SessionInfo sessionInfo;
    sessionInfo.read(QJsonDocument::fromJson(answer).object());
    sessionId = sessionInfo.session_id;
    qDebug() << "My user_id is: " << queryUserId;
    sessionInfo.isHost =
        sessionInfo.host_id ==
        queryUserId;  // using explicitly query user_id, as in prod mode it is
                      // returned from the server

    emit gotSessionInfo(sessionInfo);
}

void NetworkManager::sendMove(const QString &jsonMove) {
    QUrl route =
        HOST.resolved(PLAY).resolved(sessionId + "/").resolved(SEND_MOVE);
    route.setQuery({{"user_id", getUserId()}});
    qDebug() << route.toString();
    auto reply = oauth2->post(route, jsonMove.toUtf8());
    connect(reply, &QNetworkReply::finished, this,
            [reply, this]() { gotPostResponse(reply); });
}

void NetworkManager::gotPostResponse(QNetworkReply *reply) {
    QByteArray answer = reply->readAll();
    reply->close();
    reply->deleteLater();
    qDebug() << "Got some post response: " << answer;
}

void NetworkManager::getPlayState() {
    QUrl route =
        HOST.resolved(PLAY).resolved(sessionId + "/").resolved(GET_STATE);
    route.setQuery({{"user_id", getUserId()}});
    qDebug() << route.toString();
    auto reply = oauth2->get(route);
    connect(reply, &QNetworkReply::finished, this,
            [reply, this]() { gotPlayState(reply); });
}

void NetworkManager::gotPlayState(QNetworkReply *reply) {
    if (reply->error()) {
        qDebug() << reply->errorString();
        return;
    }
    QString answer = reply->readAll();
    reply->close();
    reply->deleteLater();
    qDebug() << answer;
    emit gotGameUpdate(answer);
}

void NetworkManager::validateSession() {
    QUrl route =
        HOST.resolved(SESSIONS).resolved(sessionId + "/").resolved(VALIDATE);
    route.setQuery({{"user_id", getUserId()}});
    qDebug() << route.toString();
    auto reply = oauth2->post(route, "{}");
    connect(reply, &QNetworkReply::finished, this,
            [reply, this]() { gotValidatedSession(reply); });
}

void NetworkManager::gotValidatedSession(QNetworkReply *reply) {
    QByteArray answer = reply->readAll();
    reply->close();
    reply->deleteLater();
    //    qDebug() << answer;

    ValidationResult validationResult;
    validationResult.read(QJsonDocument::fromJson(answer).object());

    emit gotValidationResult(validationResult);
}

void NetworkManager::getSessionInfo() {
    QUrl route =
        HOST.resolved(SESSIONS).resolved(sessionId + "/").resolved(GET_INFO);
    route.setQuery({{"user_id", getUserId()}});
    qDebug() << route.toString();
    auto reply = oauth2->get(route);
    connect(reply, &QNetworkReply::finished, this,
            [reply, this]() { gotSession(reply); });
}

void NetworkManager::startSession() {
    QUrl route =
        HOST.resolved(SESSIONS).resolved(sessionId + "/").resolved(START);
    route.setQuery({{"user_id", getUserId()}});
    qDebug() << route.toString();
    auto reply = oauth2->post(route, "{}");
    connect(reply, &QNetworkReply::finished, this,
            [reply, this]() { gotPostResponse(reply); });
}

void NetworkManager::leaveSession() {
    QUrl route =
        HOST.resolved(SESSIONS).resolved(sessionId + "/").resolved(LEAVE);
    route.setQuery({{"user_id", getUserId()}});
    auto reply = oauth2->post(route, "{}");
    connect(reply, &QNetworkReply::finished, this,
            [reply, this]() { gotPostResponse(reply); });
}

void NetworkManager::changeRoleInSession(int newRole) {
    QUrl route =
        HOST.resolved(SESSIONS).resolved(sessionId + "/").resolved(CHANGE_ROLE);
    route.setQuery(
        {{"user_id", getUserId()}, {"new_role", QString::number(newRole)}});
    qDebug() << route.toString();
    auto reply = oauth2->post(route, "{}");
    connect(reply, &QNetworkReply::finished, this,
            [reply, this]() { gotPostResponse(reply); });
}

void NetworkManager::getMe() {
    QUrl route = HOST.resolved(PROFILE).resolved(GET_ME);
    route.setQuery({{"user_id", getUserId()}});
    qDebug() << route.toString();
    auto reply = oauth2->get(route);
    connect(reply, &QNetworkReply::finished, this,
            [reply, this]() { gotMyself(reply); });
}

void NetworkManager::gotMyself(QNetworkReply *reply) {
    QByteArray answer = reply->readAll();
    reply->close();
    reply->deleteLater();
    qDebug() << "Got my profile: " << answer;

    User userInfo;
    userInfo.read(QJsonDocument::fromJson(answer).object());
    // update my user id used in query params
    queryUserId = userInfo.user_id;
    emit gotDisplayName(userInfo.display_name);
}

void NetworkManager::changeName(const QString &new_name) {
    QUrl route = HOST.resolved(PROFILE).resolved(CHANGE_NAME);
    route.setQuery({{"user_id", getUserId()}, {"new_name", new_name}});
    qDebug() << route.toString();
    auto reply = oauth2->post(route);
    connect(reply, &QNetworkReply::finished, this,
            [reply, this]() { gotPostResponse(reply); });
}

void NetworkManager::getMyUserStatistics() {
    QUrl route = HOST.resolved(PROFILE).resolved(MY_USER_STATISTICS);
    route.setQuery({{"user_id", getUserId()}});
    qDebug() << route.toString();
    auto reply = oauth2->get(route);
    connect(reply, &QNetworkReply::finished, this,
            [reply, this]() { gotUserStatistics(reply); });
}

void NetworkManager::gotUserStatistics(QNetworkReply *reply) {
    QByteArray answer = reply->readAll();
    reply->close();
    reply->deleteLater();
    qDebug() << "Got my user statistics: " << answer;

    UserStatistics userStatistics;
    userStatistics.read(QJsonDocument::fromJson(answer).object());
    emit gotUserStatistics(userStatistics);
}

void NetworkManager::getMyUserGameStatistics(const QString &gameId) {
    QUrl route = HOST.resolved(PROFILE)
                     .resolved(MY_USER_GAME_STATISTICS)
                     .resolved(gameId);
    route.setQuery({{"user_id", getUserId()}});
    qDebug() << route.toString();
    auto reply = oauth2->get(route);
    connect(reply, &QNetworkReply::finished, this,
            [reply, this]() { gotUserGameStatistics(reply); });
}

void NetworkManager::gotUserGameStatistics(QNetworkReply *reply) {
    QByteArray answer = reply->readAll();
    reply->close();
    reply->deleteLater();
    qDebug() << "Got my user game statistics: " << answer;

    UserGameStatistics userGameStatistics;
    userGameStatistics.read(QJsonDocument::fromJson(answer).object());
    emit gotUserGameStatistics(userGameStatistics);
}

void NetworkManager::getGameStatistics(const QString &gameId) {
    QUrl route = HOST.resolved(STATISTICS_GAME).resolved(gameId);
    route.setQuery({{"user_id", getUserId()}});
    qDebug() << route.toString();
    auto reply = oauth2->get(route);
    connect(reply, &QNetworkReply::finished, this,
            [reply, this]() { gotGameStatistics(reply); });
}

void NetworkManager::gotGameStatistics(QNetworkReply *reply) {
    QByteArray answer = reply->readAll();
    reply->close();
    reply->deleteLater();
    qDebug() << "Got game statistics: " << answer;

    GameStatistics gameStatistics;
    gameStatistics.read(QJsonDocument::fromJson(answer).object());
    emit gotGameStatistics(gameStatistics);
}

void NetworkManager::startGamePolling() {
    gamePollingTimer->start();
}

void NetworkManager::stopGamePolling() {
    gamePollingTimer->stop();
}

void NetworkManager::startSessionPolling() {
    sessionPollingTimer->start();
}
void NetworkManager::stopSessionPolling() {
    sessionPollingTimer->stop();
}
void NetworkManager::startValidationPolling() {
    validationPollingTimer->start();
}

void NetworkManager::stopValidationPolling() {
    validationPollingTimer->stop();
}
void NetworkManager::changeHost(const QUrl &newHost) {
    HOST = newHost;
    getGamesList();
}
QString NetworkManager::getUserId() {
    return queryUserId;
}
