#include "cavokeclientmodel.h"

#include <QtQuick>


CavokeClientModel::CavokeClientModel(QObject *parent)
        : QObject{parent} {
}

void CavokeClientModel::loadQmlGame(const QString &appPath) {
    // TODO: change to url loading from cache, not resources
    auto *gameModel = new CavokeQmlGameModel(QUrl::fromUserInput(appPath), this); // must be alive when accessed from QML

    qDebug() << "Starting game from: " << appPath;

    emit startQmlApplication(gameModel);
}
