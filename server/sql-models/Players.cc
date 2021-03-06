/**
 *
 *  Players.cc
 *  DO NOT EDIT. This file is generated by drogon_ctl
 *
 */

#include "Players.h"
#include <drogon/utils/Utilities.h>
#include <string>
#include "Sessions.h"
#include "Users.h"

using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::cavoke_orm;

const std::string Players::Cols::_session_id = "session_id";
const std::string Players::Cols::_user_id = "user_id";
const std::string Players::Cols::_player_id = "player_id";
const std::string Players::Cols::_score = "score";
const std::string Players::Cols::_playerstate = "playerstate";
const std::vector<std::string> Players::primaryKeyName = {"session_id",
                                                          "user_id"};
const bool Players::hasPrimaryKey = true;
const std::string Players::tableName = "players";

const std::vector<typename Players::MetaData> Players::metaData_ = {
    {"session_id", "std::string", "uuid", 0, 0, 1, 1},
    {"user_id", "std::string", "character varying", 0, 0, 1, 1},
    {"player_id", "int32_t", "integer", 4, 0, 0, 1},
    {"score", "int32_t", "integer", 4, 0, 0, 0},
    {"playerstate", "std::string", "text", 0, 0, 0, 1}};
const std::string &Players::getColumnName(size_t index) noexcept(false) {
    assert(index < metaData_.size());
    return metaData_[index].colName_;
}
Players::Players(const Row &r, const ssize_t indexOffset) noexcept {
    if (indexOffset < 0) {
        if (!r["session_id"].isNull()) {
            sessionId_ = std::make_shared<std::string>(
                r["session_id"].as<std::string>());
        }
        if (!r["user_id"].isNull()) {
            userId_ =
                std::make_shared<std::string>(r["user_id"].as<std::string>());
        }
        if (!r["player_id"].isNull()) {
            playerId_ = std::make_shared<int32_t>(r["player_id"].as<int32_t>());
        }
        if (!r["score"].isNull()) {
            score_ = std::make_shared<int32_t>(r["score"].as<int32_t>());
        }
        if (!r["playerstate"].isNull()) {
            playerstate_ = std::make_shared<std::string>(
                r["playerstate"].as<std::string>());
        }
    } else {
        size_t offset = (size_t)indexOffset;
        if (offset + 5 > r.size()) {
            LOG_FATAL << "Invalid SQL result for this model";
            return;
        }
        size_t index;
        index = offset + 0;
        if (!r[index].isNull()) {
            sessionId_ =
                std::make_shared<std::string>(r[index].as<std::string>());
        }
        index = offset + 1;
        if (!r[index].isNull()) {
            userId_ = std::make_shared<std::string>(r[index].as<std::string>());
        }
        index = offset + 2;
        if (!r[index].isNull()) {
            playerId_ = std::make_shared<int32_t>(r[index].as<int32_t>());
        }
        index = offset + 3;
        if (!r[index].isNull()) {
            score_ = std::make_shared<int32_t>(r[index].as<int32_t>());
        }
        index = offset + 4;
        if (!r[index].isNull()) {
            playerstate_ =
                std::make_shared<std::string>(r[index].as<std::string>());
        }
    }
}

Players::Players(
    const Json::Value &pJson,
    const std::vector<std::string> &pMasqueradingVector) noexcept(false) {
    if (pMasqueradingVector.size() != 5) {
        LOG_ERROR << "Bad masquerading vector";
        return;
    }
    if (!pMasqueradingVector[0].empty() &&
        pJson.isMember(pMasqueradingVector[0])) {
        dirtyFlag_[0] = true;
        if (!pJson[pMasqueradingVector[0]].isNull()) {
            sessionId_ = std::make_shared<std::string>(
                pJson[pMasqueradingVector[0]].asString());
        }
    }
    if (!pMasqueradingVector[1].empty() &&
        pJson.isMember(pMasqueradingVector[1])) {
        dirtyFlag_[1] = true;
        if (!pJson[pMasqueradingVector[1]].isNull()) {
            userId_ = std::make_shared<std::string>(
                pJson[pMasqueradingVector[1]].asString());
        }
    }
    if (!pMasqueradingVector[2].empty() &&
        pJson.isMember(pMasqueradingVector[2])) {
        dirtyFlag_[2] = true;
        if (!pJson[pMasqueradingVector[2]].isNull()) {
            playerId_ = std::make_shared<int32_t>(
                (int32_t)pJson[pMasqueradingVector[2]].asInt64());
        }
    }
    if (!pMasqueradingVector[3].empty() &&
        pJson.isMember(pMasqueradingVector[3])) {
        dirtyFlag_[3] = true;
        if (!pJson[pMasqueradingVector[3]].isNull()) {
            score_ = std::make_shared<int32_t>(
                (int32_t)pJson[pMasqueradingVector[3]].asInt64());
        }
    }
    if (!pMasqueradingVector[4].empty() &&
        pJson.isMember(pMasqueradingVector[4])) {
        dirtyFlag_[4] = true;
        if (!pJson[pMasqueradingVector[4]].isNull()) {
            playerstate_ = std::make_shared<std::string>(
                pJson[pMasqueradingVector[4]].asString());
        }
    }
}

