#include "../../database/database.h"

Database::Database(string name)
    : name(name), source("data/" + name + ".db")
{
    if (sqlite3_initialize() != SQLITE_OK)
        print("SQLite3 Unloaded: " + (string)sqlite3_errmsg(db));
    if (sqlite3_open(source.c_str(), &db) != SQLITE_OK)
        print("Database Unloaded: " + (string)sqlite3_errmsg(db));
}

void Database::execute(string sql)
{
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
};

Database::~Database()
{
    if (db != nullptr)
        sqlite3_close(db);
}