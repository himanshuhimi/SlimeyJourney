#include "../ball.h"

Ball::Ball(SDL_Renderer *renderer, float x, float y, string type, Vector2D Direction)
    : Sprite(renderer, type + "/ball.png", x, y)
{
    speed = 180;
    prevPos = Position;
    Velocity.x = Direction.x * speed;
    Velocity.y = Direction.y * speed;
    anims = {{"explosion", Animation(renderer, "explosion.png")}};
}

void Ball::handle(double dt, const vector<Object> &objects)
{
    if (used)
        return;
    for (auto &grass : objects)
        if (checkCollision(rect, grass.rect))
            destroy();
    Sprite::handle(dt, objects);
}

void Ball::render(Vector2D Camera)
{
    if (used)
        return;
    Sprite::render(Camera);
}

void Ball::destroy()
{
    used = true;
}