/**
 *
 *  Sessions.cc
 *  DO NOT EDIT. This file is generated by drogon_ctl
 *
 */

#include "Sessions.h"
#include <drogon/utils/Utilities.h>
#include <string>
#include "Players.h"

using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::cavoke;

const std::string Sessions::Cols::_id = "id";
const std::string Sessions::Cols::_game_id = "game_id";
const std::string Sessions::Cols::_invite_code = "invite_code";
const std::string Sessions::Cols::_host_id = "host_id";
const std::string Sessions::Cols::_game_settings = "game_settings";
const std::string Sessions::Cols::_status = "status";
const std::string Sessions::primaryKeyName = "id";
const bool Sessions::hasPrimaryKey = true;
const std::string Sessions::tableName = "sessions";

const std::vector<typename Sessions::MetaData> Sessions::metaData_ = {
    {"id", "std::string", "uuid", 0, 0, 1, 1},
    {"game_id", "std::string", "character varying", 0, 0, 0, 1},
    {"invite_code", "std::string", "character varying", 0, 0, 0, 1},
    {"host_id", "std::string", "uuid", 0, 0, 0, 0},
    {"game_settings", "std::string", "json", 0, 0, 0, 0},
    {"status", "int32_t", "integer", 4, 0, 0, 0}};
const std::string &Sessions::getColumnName(size_t index) noexcept(false) {
    assert(index < metaData_.size());
    return metaData_[index].colName_;
}
Sessions::Sessions(const Row &r, const ssize_t indexOffset) noexcept {
    if (indexOffset < 0) {
        if (!r["id"].isNull()) {
            id_ = std::make_shared<std::string>(r["id"].as<std::string>());
        }
        if (!r["game_id"].isNull()) {
            gameId_ =
                std::make_shared<std::string>(r["game_id"].as<std::string>());
        }
        if (!r["invite_code"].isNull()) {
            inviteCode_ = std::make_shared<std::string>(
                r["invite_code"].as<std::string>());
        }
        if (!r["host_id"].isNull()) {
            hostId_ =
                std::make_shared<std::string>(r["host_id"].as<std::string>());
        }
        if (!r["game_settings"].isNull()) {
            gameSettings_ = std::make_shared<std::string>(
                r["game_settings"].as<std::string>());
        }
        if (!r["status"].isNull()) {
            status_ = std::make_shared<int32_t>(r["status"].as<int32_t>());
        }
    } else {
        size_t offset = (size_t)indexOffset;
        if (offset + 6 > r.size()) {
            LOG_FATAL << "Invalid SQL result for this model";
            return;
        }
        size_t index;
        index = offset + 0;
        if (!r[index].isNull()) {
            id_ = std::make_shared<std::string>(r[index].as<std::string>());
        }
        index = offset + 1;
        if (!r[index].isNull()) {
            gameId_ = std::make_shared<std::string>(r[index].as<std::string>());
        }
        index = offset + 2;
        if (!r[index].isNull()) {
            inviteCode_ =
                std::make_shared<std::string>(r[index].as<std::string>());
        }
        index = offset + 3;
        if (!r[index].isNull()) {
            hostId_ = std::make_shared<std::string>(r[index].as<std::string>());
        }
        index = offset + 4;
        if (!r[index].isNull()) {
            gameSettings_ =
                std::make_shared<std::string>(r[index].as<std::string>());
        }
        index = offset + 5;
        if (!r[index].isNull()) {
            status_ = std::make_shared<int32_t>(r[index].as<int32_t>());
        }
    }
}

