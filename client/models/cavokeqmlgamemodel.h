#ifndef CAVOKEQMLGAMEMODEL_H
#define CAVOKEQMLGAMEMODEL_H

#include <QDebug>
#include <QObject>
#include <QUrl>
#include <QtCore/QUuid>
#include <QtQuick/QtQuick>
#include "tictactoelogic.h"

class CavokeQmlGameModel : public QObject {
    Q_OBJECT
    tictactoe::tictactoelogic logic{};

public:
    explicit CavokeQmlGameModel(QUrl qmlPath, QObject *parent = nullptr);

public slots:
    void getMoveFromQml(const QString &jsonMove);
    void getUpdateFromNetwork(const QString &jsonUpdate);
    void getClosingFromQml(QQuickCloseEvent *close);

signals:
    void receiveUpdate(QString jsonUpdate);
    void sendMoveToNetwork(const QString &jsonMove);
    void closingQml();

public:
    QUrl qmlPath;
};

#endif  // CAVOKEQMLGAMEMODEL_H
