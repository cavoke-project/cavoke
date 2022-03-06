#include "network_manager.h"

NetworkManager::NetworkManager(QObject *parent) : manager(parent) {
    pollingTimer = new QTimer(this);
    pollingTimer->setInterval(500);
    pollingTimer->callOnTimeout([this]() { getUpdate(); });
}
void NetworkManager::doTestHealthCheck() {
    auto reply = manager.get(QNetworkRequest(HOST.resolved(HEALTH)));
    connect(reply, &QNetworkReply::finished, this,
            [reply, this]() { doneTestHealthCheck(reply); });
}

void NetworkManager::doneTestHealthCheck(QNetworkReply *reply) {
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

void NetworkManager::sendMove(const QString &jsonMove) {
    QUrl route = HOST.resolved(PLAY)
                     .resolved(sessionId.toString(QUuid::WithoutBraces) + "/")
                     .resolved(SEND_MOVE);
    route.setQuery({{"user_id", userId.toString(QUuid::WithoutBraces)}});
    qDebug() << route.toString();
    auto request = QNetworkRequest(route);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader,
                      "text/plain");
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
    QUrl route = HOST.resolved(PLAY)
                     .resolved(sessionId.toString(QUuid::WithoutBraces) + "/")
                     .resolved(GET_UPDATE);
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
    sessionId = QUuid::createUuid();
    userId = QUuid::createUuid();
    pollingTimer->start();
}

void NetworkManager::stopPolling() {
    pollingTimer->stop();
}

void NetworkManager::downloadGame(const QString &gameId) {
    QUrl route = HOST.resolved(GAMES)
        .resolved(gameId + "/")
        .resolved(GET_CLIENT);
    qDebug() << route.toString();
    auto request = QNetworkRequest(route);
    auto reply = manager.get(request);
    connect(reply, &QNetworkReply::finished, this,
            [reply, this]() { gotGameDownloaded(reply); });
}

void NetworkManager::gotGameDownloaded(QNetworkReply *reply) {
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
    emit downloadedGameFile(file);
    // End of the part
    reply->close();
    reply->deleteLater();
    
}
