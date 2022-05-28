#ifndef CAVOKE_CLIENT_ROOMVIEW_H
#define CAVOKE_CLIENT_ROOMVIEW_H

#include <entities/role.h>
#include <entities/sessioninfo.h>
#include <entities/validationresult.h>
#include <QMainWindow>

namespace Ui {
class RoomView;
}

class RoomView : public QMainWindow {
    Q_OBJECT
public:
    enum class CreatingGameStatus { UNKNOWN, DOWNLOAD, REQUESTED, DONE };
    explicit RoomView(QWidget *parent = nullptr);
    ~RoomView();

//public slots:
//    void updateStatus(CreatingGameStatus newStatus);
//    void updateSessionInfo(const SessionInfo &sessionInfo);
//    void clear();
//    void updateValidationResult(const ValidationResult &validationResult);
//    void updateGameName(const QString &gameName);
//    void gotRolesListUpdate(
//        const std::vector<Role>
//            &newRolesList);  // Probably should use some struct...

signals:
//    void createdGame();
//    void joinedCreatedGame();
    void shownStartView();
//    void leftRoom();
//    void newRoleChosen(int roleId);

private slots:
//    void on_joinGameButton_clicked();
    void on_backButton_clicked();
//    void repeaterCurrentIndexChanged(int index);

private:
    Ui::RoomView *ui;
    void show_as_host();
    void show_as_guest();
//    const std::map<CreatingGameStatus, QString> STATUS = {
//        {CreatingGameStatus::UNKNOWN, "Unknown"},
//        {CreatingGameStatus::DOWNLOAD, "Downloading"},
//        {CreatingGameStatus::REQUESTED, "Sending request"},
//        {CreatingGameStatus::DONE, "Done"},
//    };
//    int ourRole = -1;
};

#endif  // CAVOKE_CLIENT_ROOMVIEW_H
