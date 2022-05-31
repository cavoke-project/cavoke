#ifndef CAVOKE_CLIENT_CREATEGAMEVIEW_H
#define CAVOKE_CLIENT_CREATEGAMEVIEW_H

#include <QMainWindow>

namespace Ui {
class CreateGameView;
}

class CreateGameView : public QMainWindow {
    Q_OBJECT
public:
    explicit CreateGameView(QWidget *parent = nullptr);
    ~CreateGameView();

public slots:
signals:
    void shownStartView();
    void startedCreateGameRoutine(const QString &roomName);

private slots:
    void on_backButton_clicked();
    void on_createGameButton_clicked();

private:
    Ui::CreateGameView *ui;
};

#endif  // CAVOKE_CLIENT_CREATEGAMEVIEW_H
