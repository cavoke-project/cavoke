/**
 *
 *  Globalstates.cc
 *  DO NOT EDIT. This file is generated by drogon_ctl
 *
 */

#include "Globalstates.h"
#include <drogon/utils/Utilities.h>
#include <string>

using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::cavoke_orm;

const std::string Globalstates::Cols::_session_id = "session_id";
const std::string Globalstates::Cols::_globalstate = "globalstate";
const std::string Globalstates::Cols::_is_terminal = "is_terminal";
const std::string Globalstates::Cols::_saved_on = "saved_on";
const std::string Globalstates::primaryKeyName = "session_id";
const bool Globalstates::hasPrimaryKey = true;
const std::string Globalstates::tableName = "globalstates";

const std::vector<typename Globalstates::MetaData> Globalstates::metaData_ = {
    {"session_id", "std::string", "uuid", 0, 0, 1, 1},
    {"globalstate", "std::string", "text", 0, 0, 0, 0},
    {"is_terminal", "bool", "boolean", 1, 0, 0, 1},
    {"saved_on", "::trantor::Date", "timestamp without time zone", 0, 0, 0, 0}};
const std::string &Globalstates::getColumnName(size_t index) noexcept(false) {
    assert(index < metaData_.size());
    return metaData_[index].colName_;
}
Globalstates::Globalstates(const Row &r, const ssize_t indexOffset) noexcept {
    if (indexOffset < 0) {
        if (!r["session_id"].isNull()) {
            sessionId_ = std::make_shared<std::string>(
                r["session_id"].as<std::string>());
        }
        if (!r["globalstate"].isNull()) {
            globalstate_ = std::make_shared<std::string>(
                r["globalstate"].as<std::string>());
        }
        if (!r["is_terminal"].isNull()) {
            isTerminal_ = std::make_shared<bool>(r["is_terminal"].as<bool>());
        }
        if (!r["saved_on"].isNull()) {
            auto timeStr = r["saved_on"].as<std::string>();
            struct tm stm;
            memset(&stm, 0, sizeof(stm));
            auto p = strptime(timeStr.c_str(), "%Y-%m-%d %H:%M:%S", &stm);
            time_t t = mktime(&stm);
            size_t decimalNum = 0;
            if (p) {
                if (*p == '.') {
                    std::string decimals(p + 1, &timeStr[timeStr.length()]);
                    while (decimals.length() < 6) {
                        decimals += "0";
                    }
                    decimalNum = (size_t)atol(decimals.c_str());
                }
                savedOn_ =
                    std::make_shared<::trantor::Date>(t * 1000000 + decimalNum);
            }
        }
    } else {
        size_t offset = (size_t)indexOffset;
        if (offset + 4 > r.size()) {
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
            globalstate_ =
                std::make_shared<std::string>(r[index].as<std::string>());
        }
        index = offset + 2;
        if (!r[index].isNull()) {
            isTerminal_ = std::make_shared<bool>(r[index].as<bool>());
        }
        index = offset + 3;
        if (!r[index].isNull()) {
            auto timeStr = r[index].as<std::string>();
            struct tm stm;
            memset(&stm, 0, sizeof(stm));
            auto p = strptime(timeStr.c_str(), "%Y-%m-%d %H:%M:%S", &stm);
            time_t t = mktime(&stm);
            size_t decimalNum = 0;
            if (p) {
                if (*p == '.') {
                    std::string decimals(p + 1, &timeStr[timeStr.length()]);
                    while (decimals.length() < 6) {
                        decimals += "0";
                    }
                    decimalNum = (size_t)atol(decimals.c_str());
                }
                savedOn_ =
                    std::make_shared<::trantor::Date>(t * 1000000 + decimalNum);
            }
        }
    }
}

