#include "gameslistview.h"
#include "ui_gameslistview.h"

GamesListView::GamesListView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::GamesListView) {
    ui->setupUi(this);
    connect(ui->gamesListWidget, SIGNAL(currentRowChanged(int)), this,
            SLOT(repeaterCurrentIndexChanged(int)));
}

void GamesListView::repeaterCurrentIndexChanged(int index) {
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
    for (const auto &gameInfo : newGamesList) {
        ui->gamesListWidget->addItem(gameInfo.display_name);
    }
}

void GamesListView::gotNewSelectedGame(const GameInfo &gameInfo) {
    ui->gameNameLabel->setText(gameInfo.display_name);
    ui->gameDescriptionTextBrowser->setText(gameInfo.description);
    ui->playersAmountLabel->setText(QString::number(gameInfo.players_num));
    //    ui->createGameButton->setEnabled(true);
}
