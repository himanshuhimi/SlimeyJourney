#include "../ball.h"

Ball::Ball(SDL_Renderer *renderer, float x, float y, Vector2D Direction)
    : Sprite(renderer, "ball.png", x, y) 
{ 
    prevPos = Position;
    Velocity.x = Direction.x * speed;
    Velocity.y = Direction.y * speed;
}

void Ball::handle(double dt, const vector<Grass> &grasses)
{
    for (auto &grass : grasses)
        if (checkCollision(rect, grass.rect))
            used = true;
    Sprite::handle(dt, grasses);
}

void Ball::render(Vector2D Camera)
{
    if (used)
        return;
    Sprite::render(Camera);
}