Globalstates::Globalstates(
    const Json::Value &pJson,
    const std::vector<std::string> &pMasqueradingVector) noexcept(false) {
    if (pMasqueradingVector.size() != 4) {
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
            globalstate_ = std::make_shared<std::string>(
                pJson[pMasqueradingVector[1]].asString());
        }
    }
    if (!pMasqueradingVector[2].empty() &&
        pJson.isMember(pMasqueradingVector[2])) {
        dirtyFlag_[2] = true;
        if (!pJson[pMasqueradingVector[2]].isNull()) {
            isTerminal_ =
                std::make_shared<bool>(pJson[pMasqueradingVector[2]].asBool());
        }
    }
    if (!pMasqueradingVector[3].empty() &&
        pJson.isMember(pMasqueradingVector[3])) {
        dirtyFlag_[3] = true;
        if (!pJson[pMasqueradingVector[3]].isNull()) {
            auto timeStr = pJson[pMasqueradingVector[3]].asString();
            struct tm stm;
            memset(&stm, 0, sizeof(stm));
            auto p = strptime(timeStr.c_str(), "%Y-%m-%d %H:%M:%S", &stm);
            time_t t = mktime(&stm);
            size_t decimalNum = 0;
            if (p) {
                if (*p == '.') {
                    std::string decimals(p + 1, &timeStr[timeStr.length()]);
                    while (decimals.length() < 6) {
                        decimals += "0";
                    }
                    decimalNum = (size_t)atol(decimals.c_str());
                }
                savedOn_ =
                    std::make_shared<::trantor::Date>(t * 1000000 + decimalNum);
            }
        }
    }
}

Globalstates::Globalstates(const Json::Value &pJson) noexcept(false) {
    if (pJson.isMember("session_id")) {
        dirtyFlag_[0] = true;
        if (!pJson["session_id"].isNull()) {
            sessionId_ =
                std::make_shared<std::string>(pJson["session_id"].asString());
        }
    }
    if (pJson.isMember("globalstate")) {
        dirtyFlag_[1] = true;
        if (!pJson["globalstate"].isNull()) {
            globalstate_ =
                std::make_shared<std::string>(pJson["globalstate"].asString());
        }
    }
    if (pJson.isMember("is_terminal")) {
        dirtyFlag_[2] = true;
        if (!pJson["is_terminal"].isNull()) {
            isTerminal_ = std::make_shared<bool>(pJson["is_terminal"].asBool());
        }
    }
    if (pJson.isMember("saved_on")) {
        dirtyFlag_[3] = true;
        if (!pJson["saved_on"].isNull()) {
            auto timeStr = pJson["saved_on"].asString();
            struct tm stm;
            memset(&stm, 0, sizeof(stm));
            auto p = strptime(timeStr.c_str(), "%Y-%m-%d %H:%M:%S", &stm);
            time_t t = mktime(&stm);
            size_t decimalNum = 0;
            if (p) {
                if (*p == '.') {
                    std::string decimals(p + 1, &timeStr[timeStr.length()]);
                    while (decimals.length() < 6) {
                        decimals += "0";
                    }
                    decimalNum = (size_t)atol(decimals.c_str());
                }
                savedOn_ =
                    std::make_shared<::trantor::Date>(t * 1000000 + decimalNum);
            }
        }
    }
}

void Globalstates::updateByMasqueradedJson(
    const Json::Value &pJson,
    const std::vector<std::string> &pMasqueradingVector) noexcept(false) {
    if (pMasqueradingVector.size() != 4) {
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
        dirtyFlag_[1] = true;
        if (!pJson[pMasqueradingVector[1]].isNull()) {
            globalstate_ = std::make_shared<std::string>(
                pJson[pMasqueradingVector[1]].asString());
        }
    }
    if (!pMasqueradingVector[2].empty() &&
        pJson.isMember(pMasqueradingVector[2])) {
        dirtyFlag_[2] = true;
        if (!pJson[pMasqueradingVector[2]].isNull()) {
            isTerminal_ =
                std::make_shared<bool>(pJson[pMasqueradingVector[2]].asBool());
        }
    }
    if (!pMasqueradingVector[3].empty() &&
        pJson.isMember(pMasqueradingVector[3])) {
        dirtyFlag_[3] = true;
        if (!pJson[pMasqueradingVector[3]].isNull()) {
            auto timeStr = pJson[pMasqueradingVector[3]].asString();
            struct tm stm;
            memset(&stm, 0, sizeof(stm));
            auto p = strptime(timeStr.c_str(), "%Y-%m-%d %H:%M:%S", &stm);
            time_t t = mktime(&stm);
            size_t decimalNum = 0;
            if (p) {
                if (*p == '.') {
                    std::string decimals(p + 1, &timeStr[timeStr.length()]);
                    while (decimals.length() < 6) {
                        decimals += "0";
                    }
                    decimalNum = (size_t)atol(decimals.c_str());
                }
                savedOn_ =
                    std::make_shared<::trantor::Date>(t * 1000000 + decimalNum);
            }
        }
    }
}

