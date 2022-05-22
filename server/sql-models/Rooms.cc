/**
 *
 *  Rooms.cc
 *  DO NOT EDIT. This file is generated by drogon_ctl
 *
 */

#include "Rooms.h"
#include <drogon/utils/Utilities.h>
#include <string>
#include "RoomJoins.h"

using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::postgres_db;

const std::string Rooms::Cols::_id = "id";
const std::string Rooms::Cols::_display_name = "display_name";
const std::string Rooms::Cols::_invite_code = "invite_code";
const std::string Rooms::Cols::_host_id = "host_id";
const std::string Rooms::Cols::_session_id = "session_id";
const std::string Rooms::primaryKeyName = "id";
const bool Rooms::hasPrimaryKey = true;
const std::string Rooms::tableName = "rooms";

const std::vector<typename Rooms::MetaData> Rooms::metaData_ = {
    {"id", "std::string", "uuid", 0, 0, 1, 1},
    {"display_name", "std::string", "character varying", 0, 0, 0, 1},
    {"invite_code", "std::string", "character varying", 0, 0, 0, 1},
    {"host_id", "std::string", "character varying", 0, 0, 0, 0},
    {"session_id", "std::string", "uuid", 0, 0, 0, 0}};
const std::string &Rooms::getColumnName(size_t index) noexcept(false) {
    assert(index < metaData_.size());
    return metaData_[index].colName_;
}
Rooms::Rooms(const Row &r, const ssize_t indexOffset) noexcept {
    if (indexOffset < 0) {
        if (!r["id"].isNull()) {
            id_ = std::make_shared<std::string>(r["id"].as<std::string>());
        }
        if (!r["display_name"].isNull()) {
            displayName_ = std::make_shared<std::string>(
                r["display_name"].as<std::string>());
        }
        if (!r["invite_code"].isNull()) {
            inviteCode_ = std::make_shared<std::string>(
                r["invite_code"].as<std::string>());
        }
        if (!r["host_id"].isNull()) {
            hostId_ =
                std::make_shared<std::string>(r["host_id"].as<std::string>());
        }
        if (!r["session_id"].isNull()) {
            sessionId_ = std::make_shared<std::string>(
                r["session_id"].as<std::string>());
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
            id_ = std::make_shared<std::string>(r[index].as<std::string>());
        }
        index = offset + 1;
        if (!r[index].isNull()) {
            displayName_ =
                std::make_shared<std::string>(r[index].as<std::string>());
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
            sessionId_ =
                std::make_shared<std::string>(r[index].as<std::string>());
        }
    }
}

Rooms::Rooms(
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
            id_ = std::make_shared<std::string>(
                pJson[pMasqueradingVector[0]].asString());
        }
    }
    if (!pMasqueradingVector[1].empty() &&
        pJson.isMember(pMasqueradingVector[1])) {
        dirtyFlag_[1] = true;
        if (!pJson[pMasqueradingVector[1]].isNull()) {
            displayName_ = std::make_shared<std::string>(
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
            sessionId_ = std::make_shared<std::string>(
                pJson[pMasqueradingVector[4]].asString());
        }
    }
}

