#include "network_manager.h"
NetworkManager::NetworkManager(QObject *parent) : manager(parent) {
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
//    QUrl route = HOST.resolved(PLAY);
//    qDebug() << route.toString();
//    route = route.resolved(QUrl(sessionId));
//    qDebug() << route.toString();
//    route = route.resolved(SEND_MOVE);
//    qDebug() << route.toString();
    QUrl route = HOST.resolved(PLAY.toString() + "/" + sessionId.toString() + SEND_MOVE.toString());
    route.setQuery(QUrlQuery({QPair<QString, QString>(
        "user_id", userId.toString())}));  // FIXME: I hope there exists a better way
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
//    QUrl route = HOST.resolved(PLAY);
//    route = route.resolved(QUrl(sessionId));
//    route = route.resolved(GET_UPDATE); // ???
    QUrl route = HOST.resolved(PLAY.toString() + "/" + sessionId.toString() + GET_UPDATE.toString());
    route.setQuery(QUrlQuery({QPair<QString, QString>("user_id", userId.toString())}));
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
    pollingTimer = new QTimer(this);
    pollingTimer->setInterval(500);
    pollingTimer->callOnTimeout([this]() {
      getUpdate();
    });
    pollingTimer->start();
}

void NetworkManager::stopPolling() {
    pollingTimer->stop();
}
