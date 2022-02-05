#include <QDebug>

#include "cavokeqmlgamemodel.h"

CavokeQmlGameModel::CavokeQmlGameModel(QUrl qmlPath, QObject *parent)
    : QObject{parent}, qmlPath{qmlPath} {  // TODO: std::move
    logic.restartGame();
}

void CavokeQmlGameModel::sendMove(const QString &jsonMove) {
    // FIXME: Replace with JSON
    qDebug() << "c++: Received! " << jsonMove;
    QString processing_result = logic.processAction(jsonMove);
    QString board = logic.get_board_as_string();
    
    if (!logic.running) {       // FIXME: should be moved into logic part
        logic.restartGame();
    }
    QString result = processing_result + "\n" + board;
    emit receiveUpdate(result);
}
