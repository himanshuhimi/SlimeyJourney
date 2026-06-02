#include "../level.h"

Level::Level(SDL_Renderer *renderer, int number)
    : renderer(renderer),
      player(renderer, 0, 0),
      map(
          renderer,
          "maps/" + std::to_string(number - 1) + ".tmx"),
      pointsText(renderer, WIDTH / 8, HEIGHT / 8, "0", colors.white, 18),
      bottleImage(renderer, "assets/images/ui/bottle.png")
{
    for (Map::Object obj : map.objectGroup.objects)
    {
        const char *name = obj.name.c_str();
        if (!strcmp(name, "player"))
            player = Player(renderer, obj.x, obj.y - SPRITE_SIZE);
        else if (!strcmp(name, "grasses"))
            for (int x = 0; x < obj.width; x += SPRITE_SIZE)
                grasses.push_back(Grass(renderer, obj.x + x + SPRITE_SIZE / 2, obj.y));
        else if (!strcmp(name, "fruit"))
            fruits.push_back(Fruit(renderer, obj.x, obj.y - SPRITE_SIZE));
        else if (!strcmp(name, "slime"))
            enemies.push_back(Slime(renderer, obj.x, obj.y));
    }
    bottleRect = SDL_FRect{
        pointsText.rect.x - (pointsText.rect.w),
        pointsText.rect.y - (pointsText.rect.h / 4),
        bottleImage.width,
        bottleImage.height};
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
    bottleImage.render(nullptr, &bottleRect);
    pointsText.render();
    player.render(Camera);
    for (auto &ball : player.balls)
        ball.render(Camera);
}

void Level::handle(double dt)
{
    float targetX = player.Position.x - CAMERA_X;
    float targetY = player.Position.y - CAMERA_Y;
    float maxX = std::max<float>(0.0f, map.pixelWidth - WIDTH);
    float maxY = std::max<float>(0.0f, map.pixelHeight - HEIGHT);
    Camera.x = std::clamp(targetX, 0.0f, maxX);
    Camera.y = std::clamp(targetY, 0.0f, maxY);
    player.handle(dt, grasses);
    for (auto &ball : player.balls)
        ball.handle(dt, grasses);
    for (auto &enemy : enemies)
        enemy.handle(dt, grasses);
    pointsText.updateData(std::to_string(points));
}