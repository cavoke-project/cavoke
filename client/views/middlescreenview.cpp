#include "middlescreenview.h"
#include "ui_middlescreenview.h"

MiddleScreenView::MiddleScreenView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MiddleScreenView) {
    ui->setupUi(this);
}

MiddleScreenView::~MiddleScreenView() {
    delete ui;
}

void MiddleScreenView::updateStatus(CreatingGameStatus newStatus) {
    ui->statusLabel->setText(STATUS.at(newStatus));
    if (newStatus == CreatingGameStatus::DONE) {
        ui->joinGameButton->setEnabled(true);
    } else {
        ui->joinGameButton->setEnabled(false);
    }
}

void MiddleScreenView::updateInviteCode(const QString &newInviteCode) {
    ui->inviteCodeLabel->setText(newInviteCode);
}

void MiddleScreenView::on_joinGameButton_clicked() {
    this->close();
    emit joinedCreatedGame(gameName);
}
void MiddleScreenView::updateGameName(const QString &name) {
    gameName = name;
}
void MiddleScreenView::prepareJoinCreate(bool _isJoining) {
    isJoining = _isJoining;
    if (isJoining) {
        ui->headerLabel->setText("Joining game session");
        ui->gameNameHLabel->show();
        ui->gameNameLabel->show();
        ui->inviteCodeHLabel->hide();
        ui->inviteCodeLabel->hide();
    } else {
        ui->headerLabel->setText("Creating game session");
        ui->gameNameHLabel->hide();
        ui->gameNameLabel->hide();
        ui->inviteCodeHLabel->show();
        ui->inviteCodeLabel->show();
    }
}
