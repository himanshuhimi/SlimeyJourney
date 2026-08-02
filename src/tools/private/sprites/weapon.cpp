#include "../weapon.h"

Weapon::Weapon(SDL_Renderer *renderer, string imgSource, 
        float x, float y, Vector2D Direction)
    : Sprite(renderer, imgSource, x, y), Direction(Direction)
{
    speed = 180;
    prevPos = Position;
    Velocity.x = Direction.x * speed;
    Velocity.y = Direction.y * speed;
}

void Weapon::handle(double dt, const vector<Object> &objects)
{
    if (used)
        return;
    for (auto &grass : objects)
        if (checkCollision(rect, grass.rect))
            destroy();
    Sprite::handle(dt, objects);
}

void Weapon::render(Vector2D Camera)
{
    if (used)
        return;
    Sprite::render(Camera);
}

void Weapon::destroy()
{
    used = true;
}