#include "../table.h"

Table::Table(string name, Database database, string content = "") 
    : name(name), database(&database) 
{
    database.execute("CREATE TABLE IF NOT EXISTS " + name + "(" + content + ")");
}