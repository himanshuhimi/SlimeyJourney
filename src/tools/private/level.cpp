#include "../level.h"

Level::Level(SDL_Renderer *renderer, int number)
    : renderer(renderer), player(renderer, 0, 0), flag(renderer, 0, 0),
      timer(renderer, durations.at(number)),
      map(renderer, std::to_string(number) + ".tmx"),
      fruitBar(renderer, 0, 0, SDL_Color{95, 90, 204, 255},
               Image(renderer, "ui/bottle.png")),
      healthBar(renderer, 5, HEIGHT / 16.0f, colors.red,
                Image(renderer, "ui/heart.png"), 1.0, 150)
{
    loadObjects();
    fruitLength = enemies.size() + fruits.size();
    increment = (double)1 / fruitLength;
    fruitBar.rect.x = healthBar.rect.x;
    fruitBar.rect.y = healthBar.rect.y + SPRITE_SIZE + (SPRITE_SIZE / 2);
    audios = {
        {"pickup", Audio("audios/player/pickup.wav")},
        {"hurt", Audio("audios/hurt.wav")}};
}

void Level::handle(double dt)
{
    clampCamera();
    for (auto &ball : player.balls)
        ball.handle(dt, grasses);
    for (auto &enemy : enemies)
        enemy.handle(dt, grasses);
    fruitBar.handle(dt);
    healthBar.handle(dt);
    flag.handle(dt);
    player.handle(dt, grasses);
    if (player.rect.y >= map.pixelHeight)
        player.resetPos(healthBar);
    collision();
    timer.handle(dt);
    if (timer.currentTime <= 0.0)
        player.damage(healthBar);
}

void Level::render()
{
    map.render(Camera);
    for (auto &grass : grasses)
        grass.render(Camera);
    for (auto &fruit : fruits)
        fruit.render(Camera);
    for (auto &enemy : enemies)
        enemy.render(Camera);
    for (auto &ball : player.balls)
        ball.render(Camera);
    fruitBar.render();
    healthBar.render();
    flag.render(Camera);
    player.render(Camera);
    timer.render();
}

void Level::collision()
{
    for (auto fruitIt = fruits.begin(); fruitIt != fruits.end();)
        if (!fruitIt->picked && checkCollision(fruitIt->rect, player.rect))
        {
            fruitIt->picked = true;
            points += 1;
            player.audios.at("pickup").play();
            fruitBar.update(increment);
        }
        else
            fruitIt++;
}

void Level::loadObjects()
{
    for (Map::Object obj : map.objectGroup.objects)
    {
        string name = obj.name;
        if (name == "player")
            player = Player(renderer, obj.x, obj.y - SPRITE_SIZE);
        if (name == "flag")
            flag = Flag(renderer, obj.x, obj.y - SPRITE_SIZE);
        if (name == "fruit")
            fruits.push_back(Fruit(renderer, obj.x, obj.y - SPRITE_SIZE));
        // if (name == "slime")
        //     enemies.push_back(Slime(renderer, obj.x, obj.y));
        // if (name == "snail")
        //     enemies.push_back(Snail(renderer, obj.x, obj.y));
        if (name == "object")
            for (int x = 0; x < obj.width; x += SPRITE_SIZE)
                grasses.push_back(Object(renderer, obj.x + x + SPRITE_SIZE / 2, obj.y));
    }
}

void Level::clampCamera()
{
    float targetX = player.Position.x - CAMERA_X;
    float targetY = player.Position.y - CAMERA_Y;
    float maxX = std::max<float>(0.0f, map.pixelWidth - WIDTH);
    float maxY = std::max<float>(0.0f, map.pixelHeight - HEIGHT);
    Camera.x = std::clamp(targetX, 0.0f, maxX);
    Camera.y = std::clamp(targetY, 0.0f, maxY);
}