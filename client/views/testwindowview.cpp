#include <QFileDialog>
#include "testwindowview.h"
#include "ui_testwindowview.h"
#include "cache_manager.h"

TestWindowView::TestWindowView(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::TestWindowView) {
    ui->setupUi(this);
}

TestWindowView::~TestWindowView() {
    delete ui;
}

void TestWindowView::on_runButton_clicked() {
    QString curAppPath = ui->appPathInput->text();
    if (!curAppPath.isNull()) {
        emit startGame(curAppPath);
    }
}

void TestWindowView::on_selectAppPathButton_clicked() {
    QString appPath = QFileDialog::getOpenFileName(this, tr("Open App"), QDir::currentPath(),
                                                   tr("QML App (*.qml)"));
    if (!appPath.isNull()) {
        ui->appPathInput->setText(appPath);
    }
}

void TestWindowView::on_loadZipButton_clicked() {
    QString zipPath = QFileDialog::getOpenFileName(this, tr("Select Archive"), QDir::currentPath(),
                                                   tr("ZIP Archive (*.zip)"));
    if (!zipPath.isNull()) {
        ui->appPathInput->setText(cache_manager::save_zip_to_cache(zipPath));
    }
}

void TestWindowView::on_backButton_clicked() {
    this->close();
    emit shownStartView();
}
