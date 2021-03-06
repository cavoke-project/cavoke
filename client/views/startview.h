#ifndef CLIENT_STARTVIEW_H
#define CLIENT_STARTVIEW_H

#include <QMainWindow>

namespace Ui {
class StartView;
}

class StartView : public QMainWindow {
    Q_OBJECT
public:
    explicit StartView(QWidget *parent = nullptr);

    ~StartView();

signals:
    void shownTestWindowView();
    void shownJoinGameView();
    void shownCreateGameView();
    void shownSettingsView();
    void shownGamesListView();
    void shownStatisticsView();
    void clickedExitButton();

private slots:
    void on_joinGameButton_clicked();
    void on_createGameButton_clicked();
    void on_gamesListButton_clicked();
    void on_cavokeTestWindowButton_clicked();
    void on_statisticsButton_clicked();
    void on_settingsButton_clicked();
    void on_exitButton_clicked();

private:
    Ui::StartView *ui;
};

#endif  // CLIENT_STARTVIEW_H
