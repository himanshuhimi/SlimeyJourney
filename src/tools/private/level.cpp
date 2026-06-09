#include "../level.h"

Level::Level(SDL_Renderer *renderer, int number)
    : renderer(renderer), player(renderer, 0, 0), flag(renderer, 0, 0),
      timer(renderer, durations.at(number)),
      map(renderer, "maps/" + std::to_string(number) + ".tmx"),
      fruitBar(renderer, 0, 0, SDL_Color{95, 90, 204, 255},
               Image(renderer, "assets/ui/bottle.png"))
{
    loadObjects();
    fruitLength = enemies.size() + fruits.size();
    increment = (double)1 / fruitLength;
    fruitBar.rect.x = player.healthBar.rect.x;
    fruitBar.rect.y = player.healthBar.rect.y + SPRITE_SIZE + (SPRITE_SIZE / 2);
    audios = {
        {"pickup", Audio("assets/audios/player/pickup.wav")},
        {"hurt", Audio("assets/audios/hurt.wav")}};
}

void Level::handle(double dt)
{
    clampCamera();
    for (auto &ball : player.balls)
        ball.handle(dt, grasses);
    for (auto &enemy : enemies)
        enemy.handle(dt, grasses);
    fruitBar.handle(dt);
    flag.handle(dt);
    player.handle(dt, grasses);
    if (player.rect.y >= map.pixelHeight)
        player.resetPos();
    collision();
    timer.handle(dt);
    if (timer.currentTime <= 0.0)
        player.damage(5);
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
    for (auto ballIt = player.balls.begin(); ballIt != player.balls.end(); ballIt++)
        for (auto eneIt = enemies.begin(); eneIt != enemies.end();)
            if (!ballIt->used && checkCollision(ballIt->rect, eneIt->rect))
            {
                eneIt->damage();
                audios.at("hurt").play();
                ballIt->used = true;
                player.inCombat = true;
                player.combatEnemy = &(*eneIt);
                if (eneIt->dead)
                    eneIt = enemies.erase(eneIt);
            }
            else
                eneIt++;
    for (auto &enemy : enemies)
        if (checkCollision(player.rect, enemy.lineOfSight.rect))
        {
            player.inCombat = true;
            player.combatEnemy = &enemy;
            break;
        }
        else
        {
            player.inCombat = false;
            player.combatEnemy = nullptr;
        }
    for (auto &enemy : enemies)
    {
        for (auto ballIt = enemy.balls.begin(); ballIt != enemy.balls.end(); ballIt++)
        {
            Ball ball = *ballIt;
            bool collided = checkCollision(ball.rect, player.rect);
            if (player.movable && !ball.used && collided)
            {
                player.damage();
                ball.used = true;
                ballIt = enemy.balls.erase(ballIt);
                break;
            }
        }
        enemy.drop<Fruit>(fruits);
    }
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
        if (name == "slime")
            enemies.push_back(Slime(renderer, obj.x, obj.y));
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