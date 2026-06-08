#pragma once

#include "core/config.h"

using namespace tinyxml2;

class Map
{
public:
    SDL_Renderer *renderer = nullptr;
    tinyxml2::XMLDocument doc;
    XMLElement *mapElement;
    struct Layer
    {
        int width, height;
        string num, csvText;
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
    };
    struct ObjectGroup
    {
        vector<Text> texts;
        vector<Object> objects;
    };
    ObjectGroup objectGroup;
    vector<Layer> layers;
    Tileset tileset;
    string source = "";
    int width, height, tileWidth, tileHeight, pixelWidth, pixelHeight;
    Map(SDL_Renderer *renderer, string source);
    void render(Vector2D Camera);
    void loadChildren();
    void loadLayer(XMLElement *child);
    void loadTileset(XMLElement *child);
    void loadObjectGroup(XMLElement *child);
    void loadGroups(XMLElement *child);
    ~Map();
};