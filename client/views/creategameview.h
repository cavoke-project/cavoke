#ifndef CAVOKE_CLIENT_CREATEGAMEVIEW_H
#define CAVOKE_CLIENT_CREATEGAMEVIEW_H

#include <QMainWindow>

namespace Ui {
    class CreateGameView;
}

class CreateGameView : public QMainWindow
{
Q_OBJECT
public:
    explicit CreateGameView(QWidget *parent = nullptr);
    ~CreateGameView();

signals:
    void shownStartView();

private slots:
    void on_backButton_clicked();
private:
    Ui::CreateGameView *ui;
};

#endif //CAVOKE_CLIENT_CREATEGAMEVIEW_H
