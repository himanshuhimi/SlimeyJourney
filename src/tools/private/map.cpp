#include "../map.h"

Map::Map(SDL_Renderer *renderer, string source)
    : renderer(renderer), source(source)
{
    if (doc.LoadFile(source.c_str()) != XML_SUCCESS)
        print("Map Uninitialized: " + source);
    mapElement = doc.FirstChildElement("map");
    width = mapElement->IntAttribute("width");
    height = mapElement->IntAttribute("height");
    tileWidth = mapElement->IntAttribute("tilewidth");
    tileHeight = mapElement->IntAttribute("tileheight");
    pixelWidth = width * tileWidth;
    pixelHeight = height * tileHeight;
    loadChildren();
}

void Map::render(Vector2D Camera)
{
    for (Layer &layer : layers)
        for (int i = 0; i < layer.gids.size(); i++)
        {
            int gid = layer.gids[i];
            if (gid == 0)
                continue;
            int index = gid - tileset.firstGID;
            SDL_FRect src = {
                (float)((index % tileset.columns) * tileWidth),
                (float)((index / tileset.columns) * tileHeight),
                (float)tileWidth,
                (float)tileHeight};
            SDL_FRect dest = {
                (float)((i % layer.width) * tileWidth) - Camera.x,
                (float)((i / layer.width) * tileHeight) - Camera.y,
                (float)(tileWidth),
                (float)(tileHeight)};
            tileset.image->render(&src, &dest);
        }
    for (auto &text : objectGroup.texts)
        text.render();
}

void Map::loadChildren()
{
    for (XMLElement *child = mapElement->FirstChildElement();
         child != nullptr;
         child = child->NextSiblingElement())
    {
        string name = (string)child->Name();
        if (name == "layer")
            loadLayer(child);
        else if (name == "tileset")
            loadTileset(child);
        else if (name == "objectgroup")
            loadObjectGroup(child);
        else if (name == "group")
            loadGroups(child);
    }
}

void Map::loadLayer(XMLElement *child)
{
    Layer layer;
    layer.width = child->IntAttribute("width");
    layer.height = child->IntAttribute("height");
    layer.dataElement = child->FirstChildElement("data");
    if (!layer.dataElement)
        print("No <data> element found in: " + source);
    layer.csvText = (string)layer.dataElement->GetText();
    for (char c : layer.csvText)
        if (isdigit(c))
            layer.num += c;
        else if (c == ',' || c == '\n')
            if (!layer.num.empty())
            {
                layer.gids.push_back(std::stoi(layer.num));
                layer.num.clear();
            }
    if (!layer.num.empty())
        layer.gids.push_back(std::stoi(layer.num));
    layers.push_back(layer);
}

void Map::loadTileset(XMLElement *child)
{
    tileset.firstGID = child->IntAttribute("firstgid");
    string tsxSource = "maps/" + (string)child->Attribute("source");
    if (tsxSource.empty())
    {
        print("TSX Source Uninitialized");
        return;
    }
    tinyxml2::XMLDocument tsxDoc;
    tsxDoc.LoadFile(tsxSource.c_str());
    XMLElement *tsxRoot = tsxDoc.FirstChildElement("tileset");
    if (!tsxRoot)
    {
        print("TSX Uninitialized: " + (string)tsxSource);
        return;
    }
    XMLElement *imageElement = tsxRoot->FirstChildElement("image");
    if (!imageElement)
    {
        print("imageElement Uninitialized");
        return;
    }
    tileset.image = new Image(
        renderer,
        "maps/tilesets/" + (string)imageElement->Attribute("source"));
    tileset.columns = tileset.image->width / tileWidth;
    tileset.rows = tileset.image->height / tileHeight;
}

void Map::loadObjectGroup(XMLElement *child)
{
    for (
        XMLElement *objectElement = child->FirstChildElement("object");
        objectElement != nullptr;
        objectElement = objectElement->NextSiblingElement("object"))
    {
        Object object;
        object.name = objectElement->Attribute("name");
        object.x = objectElement->FloatAttribute("x");
        object.y = objectElement->FloatAttribute("y");
        object.width = objectElement->FloatAttribute("width");
        object.height = objectElement->FloatAttribute("height");
        objectGroup.objects.push_back(object);
    }
}

void Map::loadGroups(XMLElement *child)
{
    for (XMLElement *layerElement = child->FirstChildElement("layer");
         layerElement != nullptr;
         layerElement = layerElement->NextSiblingElement())
        loadLayer(layerElement);
}

Map::~Map()
{
    if (tileset.image)
    {
        delete tileset.image;
        tileset.image = nullptr;
    }
}