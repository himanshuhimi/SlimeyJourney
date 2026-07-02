#pragma once

#include "core/config.h"
#include "engine/database.h"

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
    map<string, Option> graphicsValues = {};
    Settings();
    void load();
    void loadTables();
    void loadDefaults();
    void update(string tableName, string key, string value);
    string get(string tableName, string key);
};