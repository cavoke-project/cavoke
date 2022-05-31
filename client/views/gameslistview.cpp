#include "gameslistview.h"
#include "ui_gameslistview.h"

GamesListView::GamesListView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::GamesListView) {
    ui->setupUi(this);
    connect(ui->gameNameComboBox, SIGNAL(currentIndexChanged(int)), this,
            SLOT(repeaterCurrentIndexChanged(int)));
}

void GamesListView::repeaterCurrentIndexChanged(int index) {
    if (index == -1) {
        displayEmpty();
        return;
    }
    emit currentIndexChanged(index);
    emit requestGameStatistics(ui->gameNameComboBox->currentData().toString());
}

GamesListView::~GamesListView() {
    delete ui;
}

void GamesListView::on_backButton_clicked() {
    this->close();
    emit shownStartView();
}

void GamesListView::on_downloadQmlButton_clicked() {
    emit requestedDownloadGame(ui->gameNameComboBox->currentIndex());
}

void GamesListView::gotGamesListUpdate(
    const std::vector<GameInfo> &newGamesList) {
    ui->gameNameComboBox->clear();
    for (const auto &gameInfo : newGamesList) {
        ui->gameNameComboBox->addItem(gameInfo.display_name, gameInfo.id);
    }
}

void GamesListView::gotNewSelectedGame(const GameInfo &gameInfo) {
    ui->gameDescriptionTextBrowser->setText(gameInfo.description);
    ui->playersAllowedLabel->setText(QString::number(gameInfo.players_num));
}

void GamesListView::gotNewGameStatistics(const GameStatistics &gameStatistics) {
    ui->averageDurationLabel->setText(
        QString::number(gameStatistics.average_duration_sec));
    ui->averagePlayersLabel->setText(
        QString::number(gameStatistics.average_players_count));
    ui->totalTimePlayedLabel->setText(
        QString::number(gameStatistics.total_time_played_sec));
    ui->totalGamesPlayedLabel->setText(
        QString::number(gameStatistics.total_games_played));
}

void GamesListView::displayEmpty() {
    ui->gameDescriptionTextBrowser->clear();
    ui->playersAllowedLabel->clear();
}
