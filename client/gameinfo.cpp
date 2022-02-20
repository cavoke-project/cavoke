#include "gameinfo.h"
#include <utility>
GameInfo::GameInfo() = default;
GameInfo::GameInfo(QString _id,
                   QString _display_name,
                   QString _description,
                   int _players_num)
    : id(std::move(_id)),
      display_name(std::move(_display_name)),
      description(std::move(_description)),
      players_num(_players_num) {
}
void GameInfo::read(const QJsonObject &json) {
    if (json.contains("id") && json["id"].isString())
        id = json["id"].toString();

    if (json.contains("display_name") && json["display_name"].isString())
        display_name = json["display_name"].toString();
    
    if (json.contains("description") && json["description"].isString())
        description = json["description"].toString();

    if (json.contains("players_num") && json["players_num"].isDouble())
        players_num = json["players_num"].toInt();
}
void GameInfo::write(QJsonObject &json) const {
    json["id"] = id;
    json["display_name"] = display_name;
    json["description"] = description;
    json["players_num"] = players_num;
}
