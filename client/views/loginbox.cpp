#include "loginbox.h"
#include "ui_loginbox.h"

LoginBox::LoginBox(QWidget *parent) : QWidget(parent), ui(new Ui::LoginBox) {
    ui->setupUi(this);
}

LoginBox::~LoginBox() {
    delete ui;
}
