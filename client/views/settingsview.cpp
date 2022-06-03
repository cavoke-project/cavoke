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

void SettingsView::initStartValues(const QString &displayName,
                                   const QString &host) {
    ui->nicknameInput->setText(displayName);
    oldDisplayName = displayName;
    ui->serverAddressInput->setText(host);
}

void SettingsView::updateDisplayName(const QString &displayName) {
    ui->nicknameInput->setText(displayName);
    oldDisplayName = displayName;
}

void SettingsView::on_backButton_clicked() {
    this->close();
    emit shownStartView();
}
void SettingsView::on_updateSettingsButton_clicked() {
    if (ui->nicknameInput->text() != oldDisplayName) {
        if (!AuthDialog::verifyAuth(this)) {
            return;
        }
    }
    emit updatedSettings(ui->nicknameInput->text(),
                         ui->serverAddressInput->text());
    this->close();
    emit shownStartView();
}
void SettingsView::on_reloginButton_clicked() {
    cavoke::auth::AuthenticationManager::getInstance().relogin();
}
