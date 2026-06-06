#include "../ball.h"

Ball::Ball(SDL_Renderer *renderer, float x, float y, string type, Vector2D Direction)
    : Sprite(renderer, type + "/ball.png", x, y)
{
    speed = 180;
    prevPos = Position;
    Velocity.x = Direction.x * speed;
    Velocity.y = Direction.y * speed;
    anims = {{"explosion", Animation(renderer, "assets/anims/explosion.png")}};
}

void Ball::handle(double dt, const vector<Object> &grasses)
{
    if (used)
        return;
    for (auto &grass : grasses)
        if (checkCollision(rect, grass.rect))
            used = true;
    if (anims.at("explosion").active)
        anims.at("explosion").handle(dt);
    Sprite::handle(dt, grasses);
}

void Ball::render(Vector2D Camera)
{
    if (used && !anims.at("explosion").complete)
        return;
    if (anims.at("explosion").active)
        anims.at("explosion").render(Camera, rect);
    else
        Sprite::render(Camera);
}