#include <QDebug>

#include "cavokeqmlgamemodel.h"

CavokeQmlGameModel::CavokeQmlGameModel(QUrl qmlPath, QObject *parent)
    : QObject{parent}, qmlPath{qmlPath} {  // TODO: std::move
}

void CavokeQmlGameModel::sendMove(const QString &jsonMove) {
    qDebug() << "c++: Received! " << jsonMove;
    // TODO
    emit receiveUpdate("pong");
}
