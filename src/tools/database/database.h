#pragma once

#include "core/config.h"

class Database
{
public:
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt;
    string name = "", source = "";
    Database(string name);
    void execute(string sql);
    ~Database();
};