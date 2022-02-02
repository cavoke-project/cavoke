#include "cavokeclientmodel.h"

#include <QtQuick>


CavokeClientModel::CavokeClientModel(QObject *parent)
    : QObject{parent} {
}

void CavokeClientModel::loadQmlGame(const QString &gameName) {
    // TODO: different games loading

    // TODO: change to url loading from cache, not resources
    // TODO: delete resource.qrc
    auto* gameModel = new CavokeQmlGameModel (QUrl("qrc:/tictactoe-files/tic-tac-toe.qml"), this); // must be alive when accessed from QML

    qDebug() << "Starting game : " << gameName;

    emit startQmlApplication(gameModel);
}
