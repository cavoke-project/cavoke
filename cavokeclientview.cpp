#include "cavokeclientview.h"
#include "ui_cavokeclientview.h"

CavokeClientView::CavokeClientView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CavokeClientView) {
    ui->setupUi(this);
}

CavokeClientView::~CavokeClientView() {
    delete ui;
}

void CavokeClientView::on_tictactoebutton_clicked() {
    // TODO: in the long-term probably not a string, but some config object
    emit startGame("tictactoe");
}
