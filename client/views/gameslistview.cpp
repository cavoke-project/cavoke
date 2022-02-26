#include "gameslistview.h"

#include "ui_gameslistview.h"

GamesListView::GamesListView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::GamesListView) {
    ui->setupUi(this);
}

GamesListView::~GamesListView() {
    delete ui;
}

void GamesListView::on_backButton_clicked() {
    this->close();
    emit shownStartView();
}

void GamesListView::gotGamesListUpdate(
    const std::vector<GameInfo> &newGamesList) {
    for (const auto &gameInfo : newGamesList) {
        ui->gamesListWidget->addItem(gameInfo.display_name);
    }
}
