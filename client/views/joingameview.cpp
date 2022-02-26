#include "joingameview.h"
#include <QFileDialog>
#include "ui_joingameview.h"

JoinGameView::JoinGameView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::JoinGameView) {
    ui->setupUi(this);
}

JoinGameView::~JoinGameView() {
    delete ui;
}

void JoinGameView::on_backButton_clicked() {
    this->close();
    emit shownStartView();
}

void JoinGameView::on_joinTicTacToeButton_clicked() {
    QString curAppPath = ui->appPathInput->text();
    if (!curAppPath.isEmpty()) {
        emit joinedTicTacToe(curAppPath);
        this->close();
    }
}

void JoinGameView::on_selectAppPathButton_clicked() {
    QString appPath = QFileDialog::getOpenFileName(
        this, tr("Open App"), QDir::currentPath(), tr("QML App (*.qml)"));
    if (!appPath.isNull()) {
        ui->appPathInput->setText(appPath);
    }
}
