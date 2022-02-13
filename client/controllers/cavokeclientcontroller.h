#ifndef CAVOKECLIENTCONTROLLER_H
#define CAVOKECLIENTCONTROLLER_H

#include <QObject>
#include <QtQuick>

#include "cavokeclientmodel.h"
#include "startview.h"
#include "testwindowview.h"

class CavokeClientController : public QObject
{
    Q_OBJECT
public:
    explicit CavokeClientController(QObject *parent = nullptr);
    
public slots:
    void showTestWindowView();
    void showStartView();

private slots:
    void startQmlApplication(CavokeQmlGameModel*);
private:
    CavokeClientModel model;
    TestWindowView clientView;
    StartView startView;
};

#endif // CAVOKECLIENTCONTROLLER_H