Sessions::Sessions(
    const Json::Value &pJson,
    const std::vector<std::string> &pMasqueradingVector) noexcept(false) {
    if (pMasqueradingVector.size() != 6) {
        LOG_ERROR << "Bad masquerading vector";
        return;
    }
    if (!pMasqueradingVector[0].empty() &&
        pJson.isMember(pMasqueradingVector[0])) {
        dirtyFlag_[0] = true;
        if (!pJson[pMasqueradingVector[0]].isNull()) {
            id_ = std::make_shared<std::string>(
                pJson[pMasqueradingVector[0]].asString());
        }
    }
    if (!pMasqueradingVector[1].empty() &&
        pJson.isMember(pMasqueradingVector[1])) {
        dirtyFlag_[1] = true;
        if (!pJson[pMasqueradingVector[1]].isNull()) {
            gameId_ = std::make_shared<std::string>(
                pJson[pMasqueradingVector[1]].asString());
        }
    }
    if (!pMasqueradingVector[2].empty() &&
        pJson.isMember(pMasqueradingVector[2])) {
        dirtyFlag_[2] = true;
        if (!pJson[pMasqueradingVector[2]].isNull()) {
            inviteCode_ = std::make_shared<std::string>(
                pJson[pMasqueradingVector[2]].asString());
        }
    }
    if (!pMasqueradingVector[3].empty() &&
        pJson.isMember(pMasqueradingVector[3])) {
        dirtyFlag_[3] = true;
        if (!pJson[pMasqueradingVector[3]].isNull()) {
            hostId_ = std::make_shared<std::string>(
                pJson[pMasqueradingVector[3]].asString());
        }
    }
    if (!pMasqueradingVector[4].empty() &&
        pJson.isMember(pMasqueradingVector[4])) {
        dirtyFlag_[4] = true;
        if (!pJson[pMasqueradingVector[4]].isNull()) {
            gameSettings_ = std::make_shared<std::string>(
                pJson[pMasqueradingVector[4]].asString());
        }
    }
    if (!pMasqueradingVector[5].empty() &&
        pJson.isMember(pMasqueradingVector[5])) {
        dirtyFlag_[5] = true;
        if (!pJson[pMasqueradingVector[5]].isNull()) {
            status_ = std::make_shared<int32_t>(
                (int32_t)pJson[pMasqueradingVector[5]].asInt64());
        }
    }
}

Sessions::Sessions(const Json::Value &pJson) noexcept(false) {
    if (pJson.isMember("id")) {
        dirtyFlag_[0] = true;
        if (!pJson["id"].isNull()) {
            id_ = std::make_shared<std::string>(pJson["id"].asString());
        }
    }
    if (pJson.isMember("game_id")) {
        dirtyFlag_[1] = true;
        if (!pJson["game_id"].isNull()) {
            gameId_ =
                std::make_shared<std::string>(pJson["game_id"].asString());
        }
    }
    if (pJson.isMember("invite_code")) {
        dirtyFlag_[2] = true;
        if (!pJson["invite_code"].isNull()) {
            inviteCode_ =
                std::make_shared<std::string>(pJson["invite_code"].asString());
        }
    }
    if (pJson.isMember("host_id")) {
        dirtyFlag_[3] = true;
        if (!pJson["host_id"].isNull()) {
            hostId_ =
                std::make_shared<std::string>(pJson["host_id"].asString());
        }
    }
    if (pJson.isMember("game_settings")) {
        dirtyFlag_[4] = true;
        if (!pJson["game_settings"].isNull()) {
            gameSettings_ = std::make_shared<std::string>(
                pJson["game_settings"].asString());
        }
    }
    if (pJson.isMember("status")) {
        dirtyFlag_[5] = true;
        if (!pJson["status"].isNull()) {
            status_ =
                std::make_shared<int32_t>((int32_t)pJson["status"].asInt64());
        }
    }
}

