#include "roomview.h"
#include "ui_roomview.h"

RoomView::RoomView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::RoomView) {
    ui->setupUi(this);
}

RoomView::~RoomView() {
    delete ui;
}

void RoomView::updateStatus(CreatingSessionStatus newStatus) {
    status = newStatus;
    ui->waitForHostLabel->setText(STATUS.at(status));
    ui->joinSessionButton->setEnabled(status ==
                                      CreatingSessionStatus::NOW_YOU_CAN_JOIN);
}

void RoomView::updateRoomInfo(const RoomInfo &roomInfo) {
    sessionId = roomInfo.session_id;
    is_host = roomInfo.isHost;
    ui->roomNameLabel->setText(roomInfo.display_name);
    ui->inviteCodeLabel->setText(roomInfo.invite_code);
    ui->membersListWidget->clear();
    for (const auto &member : roomInfo.members) {
        ui->membersListWidget->addItem(member.display_name);
    }

    if (!sessionId.isEmpty()) {
        requestedSessionUpdate(sessionId);
    } else {
        updateSessionInfo(SessionInfo());
    }
}

void RoomView::updateSessionInfo(const SessionInfo &sessionInfo) {
    if (!is_host) {
        ui->sessionGameLabel->hide();
        ui->sessionGameComboBox->hide();
        ui->createSessionButton->hide();
        ui->joinSessionButton->show();
        ui->joinSessionButton->setEnabled(sessionInfo.status !=
                                          SessionInfo::Status::FINISHED);
        updateStatus(sessionInfo.status != SessionInfo::Status::FINISHED
                         ? CreatingSessionStatus::NOW_YOU_CAN_JOIN
                         : CreatingSessionStatus::WAIT_FOR_THE_HOST);
        return;
    }
    if (sessionInfo.status != SessionInfo::Status::FINISHED) {
        ui->createSessionButton->hide();
        ui->joinSessionButton->show();
        ui->joinSessionButton->setEnabled(true);
        ui->sessionGameLabel->hide();
        ui->sessionGameComboBox->hide();
        updateStatus(CreatingSessionStatus::NOW_YOU_CAN_JOIN);
    } else {
        ui->createSessionButton->show();
        ui->joinSessionButton->hide();
        ui->createSessionButton->setEnabled(true);
        ui->sessionGameLabel->show();
        ui->sessionGameComboBox->show();
        updateStatus(CreatingSessionStatus::YOU_CAN_CREATE);
    }
}

void RoomView::gotGamesListUpdate(const std::vector<GameInfo> &newGamesList) {
    ui->sessionGameComboBox->clear();
    for (const auto &gameInfo : newGamesList) {
        ui->sessionGameComboBox->addItem(gameInfo.display_name, gameInfo.id);
    }
    if (ui->sessionGameComboBox->count() > 0) {
        ui->sessionGameComboBox->setCurrentIndex(0);
    } else {
        ui->sessionGameComboBox->setCurrentIndex(-1);
    }
}

void RoomView::clear() {
    updateStatus(CreatingSessionStatus::UNKNOWN);
    ui->roomNameLabel->setText("<room_name>");
    ui->inviteCodeLabel->setText("<invite_code>");
    ui->membersListWidget->clear();
}

void RoomView::on_backButton_clicked() {
    this->clear();
    this->close();
    emit leftRoom();
    emit shownStartView();
}

void RoomView::on_joinSessionButton_clicked() {
    emit joinedSession(sessionId);
}

void RoomView::on_createSessionButton_clicked() {
    if (ui->sessionGameComboBox->currentData().toString().isEmpty()) {
        return;  // Probably no games at all
    }
    emit createdSession(ui->sessionGameComboBox->currentData().toString());
}
