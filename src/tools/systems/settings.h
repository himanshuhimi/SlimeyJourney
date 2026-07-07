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
    map<string, vector<string>> allowedData = {};
    map<string, Option> graphicsData = {};
    map<string, string> defaultData = {};
    Settings();
    void load();
    void uploadData(string tableName);
    void loadTables();
    void loadData();
    void loadDefaults();
    void update(string tableName, string key, string value);
    string get(string tableName, string key);
};