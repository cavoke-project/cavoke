#ifndef CAVOKE_GAMESLISTVIEW_H
#define CAVOKE_GAMESLISTVIEW_H

#include <QMainWindow>
#include "entities/gameinfo.h"
#include "entities/gamestatistics.h"

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
    void displayEmpty();
    void gotNewGameStatistics(const GameStatistics &gameStatistics);

signals:
    void shownStartView();
    void currentIndexChanged(int index);
    void requestGameStatistics(const QString &gameId);
    void requestedDownloadGame(int index);

private slots:
    void on_backButton_clicked();
    void on_downloadQmlButton_clicked();
    void repeaterCurrentIndexChanged(int index);

private:
    Ui::GamesListView *ui;
};

#endif  // CAVOKE_GAMESLISTVIEW_H
