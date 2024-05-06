#include "connectors/cringe_database.h"

#include <stdexcept>

CringeDB::CringeDB(const std::string &dbFile) : db(nullptr) {
    if (sqlite3_open(dbFile.c_str(), &db) != SQLITE_OK) {
        throw std::runtime_error("Can't open database: " +
                                 std::string(sqlite3_errmsg(db)));
    }
}

CringeDB::~CringeDB() {
    if (db) {
        sqlite3_close(db);
    }
}

void CringeDB::execute(const std::string &sql,
                       const std::vector<std::string> &params) const {
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &statement, nullptr) !=
        SQLITE_OK) {
        throw std::runtime_error("Failed to prepare SQL statement: " +
                                 std::string(sqlite3_errmsg(db)));
    }
    int paramIndex = 1;
    for (const auto &param : params) {
        sqlite3_bind_text(statement, paramIndex++, param.c_str(), -1,
                          SQLITE_STATIC);
    }
    if (sqlite3_step(statement) != SQLITE_DONE) {
        sqlite3_finalize(statement);
        throw std::runtime_error("Failed to execute SQL statement: " +
                                 std::string(sqlite3_errmsg(db)));
    }
    sqlite3_finalize(statement);
}

std::vector<std::vector<std::string>> CringeDB::query(const std::string &sql) {
    std::vector<std::vector<std::string>> result;
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &statement, nullptr) !=
        SQLITE_OK) {
        throw std::runtime_error("Failed to prepare SQL statement: " +
                                 std::string(sqlite3_errmsg(db)));
    }
    while (sqlite3_step(statement) == SQLITE_ROW) {
        std::vector<std::string> row;
        row.reserve(sqlite3_column_count(statement));
        for (int i = 0; i < sqlite3_column_count(statement); ++i) {
            row.emplace_back((const char *)(sqlite3_column_text(statement, i)));
        }
        result.push_back(row);
    }
    sqlite3_finalize(statement);
    return result;
}