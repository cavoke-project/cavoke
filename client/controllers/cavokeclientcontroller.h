#ifndef CAVOKECLIENTCONTROLLER_H
#define CAVOKECLIENTCONTROLLER_H

#include <QObject>
#include <QtQuick>

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

private slots:
    void startQmlApplication(CavokeQmlGameModel*);
    void exitApplication();
private:
    CavokeClientModel model;
    TestWindowView testWindowView;
    StartView startView;
    JoinGameView joinGameView;
    CreateGameView createGameView;
    SettingsView settingsView;
};

#endif // CAVOKECLIENTCONTROLLER_H
