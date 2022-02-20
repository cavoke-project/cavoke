#include "creategameview.h"
#include "ui_creategameview.h"

CreateGameView::CreateGameView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::CreateGameView) {
    ui->setupUi(this);
    connect(ui->gamesListComboBox, SIGNAL(currentIndexChanged(int)), this,
            SLOT(repeaterCurrentIndexChanged(int)));
}

void CreateGameView::repeaterCurrentIndexChanged(int index) {
    emit currentIndexChanged(index);
}

CreateGameView::~CreateGameView() {
    delete ui;
}

void CreateGameView::on_backButton_clicked() {
    this->close();
    emit shownStartView();
}
void CreateGameView::gotGamesListUpdate(
    const std::vector<GameInfo> &newGamesList) {
    for (const auto &gameInfo : newGamesList) {
        ui->gamesListComboBox->addItem(gameInfo.display_name);
    }
}
void CreateGameView::gotNewSelectedGame(const GameInfo &gameInfo) {
    ui->gameNameLabel->setText(gameInfo.display_name);
    ui->gameDescriptionTextBrowser->setText(gameInfo.description);
    ui->playersAmountLabel->setText(
        QString::fromStdString(std::to_string(gameInfo.players_num)));
    ui->createGameButton->setEnabled(true);
}
