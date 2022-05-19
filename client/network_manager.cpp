#include "network_manager.h"
#include <utility>
#include "entities/gameinfo.h"

NetworkManager::NetworkManager(QObject *parent) : manager(parent) {
    gamePollingTimer = new QTimer(this);
    gamePollingTimer->setInterval(500);
    gamePollingTimer->callOnTimeout([this]() { getPlayState(); });
    sessionPollingTimer = new QTimer(this);
    sessionPollingTimer->setInterval(500);
    sessionPollingTimer->callOnTimeout([this]() { getSessionInfo(); });
    validationPollingTimer = new QTimer(this);
    validationPollingTimer->setInterval(500);
    validationPollingTimer->callOnTimeout([this]() { validateSession(); });
    userId = QUuid::createUuid();
}

void NetworkManager::getHealth() {
    auto reply = manager.get(QNetworkRequest(HOST.resolved(HEALTH)));
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
    auto reply = manager.get(QNetworkRequest(HOST.resolved(GAMES_LIST)));
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
    auto request = QNetworkRequest(route);
    auto reply = manager.get(request);
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
    qDebug() << route.toString();
    auto request = QNetworkRequest(route);
    auto reply = manager.get(request);
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
    route.setQuery({{"game_id", gameId},
                    {"user_id", userId.toString(QUuid::WithoutBraces)}});
    qDebug() << route.toString();
    auto request = QNetworkRequest(route);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader,
                      "application/json");
    auto reply = manager.post(request, "{}");
    connect(reply, &QNetworkReply::finished, this,
            [reply, this]() { gotSession(reply); });
}

void NetworkManager::joinSession(const QString &inviteCode) {
    QUrl route = HOST.resolved(SESSIONS_JOIN);
    route.setQuery({{"user_id", userId.toString(QUuid::WithoutBraces)},
                    {"invite_code", inviteCode}});
    qDebug() << route.toString();
    auto request = QNetworkRequest(route);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader,
                      "application/json");
    auto reply = manager.post(request, "{}");
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
    sessionInfo.isHost =
        sessionInfo.host_id == userId.toString(QUuid::WithoutBraces);

    emit gotSessionInfo(sessionInfo);
}

void NetworkManager::sendMove(const QString &jsonMove) {
    QUrl route =
        HOST.resolved(PLAY).resolved(sessionId + "/").resolved(SEND_MOVE);
    route.setQuery({{"user_id", userId.toString(QUuid::WithoutBraces)}});
    qDebug() << route.toString();
    auto request = QNetworkRequest(route);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader,
                      "application/json");
    auto reply = manager.post(request, jsonMove.toUtf8());
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
    route.setQuery({{"user_id", userId.toString(QUuid::WithoutBraces)}});
    qDebug() << route.toString();
    auto request = QNetworkRequest(route);
    auto reply = manager.get(request);
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
    route.setQuery({{"user_id", userId.toString(QUuid::WithoutBraces)}});
    qDebug() << route.toString();
    auto request = QNetworkRequest(route);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader,
                      "application/json");
    auto reply = manager.post(request, "{}");
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
    route.setQuery({{"user_id", userId.toString(QUuid::WithoutBraces)}});
    //    qDebug() << route.toString();
    auto request = QNetworkRequest(route);
    auto reply = manager.get(request);
    connect(reply, &QNetworkReply::finished, this,
            [reply, this]() { gotSession(reply); });
}

void NetworkManager::startSession() {
    QUrl route =
        HOST.resolved(SESSIONS).resolved(sessionId + "/").resolved(START);
    route.setQuery({{"user_id", userId.toString(QUuid::WithoutBraces)}});
    qDebug() << route.toString();
    auto request = QNetworkRequest(route);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader,
                      "application/json");
    auto reply = manager.post(request, "{}");
    connect(reply, &QNetworkReply::finished, this,
            [reply, this]() { gotPostResponse(reply); });
}

void NetworkManager::leaveSession() {
    QUrl route =
        HOST.resolved(SESSIONS).resolved(sessionId + "/").resolved(LEAVE);
    route.setQuery({{"user_id", userId.toString(QUuid::WithoutBraces)}});
    qDebug() << route.toString();
    auto request = QNetworkRequest(route);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader,
                      "application/json");
    auto reply = manager.post(request, "{}");
    connect(reply, &QNetworkReply::finished, this,
            [reply, this]() { gotPostResponse(reply); });
}

void NetworkManager::changeRoleInSession(int newRole) {
    QUrl route =
        HOST.resolved(SESSIONS).resolved(sessionId + "/").resolved(CHANGE_ROLE);
    route.setQuery({{"user_id", userId.toString(QUuid::WithoutBraces)},
                    {"new_role", QString::number(newRole)}});
    qDebug() << route.toString();
    auto request = QNetworkRequest(route);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader,
                      "application/json");
    auto reply = manager.post(request, "{}");
    connect(reply, &QNetworkReply::finished, this,
            [reply, this]() { gotPostResponse(reply); });
}

void NetworkManager::getMe() {
    QUrl route = HOST.resolved(PROFILE).resolved(GET_ME);
    // TODO: WILL BE DONE IN ANOTHER BRANCH
}

void NetworkManager::changeName(const QString &new_name) {
    // TODO: WILL BE DONE IN ANOTHER BRANCH
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
    return userId.toString(QUuid::WithoutBraces);
}
