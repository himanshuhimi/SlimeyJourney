#include "../../enemies/slime.h"

Slime::Slime(SDL_Renderer *renderer, float x, float y)
    : Enemy(renderer, x, y,
            "slime",
            EnemyData{
                (float)Random.randint(5, 15),
                128,
                5,
                [this](Vector2D Direction)
                { attack(Direction); }})
{
    Velocity.x = data.speed;
    anims = {
        {"walking", Anim(renderer, "slime/walking.gif")},
        {"damage", Anim(renderer, "damage.gif")}};
}

void Slime::handle(double dt, const vector<Object> &objects)
{
    Enemy::handle(dt, objects);
    if (states.prevOnGround && !states.onGround)
        Velocity.x *= -1;
    for (auto &ball : balls)
        ball.handle(dt, objects);
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
    throws.emplace_back(make_unique<Ball>(
        renderer, 
        Center.x, 
        Center.y, 
        type, 
        Direction));
    atkCooldown.reset();
}