#include "cavokeqmlgamemodel.h"
#include <QDebug>

CavokeQmlGameModel::CavokeQmlGameModel(QUrl qmlPath, QObject *parent)
    : QObject{parent}, qmlPath{qmlPath} {  // TODO: std::move
}

void CavokeQmlGameModel::getMoveFromQml(const QString &jsonMove) {
    qDebug() << "CQGM: Received move! " << jsonMove;
    // Maybe we should draw move on board even without network result?
    emit sendMoveToNetwork(jsonMove);
}

void CavokeQmlGameModel::getUpdateFromNetwork(const QString &jsonUpdate) {
    qDebug() << "CQGM: Received update! " << jsonUpdate;
    emit receiveUpdate(jsonUpdate);
}

void CavokeQmlGameModel::getClosingFromQml(QQuickCloseEvent *close) {
    qDebug() << "CQGM: Closing!! ";
    emit closingQml();
}
