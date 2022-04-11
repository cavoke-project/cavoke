#include "creategameview.h"
#include "ui_creategameview.h"

CreateGameView::CreateGameView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::CreateGameView) {
    ui->setupUi(this);
    connect(ui->gamesListComboBox, SIGNAL(currentIndexChanged(int)), this,
            SLOT(repeaterCurrentIndexChanged(int)));
}

void CreateGameView::repeaterCurrentIndexChanged(int index) {
    if (index == -1) {
        displayEmpty();
        return;
    }
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
    ui->gamesListComboBox->clear();
    for (const auto &gameInfo : newGamesList) {
        ui->gamesListComboBox->addItem(gameInfo.display_name);
    }
    ui->gamesListComboBox->setCurrentIndex(-1);
}

void CreateGameView::gotNewSelectedGame(const GameInfo &gameInfo) {
    ui->gameNameLabel->setText(gameInfo.display_name);
    ui->gameDescriptionTextBrowser->setText(gameInfo.description);
    ui->playersAmountLabel->setText(QString::number(gameInfo.players_num));
    ui->createGameButton->setEnabled(true);
}
void CreateGameView::on_createGameButton_clicked() {
    //    this->close();
    emit startedCreateGameRoutine(ui->gamesListComboBox->currentIndex());
}
void CreateGameView::displayEmpty() {
    ui->gameNameLabel->clear();
    ui->gameDescriptionTextBrowser->clear();
    ui->playersAmountLabel->clear();
    ui->createGameButton->setEnabled(false);
}
