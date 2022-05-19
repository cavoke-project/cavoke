#ifndef CAVOKE_USER_H
#define CAVOKE_USER_H

#include <QtCore/QJsonObject>
#include <QtCore/QString>
struct User {
public:
    User();
    User(QString _display_name, QString _user_id);

    void read(const QJsonObject &json);
    static void write(QJsonObject &json);

    QString display_name;
    QString user_id;

private:
    static inline const QString DISPLAY_NAME = "display_name";
    static inline const QString USER_ID = "user_id";
};

#endif  // CAVOKE_USER_H
