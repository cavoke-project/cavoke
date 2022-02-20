#ifndef CAVOKE_CLIENT_NETWORK_MANAGER_H
#define CAVOKE_CLIENT_NETWORK_MANAGER_H

#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
struct NetworkManager : public QObject {
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = nullptr);

public slots:
    void doTestHealthCheck();
    void getGamesList();

signals:
    void finalizedGamesList(QJsonArray list);

private slots:
    void doneTestHealthCheck(QNetworkReply *reply);
    void gotGamesList(QNetworkReply *reply);

private:
    QNetworkAccessManager manager;
    const static inline QUrl HOST{
#ifdef MOCK
        "https://764bbfca-c45a-46fc-9c79-11d9094b9ba8.mock.pstmn.io/"
#else
        "http://127.0.0.1:8080/"
#endif
    };
    const static inline QUrl HEALTH{"/health"};  // FIXME: move to routes module
    const static inline QUrl GAMES_LIST{"/games/list"};
};

#endif  // CAVOKE_CLIENT_NETWORK_MANAGER_H
