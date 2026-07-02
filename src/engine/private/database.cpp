#include "../database.h"

Table::Table(string name, string dbName, string columns)
    : name(name)
{
    database = new Database(dbName);
    database->execute(
        "CREATE TABLE IF NOT EXISTS " + name + "(id INT AUTO_INCREMENT PRIMARY KEY, " +
        columns + ")");
}

Database::Database(string name)
    : name(name), source("data/database/" + name + ".db")
{
    if (sqlite3_initialize() != SQLITE_OK)
        print("SQLite3 Unloaded: " + (string)sqlite3_errmsg(db));
    if (sqlite3_open(source.c_str(), &db) != SQLITE_OK)
        print("Database Unloaded: " + (string)sqlite3_errmsg(db));
}

int Database::execute(string sql)
{
    print("Executing Database Statement: \n" + sql);
    char *errMsg = nullptr;
    return sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
};

void Database::createTable(string tableName, string sql)
{
    tables.insert({tableName, Table(tableName, name, sql)});
};

void Database::deleteTable(string tableName)
{
    execute("DROP TABLE " + tableName + ";");
}

Database::~Database()
{
    if (db != nullptr)
        sqlite3_close(db);
}