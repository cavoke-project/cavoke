#ifndef CAVOKEQMLGAMEMODEL_H
#define CAVOKEQMLGAMEMODEL_H

#include <QDebug>
#include <QObject>
#include <QUrl>
#include "tictactoelogic.h"

class CavokeQmlGameModel : public QObject
{
    Q_OBJECT
    tictactoe::tictactoelogic logic{};
public:
    explicit CavokeQmlGameModel(QUrl qmlPath, QObject *parent = nullptr);

signals:
    void receiveUpdate(QString jsonUpdate);
public slots:
    void sendMove(const QString& jsonMove);

public:
    QUrl qmlPath;
};

#endif // CAVOKEQMLGAMEMODEL_H
