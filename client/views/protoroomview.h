#ifndef CAVOKE_CLIENT_MIDDLESCREENVIEW_H
#define CAVOKE_CLIENT_MIDDLESCREENVIEW_H

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
    void updateInviteCode(const QString &newInviteCode);
    void updateGameName(const QString &name);
    void prepareJoinCreate(bool _isJoining);

signals:
    void joinedCreatedGame(const QString &appName);
    void shownStartView();

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
    QString gameName;
    bool isJoining = false;
};

#endif  // CAVOKE_CLIENT_MIDDLESCREENVIEW_H
