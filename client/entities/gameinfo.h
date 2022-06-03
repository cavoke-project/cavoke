#ifndef CAVOKE_CLIENT_GAMEINFO_H
#define CAVOKE_CLIENT_GAMEINFO_H

#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QString>
struct GameInfo {
public:
    GameInfo();
    GameInfo(QString _id,
             QString _display_name,
             QString _description,
             int _players_num,
             QVector<QString> _role_names);

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

    QString id;
    QString display_name;
    QString description;
    int players_num = 0;
    QVector<QString> role_names;

private:
    static inline const QString ID = "id";
    static inline const QString DISPLAY_NAME = "display_name";
    static inline const QString DESCRIPTION = "description";
    static inline const QString PLAYERS_NUM = "players_num";
    static inline const QString ROLE_NAMES = "role_names";
};

#endif  // CAVOKE_CLIENT_GAMEINFO_H
