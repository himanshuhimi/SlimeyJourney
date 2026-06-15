#include "../slime.h"

Slime::Slime(SDL_Renderer *renderer, float x, float y)
    : Enemy(renderer, x, y, "slime", EnemyData{
        (float)Random.randint(5, 15), 1 / 4, 64, 5, 
        [this](Vector2D Direction){ attack(Direction); }
    })
{
    Velocity.x = data.speed;
}

void Slime::handle(double dt, const vector<Object> &grasses)
{
    Enemy::handle(dt, grasses);
    if (state.prevOnGround && !state.onGround)
        Velocity.x *= -1;
}

void Slime::attack(Vector2D Direction)
{
    balls.emplace_back(renderer, Center.x, Center.y, "enemies/" + type, Direction);
}