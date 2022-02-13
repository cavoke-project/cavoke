#ifndef CAVOKECLIENTVIEW_H
#define CAVOKECLIENTVIEW_H

#include <QMainWindow>

namespace Ui {
class TestWindowView;
}

class TestWindowView : public QMainWindow
{
    Q_OBJECT
public:
    explicit TestWindowView(QWidget *parent = nullptr);
    ~TestWindowView();

signals:
    void startGame(const QString&);
    void shownStartView();

private slots:
    void on_runButton_clicked();
    void on_selectAppPathButton_clicked();
    void on_loadZipButton_clicked();
    void on_backButton_clicked();
private:
    Ui::TestWindowView *ui;
};

#endif // CAVOKECLIENTVIEW_H
