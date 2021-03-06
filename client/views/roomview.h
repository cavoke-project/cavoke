#ifndef CAVOKE_CLIENT_ROOMVIEW_H
#define CAVOKE_CLIENT_ROOMVIEW_H

#include <QMainWindow>
#include "entities/gameinfo.h"
#include "entities/role.h"
#include "entities/roominfo.h"
#include "entities/sessioninfo.h"
#include "entities/validationresult.h"

namespace Ui {
class RoomView;
}

class RoomView : public QMainWindow {
    Q_OBJECT
public:
    enum class CreatingSessionStatus {
        UNKNOWN,
        WAIT_FOR_THE_HOST,
        NOW_YOU_CAN_JOIN,
        YOU_CAN_CREATE
    };
    explicit RoomView(QWidget *parent = nullptr);
    ~RoomView();

public slots:
    void updateRoomInfo(const RoomInfo &roomInfo);
    void updateSessionInfo(const SessionInfo &sessionInfo);  // Temporary
    void clear();
    void gotGamesListUpdate(const std::vector<GameInfo> &newGamesList);

signals:
    void createdSession(const QString &gameId);
    void joinedSession(const QString &sessionId);
    void requestedSessionUpdate(const QString &sessionId);
    void shownStartView();
    void leftRoom();

private slots:
    void updateStatus(CreatingSessionStatus newStatus);
    void on_backButton_clicked();
    void on_joinSessionButton_clicked();
    void on_createSessionButton_clicked();

private:
    Ui::RoomView *ui;
    const std::map<CreatingSessionStatus, QString> STATUS = {
        {CreatingSessionStatus::UNKNOWN, "Unknown"},
        {CreatingSessionStatus::WAIT_FOR_THE_HOST,
         "Wait for the host to create a session"},
        {CreatingSessionStatus::NOW_YOU_CAN_JOIN,
         "Now you can join the session!"},
        {CreatingSessionStatus::YOU_CAN_CREATE, "You can create a session"},
    };
    QString sessionId;
    CreatingSessionStatus status = CreatingSessionStatus::UNKNOWN;
    bool is_host = false;  // Wow, this is very cool
};

#endif  // CAVOKE_CLIENT_ROOMVIEW_H