void Sessions::updateByMasqueradedJson(
    const Json::Value &pJson,
    const std::vector<std::string> &pMasqueradingVector) noexcept(false) {
    if (pMasqueradingVector.size() != 6) {
        LOG_ERROR << "Bad masquerading vector";
        return;
    }
    if (!pMasqueradingVector[0].empty() &&
        pJson.isMember(pMasqueradingVector[0])) {
        if (!pJson[pMasqueradingVector[0]].isNull()) {
            id_ = std::make_shared<std::string>(
                pJson[pMasqueradingVector[0]].asString());
        }
    }
    if (!pMasqueradingVector[1].empty() &&
        pJson.isMember(pMasqueradingVector[1])) {
        dirtyFlag_[1] = true;
        if (!pJson[pMasqueradingVector[1]].isNull()) {
            gameId_ = std::make_shared<std::string>(
                pJson[pMasqueradingVector[1]].asString());
        }
    }
    if (!pMasqueradingVector[2].empty() &&
        pJson.isMember(pMasqueradingVector[2])) {
        dirtyFlag_[2] = true;
        if (!pJson[pMasqueradingVector[2]].isNull()) {
            inviteCode_ = std::make_shared<std::string>(
                pJson[pMasqueradingVector[2]].asString());
        }
    }
    if (!pMasqueradingVector[3].empty() &&
        pJson.isMember(pMasqueradingVector[3])) {
        dirtyFlag_[3] = true;
        if (!pJson[pMasqueradingVector[3]].isNull()) {
            hostId_ = std::make_shared<std::string>(
                pJson[pMasqueradingVector[3]].asString());
        }
    }
    if (!pMasqueradingVector[4].empty() &&
        pJson.isMember(pMasqueradingVector[4])) {
        dirtyFlag_[4] = true;
        if (!pJson[pMasqueradingVector[4]].isNull()) {
            gameSettings_ = std::make_shared<std::string>(
                pJson[pMasqueradingVector[4]].asString());
        }
    }
    if (!pMasqueradingVector[5].empty() &&
        pJson.isMember(pMasqueradingVector[5])) {
        dirtyFlag_[5] = true;
        if (!pJson[pMasqueradingVector[5]].isNull()) {
            status_ = std::make_shared<int32_t>(
                (int32_t)pJson[pMasqueradingVector[5]].asInt64());
        }
    }
}

void Sessions::updateByJson(const Json::Value &pJson) noexcept(false) {
    if (pJson.isMember("id")) {
        if (!pJson["id"].isNull()) {
            id_ = std::make_shared<std::string>(pJson["id"].asString());
        }
    }
    if (pJson.isMember("game_id")) {
        dirtyFlag_[1] = true;
        if (!pJson["game_id"].isNull()) {
            gameId_ =
                std::make_shared<std::string>(pJson["game_id"].asString());
        }
    }
    if (pJson.isMember("invite_code")) {
        dirtyFlag_[2] = true;
        if (!pJson["invite_code"].isNull()) {
            inviteCode_ =
                std::make_shared<std::string>(pJson["invite_code"].asString());
        }
    }
    if (pJson.isMember("host_id")) {
        dirtyFlag_[3] = true;
        if (!pJson["host_id"].isNull()) {
            hostId_ =
                std::make_shared<std::string>(pJson["host_id"].asString());
        }
    }
    if (pJson.isMember("game_settings")) {
        dirtyFlag_[4] = true;
        if (!pJson["game_settings"].isNull()) {
            gameSettings_ = std::make_shared<std::string>(
                pJson["game_settings"].asString());
        }
    }
    if (pJson.isMember("status")) {
        dirtyFlag_[5] = true;
        if (!pJson["status"].isNull()) {
            status_ =
                std::make_shared<int32_t>((int32_t)pJson["status"].asInt64());
        }
    }
}

const std::string &Sessions::getValueOfId() const noexcept {
    const static std::string defaultValue = std::string();
    if (id_)
        return *id_;
    return defaultValue;
}
const std::shared_ptr<std::string> &Sessions::getId() const noexcept {
    return id_;
}
void Sessions::setId(const std::string &pId) noexcept {
    id_ = std::make_shared<std::string>(pId);
    dirtyFlag_[0] = true;
}
void Sessions::setId(std::string &&pId) noexcept {
    id_ = std::make_shared<std::string>(std::move(pId));
    dirtyFlag_[0] = true;
}
const typename Sessions::PrimaryKeyType &Sessions::getPrimaryKey() const {
    assert(id_);
    return *id_;
}

