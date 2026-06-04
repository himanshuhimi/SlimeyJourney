#include "../enemy.h"

Enemy::Enemy(
    SDL_Renderer *renderer, string type,
    float x, float y,
    int speed, int HP)
    : Sprite(renderer, "enemies/" + type + "/idle.png", x, y), maxHP(HP),
      HP(HP), type(type),
      healthBar(renderer, x, y - rect.h, colors.red,
                Image(renderer, "assets/images/ui/heart.png"), 1.0, 32)
{
    Velocity.x = speed;
    Position.y -= rect.h;
    folderPath = "assets/images/enemies/" + type + "/";
    anims = {{"damage", new Animation(renderer, folderPath + "anims/damage.png", 0.1)}};
}

void Enemy::handle(double dt, const vector<Grass> &grasses)
{
    dead = HP <= 0;
    if (dead)
        return;
    handleMovement(dt);
    handleLOS();
    handleGravity(dt, grasses);
    if (state.prevOnGround && !state.onGround)
    {
        Velocity.x *= -1;
        lineOfSight.rect.w *= -1;
    }
    healthBar.handle(dt);
    healthBar.rect.x = Position.x;
    healthBar.rect.y = Position.y - (rect.h / 2);
    if (anims["damage"]->active)
        anims["damage"]->handle(dt);
}

void Enemy::render(Vector2D Camera)
{
    if (dead)
        return;
    healthBar.render(Camera);
    if (anims["damage"]->active)
        anims["damage"]->render(Camera, rect);
    else
        Sprite::render(Camera);
}

void Enemy::damage(int byPoints)
{
    if (dead)
    {
        HP = 0;
        return;
    }
    HP -= byPoints;
    healthBar.update(-(double) 1 / maxHP);
    anims["damage"]->restart();
}

void Enemy::handleShooting(double dt)
{ 

}