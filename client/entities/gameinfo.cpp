#include "gameinfo.h"
#include <utility>
GameInfo::GameInfo() = default;
GameInfo::GameInfo(QString _id,
                   QString _display_name,
                   QString _description,
                   int _players_num,
                   QVector<QString> _role_names)
    : id(std::move(_id)),
      display_name(std::move(_display_name)),
      description(std::move(_description)),
      players_num(_players_num),
      role_names(std::move(_role_names)) {
}
void GameInfo::read(const QJsonObject &json) {
    if (json.contains(ID) && json[ID].isString())
        id = json[ID].toString();

    if (json.contains(DISPLAY_NAME) && json[DISPLAY_NAME].isString())
        display_name = json[DISPLAY_NAME].toString();

    if (json.contains(DESCRIPTION) && json[DESCRIPTION].isString())
        description = json[DESCRIPTION].toString();

    if (json.contains(PLAYERS_NUM) && json[PLAYERS_NUM].isDouble())
        players_num = json[PLAYERS_NUM].toInt();

    if (json.contains(ROLE_NAMES) && json[ROLE_NAMES].isArray()) {
        for (auto obj : json[ROLE_NAMES].toArray()) {
            // I failed to use std::transform with json[ROLE_NAMES].toArray()
            // cppcheck-suppress useStlAlgorithm
            role_names.push_back(obj.toString());
        }
    }
}
void GameInfo::write(QJsonObject &json) const {
    json[ID] = id;
    json[DISPLAY_NAME] = display_name;
    json[DESCRIPTION] = description;
    json[PLAYERS_NUM] = players_num;
    // Actually, write is not used
}
