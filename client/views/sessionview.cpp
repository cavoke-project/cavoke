#include "sessionview.h"
#include "ui_sessionview.h"

SessionView::SessionView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::SessionView) {
    ui->setupUi(this);
    connect(ui->roleComboBox, SIGNAL(currentIndexChanged(int)), this,
            SLOT(repeaterCurrentIndexChanged(int)));
}

SessionView::~SessionView() {
    delete ui;
}

void SessionView::repeaterCurrentIndexChanged(int index) {
    if (index == -1 || ui->roleComboBox->itemData(index).toInt() == ourRole) {
        //        displayEmpty();
        return;
    }
    //    qDebug() << "Chosen: " << ui->roleComboBox->itemText(index);
    qDebug() << "Got new role index: "
             << ui->roleComboBox->itemData(index).toInt();
    emit newRoleChosen(ui->roleComboBox->itemData(index).toInt());
}

void SessionView::updateStatus(CreatingGameStatus newStatus) {
    ui->statusLabel->setText(STATUS.at(newStatus));
    ui->currentPlayersHLabel->setHidden(newStatus != CreatingGameStatus::DONE);
    ui->playersListWidget->setHidden(newStatus != CreatingGameStatus::DONE);
}

void SessionView::updateSessionInfo(const SessionInfo &sessionInfo) {

    ui->playersListWidget->clear();
    for (const auto &player : sessionInfo.players) {
        ui->playersListWidget->addItem(player.user.display_name);
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

void SessionView::updateValidationResult(
    const ValidationResult &validationResult) {
    ui->startGameButton->setEnabled(validationResult.success);
    ui->joinErrorLabel->setText(validationResult.message);
}

void SessionView::on_startGameButton_clicked() {
    emit createdGame();
}

void SessionView::clear() {
    ui->currentPlayersHLabel->hide();
    ui->playersListWidget->hide();

    ui->gameNameLabel->setText("Unknown");
}

void SessionView::on_backButton_clicked() {
    this->clear();
    this->close();
    emit leftSession();
    emit shownRoomView();
}
void SessionView::updateGameName(const QString &gameName) {
    ui->gameNameLabel->setText(gameName);
}
void SessionView::gotRolesListUpdate(const std::vector<Role> &newRolesList) {
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
void SessionView::show_as_host() {
    ui->gameNameHLabel->hide();
    ui->gameNameLabel->hide();
    ui->waitForHostLabel->hide();
    ui->joinErrorLabel->show();
    ui->startGameButton->show();
}
void SessionView::show_as_guest() {
    ui->gameNameHLabel->show();
    ui->gameNameLabel->show();
    ui->waitForHostLabel->show();
    ui->joinErrorLabel->hide();
    ui->startGameButton->hide();
}
