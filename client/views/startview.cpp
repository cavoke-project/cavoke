#include "startview.h"
#include "ui_startview.h"
#include "cavokeclientcontroller.h"

StartView::StartView(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::StartView) {
    ui->setupUi(this);
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

void StartView::on_cavokeTestWindowButton_clicked() {
    this->close();
    emit shownTestWindowView();
}

void StartView::on_settingsButton_clicked() {

}

void StartView::on_exitButton_clicked() {
    emit clickedExitButton();
}

