#include "../enemy.h"

Enemy::Enemy(SDL_Renderer *renderer, string type, float x, float y,
             int speed, int HP, float sightRange)
    : Sprite(renderer, "enemies/" + type + "/idle.png", x, y), maxHP(HP),
      HP(HP), type(type), healthBar(renderer, x, y - rect.h, colors.red,
                                    Image(renderer, "assets/ui/heart.png"), 1.0, 32)
{
    Velocity.x = speed;
    Position.y -= rect.h;
    anims = {
        {"damage", Animation(renderer, type + "/damage.png", 0.1)},
        {"walking", Animation(renderer, type + "/walking.png", 0.1)}};
    lineOfSight = LineOfSight(renderer, x, y, sightRange, 1);
}

void Enemy::handle(double dt, const vector<Object> &grasses)
{
    dead = HP <= 0;
    if (dead)
        return;
    handleMovement(dt);
    handleLOS();
    handleGravity(dt, grasses);
    healthBar.handle(dt);
    throwCooldown.handle(dt);
    for (auto &ball : balls)
        ball.handle(dt, grasses);
    if (anims.at("damage").active)
        anims.at("damage").handle(dt);
    else if (anims.at("walking").active)
        anims.at("walking").handle(dt);
    if (state.prevOnGround && !state.onGround)
        Velocity.x *= -1;
}

void Enemy::render(Vector2D Camera)
{
    if (dead)
        return;
    if (anims.at("walking").active)
        anims.at("walking").render(Camera, rect);
    else
        Sprite::render(Camera);
    if (anims.at("damage").active)
    {
        for (auto &[key, anim]: anims)
            if (key != "damage")
                anim.active = false;
        anims.at("damage").render(Camera, rect);
        healthBar.render(Camera);
    }
    for (auto &ball : balls)
        ball.render(Camera);
}

void Enemy::handleMovement(double dt)
{
    Sprite::handleMovement(dt);
    healthBar.rect.x = Position.x;
    healthBar.rect.y = Position.y - (rect.h / 2);
    anims.at("walking").active = state.walking;
}

void Enemy::damage(int byPoints)
{
    if (dead)
    {
        HP = 0;
        return;
    }
    HP -= byPoints;
    healthBar.update(-(double)1 / maxHP);
    anims.at("damage").restart();
}