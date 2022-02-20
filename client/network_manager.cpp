#include "network_manager.h"
NetworkManager::NetworkManager(QObject *parent) : manager(parent) {
}
void NetworkManager::doTestHealthCheck() {
    auto reply = manager.get(QNetworkRequest(QUrl(HOST + "health")));
    //        QUrl("https://private-e45526-cavoke.apiary-mock.com/health")));
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
    auto reply = manager.get(QNetworkRequest(QUrl(HOST + "games/list")));
    //        QUrl("https://private-e45526-cavoke.apiary-mock.com/games/list")));
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