Players::Players(const Json::Value &pJson) noexcept(false) {
    if (pJson.isMember("session_id")) {
        dirtyFlag_[0] = true;
        if (!pJson["session_id"].isNull()) {
            sessionId_ =
                std::make_shared<std::string>(pJson["session_id"].asString());
        }
    }
    if (pJson.isMember("user_id")) {
        dirtyFlag_[1] = true;
        if (!pJson["user_id"].isNull()) {
            userId_ =
                std::make_shared<std::string>(pJson["user_id"].asString());
        }
    }
    if (pJson.isMember("player_id")) {
        dirtyFlag_[2] = true;
        if (!pJson["player_id"].isNull()) {
            playerId_ = std::make_shared<int32_t>(
                (int32_t)pJson["player_id"].asInt64());
        }
    }
    if (pJson.isMember("score")) {
        dirtyFlag_[3] = true;
        if (!pJson["score"].isNull()) {
            score_ =
                std::make_shared<int32_t>((int32_t)pJson["score"].asInt64());
        }
    }
    if (pJson.isMember("playerstate")) {
        dirtyFlag_[4] = true;
        if (!pJson["playerstate"].isNull()) {
            playerstate_ =
                std::make_shared<std::string>(pJson["playerstate"].asString());
        }
    }
}

void Players::updateByMasqueradedJson(
    const Json::Value &pJson,
    const std::vector<std::string> &pMasqueradingVector) noexcept(false) {
    if (pMasqueradingVector.size() != 5) {
        LOG_ERROR << "Bad masquerading vector";
        return;
    }
    if (!pMasqueradingVector[0].empty() &&
        pJson.isMember(pMasqueradingVector[0])) {
        if (!pJson[pMasqueradingVector[0]].isNull()) {
            sessionId_ = std::make_shared<std::string>(
                pJson[pMasqueradingVector[0]].asString());
        }
    }
    if (!pMasqueradingVector[1].empty() &&
        pJson.isMember(pMasqueradingVector[1])) {
        if (!pJson[pMasqueradingVector[1]].isNull()) {
            userId_ = std::make_shared<std::string>(
                pJson[pMasqueradingVector[1]].asString());
        }
    }
    if (!pMasqueradingVector[2].empty() &&
        pJson.isMember(pMasqueradingVector[2])) {
        dirtyFlag_[2] = true;
        if (!pJson[pMasqueradingVector[2]].isNull()) {
            playerId_ = std::make_shared<int32_t>(
                (int32_t)pJson[pMasqueradingVector[2]].asInt64());
        }
    }
    if (!pMasqueradingVector[3].empty() &&
        pJson.isMember(pMasqueradingVector[3])) {
        dirtyFlag_[3] = true;
        if (!pJson[pMasqueradingVector[3]].isNull()) {
            score_ = std::make_shared<int32_t>(
                (int32_t)pJson[pMasqueradingVector[3]].asInt64());
        }
    }
    if (!pMasqueradingVector[4].empty() &&
        pJson.isMember(pMasqueradingVector[4])) {
        dirtyFlag_[4] = true;
        if (!pJson[pMasqueradingVector[4]].isNull()) {
            playerstate_ = std::make_shared<std::string>(
                pJson[pMasqueradingVector[4]].asString());
        }
    }
}

void Players::updateByJson(const Json::Value &pJson) noexcept(false) {
    if (pJson.isMember("session_id")) {
        if (!pJson["session_id"].isNull()) {
            sessionId_ =
                std::make_shared<std::string>(pJson["session_id"].asString());
        }
    }
    if (pJson.isMember("user_id")) {
        if (!pJson["user_id"].isNull()) {
            userId_ =
                std::make_shared<std::string>(pJson["user_id"].asString());
        }
    }
    if (pJson.isMember("player_id")) {
        dirtyFlag_[2] = true;
        if (!pJson["player_id"].isNull()) {
            playerId_ = std::make_shared<int32_t>(
                (int32_t)pJson["player_id"].asInt64());
        }
    }
    if (pJson.isMember("score")) {
        dirtyFlag_[3] = true;
        if (!pJson["score"].isNull()) {
            score_ =
                std::make_shared<int32_t>((int32_t)pJson["score"].asInt64());
        }
    }
    if (pJson.isMember("playerstate")) {
        dirtyFlag_[4] = true;
        if (!pJson["playerstate"].isNull()) {
            playerstate_ =
                std::make_shared<std::string>(pJson["playerstate"].asString());
        }
    }
}

