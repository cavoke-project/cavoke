#ifndef CLIENT_STARTVIEW_H
#define CLIENT_STARTVIEW_H

#include <QMainWindow>

namespace Ui {
    class StartView;
}

class StartView : public QMainWindow {
Q_OBJECT

public:
    explicit StartView(QWidget *parent = nullptr);

    ~StartView();

signals:
    void shownClientView();
    
private slots:
    void on_cavokeTestScreenButton_clicked();

private:
    Ui::StartView *ui;
};

#endif //CLIENT_STARTVIEW_H
