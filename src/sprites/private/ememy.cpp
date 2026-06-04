#include "../enemy.h"

Enemy::Enemy(
    SDL_Renderer *renderer, string type,
    float x, float y,
    int speed, int HP)
    : Sprite(renderer, "enemies/" + type + "/idle.png", x, y), maxHP(HP),
      HP(HP), type(type), healthBar(renderer, x, y - rect.h, colors.red,
                                    Image(renderer, "assets/images/ui/heart.png"), 1.0, 32)
{
    Velocity.x = speed;
    Position.y -= rect.h;
    folderPath = "assets/anims/" + type + "/";
    anims = {{"damage", Animation(renderer, folderPath + "damage.png", 0.1)}};
    attackRange = SDL_FRect{Center.x, Center.y, 60, 60};
}

void Enemy::handle(double dt, const vector<Grass> &grasses)
{
    dead = HP <= 0;
    if (dead)
    {
        attackRange.w = 0;
        attackRange.h = 0;
        return;
    }
    handleMovement(dt);
    handleLOS();
    handleGravity(dt, grasses);
    healthBar.handle(dt);
    throwCooldown.handle(dt);
    for (auto &ball : balls)
        ball.handle(dt, grasses);
    if (anims.at("damage").active)
        anims.at("damage").handle(dt);
}

void Enemy::render(Vector2D Camera)
{
    if (dead)
        return;
    for (auto &ball : balls)
        ball.render(Camera);
    // SDL_RenderRect(renderer, &attackRange);
    if (anims.at("damage").active)
    {
        anims.at("damage").render(Camera, rect);
        healthBar.render(Camera);
    }
    else
        Sprite::render(Camera);
}

void Enemy::handleMovement(double dt)
{
    Sprite::handleMovement(dt);
    healthBar.rect.x = Position.x;
    healthBar.rect.y = Position.y - (rect.h / 2);
    attackRange.x = dst.x - (attackRange.w / 2);
    attackRange.y = dst.y - (attackRange.h / 2);
    if (state.prevOnGround && !state.onGround)
    {
        Velocity.x *= -1;
        lineOfSight.rect.w *= -1;
    }
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