const std::string &Players::getValueOfSessionId() const noexcept {
    const static std::string defaultValue = std::string();
    if (sessionId_)
        return *sessionId_;
    return defaultValue;
}
const std::shared_ptr<std::string> &Players::getSessionId() const noexcept {
    return sessionId_;
}
void Players::setSessionId(const std::string &pSessionId) noexcept {
    sessionId_ = std::make_shared<std::string>(pSessionId);
    dirtyFlag_[0] = true;
}
void Players::setSessionId(std::string &&pSessionId) noexcept {
    sessionId_ = std::make_shared<std::string>(std::move(pSessionId));
    dirtyFlag_[0] = true;
}

const std::string &Players::getValueOfUserId() const noexcept {
    const static std::string defaultValue = std::string();
    if (userId_)
        return *userId_;
    return defaultValue;
}
const std::shared_ptr<std::string> &Players::getUserId() const noexcept {
    return userId_;
}
void Players::setUserId(const std::string &pUserId) noexcept {
    userId_ = std::make_shared<std::string>(pUserId);
    dirtyFlag_[1] = true;
}
void Players::setUserId(std::string &&pUserId) noexcept {
    userId_ = std::make_shared<std::string>(std::move(pUserId));
    dirtyFlag_[1] = true;
}

const int32_t &Players::getValueOfPlayerId() const noexcept {
    const static int32_t defaultValue = int32_t();
    if (playerId_)
        return *playerId_;
    return defaultValue;
}
const std::shared_ptr<int32_t> &Players::getPlayerId() const noexcept {
    return playerId_;
}
void Players::setPlayerId(const int32_t &pPlayerId) noexcept {
    playerId_ = std::make_shared<int32_t>(pPlayerId);
    dirtyFlag_[2] = true;
}

const int32_t &Players::getValueOfScore() const noexcept {
    const static int32_t defaultValue = int32_t();
    if (score_)
        return *score_;
    return defaultValue;
}
const std::shared_ptr<int32_t> &Players::getScore() const noexcept {
    return score_;
}
void Players::setScore(const int32_t &pScore) noexcept {
    score_ = std::make_shared<int32_t>(pScore);
    dirtyFlag_[3] = true;
}
void Players::setScoreToNull() noexcept {
    score_.reset();
    dirtyFlag_[3] = true;
}

const std::string &Players::getValueOfPlayerstate() const noexcept {
    const static std::string defaultValue = std::string();
    if (playerstate_)
        return *playerstate_;
    return defaultValue;
}
const std::shared_ptr<std::string> &Players::getPlayerstate() const noexcept {
    return playerstate_;
}
void Players::setPlayerstate(const std::string &pPlayerstate) noexcept {
    playerstate_ = std::make_shared<std::string>(pPlayerstate);
    dirtyFlag_[4] = true;
}
void Players::setPlayerstate(std::string &&pPlayerstate) noexcept {
    playerstate_ = std::make_shared<std::string>(std::move(pPlayerstate));
    dirtyFlag_[4] = true;
}

void Players::updateId(const uint64_t id) {
}
typename Players::PrimaryKeyType Players::getPrimaryKey() const {
    return std::make_tuple(*sessionId_, *userId_);
}

const std::vector<std::string> &Players::insertColumns() noexcept {
    static const std::vector<std::string> inCols = {
        "session_id", "user_id", "player_id", "score", "playerstate"};
    return inCols;
}

void Players::outputArgs(drogon::orm::internal::SqlBinder &binder) const {
    if (dirtyFlag_[0]) {
        if (getSessionId()) {
            binder << getValueOfSessionId();
        } else {
            binder << nullptr;
        }
    }
    if (dirtyFlag_[1]) {
        if (getUserId()) {
            binder << getValueOfUserId();
        } else {
            binder << nullptr;
        }
    }
    if (dirtyFlag_[2]) {
        if (getPlayerId()) {
            binder << getValueOfPlayerId();
        } else {
            binder << nullptr;
        }
    }
    if (dirtyFlag_[3]) {
        if (getScore()) {
            binder << getValueOfScore();
        } else {
            binder << nullptr;
        }
    }
    if (dirtyFlag_[4]) {
        if (getPlayerstate()) {
            binder << getValueOfPlayerstate();
        } else {
            binder << nullptr;
        }
    }
}

