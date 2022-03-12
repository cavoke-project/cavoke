#include "protoroomview.h"
#include "ui_protoroomview.h"

ProtoRoomView::ProtoRoomView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ProtoRoomView) {
    ui->setupUi(this);
}

ProtoRoomView::~ProtoRoomView() {
    delete ui;
}

void ProtoRoomView::updateStatus(CreatingGameStatus newStatus) {
    ui->statusLabel->setText(STATUS.at(newStatus));
    ui->joinGameButton->setEnabled(newStatus == CreatingGameStatus::DONE);
}

void ProtoRoomView::updateInviteCode(const QString &newInviteCode) {
    ui->inviteCodeLabel->setText(newInviteCode);
}

void ProtoRoomView::on_joinGameButton_clicked() {
    this->close();
    emit joinedCreatedGame(gameName);
}
void ProtoRoomView::updateGameName(const QString &name) {
    gameName = name;
    ui->gameNameLabel->setText(gameName);
}
void ProtoRoomView::prepareJoinCreate(bool _isJoining) {
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
    updateInviteCode("");
}

void ProtoRoomView::on_backButton_clicked() {
    this->close();
    emit shownStartView();
}
