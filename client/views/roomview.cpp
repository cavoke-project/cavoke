#include "roomview.h"
#include "ui_roomview.h"

RoomView::RoomView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::RoomView) {
    ui->setupUi(this);
//    connect(ui->roleComboBox, SIGNAL(currentIndexChanged(int)), this,
//            SLOT(repeaterCurrentIndexChanged(int)));
}

RoomView::~RoomView() {
    delete ui;
}

//void RoomView::repeaterCurrentIndexChanged(int index) {
//    if (index == -1 || ui->roleComboBox->itemData(index).toInt() == ourRole) {
//        //        displayEmpty();
//        return;
//    }
//    //    qDebug() << "Chosen: " << ui->roleComboBox->itemText(index);
//    qDebug() << "Got new role index: "
//             << ui->roleComboBox->itemData(index).toInt();
//    emit newRoleChosen(ui->roleComboBox->itemData(index).toInt());
//}

//void RoomView::updateStatus(CreatingGameStatus newStatus) {
//    ui->statusLabel->setText(STATUS.at(newStatus));
//    ui->currentPlayersHLabel->setHidden(newStatus != CreatingGameStatus::DONE);
//    ui->playersListWidget->setHidden(newStatus != CreatingGameStatus::DONE);
//}

//void RoomView::updateSessionInfo(const SessionInfo &sessionInfo) {
////    ui->inviteCodeLabel->setText(sessionInfo.invite_code); FIXME
//    //    ui->gameNameLabel->setText(sessionInfo.game_id);
//
//    ui->playersListWidget->clear();
//    for (const auto &player : sessionInfo.players) {
//        ui->playersListWidget->addItem(player.user.display_name);
//    }
//    if (sessionInfo.isHost) {
//        show_as_host();
//    } else {
//        show_as_guest();
//    }
//    if (sessionInfo.status == 1) {
//        this->close();
//        emit joinedCreatedGame();
//    }
//}

//void RoomView::updateValidationResult(
//    const ValidationResult &validationResult) {
//    ui->joinGameButton->setEnabled(validationResult.success);
//    ui->joinErrorLabel->setText(validationResult.message);
//}

//void RoomView::on_joinGameButton_clicked() {
//    emit createdGame();
//}

//void RoomView::clear() {
//    ui->currentPlayersHLabel->hide();
//    ui->playersListWidget->hide();
//
//    ui->inviteCodeLabel->setText("Unknown");
//    ui->gameNameLabel->setText("Unknown");
//}

void RoomView::on_backButton_clicked() {
//    this->clear();
    this->close();
//    emit leftRoom();
    emit shownStartView();
}
//void RoomView::updateGameName(const QString &gameName) {
//    ui->gameNameLabel->setText(gameName);
//}
//void RoomView::gotRolesListUpdate(const std::vector<Role> &newRolesList) {
//    // Some bad way to check whether list actually updated
//    // TODO: implement it
//
//    ourRole = newRolesList.front().id;
//    ui->roleComboBox->clear();
//    for (const auto &roleIdAndName : newRolesList) {
//        ui->roleComboBox->addItem(roleIdAndName.name, roleIdAndName.id);
//    }
//    if (ui->roleComboBox->count() > 0) {
//        ui->roleComboBox->setCurrentIndex(0);
//    } else {
//        ui->roleComboBox->setCurrentIndex(-1);
//    }
//}
void RoomView::show_as_host() {
//    ui->gameNameHLabel->hide();
//    ui->gameNameLabel->hide();
//    ui->inviteCodeHLabel->show();
//    ui->inviteCodeLabel->show();
//    ui->waitForHostLabel->hide();
//    ui->joinErrorLabel->show();
//    ui->joinGameButton->show();
}
void RoomView::show_as_guest() {
//    ui->gameNameHLabel->show();
//    ui->gameNameLabel->show();
//    ui->inviteCodeHLabel->hide();
//    ui->inviteCodeLabel->hide();
//    ui->waitForHostLabel->show();
//    ui->joinErrorLabel->hide();
//    ui->joinGameButton->hide();
}
