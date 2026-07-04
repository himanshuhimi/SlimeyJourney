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
    allowedData = {
        {"size", {"1920x1080", "1280x720"}},
        {"fps", {"30", "60", "120", "Infinite"}},
        {"vsync", {"0", "1"}}
    };
    defaultData = {
        {"size", "1920x1080"},
        {"fps", "60"},
        {"vsync", "1"}
    };
    db = new Database("settings");
    load();
}

void Settings::load()
{
    loadTables();
    loadData();
}

void Settings::uploadData(string tableName)
{
    bool first = true;
    string vals = "";
    for (auto &[key, option] : graphicsData)
    {
        if (!first)
            vals += ",";
        vals += "('" + key + "','" + option.currentVal + "')";
        first = false;
    };
    db->execute("INSERT INTO " + tableName + "(key, value) VALUES " + vals + ";");
}

void Settings::loadTables()
{
    for (auto &tableName : tableNames)
        if (!db->existsTable(tableName))
            db->createTable(tableName, "`key` VARCHAR(600) UNIQUE, value ANY");
}

void Settings::loadData()
{
    for (auto &tableName : tableNames)
    {
        Table table = db->tables.at(tableName);
        DBResult data = db->selectTable(tableName, "key, value");
        if (!data.empty())
        {
            for (auto &row : data)
            {
                string key = row.at("key");
                string value = row.at("value");
                Option option{value, allowedData.at(key)};
                graphicsData.insert({key, option});
            }
        }
        else
        {
            loadDefaults();
            uploadData(tableName);
        }
    }
}

void Settings::loadDefaults()
{
    vector<string> graphicKeys = {"size", "fps", "vsync"};
    for (auto &graphicKey : graphicKeys)
    {
        Option option{defaultData.at(graphicKey), allowedData.at(graphicKey)};
        graphicsData.insert({graphicKey, option});
    }
}

void Settings::update(string tableName, string key, string value)
{
    db->execute("UPDATE " + tableName + " SET value = '" + value +
                "' WHERE key = '" + key + "'");
    graphicsData.at(key).update(value);
}

string Settings::get(string tableName, string key)
{
    return graphicsData.at(key).currentVal;
}