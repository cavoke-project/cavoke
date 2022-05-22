#ifndef AUTHDIALOG_H
#define AUTHDIALOG_H

#include <QDebug>
#include <QDialog>
#include "AuthenticationManager.h"

namespace Ui {
class AuthDialog;
}

/// Dialog that asks users to log in in order to use methods that require
/// authentication.
class AuthDialog : public QDialog {
    Q_OBJECT

public:
    explicit AuthDialog(QWidget *parent = nullptr);
    ~AuthDialog();

    static bool verifyAuth(QWidget *parent = nullptr);

private slots:
    void on_acceptButton_clicked();

    void on_declineButton_clicked();

private:
    Ui::AuthDialog *ui;
    cavoke::auth::AuthenticationManager *authManager =
        &cavoke::auth::AuthenticationManager::getInstance();
};

#endif  // AUTHDIALOG_H
