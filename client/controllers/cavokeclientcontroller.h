#ifndef CAVOKECLIENTCONTROLLER_H
#define CAVOKECLIENTCONTROLLER_H

#include <cache_manager.h>
#include <views/gameslistview.h>
#include <QObject>
#include <QtQuick>
#include "cavokeclientmodel.h"
#include "creategameview.h"
#include "joingameview.h"
#include "network_manager.h"
//#include "protoroomview.h"
#include "roomview.h"
#include "sessionview.h"
#include "settingsview.h"
#include "startview.h"
#include "statisticsview.h"
#include "testwindowview.h"

class CavokeClientController : public QObject {
    //    enum class CreateJoinControllerStatus {
    //        NOTHING,
    //        CREATING,
    //        JOINING,
    //        POLLING_CREATE,
    //        POLLING_JOIN
    //    };
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
    void gotCurrentGameInfo(const GameInfo &gameInfo);
    void gotSessionInfo(const SessionInfo &sessionInfo);
    void collectListOfAvailableRoles();
    void becomeHost();
    void becomeGuest();
    void leftSession();

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
//    ProtoRoomView protoRoomView;
    CavokeQmlGameModel *currentQmlGameModel = nullptr;
    SessionInfo currentSessionInfo;
    GameInfo currentGameInfo;
    QMLDownloadStatus qmlDownloadStatus = QMLDownloadStatus::NOT_STARTED;
    HostGuestStatus hostGuestStatus = HostGuestStatus::NOT_IN;
    QSettings settings;
};

#endif  // CAVOKECLIENTCONTROLLER_H