const std::vector<std::string> Players::updateColumns() const {
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
    return ret;
}

void Players::updateArgs(drogon::orm::internal::SqlBinder &binder) const {
    if (dirtyFlag_[0]) {
        if (getSessionId()) {
            binder << getValueOfSessionId();
        } else {
            binder << nullptr;
        }
    }
    if (dirtyFlag_[1]) {
        if (getUserId()) {
            binder << getValueOfUserId();
        } else {
            binder << nullptr;
        }
    }
    if (dirtyFlag_[2]) {
        if (getPlayerId()) {
            binder << getValueOfPlayerId();
        } else {
            binder << nullptr;
        }
    }
    if (dirtyFlag_[3]) {
        if (getScore()) {
            binder << getValueOfScore();
        } else {
            binder << nullptr;
        }
    }
    if (dirtyFlag_[4]) {
        if (getPlayerstate()) {
            binder << getValueOfPlayerstate();
        } else {
            binder << nullptr;
        }
    }
}
Json::Value Players::toJson() const {
    Json::Value ret;
    if (getSessionId()) {
        ret["session_id"] = getValueOfSessionId();
    } else {
        ret["session_id"] = Json::Value();
    }
    if (getUserId()) {
        ret["user_id"] = getValueOfUserId();
    } else {
        ret["user_id"] = Json::Value();
    }
    if (getPlayerId()) {
        ret["player_id"] = getValueOfPlayerId();
    } else {
        ret["player_id"] = Json::Value();
    }
    if (getScore()) {
        ret["score"] = getValueOfScore();
    } else {
        ret["score"] = Json::Value();
    }
    if (getPlayerstate()) {
        ret["playerstate"] = getValueOfPlayerstate();
    } else {
        ret["playerstate"] = Json::Value();
    }
    return ret;
}

Json::Value Players::toMasqueradedJson(
    const std::vector<std::string> &pMasqueradingVector) const {
    Json::Value ret;
    if (pMasqueradingVector.size() == 5) {
        if (!pMasqueradingVector[0].empty()) {
            if (getSessionId()) {
                ret[pMasqueradingVector[0]] = getValueOfSessionId();
            } else {
                ret[pMasqueradingVector[0]] = Json::Value();
            }
        }
        if (!pMasqueradingVector[1].empty()) {
            if (getUserId()) {
                ret[pMasqueradingVector[1]] = getValueOfUserId();
            } else {
                ret[pMasqueradingVector[1]] = Json::Value();
            }
        }
        if (!pMasqueradingVector[2].empty()) {
            if (getPlayerId()) {
                ret[pMasqueradingVector[2]] = getValueOfPlayerId();
            } else {
                ret[pMasqueradingVector[2]] = Json::Value();
            }
        }
        if (!pMasqueradingVector[3].empty()) {
            if (getScore()) {
                ret[pMasqueradingVector[3]] = getValueOfScore();
            } else {
                ret[pMasqueradingVector[3]] = Json::Value();
            }
        }
        if (!pMasqueradingVector[4].empty()) {
            if (getPlayerstate()) {
                ret[pMasqueradingVector[4]] = getValueOfPlayerstate();
            } else {
                ret[pMasqueradingVector[4]] = Json::Value();
            }
        }
        return ret;
    }
    LOG_ERROR << "Masquerade failed";
    if (getSessionId()) {
        ret["session_id"] = getValueOfSessionId();
    } else {
        ret["session_id"] = Json::Value();
    }
    if (getUserId()) {
        ret["user_id"] = getValueOfUserId();
    } else {
        ret["user_id"] = Json::Value();
    }
    if (getPlayerId()) {
        ret["player_id"] = getValueOfPlayerId();
    } else {
        ret["player_id"] = Json::Value();
    }
    if (getScore()) {
        ret["score"] = getValueOfScore();
    } else {
        ret["score"] = Json::Value();
    }
    if (getPlayerstate()) {
        ret["playerstate"] = getValueOfPlayerstate();
    } else {
        ret["playerstate"] = Json::Value();
    }
    return ret;
}

