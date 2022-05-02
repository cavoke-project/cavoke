#ifndef LOGINBOX_H
#define LOGINBOX_H

#include <QWidget>

namespace Ui {
class LoginBox;
}

/// Widget with information about current user
class LoginBox : public QWidget {
    Q_OBJECT

public:
    explicit LoginBox(QWidget *parent = nullptr);
    ~LoginBox();

private:
    Ui::LoginBox *ui;
};

#endif  // LOGINBOX_H
