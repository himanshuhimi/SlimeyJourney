#pragma once

#include "core/config.h"
#include "engine/database.h"

class Settings
{
public:
    Database *db = nullptr;
    vector<string> tableNames = {};
    vector<std::pair<string, string>> graphicsDefault = {};
    Settings();
    void load();
    void loadTables();
    void loadDefaults();
    // void apply();
    // void get();
    // template <typename T>
    // void update(string name, T value)
    // {
    // }
};