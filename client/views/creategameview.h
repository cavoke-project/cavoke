#ifndef CAVOKE_CLIENT_CREATEGAMEVIEW_H
#define CAVOKE_CLIENT_CREATEGAMEVIEW_H

#include <QMainWindow>
#include "entities/gameinfo.h"

namespace Ui {
class CreateGameView;
}

class CreateGameView : public QMainWindow {
    Q_OBJECT
public:
    explicit CreateGameView(QWidget *parent = nullptr);
    ~CreateGameView();

public slots:
    void gotGamesListUpdate(const std::vector<GameInfo> &newGamesList);
    void gotNewSelectedGame(const GameInfo &gameInfo);

signals:
    void shownStartView();
    void currentIndexChanged(int index);
    void startedCreateGameRoutine(int gameIndex);

private slots:
    void on_backButton_clicked();
    void on_createGameButton_clicked();
    void repeaterCurrentIndexChanged(int index);

private:
    Ui::CreateGameView *ui;
};

#endif  // CAVOKE_CLIENT_CREATEGAMEVIEW_H
