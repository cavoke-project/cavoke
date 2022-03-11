#ifndef CAVOKE_CLIENT_MIDDLESCREENVIEW_H
#define CAVOKE_CLIENT_MIDDLESCREENVIEW_H

#include <QMainWindow>

namespace Ui {
class MiddleScreenView;
}

enum class CreatingGameStatus { UNKNOWN, DOWNLOAD, REQUESTED, DONE };

class MiddleScreenView : public QMainWindow {
    Q_OBJECT
public:
    explicit MiddleScreenView(QWidget *parent = nullptr);
    ~MiddleScreenView();

public slots:
    void updateStatus(CreatingGameStatus newStatus);
    void updateInviteCode(const QString &newInviteCode);
    void updateGameName(const QString &name);
    void prepareJoinCreate(bool _isJoining);

signals:
    void joinedCreatedGame(const QString &appName);

private slots:
    void on_joinGameButton_clicked();

private:
    Ui::MiddleScreenView *ui;
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