const std::string &Sessions::getValueOfGameId() const noexcept {
    const static std::string defaultValue = std::string();
    if (gameId_)
        return *gameId_;
    return defaultValue;
}
const std::shared_ptr<std::string> &Sessions::getGameId() const noexcept {
    return gameId_;
}
void Sessions::setGameId(const std::string &pGameId) noexcept {
    gameId_ = std::make_shared<std::string>(pGameId);
    dirtyFlag_[1] = true;
}
void Sessions::setGameId(std::string &&pGameId) noexcept {
    gameId_ = std::make_shared<std::string>(std::move(pGameId));
    dirtyFlag_[1] = true;
}

const std::string &Sessions::getValueOfInviteCode() const noexcept {
    const static std::string defaultValue = std::string();
    if (inviteCode_)
        return *inviteCode_;
    return defaultValue;
}
const std::shared_ptr<std::string> &Sessions::getInviteCode() const noexcept {
    return inviteCode_;
}
void Sessions::setInviteCode(const std::string &pInviteCode) noexcept {
    inviteCode_ = std::make_shared<std::string>(pInviteCode);
    dirtyFlag_[2] = true;
}
void Sessions::setInviteCode(std::string &&pInviteCode) noexcept {
    inviteCode_ = std::make_shared<std::string>(std::move(pInviteCode));
    dirtyFlag_[2] = true;
}

const std::string &Sessions::getValueOfHostId() const noexcept {
    const static std::string defaultValue = std::string();
    if (hostId_)
        return *hostId_;
    return defaultValue;
}
const std::shared_ptr<std::string> &Sessions::getHostId() const noexcept {
    return hostId_;
}
void Sessions::setHostId(const std::string &pHostId) noexcept {
    hostId_ = std::make_shared<std::string>(pHostId);
    dirtyFlag_[3] = true;
}
void Sessions::setHostId(std::string &&pHostId) noexcept {
    hostId_ = std::make_shared<std::string>(std::move(pHostId));
    dirtyFlag_[3] = true;
}
void Sessions::setHostIdToNull() noexcept {
    hostId_.reset();
    dirtyFlag_[3] = true;
}

const std::string &Sessions::getValueOfGameSettings() const noexcept {
    const static std::string defaultValue = std::string();
    if (gameSettings_)
        return *gameSettings_;
    return defaultValue;
}
const std::shared_ptr<std::string> &Sessions::getGameSettings() const noexcept {
    return gameSettings_;
}
void Sessions::setGameSettings(const std::string &pGameSettings) noexcept {
    gameSettings_ = std::make_shared<std::string>(pGameSettings);
    dirtyFlag_[4] = true;
}
void Sessions::setGameSettings(std::string &&pGameSettings) noexcept {
    gameSettings_ = std::make_shared<std::string>(std::move(pGameSettings));
    dirtyFlag_[4] = true;
}
void Sessions::setGameSettingsToNull() noexcept {
    gameSettings_.reset();
    dirtyFlag_[4] = true;
}

const int32_t &Sessions::getValueOfStatus() const noexcept {
    const static int32_t defaultValue = int32_t();
    if (status_)
        return *status_;
    return defaultValue;
}
const std::shared_ptr<int32_t> &Sessions::getStatus() const noexcept {
    return status_;
}
void Sessions::setStatus(const int32_t &pStatus) noexcept {
    status_ = std::make_shared<int32_t>(pStatus);
    dirtyFlag_[5] = true;
}
void Sessions::setStatusToNull() noexcept {
    status_.reset();
    dirtyFlag_[5] = true;
}

