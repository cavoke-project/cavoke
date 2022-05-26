#ifndef CAVOKE_CLIENT_NETWORK_MANAGER_H
#define CAVOKE_CLIENT_NETWORK_MANAGER_H

#include <QOAuth2AuthorizationCodeFlow>
#include <QUrlQuery>
#include <QUuid>
#include <QtCore/QFile>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QTemporaryFile>
#include <QtCore/QTimer>
#include <QtNetwork/QNetworkReply>
#include "AuthenticationManager.h"
#include "entities/gameinfo.h"
#include "entities/gamestatistics.h"
#include "entities/sessioninfo.h"
#include "entities/usergamestatistics.h"
#include "entities/userstatistics.h"
#include "entities/validationresult.h"

struct NetworkManager : public QObject {
    Q_OBJECT
public:
    const static inline QUrl DEFAULT_HOST{"https://develop.api.cavoke.wlko.me"};
    explicit NetworkManager(QObject *parent = nullptr);
    void changeHost(const QUrl &newHost);
    QString getUserId();

public slots:
    void getHealth();

    void getGamesList();
    void getGamesConfig(const QString &gameId);
    void getGamesClient(const QString &gameId);

    void sendMove(const QString &jsonMove);
    void getPlayState();

//    void createSession(const QString &gameId);    // Outdated
    void joinSession(const QString &inviteCode);
    void validateSession();
    void getSessionInfo();
    void startSession();
    void leaveSession();
    void changeRoleInSession(int newRole);

    void getRoomsInfo();
    void createRoom();
    void joinRoom();
    void leaveRoom();
    void roomCreateSession();

    void getMe();
    void changeName(const QString &new_name);
    void getMyUserStatistics();
    void getMyUserGameStatistics(const QString &gameId);
    void getGameStatistics(const QString &gameId);

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
    void gotGameInfo(const GameInfo &gameInfo);
    void gotSessionInfo(const SessionInfo &sessionInfo);
    void gotValidationResult(const ValidationResult &validationResult);
    void gotDisplayName(const QString &displayName);
    void gotUserStatistics(const UserStatistics &userStatistics);
    void gotUserGameStatistics(const UserGameStatistics &userGameStatistics);
    void gotGameStatistics(const GameStatistics &gameStatistics);

private slots:
    void gotHealth(QNetworkReply *reply);
    void gotGamesList(QNetworkReply *reply);
    void gotGamesConfig(QNetworkReply *reply);
    void gotSession(QNetworkReply *reply);
    void gotValidatedSession(QNetworkReply *reply);
    void gotPostResponse(QNetworkReply *reply);
    void gotPlayState(QNetworkReply *reply);
    void gotGamesClient(QNetworkReply *reply, const QString &gameId);
    void gotMyself(QNetworkReply *reply);
    void gotUserStatistics(QNetworkReply *reply);
    void gotUserGameStatistics(QNetworkReply *reply);
    void gotGameStatistics(QNetworkReply *reply);

private:
    QOAuth2AuthorizationCodeFlow *oauth2;
    QNetworkAccessManager manager;
    QTimer *gamePollingTimer = nullptr;
    QTimer *sessionPollingTimer = nullptr;
    QTimer *validationPollingTimer = nullptr;
    QString sessionId;
    /// `user_id` query param.
    /// For local servers (without jwt) is generated randomly through QUuid.
    /// For prod servers (with jwt) is acquired from the server (through
    /// `get_me`).
    QString queryUserId;
    QUrl HOST{DEFAULT_HOST};
    const static inline QUrl HEALTH{"health"};  // FIXME: move to routes module
    const static inline QUrl GAMES_LIST{"games/list"};
    const static inline QUrl GAMES{"games/"};
    const static inline QUrl GET_CONFIG{"get_config"};
    const static inline QUrl GET_CLIENT{"get_client"};
    // Rooms activity
    const static inline QUrl ROOMS{"rooms/"};                   // TODO
    const static inline QUrl GET_INFO{"get_info"};              // TODO
    const static inline QUrl ROOMS_CREATE{"rooms/create"};      // TODO
    const static inline QUrl ROOMS_JOIN{"rooms/join"};          // TODO
    const static inline QUrl LEAVE{"leave"};
    const static inline QUrl CREATE_SESSION{"create_session"};  // TODO
    // End of not implemented block
    // FIXME: the next block will be deprecated in future:
//    const static inline QUrl SESSIONS_CREATE{"sessions/create"};
    const static inline QUrl SESSIONS_JOIN{"sessions/join"};
    const static inline QUrl SESSIONS{"sessions/"};
    const static inline QUrl VALIDATE{"validate"};
    const static inline QUrl START{"start"};
    const static inline QUrl CHANGE_ROLE{"change_role"};
    // End of future deprecated block
    const static inline QUrl PLAY{"play/"};
    const static inline QUrl SEND_MOVE{"send_move"};
    const static inline QUrl GET_STATE{"get_state"};

    const static inline QUrl PROFILE{"profile/"};
    const static inline QUrl GET_ME{"get_me"};
    const static inline QUrl CHANGE_NAME{"change_name"};
    const static inline QUrl MY_USER_STATISTICS{"my_user_statistics"};
    const static inline QUrl MY_USER_GAME_STATISTICS{
        "my_user_game_statistics/"};

    const static inline QUrl STATISTICS_GAME{"statistics/game/"};
};

#endif  // CAVOKE_CLIENT_NETWORK_MANAGER_H
