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
    std::transform(newGamesList.begin(), newGamesList.end(),
                   std::back_inserter(got_from), [](const QJsonValue &obj) {
                       GameInfo game_info;
                       game_info.read(obj.toObject());
                       return game_info;
                   });
    gamesList = std::move(got_from);

    if (!newGamesList.empty()) {
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

GameInfo CavokeClientModel::getGameByIndex(int index) {
    return gamesList[index];
}

void CavokeClientModel::gotGameIdToDownload(const QString &gameId) {
    qDebug() << gameId;
    emit downloadGame(gameId);
}
