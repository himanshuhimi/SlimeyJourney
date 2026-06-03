#include "../level.h"

Level::Level(SDL_Renderer *renderer, int number)
    : renderer(renderer),
      player(renderer, 0, 0),
      map(renderer, "maps/" + std::to_string(number - 1) + ".tmx"),
      healthBar(
          renderer, WIDTH / 24.0f, HEIGHT / 16.0f,
          colors.red, Image(renderer, "assets/images/ui/heart.png"),
          1.0, 150),
      fruitBar(
          renderer, healthBar.Position.x, HEIGHT / 6.0f,
          SDL_Color{95, 90, 204, 255}, Image(renderer, "assets/images/ui/bottle.png"))
{
    loadObjects();
    fruitLength = enemies.size() + fruits.size(); // enemies also drop fruits.
    increment = (double)1 / fruitLength;
}

void Level::handle(double dt)
{
    clampCamera();
    for (auto &ball : player.balls)
        ball.handle(dt, grasses);
    for (auto &enemy : enemies)
        enemy.handle(dt, grasses);
    healthBar.handle(dt);
    fruitBar.handle(dt);
    player.handle(dt, grasses);
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
    healthBar.render();
    fruitBar.render();
    player.render(Camera);
}

void Level::loadObjects()
{
    for (Map::Object obj : map.objectGroup.objects)
    {
        const char *name = obj.name.c_str();
        if (!strcmp(name, "player"))
            player = Player(renderer, obj.x, obj.y - SPRITE_SIZE);
        else if (!strcmp(name, "fruit"))
            fruits.push_back(Fruit(renderer, obj.x, obj.y - SPRITE_SIZE));
        else if (!strcmp(name, "slime"))
            enemies.push_back(Slime(renderer, obj.x, obj.y));
        else if (!strcmp(name, "grasses"))
            for (int x = 0; x < obj.width; x += SPRITE_SIZE)
                grasses.push_back(Grass(renderer, obj.x + x + SPRITE_SIZE / 2, obj.y));
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