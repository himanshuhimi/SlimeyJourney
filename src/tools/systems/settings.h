#pragma once

#include "core/config.h"
#include "engine/database.h"

extern vector<string> SettingBool;

class Settings
{
public:
    Database *db = nullptr;
    vector<string> tableNames = {};
    struct Option
    {
        string defaultVal;
        string defaultIdx;
        string currentVal;
        vector<string> allowed;
        Option(string value, vector<string> allowed);
        void update(string value);
    };
    using SettingType = map<string, Option>;
    map<string, SettingType> data = {};
    map<string, string> defaultData = {};
    map<string, map<string, vector<string>>> allowedData = {};
    Settings();
    void load();
    void uploadData(string tableName);
    void loadTables();
    void loadData();
    void loadDefaults();
    void update(string tableName, string key, string value);
    string get(string tableName, string key);
};