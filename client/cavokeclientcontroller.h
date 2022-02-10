#ifndef CAVOKECLIENTCONTROLLER_H
#define CAVOKECLIENTCONTROLLER_H

#include <QObject>
#include <QtQuick>

#include "cavokeclientmodel.h"
#include "cavokeclientview.h"

class CavokeClientController : public QObject
{
    Q_OBJECT
public:
    explicit CavokeClientController(QObject *parent = nullptr);
private slots:
    void startQmlApplication(CavokeQmlGameModel*);
private:
    CavokeClientModel model;
    CavokeClientView view;
};

#endif // CAVOKECLIENTCONTROLLER_H
