#include "network_manager.h"
#include <QtCore/QJsonDocument>
NetworkManager::NetworkManager(QObject *parent) : manager(parent) {
}
void NetworkManager::doTestHealthCheck() {
    auto reply = manager.get(QNetworkRequest(
        QUrl("https://private-e45526-cavoke.apiary-mock.com/health")));
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
    auto reply = manager.get(QNetworkRequest(
        QUrl("https://764bbfca-c45a-46fc-9c79-11d9094b9ba8.mock.pstmn.io/games/list")));
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
