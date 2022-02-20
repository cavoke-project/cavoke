#ifndef CAVOKECLIENTMODEL_H
#define CAVOKECLIENTMODEL_H

#include <QObject>
#include "cavokeqmlgamemodel.h"

class CavokeClientModel : public QObject {
    Q_OBJECT

public:
    explicit CavokeClientModel(QObject *parent = nullptr);
public slots:
    void loadQmlGame(const QString &gameName);
    void updateGamesList(const QString &newGamesList);
signals:
    void startQmlApplication(CavokeQmlGameModel *);
    void gamesListUpdated(const QString &newGamesList);  // FIXME: same

private:
    QString tempGamesList;  // FIXME: will become a vector of GameInfo
};

#endif  // CAVOKECLIENTMODEL_H
