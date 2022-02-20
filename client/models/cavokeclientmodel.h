#ifndef CAVOKECLIENTMODEL_H
#define CAVOKECLIENTMODEL_H

#include <QObject>
#include "cavokeqmlgamemodel.h"
#include "gameinfo.h"

class CavokeClientModel : public QObject {
    Q_OBJECT

public:
    explicit CavokeClientModel(QObject *parent = nullptr);
public slots:
    void loadQmlGame(const QString &gameName);
    void updateGamesList(const QJsonArray &newGamesList);
signals:
    void startQmlApplication(CavokeQmlGameModel *);
    void gamesListUpdated(const std::vector<GameInfo> &newGamesList);

private:
    std::vector<GameInfo> gamesList;
};

#endif  // CAVOKECLIENTMODEL_H
