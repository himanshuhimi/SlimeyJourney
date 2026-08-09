#include "../../systems/level.h"

Level::Level(SDL_Renderer *renderer, string region, int number)
    : renderer(renderer), player(renderer, 0, 0), fren(renderer, 0, 0),
      map(renderer, region + "/" + std::to_string(number) + ".tmx")
{
    loadObjects();
    fruitLength = fruits.size();
    enemyLength = enemies.size();
    if (fruitLength > 0)
        increment = (double)1 / fruitLength;
    audios = {
        {"pickup", Audio("player/pickup.wav")},
        {"hurt", Audio("player/hurt.wav")}};
    quests = {{"fed",
               Quest(renderer, SPRITE_SIZE, SPRITE_SIZE,
                     "Find & Feed Your friend, Fren!")}};
    if (fruitLength > 0)
        quests.insert({"fruit", Quest(renderer, SPRITE_SIZE, 0, "Collect All Fruits")});
    if (enemies.size() > 0)
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

void Level::handle(double dt)
{
    clampCamera();
    for (auto &stone : stones)
        if (player.Position.distanceFromVec(stone.Position) < WIDTH)
            stone.handle(dt, objects);
    for (auto &ball : player.balls)
        if (player.Position.distanceFromVec(ball.Position) < WIDTH)
            ball.handle(dt, objects);
    for (auto &spike : spikes)
        if (player.Position.distanceFromVec(spike.Position) < WIDTH)
            spike.handle(dt, objects);
    for (auto &enemy : enemies)
        if (player.Position.distanceFromVec(enemy->Position) < WIDTH)
            enemy->handle(dt, objects);
    for (auto &pad : pads)
        if (player.Position.distanceFromVec(pad.Position) < WIDTH)
            pad.handle(dt, objects);
    for (auto &[_, quest] : quests)
        quest.handle(dt);
    fren.handle(dt, objects);
    player.handle(dt, objects);
    if (player.rect.y >= map.pixelHeight)
        player.resetPos();
}

void Level::render()
{
    map.render(Camera);
    for (auto &fruit : fruits)
        if (player.Position.distanceFromVec(fruit.Position) < WIDTH)
        {
            print("fruit rendering...");
            fruit.render(Camera);
        }
    for (auto &ball : player.balls)
        if (player.Position.distanceFromVec(ball.Position) < WIDTH )
            ball.render(Camera);
    for (auto &spike : spikes)
        if (player.Position.distanceFromVec(spike.Position) < WIDTH)
            spike.render(Camera);
    for (auto &enemy : enemies)
        if (player.Position.distanceFromVec(enemy->Position) < WIDTH)
            enemy->render(Camera);
    for (auto &stone : stones)
        if (player.Position.distanceFromVec(stone.Position) < WIDTH)
            stone.render(Camera);
    for (auto &pad : pads)
        if (player.Position.distanceFromVec(pad.Position) < WIDTH)
            pad.render(Camera);
    for (auto &[_, quest] : quests)
        quest.render();
    fren.render(Camera);
    player.render(Camera);
}

void Level::reset() { loadObjects(); }

void Level::clampCamera()
{
    float targetX = player.Position.x - CAMERA_X;
    float targetY = player.Position.y - CAMERA_Y;
    float maxX = std::max<float>(0.0f, map.pixelWidth - WIDTH);
    float maxY = std::max<float>(0.0f, map.pixelHeight - HEIGHT);
    Camera.x = std::clamp(targetX, 0.0f, maxX);
    Camera.y = std::clamp(targetY, 0.0f, maxY);
}

void Level::loadObjects()
{
    for (const Map::Object &obj : map.objectGroup.objects)
    {
        const string &name = obj.name;
        if (name == "player")
            player = Player(renderer, obj.x, obj.y - SPRITE_SIZE);
        else if (name == "fren")
            fren = Fren(renderer, obj.x, obj.y - SPRITE_SIZE);
        else if (name == "object")
            objects.emplace_back(obj.x, obj.y, obj.width, obj.height);
        else if (name == "spike")
            spikes.emplace_back(
                renderer,
                obj.x + SPRITE_SIZE / 2, obj.y,
                obj.width, obj.height);
        else if (name == "pad")
            pads.emplace_back(renderer, obj.x, obj.y);
        else if (name == "fruit")
            fruits.emplace_back(renderer, obj.x, obj.y - SPRITE_SIZE / 2);
        else if (name == "enemy")
        {
            vector<EnemyFactory> enems = {
                [&]
                { return make_unique<Slime>(renderer, obj.x, obj.y - SPRITE_SIZE); },
                [&]
                { return make_unique<Bee>(renderer, obj.x, obj.y - SPRITE_SIZE); },
                [&]
                { return make_unique<Snail>(renderer, obj.x, obj.y - SPRITE_SIZE); }};
            enemies.emplace_back(Random.choice(enems)());
        }
        else if (name == "slime")
            enemies.emplace_back(make_unique<Slime>(renderer, obj.x, obj.y - SPRITE_SIZE));
        else if (name == "bee")
            enemies.emplace_back(make_unique<Bee>(renderer, obj.x, obj.y - SPRITE_SIZE));
        else if (name == "snail")
            enemies.emplace_back(make_unique<Snail>(renderer, obj.x, obj.y - SPRITE_SIZE));
        else if (name == "stone")
        {
            if (obj.width > SPRITE_SIZE && obj.height == SPRITE_SIZE)
                for (int x = 0; x < obj.width; x += SPRITE_SIZE)
                    stones.emplace_back(renderer, obj.x + x + SPRITE_SIZE / 2, obj.y);
            else if (obj.height > SPRITE_SIZE && obj.width == SPRITE_SIZE)
                for (int y = 0; y < obj.height; y += SPRITE_SIZE)
                    stones.emplace_back(renderer, obj.x, obj.y + y + SPRITE_SIZE / 2);
        }
    }
}