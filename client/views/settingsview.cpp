#include "settingsview.h"
#include "ui_settingsview.h"

SettingsView::SettingsView(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::SettingsView) {
    ui->setupUi(this);
}

SettingsView::~SettingsView() {
    delete ui;
}

void SettingsView::on_backButton_clicked() {
    this->close();
    emit shownStartView();
}
