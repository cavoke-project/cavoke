#ifndef CAVOKE_CLIENT_STATISTICSVIEW_H
#define CAVOKE_CLIENT_STATISTICSVIEW_H

#include <QMainWindow>
#include "entities/gameinfo.h"

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

signals:
    void shownStartView();
//    void joinedGame(const QString &inviteCode);

private slots:
    void on_backButton_clicked();
//    void on_joinGameButton_clicked();

private:
    Ui::StatisticsView *ui;
};

#endif  // CAVOKE_CLIENT_STATISTICSVIEW_H
