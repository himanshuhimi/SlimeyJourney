#include "../../systems/world.h"

World::World(SDL_Renderer *renderer, Player player, string region, string seed)
    : renderer(renderer), player(player), checkpoint(renderer, 0, 0), region(region), seed(seed)
{
    map.world = this;
    loadMaps();
    fruitLength = map.fruits.size();
    enemyLength = map.enemies.size();
    if (fruitLength > 0)
        increment = (double)1 / fruitLength;
    audios = {
        {"pickup", Audio("player/pickup.wav")},
        {"hurt", Audio("player/hurt.wav")}};
    quests = {{"checkpoint",
               Quest(renderer, SPRITE_SIZE, SPRITE_SIZE,
                     "Reach the checkpoint!")}};
    if (fruitLength > 0)
        quests.insert({"fruit", Quest(renderer, SPRITE_SIZE, 0, "Collect All Fruits")});
    if (enemyLength > 0)
        quests.insert({"enemies", Quest(renderer, SPRITE_SIZE, 0, "Kill All Enemies")});
    int i = 0;
    for (auto [name, quest] : quests)
    {
        i++;
        float y = SPRITE_SIZE * i;
        quest.text->rect.y = y;
        quest.text->attachRect.y = y;
    }
}

void World::handle(double dt)
{
    clampCamera();
    for (auto &[_, quest] : quests)
        quest.handle(dt);
    map.handle(dt);
    checkpoint.handle(dt, map.objects);
    player.handle(dt, map.objects);
    if (player.rect.y >= map.pixelHeight)
        player.resetPos();
}

void World::render()
{
    map.render(Camera);
    for (auto &[_, quest] : quests)
        quest.render();
    checkpoint.render(Camera);
    player.render(Camera);
}

void World::reset() { loadMaps(); }

void World::clampCamera()
{
    float targetX = player.Position.x - CAMERA_X;
    float targetY = player.Position.y - CAMERA_Y;
    float maxX = std::max<float>(0.0f, map.pixelWidth - WIDTH);
    float maxY = std::max<float>(0.0f, map.pixelHeight - HEIGHT);
    Camera.x = std::clamp(targetX, 0.0f, maxX);
    Camera.y = std::clamp(targetY, 0.0f, maxY);
}

void World::loadObjects(Map *newMap, int offset)
{
    for (const Map::Object &obj : newMap->objectGroup.objects)
    {
        int x = obj.x + newMap->x, y = obj.y;
        std::cout << newMap->x << std::endl;
        // int prevX = maps.empty() ? 0 : maps.back()->objectGroup.objects.back().x;
        const string &name = obj.name;
        std::cout << newMap->source << ": " << obj.name << ": " << "(" << x << "," << y << ")" << std::endl;
        if (name == "checkpoint")
            checkpoint = Checkpoint(renderer, x, y - SPRITE_SIZE);
        else if (name == "object")
            map.objects.emplace_back(x, y, obj.width, obj.height);
        else if (name == "spike")
            map.spikes.insert(make_unique<Spike>(
                renderer,
                x + SPRITE_SIZE / 2, y,
                obj.width, obj.height));
        else if (name == "pad")
            map.pads.insert(make_unique<Pad>(renderer, x, y));
        else if (name == "fruit")
            map.fruits.insert(make_unique<Fruit>(renderer, x, y - SPRITE_SIZE / 2));
        else if (name == "enemy")
        {
            vector<EnemyFactory> enems = {
                [&]
                { return make_unique<Slime>(renderer, x, y - SPRITE_SIZE); },
                [&]
                { return make_unique<Bee>(renderer, x, y - SPRITE_SIZE); },
                [&]
                { return make_unique<Snail>(renderer, x, y - SPRITE_SIZE); }};
            map.enemies.insert(Random.choice(enems)());
        }
        else if (name == "slime")
            map.enemies.insert(make_unique<Slime>(renderer, x, y - SPRITE_SIZE));
        else if (name == "bee")
            map.enemies.insert(make_unique<Bee>(renderer, x, y - SPRITE_SIZE));
        else if (name == "snail")
            map.enemies.insert(make_unique<Snail>(renderer, x, y - SPRITE_SIZE));
        else if (name == "stone")
        {
            if (obj.width > SPRITE_SIZE && obj.height == SPRITE_SIZE)
                for (int x = 0; x < obj.width; x += SPRITE_SIZE)
                    map.stones.insert(make_unique<Stone>(
                        renderer, 
                        x + SPRITE_SIZE / 2, 
                        y));
            else if (obj.height > SPRITE_SIZE && obj.width == SPRITE_SIZE)
                for (int y = 0; y < obj.height; y += SPRITE_SIZE)
                    map.stones.insert(make_unique<Stone>(
                        renderer, 
                        x, 
                        y + SPRITE_SIZE / 2));
        }
    }
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
        loadObjects(newMap, map.pixelWidth);
        maps.emplace_back(newMap);
    }
}

void World::MapGroup::handle(double dt)
{
    stones.handle(world->player, dt, objects);
    spikes.handle(world->player, dt, objects);
    pads.handle(world->player, dt, objects);
    enemies.handle(world->player, dt, objects);
}

void World::MapGroup::render(Vector2D Camera)
{
    for (auto map : world->maps)
        map->render(Camera);
    fruits.render(world->player, Camera);
    spikes.render(world->player, Camera);
    stones.render(world->player, Camera);
    enemies.render(world->player, Camera);
    pads.render(world->player, Camera);
}

void World::MapGroup::clear()
{
    objects.clear();
    spikes.clear();
    stones.clear();
    pads.clear();
    fruits.clear();
    enemies.clear();
}