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
    void receivedGameIndexChange(int newIndex);
    void receivedGameIndexChangeInList(int newIndex);
    void gotIndexToDownload(int index);
signals:
    void startQmlApplication(CavokeQmlGameModel *);
    void gamesListUpdated(const std::vector<GameInfo> &newGamesList);
    void updateSelectedGame(const GameInfo &gameInfo);
    void updateSelectedGameInList(const GameInfo &gameInfo);
    void downloadGame(const QString &gameId);

private:
    std::vector<GameInfo> gamesList;
};

#endif  // CAVOKECLIENTMODEL_H