void Globalstates::updateByJson(const Json::Value &pJson) noexcept(false) {
    if (pJson.isMember("session_id")) {
        if (!pJson["session_id"].isNull()) {
            sessionId_ =
                std::make_shared<std::string>(pJson["session_id"].asString());
        }
    }
    if (pJson.isMember("globalstate")) {
        dirtyFlag_[1] = true;
        if (!pJson["globalstate"].isNull()) {
            globalstate_ =
                std::make_shared<std::string>(pJson["globalstate"].asString());
        }
    }
    if (pJson.isMember("is_terminal")) {
        dirtyFlag_[2] = true;
        if (!pJson["is_terminal"].isNull()) {
            isTerminal_ = std::make_shared<bool>(pJson["is_terminal"].asBool());
        }
    }
    if (pJson.isMember("saved_on")) {
        dirtyFlag_[3] = true;
        if (!pJson["saved_on"].isNull()) {
            auto timeStr = pJson["saved_on"].asString();
            struct tm stm;
            memset(&stm, 0, sizeof(stm));
            auto p = strptime(timeStr.c_str(), "%Y-%m-%d %H:%M:%S", &stm);
            time_t t = mktime(&stm);
            size_t decimalNum = 0;
            if (p) {
                if (*p == '.') {
                    std::string decimals(p + 1, &timeStr[timeStr.length()]);
                    while (decimals.length() < 6) {
                        decimals += "0";
                    }
                    decimalNum = (size_t)atol(decimals.c_str());
                }
                savedOn_ =
                    std::make_shared<::trantor::Date>(t * 1000000 + decimalNum);
            }
        }
    }
}

const std::string &Globalstates::getValueOfSessionId() const noexcept {
    const static std::string defaultValue = std::string();
    if (sessionId_)
        return *sessionId_;
    return defaultValue;
}
const std::shared_ptr<std::string> &Globalstates::getSessionId()
    const noexcept {
    return sessionId_;
}
void Globalstates::setSessionId(const std::string &pSessionId) noexcept {
    sessionId_ = std::make_shared<std::string>(pSessionId);
    dirtyFlag_[0] = true;
}
void Globalstates::setSessionId(std::string &&pSessionId) noexcept {
    sessionId_ = std::make_shared<std::string>(std::move(pSessionId));
    dirtyFlag_[0] = true;
}
const typename Globalstates::PrimaryKeyType &Globalstates::getPrimaryKey()
    const {
    assert(sessionId_);
    return *sessionId_;
}

const std::string &Globalstates::getValueOfGlobalstate() const noexcept {
    const static std::string defaultValue = std::string();
    if (globalstate_)
        return *globalstate_;
    return defaultValue;
}
const std::shared_ptr<std::string> &Globalstates::getGlobalstate()
    const noexcept {
    return globalstate_;
}
void Globalstates::setGlobalstate(const std::string &pGlobalstate) noexcept {
    globalstate_ = std::make_shared<std::string>(pGlobalstate);
    dirtyFlag_[1] = true;
}
void Globalstates::setGlobalstate(std::string &&pGlobalstate) noexcept {
    globalstate_ = std::make_shared<std::string>(std::move(pGlobalstate));
    dirtyFlag_[1] = true;
}
void Globalstates::setGlobalstateToNull() noexcept {
    globalstate_.reset();
    dirtyFlag_[1] = true;
}

