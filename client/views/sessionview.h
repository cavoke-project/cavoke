#ifndef CAVOKE_CLIENT_SESSIONVIEW_H
#define CAVOKE_CLIENT_SESSIONVIEW_H

#include <entities/role.h>
#include <entities/sessioninfo.h>
#include <entities/validationresult.h>
#include <QMainWindow>

namespace Ui {
class SessionView;
}

class SessionView : public QMainWindow {
    Q_OBJECT
public:
    enum class CreatingGameStatus { UNKNOWN, DOWNLOAD, REQUESTED, DONE };
    explicit SessionView(QWidget *parent = nullptr);
    ~SessionView();

public slots:
    void updateStatus(CreatingGameStatus newStatus);
    void updateSessionInfo(const SessionInfo &sessionInfo);
    void clear();
    void updateValidationResult(const ValidationResult &validationResult);
    void updateGameName(const QString &gameName);
    void gotRolesListUpdate(const std::vector<Role> &newRolesList);

signals:
    void createdGame();
    void joinedCreatedGame();
    void shownRoomView();
    void leftSession();
    void newRoleChosen(int roleId);

private slots:
    void on_startGameButton_clicked();
    void on_backButton_clicked();
    void repeaterCurrentIndexChanged(int index);

private:
    Ui::SessionView *ui;
    void show_as_host();
    void show_as_guest();
    const std::map<CreatingGameStatus, QString> STATUS = {
        {CreatingGameStatus::UNKNOWN, "Unknown"},
        {CreatingGameStatus::DOWNLOAD, "Downloading"},
        {CreatingGameStatus::REQUESTED, "Sending request"},
        {CreatingGameStatus::DONE, "Ready"},
    };
    int ourRole = -1;
};

#endif  // CAVOKE_CLIENT_SESSIONVIEW_H
