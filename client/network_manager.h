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
#include "entities/roominfo.h"
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

    void joinSession(const QString &sessionId);
    void validateSession();
    void getSessionInfo();
    void getSessionInfo(const QString &sessionId);
    void startSession();
    void leaveSession();
    void changeRoleInSession(int newRole);

    void getRoomInfo();
    void createRoom(const QString &display_name);
    void joinRoom(const QString &invite_code);
    void leaveRoom();
    void roomCreateSession(const QString &game_id);

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
    void startRoomPolling();
    void stopRoomPolling();

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
    void gotRoomInfo(const RoomInfo &roomInfo);

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
    void gotRoom(QNetworkReply *reply);

private:
    QOAuth2AuthorizationCodeFlow *oauth2;
    QNetworkAccessManager manager;
    QTimer *gamePollingTimer = nullptr;
    QTimer *sessionPollingTimer = nullptr;
    QTimer *validationPollingTimer = nullptr;
    QTimer *roomPollingTimer = nullptr;
    QString sessionId;
    QString roomId;
    /// `user_id` query param.
    /// For local servers (without jwt) is generated randomly through QUuid.
    /// For prod servers (with jwt) is acquired from the server (through
    /// `get_me`).
    QString queryUserId;
    QUrl HOST{DEFAULT_HOST};
    const static inline QUrl HEALTH{"health"};
    const static inline QUrl GAMES_LIST{"games/list"};
    const static inline QUrl GAMES{"games/"};
    const static inline QUrl GET_CONFIG{"get_config"};
    const static inline QUrl GET_CLIENT{"get_client"};
    // Rooms activity
    const static inline QUrl ROOMS{"rooms/"};
    const static inline QUrl GET_INFO{"get_info"};
    const static inline QUrl ROOMS_CREATE{"rooms/create"};
    const static inline QUrl ROOMS_JOIN{"rooms/join"};
    const static inline QUrl LEAVE{"leave"};
    const static inline QUrl CREATE_SESSION{"create_session"};

    const static inline QUrl JOIN{"join"};
    const static inline QUrl SESSIONS{"sessions/"};
    const static inline QUrl VALIDATE{"validate"};
    const static inline QUrl START{"start"};
    const static inline QUrl CHANGE_ROLE{"change_role"};

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
