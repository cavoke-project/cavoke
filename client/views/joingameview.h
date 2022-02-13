#ifndef CAVOKE_CLIENT_JOINGAMEVIEW_H
#define CAVOKE_CLIENT_JOINGAMEVIEW_H

#include <QMainWindow>

namespace Ui {
    class JoinGameView;
}

class JoinGameView : public QMainWindow
{
Q_OBJECT
public:
    explicit JoinGameView(QWidget *parent = nullptr);
    ~JoinGameView();

signals:
    void shownStartView();

private slots:
    void on_backButton_clicked();
private:
    Ui::JoinGameView *ui;
};

#endif //CAVOKE_CLIENT_JOINGAMEVIEW_H
