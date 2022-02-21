#ifndef CAVOKECLIENTCONTROLLER_H
#define CAVOKECLIENTCONTROLLER_H

#include <QObject>
#include <QtQuick>

#include "network_manager.h"
#include "cavokeclientmodel.h"
#include "startview.h"
#include "testwindowview.h"
#include "joingameview.h"
#include "creategameview.h"
#include "settingsview.h"

class CavokeClientController : public QObject
{
    Q_OBJECT
public:
    explicit CavokeClientController(QObject *parent = nullptr);
    
public slots:
    void showTestWindowView();
    void showStartView();
    void showJoinGameView();
    void showCreateGameView();
    void showSettingsView();
    
signals:
    void loadGamesList();

private slots:
    void startQmlApplication(CavokeQmlGameModel*);
    void exitApplication();
    void startTicTacToe();
    void stopTicTacToe();
private:
    NetworkManager networkManager;
    CavokeClientModel model;
    TestWindowView testWindowView;
    StartView startView;
    JoinGameView joinGameView;
    CreateGameView createGameView;
    SettingsView settingsView;
    CavokeQmlGameModel *currentQmlGameModel;
    QTimer *currentQmlTimer;
};

#endif // CAVOKECLIENTCONTROLLER_H
