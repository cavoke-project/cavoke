/**
 *
 *  Globalstates.h
 *  DO NOT EDIT. This file is generated by drogon_ctl
 *
 */

#pragma once
#include <drogon/orm/Field.h>
#include <drogon/orm/Mapper.h>
#include <drogon/orm/Result.h>
#include <drogon/orm/Row.h>
#include <drogon/orm/SqlBinder.h>
#ifdef __cpp_impl_coroutine
#include <drogon/orm/CoroMapper.h>
#endif
#include <json/json.h>
#include <stdint.h>
#include <trantor/utils/Date.h>
#include <trantor/utils/Logger.h>
#include <iostream>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

namespace drogon {
namespace orm {
class DbClient;
using DbClientPtr = std::shared_ptr<DbClient>;
}  // namespace orm
}  // namespace drogon
namespace drogon_model {
namespace cavoke {

class Globalstates {
public:
    struct Cols {
        static const std::string _session_id;
        static const std::string _globalstate;
        static const std::string _is_terminal;
    };

    const static int primaryKeyNumber;
    const static std::string tableName;
    const static bool hasPrimaryKey;
    const static std::string primaryKeyName;
    using PrimaryKeyType = std::string;
    const PrimaryKeyType &getPrimaryKey() const;

    /**
     * @brief constructor
     * @param r One row of records in the SQL query result.
     * @param indexOffset Set the offset to -1 to access all columns by column
     * names, otherwise access all columns by offsets.
     * @note If the SQL is not a style of 'select * from table_name ...' (select
     * all columns by an asterisk), please set the offset to -1.
     */
    explicit Globalstates(const drogon::orm::Row &r,
                          const ssize_t indexOffset = 0) noexcept;

    /**
     * @brief constructor
     * @param pJson The json object to construct a new instance.
     */
    explicit Globalstates(const Json::Value &pJson) noexcept(false);

    /**
     * @brief constructor
     * @param pJson The json object to construct a new instance.
     * @param pMasqueradingVector The aliases of table columns.
     */
    Globalstates(
        const Json::Value &pJson,
        const std::vector<std::string> &pMasqueradingVector) noexcept(false);

    Globalstates() = default;

    void updateByJson(const Json::Value &pJson) noexcept(false);
    void updateByMasqueradedJson(
        const Json::Value &pJson,
        const std::vector<std::string> &pMasqueradingVector) noexcept(false);
    static bool validateJsonForCreation(const Json::Value &pJson,
                                        std::string &err);
    static bool validateMasqueradedJsonForCreation(
        const Json::Value &,
        const std::vector<std::string> &pMasqueradingVector,
        std::string &err);
    static bool validateJsonForUpdate(const Json::Value &pJson,
                                      std::string &err);
    static bool validateMasqueradedJsonForUpdate(
        const Json::Value &,
        const std::vector<std::string> &pMasqueradingVector,
        std::string &err);
    static bool validJsonOfField(size_t index,
                                 const std::string &fieldName,
                                 const Json::Value &pJson,
                                 std::string &err,
                                 bool isForCreation);

    /**  For column session_id  */
    /// Get the value of the column session_id, returns the default value if the
    /// column is null
    const std::string &getValueOfSessionId() const noexcept;
    /// Return a shared_ptr object pointing to the column const value, or an
    /// empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getSessionId() const noexcept;
    /// Set the value of the column session_id
    void setSessionId(const std::string &pSessionId) noexcept;
    void setSessionId(std::string &&pSessionId) noexcept;

    /**  For column globalstate  */
    /// Get the value of the column globalstate, returns the default value if
    /// the column is null
    const std::string &getValueOfGlobalstate() const noexcept;
    /// Return a shared_ptr object pointing to the column const value, or an
    /// empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getGlobalstate() const noexcept;
    /// Set the value of the column globalstate
    void setGlobalstate(const std::string &pGlobalstate) noexcept;
    void setGlobalstate(std::string &&pGlobalstate) noexcept;
    void setGlobalstateToNull() noexcept;

    /**  For column is_terminal  */
    /// Get the value of the column is_terminal, returns the default value if
    /// the column is null
    const bool &getValueOfIsTerminal() const noexcept;
    /// Return a shared_ptr object pointing to the column const value, or an
    /// empty shared_ptr object if the column is null
    const std::shared_ptr<bool> &getIsTerminal() const noexcept;
    /// Set the value of the column is_terminal
    void setIsTerminal(const bool &pIsTerminal) noexcept;

    static size_t getColumnNumber() noexcept {
        return 3;
    }
    static const std::string &getColumnName(size_t index) noexcept(false);

    Json::Value toJson() const;
    Json::Value toMasqueradedJson(
        const std::vector<std::string> &pMasqueradingVector) const;
    /// Relationship interfaces
private:
    friend drogon::orm::Mapper<Globalstates>;
#ifdef __cpp_impl_coroutine
    friend drogon::orm::CoroMapper<Globalstates>;
#endif
    static const std::vector<std::string> &insertColumns() noexcept;
    void outputArgs(drogon::orm::internal::SqlBinder &binder) const;
    const std::vector<std::string> updateColumns() const;
    void updateArgs(drogon::orm::internal::SqlBinder &binder) const;
    /// For mysql or sqlite3
    void updateId(const uint64_t id);
    std::shared_ptr<std::string> sessionId_;
    std::shared_ptr<std::string> globalstate_;
    std::shared_ptr<bool> isTerminal_;
    struct MetaData {
        const std::string colName_;
        const std::string colType_;
        const std::string colDatabaseType_;
        const ssize_t colLength_;
        const bool isAutoVal_;
        const bool isPrimaryKey_;
        const bool notNull_;
    };
    static const std::vector<MetaData> metaData_;
    bool dirtyFlag_[3] = {false};

public:
    static const std::string &sqlForFindingByPrimaryKey() {
        static const std::string sql =
            "select * from " + tableName + " where session_id = $1";
        return sql;
    }

    static const std::string &sqlForDeletingByPrimaryKey() {
        static const std::string sql =
            "delete from " + tableName + " where session_id = $1";
        return sql;
    }
    std::string sqlForInserting(bool &needSelection) const {
        std::string sql = "insert into " + tableName + " (";
        size_t parametersCount = 0;
        needSelection = false;
        if (dirtyFlag_[0]) {
            sql += "session_id,";
            ++parametersCount;
        }
        if (dirtyFlag_[1]) {
            sql += "globalstate,";
            ++parametersCount;
        }
        sql += "is_terminal,";
        ++parametersCount;
        if (!dirtyFlag_[2]) {
            needSelection = true;
        }
        if (parametersCount > 0) {
            sql[sql.length() - 1] = ')';
            sql += " values (";
        } else
            sql += ") values (";

        int placeholder = 1;
        char placeholderStr[64];
        size_t n = 0;
        if (dirtyFlag_[0]) {
            n = sprintf(placeholderStr, "$%d,", placeholder++);
            sql.append(placeholderStr, n);
        }
        if (dirtyFlag_[1]) {
            n = sprintf(placeholderStr, "$%d,", placeholder++);
            sql.append(placeholderStr, n);
        }
        if (dirtyFlag_[2]) {
            n = sprintf(placeholderStr, "$%d,", placeholder++);
            sql.append(placeholderStr, n);
        } else {
            sql += "default,";
        }
        if (parametersCount > 0) {
            sql.resize(sql.length() - 1);
        }
        if (needSelection) {
            sql.append(") returning *");
        } else {
            sql.append(1, ')');
        }
        LOG_TRACE << sql;
        return sql;
    }
};
}  // namespace cavoke
}  // namespace drogon_model
