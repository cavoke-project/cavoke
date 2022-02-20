#ifndef CAVOKE_CLIENT_GAMEINFO_H
#define CAVOKE_CLIENT_GAMEINFO_H

#include <QtCore/QJsonObject>
#include <QtCore/QString>
struct GameInfo {
public:
    GameInfo();
    GameInfo(QString _id, QString _display_name, QString _description, int _players_num);

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

    QString id;
    QString display_name;
    QString description;
    int players_num = 0;
};

#endif  // CAVOKE_CLIENT_GAMEINFO_H
