#include "cavokeclientmodel.h"
#include <QtQuick>

CavokeClientModel::CavokeClientModel(QObject *parent) : QObject{parent} {
}

void CavokeClientModel::loadQmlGame(const QString &gameId) {
    // TODO: change to url loading from cache, not resources
    auto *gameModel =
        new CavokeQmlGameModel(QUrl::fromUserInput(gameId),
                               this);  // must be alive when accessed from QML

    qDebug() << "Starting game from: " << gameId;

    emit startQmlApplication(gameModel);
}
void CavokeClientModel::updateGamesList(const QJsonArray &newGamesList) {
    std::vector<GameInfo> got_from;
    for (auto obj : newGamesList) {
        got_from.emplace_back(GameInfo());
        got_from.back().read(obj.toObject());
    }
    gamesList = got_from;

    if (!gamesList.empty()) {
        qDebug() << "First In Model: " << gamesList[0].id;
    }
    emit gamesListUpdated(gamesList);
}
void CavokeClientModel::receivedGameIndexChange(int newIndex) {
    emit updateSelectedGame(gamesList[newIndex]);
}

void CavokeClientModel::receivedGameIndexChangeInList(int newIndex) {
    emit updateSelectedGameInList(gamesList[newIndex]);
}

void CavokeClientModel::gotIndexToDownload(int index) {
    gotGameIdToDownload(gamesList[index].id);
}

QString CavokeClientModel::getGameIdByIndex(int index) {
    return gamesList[index].id;
}

void CavokeClientModel::gotGameIdToDownload(const QString &gameId) {
    qDebug() << gameId;
    emit downloadGame(gameId);
}
