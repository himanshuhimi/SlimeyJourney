#pragma once

#include "database.h"

class Table
{
public:
    Database *database = nullptr;
    string name = "";
    Table(string name, Database database, string content = "");
};