#ifndef CAVOKECLIENTCONTROLLER_H
#define CAVOKECLIENTCONTROLLER_H

#include <QObject>
#include <QtQuick>
#include "cache_manager.h"
#include "cavokeclientmodel.h"
#include "network_manager.h"
#include "views/creategameview.h"
#include "views/gameslistview.h"
#include "views/joingameview.h"
#include "views/roomview.h"
#include "views/sessionview.h"
#include "views/settingsview.h"
#include "views/startview.h"
#include "views/statisticsview.h"
#include "views/testwindowview.h"

class CavokeClientController : public QObject {
    enum class UserDisplacement { NOWHERE, ROOM, SESSION, GAME };
    enum class QMLDownloadStatus { NOT_STARTED, DOWNLOADING, DOWNLOADED };
    enum class HostGuestStatus { NOT_IN, HOST, GUEST };
    Q_OBJECT
public:
    explicit CavokeClientController(QObject *parent = nullptr);

public slots:
    void showTestWindowView();
    void showStartView();
    void showJoinGameView();
    void showGamesListView();
    void showCreateGameView();
    void showStatisticsView();
    void showSettingsView();
    void showRoomView();
    void showSessionView();
    void updateSettings(const QString &displayName, const QString &host);

signals:
    void loadGamesList();
    void setGameName(const QString &gameName);
    void initSettingsValues(const QString &nickname, const QString &host);
    void clearScreens();
    void createdAvailableRolesList(const std::vector<Role> &availableRoles);

private slots:
    void startQmlApplication(CavokeQmlGameModel *);
    void exitApplication();
    void startQmlByGameId(const QString &gameId);
    void startLoadedQml();
    void stopQml();
    void unpackDownloadedQml(QFile *file, const QString &gameId);
    void createGameStart(const QString &roomName);
    void joinGameStart(const QString &inviteCode);
    void createSessionStart(const QString &gameId);
    void joinSessionStart(const QString &sessionId);
    void gotCurrentGameInfo(const GameInfo &gameInfo);
    void gotRoomInfo(const RoomInfo &roomInfo);
    void gotSessionInfo(const SessionInfo &sessionInfo);
    void collectListOfAvailableRoles();
    void becomeHost();
    void becomeGuest();
    void becomeRoomHost();
    void becomeRoomGuest();
    void leftSession(bool real_leave = true);
    void leftRoom();

private:
    void defaultSettingsInitialization();
    const static inline QString PLAYER_NICKNAME{"player/nickname"};
    const static inline QString NETWORK_HOST{"network/host"};
    const static inline QString DEFAULT_NICKNAME{"Guest"};
    NetworkManager networkManager;
    CavokeClientModel model;
    TestWindowView testWindowView;
    StartView startView;
    JoinGameView joinGameView;
    CreateGameView createGameView;
    GamesListView gamesListView;
    StatisticsView statisticsView;
    SettingsView settingsView;
    RoomView roomView;
    SessionView sessionView;
    CavokeQmlGameModel *currentQmlGameModel = nullptr;
    RoomInfo currentRoomInfo;
    SessionInfo currentSessionInfo;
    GameInfo currentGameInfo;
    UserDisplacement displacement = UserDisplacement::NOWHERE;
    QMLDownloadStatus qmlDownloadStatus = QMLDownloadStatus::NOT_STARTED;
    HostGuestStatus hostGuestStatus = HostGuestStatus::NOT_IN;
    HostGuestStatus roomHostGuestStatus = HostGuestStatus::NOT_IN;
    QSettings settings;
};

#endif  // CAVOKECLIENTCONTROLLER_H
