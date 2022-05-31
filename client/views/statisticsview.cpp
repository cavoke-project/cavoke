#include "statisticsview.h"
#include <QFileDialog>
#include "authdialog.h"
#include "ui_statisticsview.h"

StatisticsView::StatisticsView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::StatisticsView) {
    ui->setupUi(this);
    connect(ui->games_combobox, SIGNAL(currentIndexChanged(int)), this,
            SLOT(repeaterCurrentIndexChanged(int)));
}

void StatisticsView::repeaterCurrentIndexChanged(int index) {
    if (index == -1) {
        displayEmpty();
        return;
    }
    emit statisticsGameChanged(ui->games_combobox->itemData(index).toString());
}

void StatisticsView::displayEmpty() {
    ui->time_played_label->setText("0");
    ui->games_played_label->setText("0");
    ui->win_rate_label->setText("0");
}

void StatisticsView::gotUserGameStatisticsUpdate(
    const UserGameStatistics &userGameStatistics) {
    ui->time_played_label->setText(
        QString::number(userGameStatistics.time_played_sec));
    ui->games_played_label->setText(
        QString::number(userGameStatistics.games_played));
    ui->win_rate_label->setText(QString::number(userGameStatistics.win_rate));
}

StatisticsView::~StatisticsView() {
    delete ui;
}

void StatisticsView::on_backButton_clicked() {
    this->close();
    emit shownStartView();
}

void StatisticsView::gotGamesListUpdate(
    const std::vector<GameInfo> &newGamesList) {
    ui->games_combobox->clear();
    for (const auto &gameInfo : newGamesList) {
        ui->games_combobox->addItem(gameInfo.display_name, gameInfo.id);
    }
}
void StatisticsView::gotUserStatisticsUpdate(
    const UserStatistics &userStatistics) {
    ui->total_time_played_label->setText(
        QString::number(userStatistics.total_time_played_sec));
    ui->total_games_played_label->setText(
        QString::number(userStatistics.total_games_played));
}

void StatisticsView::requestUpdates() {
    emit requestedRefresh();
    emit statisticsGameChanged(
        ui->games_combobox->itemData(ui->games_combobox->currentIndex())
            .toString());
}

void StatisticsView::on_refreshButton_clicked() {
    requestUpdates();
}