void Sessions::updateId(const uint64_t id) {
}

const std::vector<std::string> &Sessions::insertColumns() noexcept {
    static const std::vector<std::string> inCols = {
        "id", "game_id", "invite_code", "host_id", "game_settings", "status"};
    return inCols;
}

void Sessions::outputArgs(drogon::orm::internal::SqlBinder &binder) const {
    if (dirtyFlag_[0]) {
        if (getId()) {
            binder << getValueOfId();
        } else {
            binder << nullptr;
        }
    }
    if (dirtyFlag_[1]) {
        if (getGameId()) {
            binder << getValueOfGameId();
        } else {
            binder << nullptr;
        }
    }
    if (dirtyFlag_[2]) {
        if (getInviteCode()) {
            binder << getValueOfInviteCode();
        } else {
            binder << nullptr;
        }
    }
    if (dirtyFlag_[3]) {
        if (getHostId()) {
            binder << getValueOfHostId();
        } else {
            binder << nullptr;
        }
    }
    if (dirtyFlag_[4]) {
        if (getGameSettings()) {
            binder << getValueOfGameSettings();
        } else {
            binder << nullptr;
        }
    }
    if (dirtyFlag_[5]) {
        if (getStatus()) {
            binder << getValueOfStatus();
        } else {
            binder << nullptr;
        }
    }
}

const std::vector<std::string> Sessions::updateColumns() const {
    std::vector<std::string> ret;
    if (dirtyFlag_[0]) {
        ret.push_back(getColumnName(0));
    }
    if (dirtyFlag_[1]) {
        ret.push_back(getColumnName(1));
    }
    if (dirtyFlag_[2]) {
        ret.push_back(getColumnName(2));
    }
    if (dirtyFlag_[3]) {
        ret.push_back(getColumnName(3));
    }
    if (dirtyFlag_[4]) {
        ret.push_back(getColumnName(4));
    }
    if (dirtyFlag_[5]) {
        ret.push_back(getColumnName(5));
    }
    return ret;
}

void Sessions::updateArgs(drogon::orm::internal::SqlBinder &binder) const {
    if (dirtyFlag_[0]) {
        if (getId()) {
            binder << getValueOfId();
        } else {
            binder << nullptr;
        }
    }
    if (dirtyFlag_[1]) {
        if (getGameId()) {
            binder << getValueOfGameId();
        } else {
            binder << nullptr;
        }
    }
    if (dirtyFlag_[2]) {
        if (getInviteCode()) {
            binder << getValueOfInviteCode();
        } else {
            binder << nullptr;
        }
    }
    if (dirtyFlag_[3]) {
        if (getHostId()) {
            binder << getValueOfHostId();
        } else {
            binder << nullptr;
        }
    }
    if (dirtyFlag_[4]) {
        if (getGameSettings()) {
            binder << getValueOfGameSettings();
        } else {
            binder << nullptr;
        }
    }
    if (dirtyFlag_[5]) {
        if (getStatus()) {
            binder << getValueOfStatus();
        } else {
            binder << nullptr;
        }
    }
}
Json::Value Sessions::toJson() const {
    Json::Value ret;
    if (getId()) {
        ret["id"] = getValueOfId();
    } else {
        ret["id"] = Json::Value();
    }
    if (getGameId()) {
        ret["game_id"] = getValueOfGameId();
    } else {
        ret["game_id"] = Json::Value();
    }
    if (getInviteCode()) {
        ret["invite_code"] = getValueOfInviteCode();
    } else {
        ret["invite_code"] = Json::Value();
    }
    if (getHostId()) {
        ret["host_id"] = getValueOfHostId();
    } else {
        ret["host_id"] = Json::Value();
    }
    if (getGameSettings()) {
        ret["game_settings"] = getValueOfGameSettings();
    } else {
        ret["game_settings"] = Json::Value();
    }
    if (getStatus()) {
        ret["status"] = getValueOfStatus();
    } else {
        ret["status"] = Json::Value();
    }
    return ret;
}

