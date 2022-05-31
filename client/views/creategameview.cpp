#include "creategameview.h"
#include "authdialog.h"
#include "ui_creategameview.h"

CreateGameView::CreateGameView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::CreateGameView) {
    ui->setupUi(this);
}

CreateGameView::~CreateGameView() {
    delete ui;
}

void CreateGameView::on_backButton_clicked() {
    this->close();
    emit shownStartView();
}

void CreateGameView::on_createGameButton_clicked() {
    if (!AuthDialog::verifyAuth(this)) {
        return;
    }
    if (ui->roomNameEdit->text().isEmpty()) {
        return;
    }
    emit startedCreateGameRoutine(ui->roomNameEdit->text());
}
