#ifndef CAVOKE_GAMESLISTVIEW_H
#define CAVOKE_GAMESLISTVIEW_H

#include <entities/gameinfo.h>
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
    void gotNewSelectedGame(const GameInfo &gameInfo);

signals:
    void shownStartView();
    void currentIndexChanged(int index);
    void requestedDownloadGame(int index);

private slots:
    void on_backButton_clicked();
    void on_downloadQmlButton_clicked();
    void repeaterCurrentIndexChanged(int index);

private:
    void displayEmpty();
    Ui::GamesListView *ui;
};

#endif  // CAVOKE_GAMESLISTVIEW_H