Rooms::Rooms(const Json::Value &pJson) noexcept(false) {
    if (pJson.isMember("id")) {
        dirtyFlag_[0] = true;
        if (!pJson["id"].isNull()) {
            id_ = std::make_shared<std::string>(pJson["id"].asString());
        }
    }
    if (pJson.isMember("display_name")) {
        dirtyFlag_[1] = true;
        if (!pJson["display_name"].isNull()) {
            displayName_ =
                std::make_shared<std::string>(pJson["display_name"].asString());
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
    if (pJson.isMember("session_id")) {
        dirtyFlag_[4] = true;
        if (!pJson["session_id"].isNull()) {
            sessionId_ =
                std::make_shared<std::string>(pJson["session_id"].asString());
        }
    }
}

void Rooms::updateByMasqueradedJson(
    const Json::Value &pJson,
    const std::vector<std::string> &pMasqueradingVector) noexcept(false) {
    if (pMasqueradingVector.size() != 5) {
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
            displayName_ = std::make_shared<std::string>(
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
            sessionId_ = std::make_shared<std::string>(
                pJson[pMasqueradingVector[4]].asString());
        }
    }
}

void Rooms::updateByJson(const Json::Value &pJson) noexcept(false) {
    if (pJson.isMember("id")) {
        if (!pJson["id"].isNull()) {
            id_ = std::make_shared<std::string>(pJson["id"].asString());
        }
    }
    if (pJson.isMember("display_name")) {
        dirtyFlag_[1] = true;
        if (!pJson["display_name"].isNull()) {
            displayName_ =
                std::make_shared<std::string>(pJson["display_name"].asString());
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
    if (pJson.isMember("session_id")) {
        dirtyFlag_[4] = true;
        if (!pJson["session_id"].isNull()) {
            sessionId_ =
                std::make_shared<std::string>(pJson["session_id"].asString());
        }
    }
}

const std::string &Rooms::getValueOfId() const noexcept {
    const static std::string defaultValue = std::string();
    if (id_)
        return *id_;
    return defaultValue;
}
const std::shared_ptr<std::string> &Rooms::getId() const noexcept {
    return id_;
}
void Rooms::setId(const std::string &pId) noexcept {
    id_ = std::make_shared<std::string>(pId);
    dirtyFlag_[0] = true;
}
void Rooms::setId(std::string &&pId) noexcept {
    id_ = std::make_shared<std::string>(std::move(pId));
    dirtyFlag_[0] = true;
}
const typename Rooms::PrimaryKeyType &Rooms::getPrimaryKey() const {
    assert(id_);
    return *id_;
}

const std::string &Rooms::getValueOfDisplayName() const noexcept {
    const static std::string defaultValue = std::string();
    if (displayName_)
        return *displayName_;
    return defaultValue;
}
const std::shared_ptr<std::string> &Rooms::getDisplayName() const noexcept {
    return displayName_;
}
void Rooms::setDisplayName(const std::string &pDisplayName) noexcept {
    displayName_ = std::make_shared<std::string>(pDisplayName);
    dirtyFlag_[1] = true;
}
void Rooms::setDisplayName(std::string &&pDisplayName) noexcept {
    displayName_ = std::make_shared<std::string>(std::move(pDisplayName));
    dirtyFlag_[1] = true;
}

const std::string &Rooms::getValueOfInviteCode() const noexcept {
    const static std::string defaultValue = std::string();
    if (inviteCode_)
        return *inviteCode_;
    return defaultValue;
}
const std::shared_ptr<std::string> &Rooms::getInviteCode() const noexcept {
    return inviteCode_;
}
void Rooms::setInviteCode(const std::string &pInviteCode) noexcept {
    inviteCode_ = std::make_shared<std::string>(pInviteCode);
    dirtyFlag_[2] = true;
}
void Rooms::setInviteCode(std::string &&pInviteCode) noexcept {
    inviteCode_ = std::make_shared<std::string>(std::move(pInviteCode));
    dirtyFlag_[2] = true;
}

const std::string &Rooms::getValueOfHostId() const noexcept {
    const static std::string defaultValue = std::string();
    if (hostId_)
        return *hostId_;
    return defaultValue;
}
const std::shared_ptr<std::string> &Rooms::getHostId() const noexcept {
    return hostId_;
}
void Rooms::setHostId(const std::string &pHostId) noexcept {
    hostId_ = std::make_shared<std::string>(pHostId);
    dirtyFlag_[3] = true;
}
void Rooms::setHostId(std::string &&pHostId) noexcept {
    hostId_ = std::make_shared<std::string>(std::move(pHostId));
    dirtyFlag_[3] = true;
}
void Rooms::setHostIdToNull() noexcept {
    hostId_.reset();
    dirtyFlag_[3] = true;
}

const std::string &Rooms::getValueOfSessionId() const noexcept {
    const static std::string defaultValue = std::string();
    if (sessionId_)
        return *sessionId_;
    return defaultValue;
}
const std::shared_ptr<std::string> &Rooms::getSessionId() const noexcept {
    return sessionId_;
}
void Rooms::setSessionId(const std::string &pSessionId) noexcept {
    sessionId_ = std::make_shared<std::string>(pSessionId);
    dirtyFlag_[4] = true;
}
void Rooms::setSessionId(std::string &&pSessionId) noexcept {
    sessionId_ = std::make_shared<std::string>(std::move(pSessionId));
    dirtyFlag_[4] = true;
}
void Rooms::setSessionIdToNull() noexcept {
    sessionId_.reset();
    dirtyFlag_[4] = true;
}

void Rooms::updateId(const uint64_t id) {
}

const std::vector<std::string> &Rooms::insertColumns() noexcept {
    static const std::vector<std::string> inCols = {
        "id", "display_name", "invite_code", "host_id", "session_id"};
    return inCols;
}

void Rooms::outputArgs(drogon::orm::internal::SqlBinder &binder) const {
    if (dirtyFlag_[0]) {
        if (getId()) {
            binder << getValueOfId();
        } else {
            binder << nullptr;
        }
    }
    if (dirtyFlag_[1]) {
        if (getDisplayName()) {
            binder << getValueOfDisplayName();
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
        if (getSessionId()) {
            binder << getValueOfSessionId();
        } else {
            binder << nullptr;
        }
    }
}

const std::vector<std::string> Rooms::updateColumns() const {
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

void Rooms::updateArgs(drogon::orm::internal::SqlBinder &binder) const {
    if (dirtyFlag_[0]) {
        if (getId()) {
            binder << getValueOfId();
        } else {
            binder << nullptr;
        }
    }
    if (dirtyFlag_[1]) {
        if (getDisplayName()) {
            binder << getValueOfDisplayName();
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
        if (getSessionId()) {
            binder << getValueOfSessionId();
        } else {
            binder << nullptr;
        }
    }
}
Json::Value Rooms::toJson() const {
    Json::Value ret;
    if (getId()) {
        ret["id"] = getValueOfId();
    } else {
        ret["id"] = Json::Value();
    }
    if (getDisplayName()) {
        ret["display_name"] = getValueOfDisplayName();
    } else {
        ret["display_name"] = Json::Value();
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
    if (getSessionId()) {
        ret["session_id"] = getValueOfSessionId();
    } else {
        ret["session_id"] = Json::Value();
    }
    return ret;
}

Json::Value Rooms::toMasqueradedJson(
    const std::vector<std::string> &pMasqueradingVector) const {
    Json::Value ret;
    if (pMasqueradingVector.size() == 5) {
        if (!pMasqueradingVector[0].empty()) {
            if (getId()) {
                ret[pMasqueradingVector[0]] = getValueOfId();
            } else {
                ret[pMasqueradingVector[0]] = Json::Value();
            }
        }
        if (!pMasqueradingVector[1].empty()) {
            if (getDisplayName()) {
                ret[pMasqueradingVector[1]] = getValueOfDisplayName();
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
            if (getSessionId()) {
                ret[pMasqueradingVector[4]] = getValueOfSessionId();
            } else {
                ret[pMasqueradingVector[4]] = Json::Value();
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
    if (getDisplayName()) {
        ret["display_name"] = getValueOfDisplayName();
    } else {
        ret["display_name"] = Json::Value();
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
    if (getSessionId()) {
        ret["session_id"] = getValueOfSessionId();
    } else {
        ret["session_id"] = Json::Value();
    }
    return ret;
}

bool Rooms::validateJsonForCreation(const Json::Value &pJson,
                                    std::string &err) {
    if (pJson.isMember("id")) {
        if (!validJsonOfField(0, "id", pJson["id"], err, true))
            return false;
    } else {
        err = "The id column cannot be null";
        return false;
    }
    if (pJson.isMember("display_name")) {
        if (!validJsonOfField(1, "display_name", pJson["display_name"], err,
                              true))
            return false;
    } else {
        err = "The display_name column cannot be null";
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
    if (pJson.isMember("session_id")) {
        if (!validJsonOfField(4, "session_id", pJson["session_id"], err, true))
            return false;
    }
    return true;
}
bool Rooms::validateMasqueradedJsonForCreation(
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
            }
        }
    } catch (const Json::LogicError &e) {
        err = e.what();
        return false;
    }
    return true;
}
bool Rooms::validateJsonForUpdate(const Json::Value &pJson, std::string &err) {
    if (pJson.isMember("id")) {
        if (!validJsonOfField(0, "id", pJson["id"], err, false))
            return false;
    } else {
        err =
            "The value of primary key must be set in the json object for "
            "update";
        return false;
    }
    if (pJson.isMember("display_name")) {
        if (!validJsonOfField(1, "display_name", pJson["display_name"], err,
                              false))
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
    if (pJson.isMember("session_id")) {
        if (!validJsonOfField(4, "session_id", pJson["session_id"], err, false))
            return false;
    }
    return true;
}
bool Rooms::validateMasqueradedJsonForUpdate(
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
bool Rooms::validJsonOfField(size_t index,
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
        default:
            err = "Internal error in the server";
            return false;
            break;
    }
    return true;
}
void Rooms::getRoomJoins(const DbClientPtr &clientPtr,
                         const std::function<void(std::vector<RoomJoins>)> &rcb,
                         const ExceptionCallback &ecb) const {
    const static std::string sql =
        "select * from room_joins where room_id = $1";
    *clientPtr << sql << *id_ >> [rcb = std::move(rcb)](const Result &r) {
        std::vector<RoomJoins> ret;
        ret.reserve(r.size());
        for (auto const &row : r) {
            ret.emplace_back(RoomJoins(row));
        }
        rcb(ret);
    } >> ecb;
}
