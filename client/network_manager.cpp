#include "network_manager.h"
NetworkManager::NetworkManager(QObject *parent) : manager(parent) {
    connect(&manager, SIGNAL(finished(QNetworkReply *)), this,
            SLOT(doneTestHealthCheck(QNetworkReply *)));
}
void NetworkManager::doTestHealthCheck() {
    manager.get(QNetworkRequest(
        QUrl("https://private-e45526-cavoke.apiary-mock.com/health")));
}

void NetworkManager::doneTestHealthCheck(QNetworkReply *reply) {
    QString answer = reply->readAll();
    reply->close();
    reply->deleteLater();
    qDebug() << answer;
}
