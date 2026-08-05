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
    if (!atkCooldown.available || dead)
        return;
    throws.emplace_back(make_unique<Spit>(renderer, Center.x, Center.y, Direction));
    atkCooldown.reset();
}