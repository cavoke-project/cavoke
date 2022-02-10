#ifndef CAVOKECLIENTMODEL_H
#define CAVOKECLIENTMODEL_H

#include <QObject>

#include "cavokeqmlgamemodel.h"

class CavokeClientModel : public QObject
{
    Q_OBJECT

public:
    explicit CavokeClientModel(QObject *parent = nullptr);
public slots:
    void loadQmlGame(const QString& gameName);
signals:
    void startQmlApplication(CavokeQmlGameModel*);
};

#endif // CAVOKECLIENTMODEL_H
