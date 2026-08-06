#include "../../passives/cloud.h"

Cloud::Cloud(SDL_Renderer *renderer)
    : Sprite(
        renderer, "passives/cloud.png",
        Random.randint(SPRITE_SIZE, WIDTH - SPRITE_SIZE),
        Random.randint(SPRITE_SIZE, HEIGHT - SPRITE_SIZE),
        Random.randint(0, 180))
{
    Velocity.x = Random.randint(1, 10);
    Velocity.x *= Random.randint(0, 1) ? -1 : 1;
}

void Cloud::handle(double dt)
{
    Position.x += Velocity.x * dt;
    rect.x = Position.x;
    if (rect.x > WIDTH || rect.x < 0)
        renderable = false;
}

void Cloud::render(Vector2D Camera)
{
    if (renderable)
        Sprite::render(Camera);
}