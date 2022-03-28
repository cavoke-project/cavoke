#ifndef CAVOKE_CLIENT_NETWORK_MANAGER_H
#define CAVOKE_CLIENT_NETWORK_MANAGER_H

#include <QUrlQuery>
#include <QtCore/QFile>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QTemporaryFile>
#include <QtCore/QTimer>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include "entities/sessioninfo.h"
#include "entities/validationresult.h"

#include "OAIDefaultApi.h"

struct NetworkManager : public QObject {
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = nullptr);

public slots:
    void getHealth();
    void getGamesList();
    void getGamesConfig(const QString &gameId);
    void getGamesClient(const QString &gameId);
    //    void createRoom();        TODO
    //    void joinRoom();          TODO
    //    void getRoomsInfo();      TODO
    //    void roomCreateSession(); TODO
    void sendMove(const QString &jsonMove);
    void getPlayState();
    void createSession(const QString &gameId);
    void joinSession(const QString &inviteCode);
    void validateSession();
    void getSessionInfo();
    void startSession();
    // TODO: Not implemented
    //    void getSessionInfoByInviteCode(const QString &inviteCode);

    void startGamePolling();
    void stopGamePolling();
    void startSessionPolling();
    void stopSessionPolling();
    void startValidationPolling();
    void stopValidationPolling();

signals:
    void finalizedGamesList(QJsonArray list);
    void gotGameUpdate(const QString &jsonField);
    void downloadedGameFile(QFile *file, const QString &gameId);
    void gotSessionInfo(const SessionInfo &sessionInfo);
    void gotValidationResult(const ValidationResult &validationResult);

private slots:
    void gotHealth(QNetworkReply *reply);
    void gotGamesList(QNetworkReply *reply);
    void gotGamesConfig(QNetworkReply *reply);
    void gotSession(QNetworkReply *reply);
    void gotValidatedSession(QNetworkReply *reply);
    void gotPostResponse(QNetworkReply *reply);
    void gotPlayState(QNetworkReply *reply);
    void gotGamesClient(QNetworkReply *reply, const QString &gameId);

private:
    QNetworkAccessManager manager;
    QTimer *gamePollingTimer = nullptr;
    QTimer *sessionPollingTimer = nullptr;
    QTimer *validationPollingTimer = nullptr;
    QString sessionId;
    QUuid userId;
    const static inline QUrl HOST{
#ifdef MOCK
        "https://764bbfca-c45a-46fc-9c79-11d9094b9ba8.mock.pstmn.io/"
#else
#ifdef REAL
        "http://34.88.243.202:8081/"
#else
        "http://127.0.0.1:8080/"
#endif
#endif
    };
    const static inline QUrl HEALTH{"health"};  // FIXME: move to routes module
    const static inline QUrl GAMES_LIST{"games/list"};
    const static inline QUrl GAMES{"games/"};
    const static inline QUrl GET_CONFIG{"get_config"};
    const static inline QUrl GET_CLIENT{"get_client"};
    // FIXME: the next block is already in the api, but is not implemented by
    // either the server or the client
    const static inline QUrl ROOMS_CREATE{"rooms/create"};      // TODO
    const static inline QUrl ROOMS_JOIN{"rooms/join"};          // TODO
    const static inline QUrl ROOMS{"rooms/"};                   // TODO
    const static inline QUrl GET_INFO{"get_info"};              // TODO
    const static inline QUrl CREATE_SESSION{"create_session"};  // TODO
    // End of not implemented block
    // FIXME: the next block will be deprecated in future:
    const static inline QUrl SESSIONS_CREATE{"sessions/create"};
    const static inline QUrl SESSIONS_JOIN{"sessions/join"};
    // End of future deprecated block
    const static inline QUrl PLAY{"play/"};
    const static inline QUrl SEND_MOVE{"send_move"};
    const static inline QUrl GET_STATE{"get_state"};
    // FIXME: the next block will be possibly deprecated in future:
    const static inline QUrl SESSIONS{"sessions/"};
    const static inline QUrl VALIDATE{"validate"};
    //    const static inline QUrl GET_INFO{"get_info"}; // already exists in
    //    rooms block
    const static inline QUrl START{"start"};
    // TODO: Not implemented
    const static inline QUrl SESSIONS_GET_INFO_BY_INVITE_CODE{
        "sessions/get_info_by_invite_code"};
};

#endif  // CAVOKE_CLIENT_NETWORK_MANAGER_H
