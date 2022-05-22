#ifndef CAVOKE_CLIENT_STATISTICSVIEW_H
#define CAVOKE_CLIENT_STATISTICSVIEW_H

#include <QMainWindow>

namespace Ui {
class StatisticsView;
}

class StatisticsView : public QMainWindow {
    Q_OBJECT
public:
    explicit StatisticsView(QWidget *parent = nullptr);
    ~StatisticsView();

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
