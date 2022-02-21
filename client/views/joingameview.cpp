#include "joingameview.h"
#include "ui_joingameview.h"

JoinGameView::JoinGameView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::JoinGameView) {
    ui->setupUi(this);
}

JoinGameView::~JoinGameView() {
    delete ui;
}

void JoinGameView::on_backButton_clicked() {
    this->close();
    emit shownStartView();
}

void JoinGameView::on_joinTicTacToeButton_clicked() {
    this->close();
    emit joinedTicTacToe();
}
