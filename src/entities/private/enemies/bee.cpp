#include "../../enemies/bee.h"

Bee::Bee(SDL_Renderer *renderer, float x, float y)
    : Enemy(renderer, x, y, 
        "bee", 
        EnemyData{
            float(Random.randint(-10, 10)),
            160,
            5,
            [this](Vector2D Direction)
            { attack(Direction); }})
{
    Velocity.x = data.speed;
}

void Bee::attack(Vector2D Direction)
{
    if (!atkCooldown.available || dead)
        return;
    throws.emplace_back(make_unique<Sting>(renderer, Center.x, Center.y, Direction));
    atkCooldown.reset();
}