#include "gameslistview.h"
#include "ui_gameslistview.h"

GamesListView::GamesListView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::GamesListView) {
    ui->setupUi(this);
    connect(ui->gamesListWidget, SIGNAL(currentRowChanged(int)), this,
            SLOT(repeaterCurrentIndexChanged(int)));
}

void GamesListView::repeaterCurrentIndexChanged(int index) {
    if (index == -1) {
        displayEmpty();
        return;
    }
    emit currentIndexChanged(index);
}

GamesListView::~GamesListView() {
    delete ui;
}

void GamesListView::on_backButton_clicked() {
    this->close();
    emit shownStartView();
}

void GamesListView::on_downloadQmlButton_clicked() {
    emit requestedDownloadGame(ui->gamesListWidget->currentRow());
}

void GamesListView::gotGamesListUpdate(
    const std::vector<GameInfo> &newGamesList) {
    ui->gamesListWidget->clear();
    for (const auto &gameInfo : newGamesList) {
        ui->gamesListWidget->addItem(gameInfo.display_name);
    }
    //    ui->gamesListWidget->setCurrentIndex(-1); // Is needed? Tests required
}

void GamesListView::gotNewSelectedGame(const GameInfo &gameInfo) {
    ui->gameNameLabel->setText(gameInfo.display_name);
    ui->gameDescriptionTextBrowser->setText(gameInfo.description);
    ui->playersAmountLabel->setText(QString::number(gameInfo.players_num));
    //    ui->createGameButton->setEnabled(true);
}

void GamesListView::displayEmpty() {
    ui->gameNameLabel->clear();
    ui->gameDescriptionTextBrowser->clear();
    ui->playersAmountLabel->clear();
}
