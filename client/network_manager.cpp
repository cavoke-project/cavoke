#include "network_manager.h"

NetworkManager::NetworkManager(QObject *parent) : manager(parent) {
    pollingTimer = new QTimer(this);
    pollingTimer->setInterval(500);
    pollingTimer->callOnTimeout([this]() { getUpdate(); });
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
    qDebug() << "Got Games Config:";
    qDebug() << reply->readAll();
    qDebug() << "Not implemented yet!!";
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
            [reply, this]() { gotSessionCreated(reply); });
}

void NetworkManager::gotSessionCreated(QNetworkReply *reply) {
    QByteArray answer = reply->readAll();
    reply->close();
    reply->deleteLater();
    qDebug() << answer;

    SessionInfo sessionInfo;
    sessionInfo.read(QJsonDocument::fromJson(answer).object());
    sessionId = sessionInfo.session_id;
    emit gotSessionInfo(sessionInfo.invite_code);
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
            [reply, this]() { gotSessionJoined(reply); });
}

void NetworkManager::gotSessionJoined(QNetworkReply *reply) {
    QByteArray answer = reply->readAll();
    reply->close();
    reply->deleteLater();
    qDebug() << answer;

    SessionInfo sessionInfo;
    sessionInfo.read(QJsonDocument::fromJson(answer).object());
    sessionId = sessionInfo.session_id;
    emit gotJoinedSessionInfo(sessionInfo.game_id);
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
    qDebug() << answer;
}

void NetworkManager::getUpdate() {
    QUrl route =
        HOST.resolved(PLAY).resolved(sessionId + "/").resolved(GET_STATE);
    route.setQuery({{"user_id", userId.toString(QUuid::WithoutBraces)}});
    qDebug() << route.toString();
    auto request = QNetworkRequest(route);
    auto reply = manager.get(request);
    connect(reply, &QNetworkReply::finished, this,
            [reply, this]() { gotUpdate(reply); });
}

void NetworkManager::gotUpdate(QNetworkReply *reply) {
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
void NetworkManager::startPolling() {
    //    sessionId = QUuid::createUuid();
    pollingTimer->start();
}

void NetworkManager::stopPolling() {
    pollingTimer->stop();
}
