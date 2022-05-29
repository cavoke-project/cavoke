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
    ui->roomNameLabel->setText(roomInfo.display_name);
    ui->inviteCodeLabel->setText(roomInfo.invite_code);
    ui->membersListWidget->clear();
    for (const auto &member : roomInfo.members) {
        ui->membersListWidget->addItem(member.display_name);
    }

    if (!sessionId.isEmpty()) {
        updateStatus(CreatingSessionStatus::NOW_YOU_CAN_JOIN);
    } else {
        if (roomInfo.isHost) {
            updateStatus(CreatingSessionStatus::YOU_CAN_CREATE);
        } else {
            updateStatus(CreatingSessionStatus::WAIT_FOR_THE_HOST);
        }
    }

    if (roomInfo.isHost) {
        show_as_host();
    } else {
        show_as_guest();
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
    show_as_guest();
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

void RoomView::show_as_host() {
    ui->sessionGameLabel->show();
    ui->sessionGameComboBox->show();
    ui->joinSessionButton->hide();
    ui->createSessionButton->show();
}
void RoomView::show_as_guest() {
    ui->sessionGameLabel->hide();
    ui->sessionGameComboBox->hide();
    ui->createSessionButton->hide();
    ui->joinSessionButton->show();
}

void RoomView::on_joinSessionButton_clicked() {
    emit joinedSession(sessionId);
}

void RoomView::on_createSessionButton_clicked() {
    emit createdSession(sessionId);
}
