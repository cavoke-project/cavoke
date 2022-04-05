/**
 *
 *  Players.h
 *  DO NOT EDIT. This file is generated by drogon_ctl
 *
 */

#pragma once
#include <drogon/orm/Result.h>
#include <drogon/orm/Row.h>
#include <drogon/orm/Field.h>
#include <drogon/orm/SqlBinder.h>
#include <drogon/orm/Mapper.h>
#ifdef __cpp_impl_coroutine
#include <drogon/orm/CoroMapper.h>
#endif
#include <trantor/utils/Date.h>
#include <trantor/utils/Logger.h>
#include <json/json.h>
#include <string>
#include <memory>
#include <vector>
#include <tuple>
#include <stdint.h>
#include <iostream>

namespace drogon
{
namespace orm
{
class DbClient;
using DbClientPtr = std::shared_ptr<DbClient>;
}
}
namespace drogon_model
{
namespace cavoke_test
{
class Sessions;
class Users;

class Players
{
  public:
    struct Cols
    {
        static const std::string _session_id;
        static const std::string _user_id;
        static const std::string _player_id;
    };

    const static int primaryKeyNumber;
    const static std::string tableName;
    const static bool hasPrimaryKey;
    const static std::vector<std::string> primaryKeyName;
    using PrimaryKeyType = std::tuple<std::string,std::string>;//session_id,user_id
    PrimaryKeyType getPrimaryKey() const;

    /**
     * @brief constructor
     * @param r One row of records in the SQL query result.
     * @param indexOffset Set the offset to -1 to access all columns by column names,
     * otherwise access all columns by offsets.
     * @note If the SQL is not a style of 'select * from table_name ...' (select all
     * columns by an asterisk), please set the offset to -1.
     */
    explicit Players(const drogon::orm::Row &r, const ssize_t indexOffset = 0) noexcept;

    /**
     * @brief constructor
     * @param pJson The json object to construct a new instance.
     */
    explicit Players(const Json::Value &pJson) noexcept(false);

    /**
     * @brief constructor
     * @param pJson The json object to construct a new instance.
     * @param pMasqueradingVector The aliases of table columns.
     */
    Players(const Json::Value &pJson, const std::vector<std::string> &pMasqueradingVector) noexcept(false);

    Players() = default;

    void updateByJson(const Json::Value &pJson) noexcept(false);
    void updateByMasqueradedJson(const Json::Value &pJson,
                                 const std::vector<std::string> &pMasqueradingVector) noexcept(false);
    static bool validateJsonForCreation(const Json::Value &pJson, std::string &err);
    static bool validateMasqueradedJsonForCreation(const Json::Value &,
                                                const std::vector<std::string> &pMasqueradingVector,
                                                    std::string &err);
    static bool validateJsonForUpdate(const Json::Value &pJson, std::string &err);
    static bool validateMasqueradedJsonForUpdate(const Json::Value &,
                                          const std::vector<std::string> &pMasqueradingVector,
                                          std::string &err);
    static bool validJsonOfField(size_t index,
                          const std::string &fieldName,
                          const Json::Value &pJson,
                          std::string &err,
                          bool isForCreation);

    /**  For column session_id  */
    ///Get the value of the column session_id, returns the default value if the column is null
    const std::string &getValueOfSessionId() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getSessionId() const noexcept;
    ///Set the value of the column session_id
    void setSessionId(const std::string &pSessionId) noexcept;
    void setSessionId(std::string &&pSessionId) noexcept;

    /**  For column user_id  */
    ///Get the value of the column user_id, returns the default value if the column is null
    const std::string &getValueOfUserId() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getUserId() const noexcept;
    ///Set the value of the column user_id
    void setUserId(const std::string &pUserId) noexcept;
    void setUserId(std::string &&pUserId) noexcept;

    /**  For column player_id  */
    ///Get the value of the column player_id, returns the default value if the column is null
    const int32_t &getValueOfPlayerId() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<int32_t> &getPlayerId() const noexcept;
    ///Set the value of the column player_id
    void setPlayerId(const int32_t &pPlayerId) noexcept;


    static size_t getColumnNumber() noexcept {  return 3;  }
    static const std::string &getColumnName(size_t index) noexcept(false);

    Json::Value toJson() const;
    Json::Value toMasqueradedJson(const std::vector<std::string> &pMasqueradingVector) const;
    /// Relationship interfaces
    void getSession(const drogon::orm::DbClientPtr &clientPtr,
                    const std::function<void(Sessions)> &rcb,
                    const drogon::orm::ExceptionCallback &ecb) const;
    void getUser(const drogon::orm::DbClientPtr &clientPtr,
                 const std::function<void(Users)> &rcb,
                 const drogon::orm::ExceptionCallback &ecb) const;
  private:
    friend drogon::orm::Mapper<Players>;
#ifdef __cpp_impl_coroutine
    friend drogon::orm::CoroMapper<Players>;
#endif
    static const std::vector<std::string> &insertColumns() noexcept;
    void outputArgs(drogon::orm::internal::SqlBinder &binder) const;
    const std::vector<std::string> updateColumns() const;
    void updateArgs(drogon::orm::internal::SqlBinder &binder) const;
    ///For mysql or sqlite3
    void updateId(const uint64_t id);
    std::shared_ptr<std::string> sessionId_;
    std::shared_ptr<std::string> userId_;
    std::shared_ptr<int32_t> playerId_;
    struct MetaData
    {
        const std::string colName_;
        const std::string colType_;
        const std::string colDatabaseType_;
        const ssize_t colLength_;
        const bool isAutoVal_;
        const bool isPrimaryKey_;
        const bool notNull_;
    };
    static const std::vector<MetaData> metaData_;
    bool dirtyFlag_[3]={ false };
  public:
    static const std::string &sqlForFindingByPrimaryKey()
    {
        static const std::string sql="select * from " + tableName + " where session_id = $1 and user_id = $2";
        return sql;
    }

    static const std::string &sqlForDeletingByPrimaryKey()
    {
        static const std::string sql="delete from " + tableName + " where session_id = $1 and user_id = $2";
        return sql;
    }
    std::string sqlForInserting(bool &needSelection) const
    {
        std::string sql="insert into " + tableName + " (";
        size_t parametersCount = 0;
        needSelection = false;
        if(dirtyFlag_[0])
        {
            sql += "session_id,";
            ++parametersCount;
        }
        if(dirtyFlag_[1])
        {
            sql += "user_id,";
            ++parametersCount;
        }
        if(dirtyFlag_[2])
        {
            sql += "player_id,";
            ++parametersCount;
        }
        if(parametersCount > 0)
        {
            sql[sql.length()-1]=')';
            sql += " values (";
        }
        else
            sql += ") values (";

        int placeholder=1;
        char placeholderStr[64];
        size_t n=0;
        if(dirtyFlag_[0])
        {
            n = sprintf(placeholderStr,"$%d,",placeholder++);
            sql.append(placeholderStr, n);
        }
        if(dirtyFlag_[1])
        {
            n = sprintf(placeholderStr,"$%d,",placeholder++);
            sql.append(placeholderStr, n);
        }
        if(dirtyFlag_[2])
        {
            n = sprintf(placeholderStr,"$%d,",placeholder++);
            sql.append(placeholderStr, n);
        }
        if(parametersCount > 0)
        {
            sql.resize(sql.length() - 1);
        }
        if(needSelection)
        {
            sql.append(") returning *");
        }
        else
        {
            sql.append(1, ')');
        }
        LOG_TRACE << sql;
        return sql;
    }
};
} // namespace cavoke_test
} // namespace drogon_model
