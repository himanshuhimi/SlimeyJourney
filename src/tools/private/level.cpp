#include "../level.h"

Level::Level(SDL_Renderer *renderer, int number)
    : renderer(renderer), player(renderer, 0, 0), flag(renderer, 0, 0),
      timer(renderer, durations.at(number)), fren(renderer, 0, 0),
      map(renderer, std::to_string(number) + ".tmx")
{
    loadObjects();
    fruitLength = fruits.size();
    increment = (double)1 / fruitLength;
    audios = {
        {"pickup", Audio("audios/player/pickup.wav")},
        {"hurt", Audio("audios/hurt.wav")}};
    quests = {
        {"fruitColl", Quest(renderer, 32, 32, "Collect all fruits")},
        {"fedFren", Quest(renderer, 32, 64, "Feed your friend, Fren! [Press F]")}
    };
}

void Level::handle(double dt)
{
    clampCamera();
    for (auto &ball : player.balls)
        ball.handle(dt, grasses);
    for (auto &slime : slimes)
        slime.handle(dt, grasses);
    for (auto &[_, quest] : quests)
        quest.handle(dt);
    flag.handle(dt);
    player.handle(dt, grasses);
    fren.handle(dt, grasses);
    if (player.rect.y >= map.pixelHeight)
        player.resetPos();
    timer.handle(dt);
    if (timer.currentTime <= 0.0)
        player.damage();
}

void Level::render()
{
    map.render(Camera);
    for (auto &grass : grasses)
        grass.render(Camera);
    for (auto &fruit : fruits)
        fruit.render(Camera);
    for (auto &ball : player.balls)
        ball.render(Camera);
    for (auto &slime : slimes)
        slime.render(Camera);
    for (auto &[_, quest] : quests)
        quest.render();
    timer.render();
    flag.render(Camera);
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
        else if (name == "flag")
            flag = Flag(renderer, obj.x, obj.y - SPRITE_SIZE);
        else if (name == "fren")
            fren = Fren(renderer, obj.x, obj.y - SPRITE_SIZE);
        else if (name == "fruit")
            fruits.push_back(Fruit(renderer, obj.x, obj.y - SPRITE_SIZE));
        else if (name == "slime")
            slimes.push_back(Slime(renderer, obj.x, obj.y - SPRITE_SIZE));
        else if (name == "object")
            for (int x = 0; x < obj.width; x += SPRITE_SIZE)
                grasses.push_back(Object(renderer, obj.x + x + SPRITE_SIZE / 2, obj.y));
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