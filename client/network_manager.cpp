#include "network_manager.h"
NetworkManager::NetworkManager(QObject *parent) : manager(parent) {
}
void NetworkManager::doTestHealthCheck() {
    connect(&manager, SIGNAL(finished(QNetworkReply *)), this,
            SLOT(doneTestHealthCheck(QNetworkReply *)),
            Qt::SingleShotConnection);  // TODO: This is Qt6 feature only!

    manager.get(QNetworkRequest(
        QUrl("https://private-e45526-cavoke.apiary-mock.com/health")));
}

void NetworkManager::doneTestHealthCheck(QNetworkReply *reply) {
    QString answer = reply->readAll();
    reply->close();
    qDebug() << "Entered here";
    reply->deleteLater();
    qDebug() << answer;
}
