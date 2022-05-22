#include "statisticsview.h"
#include <QFileDialog>
#include "authdialog.h"
#include "ui_statisticsview.h"

StatisticsView::StatisticsView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::StatisticsView) {
    ui->setupUi(this);
}

StatisticsView::~StatisticsView() {
    delete ui;
}

void StatisticsView::on_backButton_clicked() {
    this->close();
    emit shownStartView();
}

//void StatisticsView::on_StatisticsButton_clicked() {
//    QString inviteCode = ui->inviteCodeInput->text();
//    // verify user is authenticated
//    if (!AuthDialog::verifyAuth(this)) {
//        return;
//    }
//    if (!inviteCode.isEmpty()) {
//        emit joinedGame(inviteCode);
//    }
//}
