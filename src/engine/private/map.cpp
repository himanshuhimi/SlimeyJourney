#include "../map.h"

Map::Map(SDL_Renderer *renderer, string source, int x, int y)
    : renderer(renderer), source(source), x(x), y(y)
{
    mapPath = fs::path("data/") / directory / source;
    string path = mapPath.string();
    if (doc.LoadFile(path.c_str()) != XML_SUCCESS)
        print("Map Uninitialized: " + path);
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
    {
        if (!layer.visible)
            continue;
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
                (float)x + ((i % layer.width) * tileWidth) - Camera.x,
                (float)y + ((i / layer.width) * tileHeight) - Camera.y,
                (float)(tileWidth),
                (float)(tileHeight)};
            tileset.image->render(&src, &dest);
        }
    }
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
    layer.visible = child->BoolAttribute("visible", true);
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
    fs::path tsxPath = mapPath.parent_path() / child->Attribute("source");
    string tsxSource = tsxPath.lexically_normal().string();
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
    tileset.image = new Image(renderer, (string)imageElement->Attribute("source"));
    tileset.columns = tileset.image->width / tileWidth;
    tileset.rows = tileset.image->height / tileHeight;
}

void Map::loadObjectGroup(XMLElement *child)
{
    for (
        XMLElement *objElem = child->FirstChildElement("object");
        objElem != nullptr;
        objElem = objElem->NextSiblingElement("object"))
    {
        Object obj;
        obj.id = objElem->IntAttribute("id");
        obj.name = objElem->Attribute("name");
        obj.x = objElem->FloatAttribute("x");
        obj.y = objElem->FloatAttribute("y");
        obj.width = objElem->FloatAttribute("width");
        obj.height = objElem->FloatAttribute("height");
        XMLElement *props = objElem->FirstChildElement("properties");
        if (props)
            for (XMLElement *propElem = props->FirstChildElement("property");
                 propElem != nullptr;
                 propElem = propElem->NextSiblingElement("property"))
            {
                string name = propElem->Attribute("name");
                string type = propElem->Attribute("type");
                string value = propElem->Attribute("value");
                if (type == "object")
                {
                    auto propObjIt = std::find_if(objectGroup.objects.begin(),
                                                  objectGroup.objects.end(), [value](const auto &pair)
                                                  { return pair.first == std::stoi(value); });
                    if (propObjIt != objectGroup.objects.end())
                        obj.properties.insert({name, propObjIt->second});
                }
                else
                    obj.properties.insert({name, value});
            }
        objectGroup.objects.insert({obj.id, obj});
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