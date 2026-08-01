#include "../../enemies/snail.h"

Snail::Snail(SDL_Renderer *renderer, float x, float y)
    : Enemy(renderer, x, y,
        "snail",
        EnemyData{
            (float)Random.randint(-5, 5),
            160,
            5,
            [this](Vector2D Direction){ attack(Direction); }})
{
    Velocity.x = data.speed;
}

void Snail::attack(Vector2D Direction)
{
    
}