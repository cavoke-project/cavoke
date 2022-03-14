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
    ui->currentPlayersHLabel->setHidden(newStatus != CreatingGameStatus::DONE);
    ui->playersListWidget->setHidden(newStatus != CreatingGameStatus::DONE);
}

void ProtoRoomView::updateSessionInfo(const SessionInfo &sessionInfo) {
    ui->inviteCodeLabel->setText(sessionInfo.invite_code);
    ui->gameNameLabel->setText(sessionInfo.game_id);
    gameName = sessionInfo.game_id;

    ui->playersListWidget->clear();
    for (const auto &player : sessionInfo.players) {
        ui->playersListWidget->addItem(player.user_id);
    }
}

void ProtoRoomView::on_joinGameButton_clicked() {
    this->close();
    emit joinedCreatedGame(gameName);
}

void ProtoRoomView::prepareJoinCreate(bool _isJoining) {
    ui->currentPlayersHLabel->hide();
    ui->playersListWidget->hide();

    ui->inviteCodeLabel->setText("Unknown");
    ui->gameNameLabel->setText("Unknown");

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

void ProtoRoomView::on_backButton_clicked() {
    this->close();
    emit shownStartView();
}
