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
#include "protoroomview.h"
#include "settingsview.h"
#include "startview.h"
#include "testwindowview.h"

class CavokeClientController : public QObject {
    enum class CreateJoinControllerStatus {
        NOTHING,
        CREATING,
        JOINING,
        POLLING_CREATE,
        POLLING_JOIN
    };
    Q_OBJECT
public:
    explicit CavokeClientController(QObject *parent = nullptr);

public slots:
    void showTestWindowView();
    void showStartView();
    void showJoinGameView();
    void showGamesListView();
    void showCreateGameView();
    void showSettingsView();
    void updateSettings(const QString &nickname, const QString &host);

signals:
    void loadGamesList();
    void createGameDownloaded();
    void joinGameDownloaded();
    void initSettingsValues(const QString &nickname, const QString &host);

private slots:
    void startQmlApplication(CavokeQmlGameModel *);
    void exitApplication();
    void startQmlByGameId(const QString &gameId);
    void startLoadedQml();
    void stopQml();
    void unpackDownloadedQml(QFile *file, const QString &gameId);
    void createGameStart(int gameIndex);
    void joinGameStart(const QString &inviteCode);
    void downloadCurrentGame();
    void creatingJoiningGameDone();
    void createGameSendRequest();
    void gotSessionInfo(const SessionInfo &sessionInfo);

private:
    NetworkManager networkManager;
    CavokeClientModel model;
    TestWindowView testWindowView;
    StartView startView;
    JoinGameView joinGameView;
    CreateGameView createGameView;
    GamesListView gamesListView;
    SettingsView settingsView;
    ProtoRoomView protoRoomView;
    CavokeQmlGameModel *currentQmlGameModel = nullptr;
    CreateJoinControllerStatus status = CreateJoinControllerStatus::NOTHING;
    QString currentGameId;
    QSettings settings;
};

#endif  // CAVOKECLIENTCONTROLLER_H
