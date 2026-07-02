#pragma once

#include "core/config.h"

class Database;
class Table
{
public:
    Database *database = nullptr;
    string name = "";
    Table(string name, string dbName, string columns = "");
};

class Database
{
public:
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt;
    map<string, Table> tables = {};
    string name = "", source = "";
    Database(string name);
    int execute(string sql);
    void createTable(string tableName, string sql = "");
    void deleteTable(string tableName);
    ~Database();
};