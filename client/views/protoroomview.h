#ifndef CAVOKE_CLIENT_MIDDLESCREENVIEW_H
#define CAVOKE_CLIENT_MIDDLESCREENVIEW_H

#include <entities/sessioninfo.h>
#include <entities/validationresult.h>
#include <QMainWindow>

namespace Ui {
class ProtoRoomView;
}

class ProtoRoomView : public QMainWindow {
    Q_OBJECT
public:
    enum class CreatingGameStatus { UNKNOWN, DOWNLOAD, REQUESTED, DONE };
    explicit ProtoRoomView(QWidget *parent = nullptr);
    ~ProtoRoomView();

public slots:
    void updateStatus(CreatingGameStatus newStatus);
    void updateSessionInfo(const SessionInfo &sessionInfo);
    void prepareJoinCreate(bool _isJoining);
    void updateValidationResult(const ValidationResult &validationResult);
    void updateGameName(const QString &gameName);

signals:
    void createdGame();
    void joinedCreatedGame();
    void shownStartView();
    void leftRoom();

private slots:
    void on_joinGameButton_clicked();
    void on_backButton_clicked();

private:
    Ui::ProtoRoomView *ui;
    const std::map<CreatingGameStatus, QString> STATUS = {
        {CreatingGameStatus::UNKNOWN, "Unknown"},
        {CreatingGameStatus::DOWNLOAD, "Downloading"},
        {CreatingGameStatus::REQUESTED, "Sending request"},
        {CreatingGameStatus::DONE, "Done"},
    };
    bool isJoining = false;
};

#endif  // CAVOKE_CLIENT_MIDDLESCREENVIEW_H
