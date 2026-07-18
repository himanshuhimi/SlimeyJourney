#include "../../systems/settings.h"

vector<string> SettingBool = {"0", "1"};

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
        {
            "graphics",
            {{"size", {"1920x1080", "1280x720", "640x360"}},
            {"fps", {"30", "60", "120", "Infinite"}},
            {"vsync", SettingBool}}
        }
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

void Settings::uploadData(string tableName, SettingType data)
{
    bool first = true;
    string vals = "";
    for (auto &[key, option] : data)
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
        DBResult dbData = db->selectTable(tableName, "key, value");
        if (!dbData.empty())
        {
            for (auto &row : dbData)
            {
                string key = row.at("key");
                string value = row.at("value");
                Option option{value, allowedData.at(tableName).at(key)};
                try
                {
                    data.at(tableName).insert({key, option});
                }
                catch (std::exception &e)
                {
                    data[tableName];
                    data.at(tableName).insert({key, option});
                }
            }
        }
        else
            loadDefaults();
    }
}

void Settings::loadDefaults()
{
    map<string, vector<string>> tableKeys = {
        {"graphics", {"size", "fps", "vsync"}}
    };
    for (auto &[table, keys] : tableKeys)
    {
        for (auto &key : keys)
        {
            Option option{defaultData.at(key), allowedData.at(table).at(key)};
            data[table].insert({{key, option}});
        }
    }
    for (auto &[table, _] : tableKeys)
        uploadData(table, data.at(table));
}

void Settings::update(string tableName, string key, string value)
{
    db->execute("UPDATE " + tableName + " SET value = '" + value +
                "' WHERE key = '" + key + "'");
    try
    {
        data.at(tableName).at(key).update(value);
    }
    catch (std::exception &e)
    {
        print("");
    }
}

string Settings::get(string tableName, string key)
{
    try
    {
        return data.at(tableName).at(key).currentVal;
    }
    catch (std::exception &e)
    {
        return "";
    }
};