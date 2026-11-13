#pragma once

#include "core/config.h"

using namespace tinyxml2;

struct Map
{

    SDL_Renderer *renderer = nullptr;
    tinyxml2::XMLDocument doc;
    XMLElement *mapElement;
    fs::path mapPath;
    struct Layer
    {
        int width, height;
        string num, csvText;
        bool visible;
        vector<int> gids;
        XMLElement *dataElement;
    };
    struct Tileset
    {
        int firstGID, columns, rows;
        Image *image;
    };
    struct Object
    {
        string name;
        float x, y, width, height;
        map<string, std::variant<string, bool, int, float>> properties;
    };
    struct ObjectGroup
    {
        vector<Object> objects;
    };
    ObjectGroup objectGroup;
    vector<Layer> layers;
    Tileset tileset;
    string directory = "maps/", source = "";
    int width, height, tileWidth, tileHeight, pixelWidth, pixelHeight, x, y;
    Map(SDL_Renderer *renderer, string source, int x = 0, int y = 0);
    void render(Vector2D Camera);
    void loadChildren();
    void loadLayer(XMLElement *child);
    void loadTileset(XMLElement *child);
    void loadObjectGroup(XMLElement *child);
    void loadGroups(XMLElement *child);
    ~Map();
};