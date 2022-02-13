#include <QFileDialog>
#include "cavokeclientview.h"
#include "ui_cavokeclientview.h"
#include "cache_manager.h"

CavokeClientView::CavokeClientView(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::CavokeClientView) {
    ui->setupUi(this);
}

CavokeClientView::~CavokeClientView() {
    delete ui;
}

void CavokeClientView::on_selectAppPathButton_clicked() {
    QString appPath = QFileDialog::getOpenFileName(this, tr("Open App"), QDir::currentPath(),
                                                   tr("QML App (*.qml)"));
    if (!appPath.isNull()) {
        ui->appPathInput->setText(appPath);
    }
}

void CavokeClientView::on_runButton_clicked() {
    QString curAppPath = ui->appPathInput->text();
    if (!curAppPath.isNull()) {
        emit startGame(curAppPath);
    }
}

void CavokeClientView::on_loadZipButton_clicked() {
    QString zipPath = QFileDialog::getOpenFileName(this, tr("Select Archive"), QDir::currentPath(),
                                                   tr("ZIP Archive (*.zip)"));
    if (!zipPath.isNull()) {
        ui->appPathInput->setText(cache_manager::save_zip_to_cache(zipPath));
    }
}

void CavokeClientView::on_backButton_clicked() {
    this->close();
    emit shownStartView();
}
