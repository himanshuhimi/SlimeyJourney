#include "../database.h"

Table::Table(string name, string dbName, string columns)
    : name(name)
{
    db = new Database(dbName);
}

Database::Database(string name)
    : name(name), source("data/database/" + name + ".db")
{
    if (sqlite3_initialize() != SQLITE_OK)
        print("SQLite3 Unloaded: " + (string)sqlite3_errmsg(db));
    if (sqlite3_open(source.c_str(), &db) != SQLITE_OK)
        print("Database Unloaded: " + (string)sqlite3_errmsg(db));
}

int Database::execute(
    string sql,
    int (*callback)(void *, int, char **, char **),
    void *data)
{
    print("Executing SQL Statement: \n" + sql);
    char *errMsg = nullptr;
    return sqlite3_exec(db, sql.c_str(), callback, data, &errMsg);
};

void Database::createTable(string tableName, string sql)
{
    if (existsTable(tableName))
        return;
    execute("CREATE TABLE IF NOT EXISTS " + tableName + "(" + sql + ");");
    tables.insert({tableName, Table(tableName, name, sql)});
};

DBResult Database::selectTable(string tableName, string what, string where)
{
    string sql = "SELECT " + what + " FROM " + tableName +
                 (where.empty() ? " WHERE " + where : " ") + ";";
    print("Executing SQL Statement: \n" + sql);
    DBResult res;
    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
        print("Cannot prepare Statement");
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        DBRow row;
        int cols = sqlite3_column_count(stmt);
        for (int i = 0; i < cols; i++)
        {
            string columnName = sqlite3_column_name(stmt, i);
            if (sqlite3_column_type(stmt, i) == SQLITE_NULL)
                row[columnName] = "";
            else
            {
                const unsigned char *text = sqlite3_column_text(stmt, i);
                row[columnName] = reinterpret_cast<const char *>(text);
            }
        }
        res.emplace_back(std::move(row));
    }
    sqlite3_finalize(stmt);
    return res;
}

bool Database::existsTable(string tableName)
{
    return tables.find(tableName) != tables.end();
}

void Database::dropTable(string tableName)
{
    execute("DROP TABLE " + tableName + ";");
}

Database::~Database()
{
    if (db != nullptr)
        sqlite3_close(db);
}