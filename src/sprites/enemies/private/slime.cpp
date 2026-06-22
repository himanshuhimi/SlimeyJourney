#include "../slime.h"

Slime::Slime(SDL_Renderer *renderer, float x, float y)
    : Enemy(renderer, x, y, "slime", EnemyData{(float)Random.randint(5, 15), 1.0f / 4, 64, 5, [this](Vector2D Direction)
                                               { attack(Direction); }})
{
    Velocity.x = data.speed;
    anims = {
        {"walking", Animation(renderer, "slime/walking.png")},
        {"damage", Animation(renderer, "slime/damage.png")}};
}

void Slime::handle(double dt, const vector<Object> &grasses)
{
    Enemy::handle(dt, grasses);
    if (states.prevOnGround && !states.onGround)
        Velocity.x *= -1;
    atkCooldown.handle(dt);
    for (auto &ball : balls)
        ball.handle(dt, grasses);
}

void Slime::render(Vector2D Camera)
{
    Enemy::render(Camera);
    for (auto &ball : balls)
        ball.render(Camera);
}

void Slime::attack(Vector2D Direction)
{
    if (!atkCooldown.available || dead)
        return;
    balls.emplace_back(renderer, rect.x, rect.y, "enemies/" + type, Direction);
    atkCooldown.reset();
}

void Slime::damage(int byPoints)
{
    if (dead)
        return;
    HP -= byPoints;
}