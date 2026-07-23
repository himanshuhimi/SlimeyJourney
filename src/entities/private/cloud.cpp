#include "../cloud.h"

Cloud::Cloud(SDL_Renderer *renderer)
    : Sprite(
        renderer, 
        "cloud.png",
        Random.randint(SPRITE_SIZE, WIDTH - SPRITE_SIZE),
        Random.randint(SPRITE_SIZE, HEIGHT - SPRITE_SIZE)) 
{
    Velocity.x = Random.randint(1, 10);
    Velocity.x *= Random.choice<int>({-1, 1});
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