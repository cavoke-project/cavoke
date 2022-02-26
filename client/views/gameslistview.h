#ifndef CAVOKE_GAMESLISTVIEW_H
#define CAVOKE_GAMESLISTVIEW_H

#include <gameinfo.h>
#include <QMainWindow>

namespace Ui {
class GamesListView;
}

class GamesListView : public QMainWindow {
Q_OBJECT
public:
    explicit GamesListView(QWidget *parent = nullptr);
    ~GamesListView();
    
public slots:
    void gotGamesListUpdate(const std::vector<GameInfo> &newGamesList);
    
signals:
    void shownStartView();

private slots:
    void on_backButton_clicked();

private:
    Ui::GamesListView *ui;
};

#endif  // CAVOKE_GAMESLISTVIEW_H
