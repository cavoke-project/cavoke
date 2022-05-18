#include "settingsview.h"
#include "AuthenticationManager.h"
#include "ui_settingsview.h"

SettingsView::SettingsView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::SettingsView) {
    ui->setupUi(this);
}

SettingsView::~SettingsView() {
    delete ui;
}

void SettingsView::initStartValues(const QString &nickname,
                                   const QString &host) {
    ui->nicknameInput->setText(nickname);
    ui->serverAddressInput->setText(host);
}

void SettingsView::on_backButton_clicked() {
    this->close();
    emit shownStartView();
}
void SettingsView::on_updateSettingsButton_clicked() {
    emit updatedSettings(ui->nicknameInput->text(),
                         ui->serverAddressInput->text());
    this->close();
    emit shownStartView();
}
void SettingsView::on_reloginButton_clicked() {
    cavoke::auth::AuthenticationManager::getInstance().relogin();
}