bool Players::validateJsonForCreation(const Json::Value &pJson,
                                      std::string &err) {
    if (pJson.isMember("session_id")) {
        if (!validJsonOfField(0, "session_id", pJson["session_id"], err, true))
            return false;
    } else {
        err = "The session_id column cannot be null";
        return false;
    }
    if (pJson.isMember("user_id")) {
        if (!validJsonOfField(1, "user_id", pJson["user_id"], err, true))
            return false;
    } else {
        err = "The user_id column cannot be null";
        return false;
    }
    if (pJson.isMember("player_id")) {
        if (!validJsonOfField(2, "player_id", pJson["player_id"], err, true))
            return false;
    } else {
        err = "The player_id column cannot be null";
        return false;
    }
    if (pJson.isMember("score")) {
        if (!validJsonOfField(3, "score", pJson["score"], err, true))
            return false;
    }
    if (pJson.isMember("playerstate")) {
        if (!validJsonOfField(4, "playerstate", pJson["playerstate"], err,
                              true))
            return false;
    } else {
        err = "The playerstate column cannot be null";
        return false;
    }
    return true;
}
bool Players::validateMasqueradedJsonForCreation(
    const Json::Value &pJson,
    const std::vector<std::string> &pMasqueradingVector,
    std::string &err) {
    if (pMasqueradingVector.size() != 5) {
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
            } else {
                err =
                    "The " + pMasqueradingVector[4] + " column cannot be null";
                return false;
            }
        }
    } catch (const Json::LogicError &e) {
        err = e.what();
        return false;
    }
    return true;
}
bool Players::validateJsonForUpdate(const Json::Value &pJson,
                                    std::string &err) {
    if (pJson.isMember("session_id")) {
        if (!validJsonOfField(0, "session_id", pJson["session_id"], err, false))
            return false;
    } else {
        err =
            "The value of primary key must be set in the json object for "
            "update";
        return false;
    }
    if (pJson.isMember("user_id")) {
        if (!validJsonOfField(1, "user_id", pJson["user_id"], err, false))
            return false;
    } else {
        err =
            "The value of primary key must be set in the json object for "
            "update";
        return false;
    }
    if (pJson.isMember("player_id")) {
        if (!validJsonOfField(2, "player_id", pJson["player_id"], err, false))
            return false;
    }
    if (pJson.isMember("score")) {
        if (!validJsonOfField(3, "score", pJson["score"], err, false))
            return false;
    }
    if (pJson.isMember("playerstate")) {
        if (!validJsonOfField(4, "playerstate", pJson["playerstate"], err,
                              false))
            return false;
    }
    return true;
}
bool Players::validateMasqueradedJsonForUpdate(
    const Json::Value &pJson,
    const std::vector<std::string> &pMasqueradingVector,
    std::string &err) {
    if (pMasqueradingVector.size() != 5) {
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
        } else {
            err =
                "The value of primary key must be set in the json object for "
                "update";
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
    } catch (const Json::LogicError &e) {
        err = e.what();
        return false;
    }
    return true;
}
bool Players::validJsonOfField(size_t index,
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
            if (!pJson.isInt()) {
                err = "Type error in the " + fieldName + " field";
                return false;
            }
            break;
        case 3:
            if (pJson.isNull()) {
                return true;
            }
            if (!pJson.isInt()) {
                err = "Type error in the " + fieldName + " field";
                return false;
            }
            break;
        case 4:
            if (pJson.isNull()) {
                err = "The " + fieldName + " column cannot be null";
                return false;
            }
            if (!pJson.isString()) {
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
void Players::getSession(const DbClientPtr &clientPtr,
                         const std::function<void(Sessions)> &rcb,
                         const ExceptionCallback &ecb) const {
    const static std::string sql = "select * from sessions where id = $1";
    *clientPtr << sql << *sessionId_ >> [rcb = std::move(rcb),
                                         ecb](const Result &r) {
        if (r.size() == 0) {
            ecb(UnexpectedRows("0 rows found"));
        } else if (r.size() > 1) {
            ecb(UnexpectedRows("Found more than one row"));
        } else {
            rcb(Sessions(r[0]));
        }
    } >> ecb;
}
void Players::getUser(const DbClientPtr &clientPtr,
                      const std::function<void(Users)> &rcb,
                      const ExceptionCallback &ecb) const {
    const static std::string sql = "select * from users where id = $1";
    *clientPtr << sql << *userId_ >> [rcb = std::move(rcb),
                                      ecb](const Result &r) {
        if (r.size() == 0) {
            ecb(UnexpectedRows("0 rows found"));
        } else if (r.size() > 1) {
            ecb(UnexpectedRows("Found more than one row"));
        } else {
            rcb(Users(r[0]));
        }
    } >> ecb;
}
