#include "protoroomview.h"
#include "ui_protoroomview.h"

ProtoRoomView::ProtoRoomView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ProtoRoomView) {
    ui->setupUi(this);
    connect(ui->roleComboBox, SIGNAL(currentIndexChanged(int)), this,
            SLOT(repeaterCurrentIndexChanged(int)));
}

ProtoRoomView::~ProtoRoomView() {
    delete ui;
}

void ProtoRoomView::repeaterCurrentIndexChanged(int index) {
    if (index == -1 || ui->roleComboBox->itemData(index).toInt() == ourRole) {
        //        displayEmpty();
        return;
    }
    //    qDebug() << "Chosen: " << ui->roleComboBox->itemText(index);
    qDebug() << "Got new role index: "
             << ui->roleComboBox->itemData(index).toInt();
    emit newRoleChosen(ui->roleComboBox->itemData(index).toInt());
}

void ProtoRoomView::updateStatus(CreatingGameStatus newStatus) {
    ui->statusLabel->setText(STATUS.at(newStatus));
    ui->currentPlayersHLabel->setHidden(newStatus != CreatingGameStatus::DONE);
    ui->playersListWidget->setHidden(newStatus != CreatingGameStatus::DONE);
}

void ProtoRoomView::updateSessionInfo(const SessionInfo &sessionInfo) {
    ui->inviteCodeLabel->setText(sessionInfo.invite_code);
    //    ui->gameNameLabel->setText(sessionInfo.game_id);

    ui->playersListWidget->clear();
    int cnt = 0;
    for (const auto &player : sessionInfo.players) {
        if (cnt == 0) {
            ui->playersListWidget->addItem("Саша");
        } else if (cnt == 1) {
            ui->playersListWidget->addItem("Марк");
        } else {
            ui->playersListWidget->addItem(player.user_id);
        }
        cnt++;
    }
    if (sessionInfo.isHost) {
        show_as_host();
    } else {
        show_as_guest();
    }
    if (sessionInfo.status == 1) {
        this->close();
        emit joinedCreatedGame();
    }
}

void ProtoRoomView::updateValidationResult(
    const ValidationResult &validationResult) {
    ui->joinGameButton->setEnabled(validationResult.success);
    ui->joinErrorLabel->setText(validationResult.message);
}

void ProtoRoomView::on_joinGameButton_clicked() {
    emit createdGame();
}

void ProtoRoomView::clear() {
    ui->currentPlayersHLabel->hide();
    ui->playersListWidget->hide();

    ui->inviteCodeLabel->setText("Unknown");
    ui->gameNameLabel->setText("Unknown");
}

void ProtoRoomView::on_backButton_clicked() {
    this->clear();
    this->close();
    emit leftRoom();
    emit shownStartView();
}
void ProtoRoomView::updateGameName(const QString &gameName) {
    ui->gameNameLabel->setText(gameName);
}
void ProtoRoomView::gotRolesListUpdate(const std::vector<Role> &newRolesList) {
    // Some bad way to check whether list actually updated
    // TODO: implement it

    ourRole = newRolesList.front().id;
    ui->roleComboBox->clear();
    for (const auto &roleIdAndName : newRolesList) {
        ui->roleComboBox->addItem(roleIdAndName.name, roleIdAndName.id);
    }
    if (ui->roleComboBox->count() > 0) {
        ui->roleComboBox->setCurrentIndex(0);
    } else {
        ui->roleComboBox->setCurrentIndex(-1);
    }
}
void ProtoRoomView::show_as_host() {
    ui->gameNameHLabel->hide();
    ui->gameNameLabel->hide();
    ui->inviteCodeHLabel->show();
    ui->inviteCodeLabel->show();
    ui->waitForHostLabel->hide();
    ui->joinErrorLabel->show();
    ui->joinGameButton->show();
}
void ProtoRoomView::show_as_guest() {
    ui->gameNameHLabel->show();
    ui->gameNameLabel->show();
    ui->inviteCodeHLabel->hide();
    ui->inviteCodeLabel->hide();
    ui->waitForHostLabel->show();
    ui->joinErrorLabel->hide();
    ui->joinGameButton->hide();
}
