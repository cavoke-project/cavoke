#include "startview.h"
#include "cavokeclientcontroller.h"
#include "ui_startview.h"

StartView::StartView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::StartView) {
    ui->setupUi(this);
    ui->cavokeTestWindowButton->hide();
}

StartView::~StartView() {
    delete ui;
}

void StartView::on_joinGameButton_clicked() {
    this->close();
    emit shownJoinGameView();
}

void StartView::on_createGameButton_clicked() {
    this->close();
    emit shownCreateGameView();
}

void StartView::on_gamesListButton_clicked() {
    this->close();
    emit shownGamesListView();
}

void StartView::on_cavokeTestWindowButton_clicked() {
    this->close();
    emit shownTestWindowView();
}

void StartView::on_statisticsButton_clicked() {
    if (!AuthDialog::verifyAuth(this)) {
        return;
    }
    this->close();
    emit shownStatisticsView();
}

void StartView::on_settingsButton_clicked() {
    this->close();
    emit shownSettingsView();
}

void StartView::on_exitButton_clicked() {
    emit clickedExitButton();
}
