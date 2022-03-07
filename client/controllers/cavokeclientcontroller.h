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

private slots:
    void startQmlApplication(CavokeQmlGameModel *);
    void exitApplication();
    void startQmlByPath(const QString &path);
    void stopQml();
    void unpackDownloadedQml(QFile *file, const QString &app_name);

private:
    NetworkManager networkManager;
    CavokeClientModel model;
    TestWindowView testWindowView;
    StartView startView;
    JoinGameView joinGameView;
    CreateGameView createGameView;
    GamesListView gamesListView;
    SettingsView settingsView;
    CavokeQmlGameModel *currentQmlGameModel = nullptr;
};

#endif  // CAVOKECLIENTCONTROLLER_H
