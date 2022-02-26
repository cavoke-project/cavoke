#include "gameslistview.h"

#include "ui_gameslistview.h"

GamesListView::GamesListView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::GamesListView) {
    ui->setupUi(this);
}

GamesListView::~GamesListView() {
    delete ui;
}

void GamesListView::on_backButton_clicked() {
    this->close();
    emit shownStartView();
}
