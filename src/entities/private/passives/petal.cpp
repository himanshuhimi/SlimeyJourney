#include "../../passives/petal.h"

Petal::Petal(SDL_Renderer *renderer)
    : Sprite(renderer, "petals/leaf.png", 
        Random.randint(0, WIDTH), 
        Random.randint(0, HEIGHT))
{
    Velocity.x = Random.randint(1, 10);
    Velocity.x *= Random.randint(0, 1) ? -1 : 1;
    Velocity.y = Random.randint(1, 10);
    Velocity.y *= Random.randint(0, 1) ? -1 : 1;
}

void Petal::handle(double dt)
{
    Position.x += Velocity.x * dt;
    rect.x = Position.x;
    Position.y += Velocity.y * dt;
    rect.y = Position.y;
}