#ifndef CAVOKE_CLIENT_SETTINGSVIEW_H
#define CAVOKE_CLIENT_SETTINGSVIEW_H

#include <QMainWindow>

namespace Ui {
class SettingsView;
}

class SettingsView : public QMainWindow {
    Q_OBJECT
public:
    explicit SettingsView(QWidget *parent = nullptr);
    ~SettingsView();

public slots:
    void initStartValues(const QString &nickname, const QString &host);

signals:
    void shownStartView();
    void updatedSettings(const QString &nickname, const QString &host);

private slots:
    void on_backButton_clicked();
    void on_updateSettingsButton_clicked();
    void on_reloginButton_clicked();

private:
    Ui::SettingsView *ui;
};

#endif  // CAVOKE_CLIENT_SETTINGSVIEW_H
