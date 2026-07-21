#include "../cloud.h"

Cloud::Cloud(SDL_Renderer *renderer)
    : Sprite(
        renderer, 
        "cloud.png",
        Random.randint(SPRITE_SIZE, WIDTH - SPRITE_SIZE),
        Random.randint(SPRITE_SIZE, HEIGHT - SPRITE_SIZE)) 
{
    Velocity.x = Random.randint(0, 10);
}

void Cloud::handle(double dt)
{
    handleMovement(dt);
}