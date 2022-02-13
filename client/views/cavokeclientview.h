#ifndef CAVOKECLIENTVIEW_H
#define CAVOKECLIENTVIEW_H

#include <QMainWindow>

namespace Ui {
class CavokeClientView;
}

class CavokeClientView : public QMainWindow
{
    Q_OBJECT

signals:
    void startGame(const QString&);
    void shownStartView();
    void closedCavokeView();

public:
    explicit CavokeClientView(QWidget *parent = nullptr);
    ~CavokeClientView();

private slots:
    void on_runButton_clicked();
    void on_selectAppPathButton_clicked();
    void on_loadZipButton_clicked();
    void on_backButton_clicked();

private:
    Ui::CavokeClientView *ui;
};

#endif // CAVOKECLIENTVIEW_H
