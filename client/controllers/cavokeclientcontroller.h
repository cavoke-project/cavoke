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

signals:
    void loadGamesList();
    void createGameDownloaded();
    void joinGameDownloaded();

private slots:
    void startQmlApplication(CavokeQmlGameModel *);
    void exitApplication();
    void startQmlByName(const QString &name);
    void stopQml();
    void unpackDownloadedQml(QFile *file, const QString &app_name);
    void createGameDownload(int gameIndex);
    void createGameSendRequest();
    void createGameShowProtoRoomView(const QString &inviteCode);
    void joinGameRequest(const QString &inviteCode);
    void joinGameDownload(const QString &app_name);
    void joinGameShowProtoRoomView();

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
    bool isCreatingSession = false; // FIXME: oh no, flags
    bool isJoiningSession = false; // FIXME: oh no, flags
    QString creatingGameId;
};

#endif  // CAVOKECLIENTCONTROLLER_H
