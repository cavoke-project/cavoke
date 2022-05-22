#ifndef CAVOKE_CLIENT_STATISTICSVIEW_H
#define CAVOKE_CLIENT_STATISTICSVIEW_H

#include <QMainWindow>
#include "entities/gameinfo.h"
#include "entities/usergamestatistics.h"
#include "entities/userstatistics.h"

namespace Ui {
class StatisticsView;
}

class StatisticsView : public QMainWindow {
    Q_OBJECT
public:
    explicit StatisticsView(QWidget *parent = nullptr);
    ~StatisticsView();

public slots:
    void gotGamesListUpdate(const std::vector<GameInfo> &newGamesList);
    void gotUserStatisticsUpdate(const UserStatistics &userStatistics);
    void gotUserGameStatisticsUpdate(
        const UserGameStatistics &userGameStatistics);

signals:
    void shownStartView();
    void requestedRefresh();
    void statisticsGameChanged(const QString &gameId);

private slots:
    void on_backButton_clicked();
    void on_refreshButton_clicked();
    void displayEmpty();
    void repeaterCurrentIndexChanged(int index);

private:
    Ui::StatisticsView *ui;
};

#endif  // CAVOKE_CLIENT_STATISTICSVIEW_H
