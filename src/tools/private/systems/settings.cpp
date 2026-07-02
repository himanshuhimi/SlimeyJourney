#include "../../systems/settings.h"

Settings::Settings()
{
    tableNames = {"graphics"};
    graphicsDefault = {
        {"size", "1920x1080"},
        {"FPS", "60"}
    };
    db = new Database("settings");
    load();
}

void Settings::load()
{
    loadTables();
    loadDefaults();
}

void Settings::loadTables()
{
    for (auto &tableName : tableNames)
        db->createTable(tableName, "`key` VARCHAR(600) UNIQUE, value ANY");
}

void Settings::loadDefaults()
{
    for (auto &[graphicName, graphicValue] : graphicsDefault)
    {
        string values = "'" + graphicName + "', " + graphicValue;
        db->execute("INSERT INTO graphics (key, value) VALUES (" + values + ")");
    };
}