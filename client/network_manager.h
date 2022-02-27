#ifndef CAVOKE_CLIENT_NETWORK_MANAGER_H
#define CAVOKE_CLIENT_NETWORK_MANAGER_H

#include <QUrlQuery>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QTimer>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
struct NetworkManager : public QObject {
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = nullptr);

public slots:
    void doTestHealthCheck();
    void getGamesList();
    void sendMove(const QString &jsonMove);
    void getUpdate();
    void startPolling();
    void stopPolling();

signals:
    void finalizedGamesList(QJsonArray list);
    void gotGameUpdate(const QString &jsonField);

private slots:
    void doneTestHealthCheck(QNetworkReply *reply);
    void gotGamesList(QNetworkReply *reply);
    void gotPostResponse(QNetworkReply *reply);
    void gotUpdate(QNetworkReply *reply);

private:
    QNetworkAccessManager manager;
    QTimer *pollingTimer = nullptr;
    QUuid sessionId;
    QUuid userId;
    const static inline QUrl HOST{
#ifdef MOCK
        "https://764bbfca-c45a-46fc-9c79-11d9094b9ba8.mock.pstmn.io/"
#else
        "http://127.0.0.1:8080/"
#endif
    };
    const static inline QUrl HEALTH{"health"};  // FIXME: move to routes module
    const static inline QUrl GAMES_LIST{"games/list"};
    const static inline QUrl PLAY{"play/"};
    const static inline QUrl SEND_MOVE{"send_move"};
    const static inline QUrl GET_UPDATE{"get_update"};
};

#endif  // CAVOKE_CLIENT_NETWORK_MANAGER_H