const bool &Globalstates::getValueOfIsTerminal() const noexcept {
    const static bool defaultValue = bool();
    if (isTerminal_)
        return *isTerminal_;
    return defaultValue;
}
const std::shared_ptr<bool> &Globalstates::getIsTerminal() const noexcept {
    return isTerminal_;
}
void Globalstates::setIsTerminal(const bool &pIsTerminal) noexcept {
    isTerminal_ = std::make_shared<bool>(pIsTerminal);
    dirtyFlag_[2] = true;
}

const ::trantor::Date &Globalstates::getValueOfSavedOn() const noexcept {
    const static ::trantor::Date defaultValue = ::trantor::Date();
    if (savedOn_)
        return *savedOn_;
    return defaultValue;
}
const std::shared_ptr<::trantor::Date> &Globalstates::getSavedOn()
    const noexcept {
    return savedOn_;
}
void Globalstates::setSavedOn(const ::trantor::Date &pSavedOn) noexcept {
    savedOn_ = std::make_shared<::trantor::Date>(pSavedOn);
    dirtyFlag_[3] = true;
}
void Globalstates::setSavedOnToNull() noexcept {
    savedOn_.reset();
    dirtyFlag_[3] = true;
}

void Globalstates::updateId(const uint64_t id) {
}

const std::vector<std::string> &Globalstates::insertColumns() noexcept {
    static const std::vector<std::string> inCols = {"session_id", "globalstate",
                                                    "is_terminal", "saved_on"};
    return inCols;
}

void Globalstates::outputArgs(drogon::orm::internal::SqlBinder &binder) const {
    if (dirtyFlag_[0]) {
        if (getSessionId()) {
            binder << getValueOfSessionId();
        } else {
            binder << nullptr;
        }
    }
    if (dirtyFlag_[1]) {
        if (getGlobalstate()) {
            binder << getValueOfGlobalstate();
        } else {
            binder << nullptr;
        }
    }
    if (dirtyFlag_[2]) {
        if (getIsTerminal()) {
            binder << getValueOfIsTerminal();
        } else {
            binder << nullptr;
        }
    }
    if (dirtyFlag_[3]) {
        if (getSavedOn()) {
            binder << getValueOfSavedOn();
        } else {
            binder << nullptr;
        }
    }
}

const std::vector<std::string> Globalstates::updateColumns() const {
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
    return ret;
}

void Globalstates::updateArgs(drogon::orm::internal::SqlBinder &binder) const {
    if (dirtyFlag_[0]) {
        if (getSessionId()) {
            binder << getValueOfSessionId();
        } else {
            binder << nullptr;
        }
    }
    if (dirtyFlag_[1]) {
        if (getGlobalstate()) {
            binder << getValueOfGlobalstate();
        } else {
            binder << nullptr;
        }
    }
    if (dirtyFlag_[2]) {
        if (getIsTerminal()) {
            binder << getValueOfIsTerminal();
        } else {
            binder << nullptr;
        }
    }
    if (dirtyFlag_[3]) {
        if (getSavedOn()) {
            binder << getValueOfSavedOn();
        } else {
            binder << nullptr;
        }
    }
}
Json::Value Globalstates::toJson() const {
    Json::Value ret;
    if (getSessionId()) {
        ret["session_id"] = getValueOfSessionId();
    } else {
        ret["session_id"] = Json::Value();
    }
    if (getGlobalstate()) {
        ret["globalstate"] = getValueOfGlobalstate();
    } else {
        ret["globalstate"] = Json::Value();
    }
    if (getIsTerminal()) {
        ret["is_terminal"] = getValueOfIsTerminal();
    } else {
        ret["is_terminal"] = Json::Value();
    }
    if (getSavedOn()) {
        ret["saved_on"] = getSavedOn()->toDbStringLocal();
    } else {
        ret["saved_on"] = Json::Value();
    }
    return ret;
}

