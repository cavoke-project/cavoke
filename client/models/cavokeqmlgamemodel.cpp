#include <QDebug>

#include "cavokeqmlgamemodel.h"

CavokeQmlGameModel::CavokeQmlGameModel(QUrl qmlPath, QObject *parent)
    : QObject{parent}, qmlPath{qmlPath} {  // TODO: std::move
    logic.restartGame();
    session_id = QUuid::createUuid();
    user_id = QUuid::createUuid();
}

void CavokeQmlGameModel::getMoveFromQml(const QString &jsonMove) {
    qDebug() << "CQGM: Received move! " << jsonMove;
    // Maybe we should draw move on board even without network result?
    emit sendMoveToNetwork(jsonMove, session_id.toString(), user_id.toString());
}

void CavokeQmlGameModel::getUpdateFromNetwork(const QString &jsonUpdate) {
    qDebug() << "CQGM: Received update! " << jsonUpdate;
    emit receiveUpdate(jsonUpdate);
}
