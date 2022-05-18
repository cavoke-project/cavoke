#include "authdialog.h"
#include "ui_authdialog.h"

AuthDialog::AuthDialog(QWidget *parent)
    : ui(new Ui::AuthDialog), QDialog(parent) {
    ui->setupUi(this);
}

AuthDialog::~AuthDialog() {
    delete ui;
}

void AuthDialog::on_acceptButton_clicked() {
    // connect authenticated status with dialog's successful closing
    connect(authManager, &cavoke::auth::AuthenticationManager::authenticated,
            this, &AuthDialog::accept);
    // authorize through browser
    authManager->oauth2.grant();
}

void AuthDialog::on_declineButton_clicked() {
    qWarning() << "User declined authorization! Unable to proceed.";
    QDialog::reject();
}

/// Auth guard for methods that require authentication (e.g. session creation)
/// @returns `true` if authenticated successfully, `false` otherwise
bool AuthDialog::verifyAuth(QWidget *parent) {
    auto *authManager = &cavoke::auth::AuthenticationManager::getInstance();
    // check if user already logged in
    if (authManager->checkAuthStatus()) {
        qDebug() << "Already authenticated";
        return true;
    }
    // otherwise launch the dialog and ask the user to log in
    return AuthDialog(parent).exec() == 1;
}