Json::Value Globalstates::toMasqueradedJson(
    const std::vector<std::string> &pMasqueradingVector) const {
    Json::Value ret;
    if (pMasqueradingVector.size() == 4) {
        if (!pMasqueradingVector[0].empty()) {
            if (getSessionId()) {
                ret[pMasqueradingVector[0]] = getValueOfSessionId();
            } else {
                ret[pMasqueradingVector[0]] = Json::Value();
            }
        }
        if (!pMasqueradingVector[1].empty()) {
            if (getGlobalstate()) {
                ret[pMasqueradingVector[1]] = getValueOfGlobalstate();
            } else {
                ret[pMasqueradingVector[1]] = Json::Value();
            }
        }
        if (!pMasqueradingVector[2].empty()) {
            if (getIsTerminal()) {
                ret[pMasqueradingVector[2]] = getValueOfIsTerminal();
            } else {
                ret[pMasqueradingVector[2]] = Json::Value();
            }
        }
        if (!pMasqueradingVector[3].empty()) {
            if (getSavedOn()) {
                ret[pMasqueradingVector[3]] = getSavedOn()->toDbStringLocal();
            } else {
                ret[pMasqueradingVector[3]] = Json::Value();
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
    if (getGlobalstate()) {
        ret["globalstate"] = getValueOfGlobalstate();
    } else {
        ret["globalstate"] = Json::Value();
    }
    if (getIsTerminal()) {
        ret["is_terminal"] = getValueOfIsTerminal();
    } else {
        ret["is_terminal"] = Json::Value();
    }
    if (getSavedOn()) {
        ret["saved_on"] = getSavedOn()->toDbStringLocal();
    } else {
        ret["saved_on"] = Json::Value();
    }
    return ret;
}

bool Globalstates::validateJsonForCreation(const Json::Value &pJson,
                                           std::string &err) {
    if (pJson.isMember("session_id")) {
        if (!validJsonOfField(0, "session_id", pJson["session_id"], err, true))
            return false;
    } else {
        err = "The session_id column cannot be null";
        return false;
    }
    if (pJson.isMember("globalstate")) {
        if (!validJsonOfField(1, "globalstate", pJson["globalstate"], err,
                              true))
            return false;
    }
    if (pJson.isMember("is_terminal")) {
        if (!validJsonOfField(2, "is_terminal", pJson["is_terminal"], err,
                              true))
            return false;
    }
    if (pJson.isMember("saved_on")) {
        if (!validJsonOfField(3, "saved_on", pJson["saved_on"], err, true))
            return false;
    }
    return true;
}
bool Globalstates::validateMasqueradedJsonForCreation(
    const Json::Value &pJson,
    const std::vector<std::string> &pMasqueradingVector,
    std::string &err) {
    if (pMasqueradingVector.size() != 4) {
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
            }
        }
        if (!pMasqueradingVector[2].empty()) {
            if (pJson.isMember(pMasqueradingVector[2])) {
                if (!validJsonOfField(2, pMasqueradingVector[2],
                                      pJson[pMasqueradingVector[2]], err, true))
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
    } catch (const Json::LogicError &e) {
        err = e.what();
        return false;
    }
    return true;
}
bool Globalstates::validateJsonForUpdate(const Json::Value &pJson,
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
    if (pJson.isMember("globalstate")) {
        if (!validJsonOfField(1, "globalstate", pJson["globalstate"], err,
                              false))
            return false;
    }
    if (pJson.isMember("is_terminal")) {
        if (!validJsonOfField(2, "is_terminal", pJson["is_terminal"], err,
                              false))
            return false;
    }
    if (pJson.isMember("saved_on")) {
        if (!validJsonOfField(3, "saved_on", pJson["saved_on"], err, false))
            return false;
    }
    return true;
}
bool Globalstates::validateMasqueradedJsonForUpdate(
    const Json::Value &pJson,
    const std::vector<std::string> &pMasqueradingVector,
    std::string &err) {
    if (pMasqueradingVector.size() != 4) {
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
    } catch (const Json::LogicError &e) {
        err = e.what();
        return false;
    }
    return true;
}
bool Globalstates::validJsonOfField(size_t index,
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
                return true;
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
            if (!pJson.isBool()) {
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
        default:
            err = "Internal error in the server";
            return false;
            break;
    }
    return true;
}
