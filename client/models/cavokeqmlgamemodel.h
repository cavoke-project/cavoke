#ifndef CAVOKEQMLGAMEMODEL_H
#define CAVOKEQMLGAMEMODEL_H

#include <QDebug>
#include <QObject>
#include <QUrl>
#include <QtCore/QUuid>
#include "tictactoelogic.h"

class CavokeQmlGameModel : public QObject {
    Q_OBJECT
    tictactoe::tictactoelogic logic{};

public:
    explicit CavokeQmlGameModel(QUrl qmlPath, QObject *parent = nullptr);

public slots:
    void getMoveFromQml(const QString &jsonMove);
    void getUpdateFromNetwork(const QString &jsonUpdate);

signals:
    void receiveUpdate(QString jsonUpdate);
    void sendMoveToNetwork(const QString &jsonMove, const QString &session_id, const QString &user_id);

public:
    QUrl qmlPath;
    
public:
    QUuid session_id;   // FIXME: Temporary here
    QUuid user_id;
};

#endif  // CAVOKEQMLGAMEMODEL_H
