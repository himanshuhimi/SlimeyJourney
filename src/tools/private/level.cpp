#include "../level.h"

Level::Level(SDL_Renderer *renderer, int number)
    : renderer(renderer),
      player(renderer, 0, 0),
      map(renderer, "maps/" + std::to_string(number - 1) + ".tmx"),
      heartImage(renderer, "assets/images/ui/heart.png"),
      bottleImage(renderer, "assets/images/ui/bottle.png"),
      heartProg(renderer, 0, 0, SDL_Color{89, 179, 0, 255}, 100.0f, 1.0),
      fruitProg(renderer, 0, 0, SDL_Color{95, 90, 204, 255})
{
    loadObjects();
    fruitLength = enemies.size() + fruits.size(); // enemies also drop fruits.
    increment = (double) 1/fruitLength;
    heartRect = SDL_FRect{
        WIDTH / 24.0f,
        HEIGHT / 16.0f,
        heartImage.width,
        heartImage.height
    };
    bottleRect = SDL_FRect{
        heartRect.x,
        HEIGHT / 6.0f,
        bottleImage.width,
        bottleImage.height
    };
    heartProg.rect.x = heartRect.x + heartImage.width;
    heartProg.rect.y = heartRect.y + (heartImage.height / 2);
    heartProg.fillRect.x = heartProg.rect.x;
    heartProg.fillRect.y = heartProg.rect.y;
    fruitProg.rect.x = bottleRect.x + bottleImage.width;
    fruitProg.rect.y = bottleRect.y + (bottleImage.height / 2);
    fruitProg.fillRect.x = fruitProg.rect.x;
    fruitProg.fillRect.y = fruitProg.rect.y;
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

void Level::handle(double dt)
{
    float targetX = player.Position.x - CAMERA_X;
    float targetY = player.Position.y - CAMERA_Y;
    float maxX = std::max<float>(0.0f, map.pixelWidth - WIDTH);
    float maxY = std::max<float>(0.0f, map.pixelHeight - HEIGHT);
    Camera.x = std::clamp(targetX, 0.0f, maxX);
    Camera.y = std::clamp(targetY, 0.0f, maxY);
    for (auto &ball : player.balls)
        ball.handle(dt, grasses);
    for (auto &enemy : enemies)
        enemy.handle(dt, grasses);
    heartProg.handle(dt);
    fruitProg.handle(dt);
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
    heartImage.render(nullptr, &heartRect);
    bottleImage.render(nullptr, &bottleRect);
    heartProg.render();
    fruitProg.render();
    player.render(Camera);
}