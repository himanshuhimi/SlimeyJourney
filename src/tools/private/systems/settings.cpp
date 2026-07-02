#include "../../systems/settings.h"

Settings::Option::Option(string value, vector<string> allowed)
    : defaultVal(value), currentVal(value), allowed(allowed)
{
    auto iterator = std::find(allowed.begin(), allowed.end(), value);
    if (iterator != allowed.end())
        defaultIdx = std::distance(allowed.begin(), iterator);
}

void Settings::Option::update(string value)
{
    if (std::find(allowed.begin(), allowed.end(), value) == allowed.end())
        print("Value not found in selectable Options.");
    else
        currentVal = value;
}

Settings::Settings()
{
    tableNames = {"graphics"};
    graphicsValues = {
        {"size", Option{"1920x1080", {"1920x1080", "1280x720"}}},
        {"fps", Option{"60", {"30", "60", "120", "Infinite"}}},
        {"vsync", Option("1", {"0", "1"})}
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
    for (auto &[setting, option] : graphicsValues)
    {
        string value = "'" + setting + "','" + option.defaultVal + "'";
        db->execute("INSERT INTO graphics (key, value) VALUES (" + value + ")");
    }
}

void Settings::update(string tableName, string key, string value)
{
    db->execute("UPDATE " + tableName + " SET value = '" + value +
                "' WHERE name = '" + key + "'");
    graphicsValues.at(key).update(value);
}

string Settings::get(string tableName, string key)
{
    return graphicsValues.at(key).currentVal;
}