#include "startview.h"
#include "ui_startview.h"
#include "cavokeclientcontroller.h"

StartView::StartView(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::StartView) {
    ui->setupUi(this);
}

StartView::~StartView() {
    delete ui;
}

void StartView::on_cavokeTestScreenButton_clicked() {
    emit shownCavokeView();
    emit closedStartView();
}
