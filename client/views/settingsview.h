#ifndef CAVOKE_CLIENT_SETTINGSVIEW_H
#define CAVOKE_CLIENT_SETTINGSVIEW_H

#include <QMainWindow>

namespace Ui {
    class SettingsView;
}

class SettingsView : public QMainWindow
{
Q_OBJECT
public:
    explicit SettingsView(QWidget *parent = nullptr);
    ~SettingsView();

signals:
    void shownStartView();

private slots:
    void on_backButton_clicked();
private:
    Ui::SettingsView *ui;
};

#endif //CAVOKE_CLIENT_SETTINGSVIEW_H
