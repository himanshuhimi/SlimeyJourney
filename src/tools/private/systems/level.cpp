#include "../../systems/level.h"

Level::Level(SDL_Renderer *renderer, string region, int number)
    : renderer(renderer), player(renderer, 0, 0),
      timer(renderer, WIDTH / 2, 20, durations.at(number)), fren(renderer, 0, 0),
      map(renderer, region + "/" + std::to_string(number) + ".tmx")
{
    loadObjects();
    fruitLength = fruits.size();
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
        stone.handle(dt, objects);
    for (auto &ball : player.balls)
        ball.handle(dt, objects);
    for (auto &spike : spikes)
        spike.handle(dt, objects);
    for (auto &slime : enemies)
        slime->handle(dt, objects);
    for (auto &[_, quest] : quests)
        quest.handle(dt);
    player.handle(dt, objects);
    fren.handle(dt, objects);
    if (player.rect.y >= map.pixelHeight)
        player.resetPos();
    timer.handle(dt);
}

void Level::render()
{
    map.render(Camera);
    for (auto &stone : stones)
        stone.render(Camera);
    for (auto &fruit : fruits)
        fruit->render(Camera);
    for (auto &ball : player.balls)
        ball.render(Camera);
    for (auto &spike : spikes)
        spike.render(Camera);
    for (auto &slime : enemies)
        slime->render(Camera);
    for (auto &[_, quest] : quests)
        quest.render();
    timer.render();
    player.render(Camera);
    fren.render(Camera);
}

void Level::loadObjects()
{
    for (Map::Object obj : map.objectGroup.objects)
    {
        string name = obj.name;
        if (name == "player")
            player = Player(renderer, obj.x, obj.y - SPRITE_SIZE);
        else if (name == "fren")
            fren = Fren(renderer, obj.x, obj.y - SPRITE_SIZE);
        else if (name == "fruit")
            fruits.emplace_back(make_unique<Fruit>(renderer, obj.x, obj.y - SPRITE_SIZE));
        else if (name == "enemy" || name == "slime")
            enemies.emplace_back(make_unique<Slime>(renderer, obj.x, obj.y - SPRITE_SIZE));
        else if (name == "object")
            objects.emplace_back(obj.x, obj.y, obj.width, obj.height);
        else if (name == "stone")
            for (int y = 0; y < obj.height; y += SPRITE_SIZE)
                stones.emplace_back(renderer, obj.x, obj.y + y + SPRITE_SIZE / 2);
        else if (name == "spike")
            for (int x = 0; x < obj.width; x += SPRITE_SIZE)
                spikes.emplace_back(renderer, obj.x + x + SPRITE_SIZE / 2, obj.y);
    }
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