#pragma once

#include "core/config.h"

using DBRow = map<string, string>;
using DBResult = vector<DBRow>;

class Database;
class Table
{
public:
    Database *db = nullptr;
    string name = "";
    Table(string name, string dbName, string columns);
};

class Database
{
public:
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt;
    map<string, Table> tables = {};
    string name = "", source = "";
    Database(string name);
    int execute(
        string sql, 
        int (*callback)(void *, int, char **, char **) = nullptr,
        void *data = nullptr);
    void createTable(string tableName, string sql = "");
    DBResult selectTable(string tableName, string what, string where = " ");
    bool existsTable(string tableName);
    void dropTable(string tableName);
    ~Database();
};