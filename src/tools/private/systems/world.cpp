#include "../../systems/world.h"

World::World(SDL_Renderer *renderer, Player *player, string region, string seed)
    : renderer(renderer), player(player),
      checkpoint(renderer, WIDTH - SPRITE_SIZE, HEIGHT - SPRITE_SIZE),
      region(region), seed(seed), map(player)
{
    map.world = this;
    loadMaps();
    quests = {{"checkpoint", Quest(renderer, SPRITE_SIZE, SPRITE_SIZE, "Reach the checkpoint!")}};
    fruitLength = map.fruits.size();
    if (fruitLength > 0)
    {
        increment = (double)1 / fruitLength;
        quests.insert({"fruit", Quest(renderer, SPRITE_SIZE, 0, "Collect All Fruits")});
    }
    if (map.enemies.size() > 0)
        quests.insert({"enemies", Quest(renderer, SPRITE_SIZE, 0, "Kill All Enemies")});
    int i = 0;
    for (auto [name, quest] : quests)
    {
        i++;
        float y = SPRITE_SIZE * i;
        quest.text->rect.y = y;
        quest.text->attachRect.y = y;
    }
    audios = {
        {"pickup", Audio("player/pickup.wav")},
        {"hurt", Audio("player/hurt.wav")}};
}

void World::handle(double dt)
{
    clampCamera();
    for (auto &[_, quest] : quests)
        quest.handle(dt);
    map.handle(dt);
    checkpoint.handle(dt, map.objects);
    if (player->rect.y >= map.pixelHeight)
        player->resetPos();
}

void World::render()
{
    map.render(Camera);
    for (auto &[_, quest] : quests)
        quest.render();
    checkpoint.render(Camera);
}

void World::reset() { loadMaps(); }

void World::clampCamera()
{
    float targetX = player->Position.x - CAMERA_X;
    float targetY = player->Position.y - CAMERA_Y;
    float maxX = std::max<float>(0.0f, map.pixelWidth - WIDTH);
    float maxY = std::max<float>(0.0f, map.pixelHeight - HEIGHT);
    Camera.x = std::clamp(targetX, 0.0f, maxX);
    Camera.y = std::clamp(targetY, 0.0f, maxY);
}

void World::loadMaps()
{
    for (Map *m : maps)
        delete m;
    maps.clear();
    map.width = 0;
    map.height = 0;
    map.pixelWidth = 0;
    map.pixelHeight = 0;
    for (char numChar : seed)
    {
        int x = 0;
        x = (!maps.empty()) ? map.pixelWidth : 0;
        Map *newMap = new Map(renderer, region + "/" + numChar + ".tmx", x);
        map.width += newMap->width;
        map.height = std::max(map.height, newMap->height);
        map.pixelWidth += newMap->pixelWidth;
        map.pixelHeight = std::max(map.pixelHeight, newMap->pixelHeight);
        map.loadObjects(newMap);
        maps.emplace_back(newMap);
    }
}

World::MapGroup::MapGroup(Player *player)
    : spikes(player), pads(player), fruits(player), enemies(player), plates(player)
{
}

void World::MapGroup::handle(double dt)
{
    spikes.handle(dt, objects);
    pads.handle(dt, objects);
    enemies.handle(dt, objects);
    plates.handle(dt, objects);
}

void World::MapGroup::render(Vector2D Camera)
{
    for (auto map : world->maps)
        map->render(Camera);
    for (auto &obj : objects)
        obj.render(Camera);
    fruits.render(Camera);
    spikes.render(Camera);
    enemies.render(Camera);
    pads.render(Camera);
    plates.render(Camera);
}

void World::MapGroup::clear()
{
    objects.clear();
    spikes.clear();
    pads.clear();
    fruits.clear();
    enemies.clear();
    plates.clear();
}

void World::MapGroup::loadObjects(Map *newMap)
{
    for (const auto &[objId, obj] : newMap->objectGroup.objects)
    {
        int x = obj.x + newMap->x, y = obj.y;
        vector<EnemyFactory> enems = {
            [&]
            { return make_unique<Slime>(world->renderer, x, y - SPRITE_SIZE); },
            [&]
            { return make_unique<Bee>(world->renderer, x, y - SPRITE_SIZE); },
            [&]
            { return make_unique<Snail>(world->renderer, x, y - SPRITE_SIZE); }};
        MapGroup::SpriteTypes spriteType = hashString(obj.name);
        switch (spriteType)
        {
        case SpriteTypes::object:
            objects.emplace_back(world->renderer, x, y, obj.id, obj.width, obj.height);
            break;
        case SpriteTypes::spike:
            spikes.insert(make_unique<Spike>(
                world->renderer,
                x + SPRITE_SIZE / 2, y,
                obj.width, obj.height));
            break;
        case SpriteTypes::pad:
            pads.insert(make_unique<Pad>(world->renderer, x, y));
            break;
        case SpriteTypes::fruit:
            fruits.insert(make_unique<Fruit>(world->renderer, x, y - SPRITE_SIZE / 2));
            break;
        case SpriteTypes::enemy:
            enemies.insert(Random.choice(enems)());
            break;
        case SpriteTypes::slime:
            enemies.insert(make_unique<Slime>(world->renderer, x, y - SPRITE_SIZE));
            break;
        case SpriteTypes::bee:
            enemies.insert(make_unique<Bee>(world->renderer, x, y - SPRITE_SIZE));
            break;
        case SpriteTypes::snail:
            enemies.insert(make_unique<Snail>(world->renderer, x, y - SPRITE_SIZE));
            break;
        case SpriteTypes::plate:
        {
            auto it = obj.properties.find("path");
            if (it == obj.properties.end())
                break;
            auto *mapObj = std::get_if<Map::Object>(&it->second);
            if (!mapObj)
                break;
            plates.insert({make_unique<Plate>(world->renderer,x, y, mapObj->id)});
            break;
        }
        default:
            break;
        }
    }
}

World::MapGroup::SpriteTypes World::MapGroup::hashString(string str)
{
    if (str == "object")
        return SpriteTypes::object;
    if (str == "spike")
        return SpriteTypes::spike;
    if (str == "pad")
        return SpriteTypes::pad;
    if (str == "fruit")
        return SpriteTypes::fruit;
    if (str == "enemy")
        return SpriteTypes::enemy;
    if (str == "slime")
        return SpriteTypes::slime;
    if (str == "bee")
        return SpriteTypes::bee;
    if (str == "snail")
        return SpriteTypes::snail;
    if (str == "plate")
        return SpriteTypes::plate;
    return SpriteTypes::none;
}