Json::Value Sessions::toMasqueradedJson(
    const std::vector<std::string> &pMasqueradingVector) const {
    Json::Value ret;
    if (pMasqueradingVector.size() == 6) {
        if (!pMasqueradingVector[0].empty()) {
            if (getId()) {
                ret[pMasqueradingVector[0]] = getValueOfId();
            } else {
                ret[pMasqueradingVector[0]] = Json::Value();
            }
        }
        if (!pMasqueradingVector[1].empty()) {
            if (getGameId()) {
                ret[pMasqueradingVector[1]] = getValueOfGameId();
            } else {
                ret[pMasqueradingVector[1]] = Json::Value();
            }
        }
        if (!pMasqueradingVector[2].empty()) {
            if (getInviteCode()) {
                ret[pMasqueradingVector[2]] = getValueOfInviteCode();
            } else {
                ret[pMasqueradingVector[2]] = Json::Value();
            }
        }
        if (!pMasqueradingVector[3].empty()) {
            if (getHostId()) {
                ret[pMasqueradingVector[3]] = getValueOfHostId();
            } else {
                ret[pMasqueradingVector[3]] = Json::Value();
            }
        }
        if (!pMasqueradingVector[4].empty()) {
            if (getGameSettings()) {
                ret[pMasqueradingVector[4]] = getValueOfGameSettings();
            } else {
                ret[pMasqueradingVector[4]] = Json::Value();
            }
        }
        if (!pMasqueradingVector[5].empty()) {
            if (getStatus()) {
                ret[pMasqueradingVector[5]] = getValueOfStatus();
            } else {
                ret[pMasqueradingVector[5]] = Json::Value();
            }
        }
        return ret;
    }
    LOG_ERROR << "Masquerade failed";
    if (getId()) {
        ret["id"] = getValueOfId();
    } else {
        ret["id"] = Json::Value();
    }
    if (getGameId()) {
        ret["game_id"] = getValueOfGameId();
    } else {
        ret["game_id"] = Json::Value();
    }
    if (getInviteCode()) {
        ret["invite_code"] = getValueOfInviteCode();
    } else {
        ret["invite_code"] = Json::Value();
    }
    if (getHostId()) {
        ret["host_id"] = getValueOfHostId();
    } else {
        ret["host_id"] = Json::Value();
    }
    if (getGameSettings()) {
        ret["game_settings"] = getValueOfGameSettings();
    } else {
        ret["game_settings"] = Json::Value();
    }
    if (getStatus()) {
        ret["status"] = getValueOfStatus();
    } else {
        ret["status"] = Json::Value();
    }
    return ret;
}

