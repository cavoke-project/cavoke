#ifndef CAVOKE_CLIENT_NETWORK_MANAGER_H
#define CAVOKE_CLIENT_NETWORK_MANAGER_H

#include <QUrlQuery>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QTimer>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QFile>
#include <QtCore/QTemporaryFile>
#include "sessioninfo.h"
struct NetworkManager : public QObject {
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = nullptr);

public slots:
    void getHealth();
    void getGamesList();
    void getGamesConfig(const QString &gameId);
    void getGamesClient(const QString &gameId);
    void createSession(const QString &gameId);
    void joinSession(const QString &inviteCode);
    void sendMove(const QString &jsonMove);
    void getUpdate();
    
    void startPolling();
    void stopPolling();

signals:
    void finalizedGamesList(QJsonArray list);
    void gotGameUpdate(const QString &jsonField);
    void downloadedGameFile(QFile *file, const QString &app_name);
    void gotSessionInfo(const QString &inviteCode);
    void gotJoinedSessionInfo(const QString &appName);

private slots:
    void gotHealth(QNetworkReply *reply);
    void gotGamesList(QNetworkReply *reply);
    void gotGamesConfig(QNetworkReply *reply);
    void gotSessionCreated(QNetworkReply *reply);
    void gotSessionJoined(QNetworkReply *reply);
    void gotPostResponse(QNetworkReply *reply);
    void gotUpdate(QNetworkReply *reply);
    void gotGamesClient(QNetworkReply *reply, const QString &app_name);

private:
    QNetworkAccessManager manager;
    QTimer *pollingTimer = nullptr;
    QString sessionId;
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
    const static inline QUrl GAMES{"games/"};
    const static inline QUrl GET_CONFIG{"get_config"};
    const static inline QUrl GET_CLIENT{"get_client"};
    const static inline QUrl SESSIONS_CREATE{"sessions/create"};
    const static inline QUrl SESSIONS_JOIN{"sessions/join"};
    const static inline QUrl PLAY{"play/"};
    const static inline QUrl SEND_MOVE{"send_move"};
    const static inline QUrl GET_STATE{"get_state"};
};

#endif  // CAVOKE_CLIENT_NETWORK_MANAGER_H
