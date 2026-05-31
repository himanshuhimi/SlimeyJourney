#include "../level.h"

Level::Level(SDL_Renderer *renderer, int number)
    : renderer(renderer),
      player(renderer, 0, 0),
      map(
          renderer,
          "maps/" + std::to_string(number - 1) + ".tmx"),
      pointsText(renderer, WIDTH / 8, HEIGHT / 8, "0", SDL_Color{143, 128, 55, 255}),
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
    }
    bottleRect = SDL_FRect{
        WIDTH / 8.0f - (SPRITE_SIZE * 2),
        HEIGHT / 8.0f,
        bottleImage.width,
        bottleImage.height};
}

void Level::render()
{
    map.render(Camera);
    for (auto grass : grasses)
        grass.render(Camera);
    for (auto fruit : fruits)
        fruit.render(Camera);
    bottleImage.render(nullptr, &bottleRect);
    pointsText.render();
    player.render(Camera);
}

void Level::handle(double dt)
{
    player.handle(dt, grasses);
    float targetX = player.Position.x - CAMERA_X;
    float targetY = player.Position.y - CAMERA_Y;
    float maxX = std::max<float>(0.0f, map.pixelWidth - WIDTH);
    float maxY = std::max<float>(0.0f, map.pixelHeight - HEIGHT);
    Camera.x = std::clamp(targetX, 0.0f, maxX);
    Camera.y = std::clamp(targetY, 0.0f, maxY);
    pointsText.updateData(std::to_string(points));
}