bool Sessions::validateJsonForCreation(const Json::Value &pJson,
                                       std::string &err) {
    if (pJson.isMember("id")) {
        if (!validJsonOfField(0, "id", pJson["id"], err, true))
            return false;
    } else {
        err = "The id column cannot be null";
        return false;
    }
    if (pJson.isMember("game_id")) {
        if (!validJsonOfField(1, "game_id", pJson["game_id"], err, true))
            return false;
    } else {
        err = "The game_id column cannot be null";
        return false;
    }
    if (pJson.isMember("invite_code")) {
        if (!validJsonOfField(2, "invite_code", pJson["invite_code"], err,
                              true))
            return false;
    } else {
        err = "The invite_code column cannot be null";
        return false;
    }
    if (pJson.isMember("host_id")) {
        if (!validJsonOfField(3, "host_id", pJson["host_id"], err, true))
            return false;
    }
    if (pJson.isMember("game_settings")) {
        if (!validJsonOfField(4, "game_settings", pJson["game_settings"], err,
                              true))
            return false;
    }
    if (pJson.isMember("status")) {
        if (!validJsonOfField(5, "status", pJson["status"], err, true))
            return false;
    }
    return true;
}
bool Sessions::validateMasqueradedJsonForCreation(
    const Json::Value &pJson,
    const std::vector<std::string> &pMasqueradingVector,
    std::string &err) {
    if (pMasqueradingVector.size() != 6) {
        err = "Bad masquerading vector";
        return false;
    }
    try {
        if (!pMasqueradingVector[0].empty()) {
            if (pJson.isMember(pMasqueradingVector[0])) {
                if (!validJsonOfField(0, pMasqueradingVector[0],
                                      pJson[pMasqueradingVector[0]], err, true))
                    return false;
            } else {
                err =
                    "The " + pMasqueradingVector[0] + " column cannot be null";
                return false;
            }
        }
        if (!pMasqueradingVector[1].empty()) {
            if (pJson.isMember(pMasqueradingVector[1])) {
                if (!validJsonOfField(1, pMasqueradingVector[1],
                                      pJson[pMasqueradingVector[1]], err, true))
                    return false;
            } else {
                err =
                    "The " + pMasqueradingVector[1] + " column cannot be null";
                return false;
            }
        }
        if (!pMasqueradingVector[2].empty()) {
            if (pJson.isMember(pMasqueradingVector[2])) {
                if (!validJsonOfField(2, pMasqueradingVector[2],
                                      pJson[pMasqueradingVector[2]], err, true))
                    return false;
            } else {
                err =
                    "The " + pMasqueradingVector[2] + " column cannot be null";
                return false;
            }
        }
        if (!pMasqueradingVector[3].empty()) {
            if (pJson.isMember(pMasqueradingVector[3])) {
                if (!validJsonOfField(3, pMasqueradingVector[3],
                                      pJson[pMasqueradingVector[3]], err, true))
                    return false;
            }
        }
        if (!pMasqueradingVector[4].empty()) {
            if (pJson.isMember(pMasqueradingVector[4])) {
                if (!validJsonOfField(4, pMasqueradingVector[4],
                                      pJson[pMasqueradingVector[4]], err, true))
                    return false;
            }
        }
        if (!pMasqueradingVector[5].empty()) {
            if (pJson.isMember(pMasqueradingVector[5])) {
                if (!validJsonOfField(5, pMasqueradingVector[5],
                                      pJson[pMasqueradingVector[5]], err, true))
                    return false;
            }
        }
    } catch (const Json::LogicError &e) {
        err = e.what();
        return false;
    }
    return true;
}
bool Sessions::validateJsonForUpdate(const Json::Value &pJson,
                                     std::string &err) {
    if (pJson.isMember("id")) {
        if (!validJsonOfField(0, "id", pJson["id"], err, false))
            return false;
    } else {
        err =
            "The value of primary key must be set in the json object for "
            "update";
        return false;
    }
    if (pJson.isMember("game_id")) {
        if (!validJsonOfField(1, "game_id", pJson["game_id"], err, false))
            return false;
    }
    if (pJson.isMember("invite_code")) {
        if (!validJsonOfField(2, "invite_code", pJson["invite_code"], err,
                              false))
            return false;
    }
    if (pJson.isMember("host_id")) {
        if (!validJsonOfField(3, "host_id", pJson["host_id"], err, false))
            return false;
    }
    if (pJson.isMember("game_settings")) {
        if (!validJsonOfField(4, "game_settings", pJson["game_settings"], err,
                              false))
            return false;
    }
    if (pJson.isMember("status")) {
        if (!validJsonOfField(5, "status", pJson["status"], err, false))
            return false;
    }
    return true;
}
bool Sessions::validateMasqueradedJsonForUpdate(
    const Json::Value &pJson,
    const std::vector<std::string> &pMasqueradingVector,
    std::string &err) {
    if (pMasqueradingVector.size() != 6) {
        err = "Bad masquerading vector";
        return false;
    }
    try {
        if (!pMasqueradingVector[0].empty() &&
            pJson.isMember(pMasqueradingVector[0])) {
            if (!validJsonOfField(0, pMasqueradingVector[0],
                                  pJson[pMasqueradingVector[0]], err, false))
                return false;
        } else {
            err =
                "The value of primary key must be set in the json object for "
                "update";
            return false;
        }
        if (!pMasqueradingVector[1].empty() &&
            pJson.isMember(pMasqueradingVector[1])) {
            if (!validJsonOfField(1, pMasqueradingVector[1],
                                  pJson[pMasqueradingVector[1]], err, false))
                return false;
        }
        if (!pMasqueradingVector[2].empty() &&
            pJson.isMember(pMasqueradingVector[2])) {
            if (!validJsonOfField(2, pMasqueradingVector[2],
                                  pJson[pMasqueradingVector[2]], err, false))
                return false;
        }
        if (!pMasqueradingVector[3].empty() &&
            pJson.isMember(pMasqueradingVector[3])) {
            if (!validJsonOfField(3, pMasqueradingVector[3],
                                  pJson[pMasqueradingVector[3]], err, false))
                return false;
        }
        if (!pMasqueradingVector[4].empty() &&
            pJson.isMember(pMasqueradingVector[4])) {
            if (!validJsonOfField(4, pMasqueradingVector[4],
                                  pJson[pMasqueradingVector[4]], err, false))
                return false;
        }
        if (!pMasqueradingVector[5].empty() &&
            pJson.isMember(pMasqueradingVector[5])) {
            if (!validJsonOfField(5, pMasqueradingVector[5],
                                  pJson[pMasqueradingVector[5]], err, false))
                return false;
        }
    } catch (const Json::LogicError &e) {
        err = e.what();
        return false;
    }
    return true;
}
bool Sessions::validJsonOfField(size_t index,
                                const std::string &fieldName,
                                const Json::Value &pJson,
                                std::string &err,
                                bool isForCreation) {
    switch (index) {
        case 0:
            if (pJson.isNull()) {
                err = "The " + fieldName + " column cannot be null";
                return false;
            }
            if (!pJson.isString()) {
                err = "Type error in the " + fieldName + " field";
                return false;
            }
            break;
        case 1:
            if (pJson.isNull()) {
                err = "The " + fieldName + " column cannot be null";
                return false;
            }
            if (!pJson.isString()) {
                err = "Type error in the " + fieldName + " field";
                return false;
            }
            break;
        case 2:
            if (pJson.isNull()) {
                err = "The " + fieldName + " column cannot be null";
                return false;
            }
            if (!pJson.isString()) {
                err = "Type error in the " + fieldName + " field";
                return false;
            }
            break;
        case 3:
            if (pJson.isNull()) {
                return true;
            }
            if (!pJson.isString()) {
                err = "Type error in the " + fieldName + " field";
                return false;
            }
            break;
        case 4:
            if (pJson.isNull()) {
                return true;
            }
            if (!pJson.isString()) {
                err = "Type error in the " + fieldName + " field";
                return false;
            }
            break;
        case 5:
            if (pJson.isNull()) {
                return true;
            }
            if (!pJson.isInt()) {
                err = "Type error in the " + fieldName + " field";
                return false;
            }
            break;
        default:
            err = "Internal error in the server";
            return false;
            break;
    }
    return true;
}
void Sessions::getPlayers(const DbClientPtr &clientPtr,
                          const std::function<void(std::vector<Players>)> &rcb,
                          const ExceptionCallback &ecb) const {
    const static std::string sql =
        "select * from players where session_id = $1";
    *clientPtr << sql << *id_ >> [rcb = std::move(rcb)](const Result &r) {
        std::vector<Players> ret;
        ret.reserve(r.size());
        for (auto const &row : r) {
            ret.emplace_back(Players(row));
        }
        rcb(ret);
    } >> ecb;
}
