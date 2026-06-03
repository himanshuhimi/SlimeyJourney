#include "../enemy.h"

Enemy::Enemy(
        SDL_Renderer *renderer, 
        string foldertype, 
        float x, 
        float y, 
        int speed,
        int healthPoints
    )
    : Sprite(renderer, "enemies/" + foldertype + "/idle.png", x, y),
        maxHealthPoints(healthPoints),
        healthPoints(healthPoints),
        foldertype(foldertype),
        healthBar(renderer, 0, 0, colors.red, 32, 1.0)
{
    Velocity.x = speed;
    Position.y -= rect.h;
    healthPoints = maxHealthPoints;
}

void Enemy::handle(double dt, const vector<Grass> &grasses)
{
    hasDied = healthPoints <= 0;
    if (hasDied)
    {
        healthPoints = 0;
        return;
    }
    state.walking = (bool)Velocity.x;
    Position.x += Velocity.x * dt;
    Position.y += Velocity.y * dt;
    rect.x = Position.x;
    rect.y = Position.y;
    healthBar.rect.x = Position.x;
    healthBar.rect.y = Position.y - (rect.w / 2);
    lineOfSight.matchPosition(rect);
    gravityLOS.matchPosition(rect);
    bool onGround = false;
    bool prevOnGround = state.onGround;
    for (auto &grass : grasses)
        if (checkCollision(gravityLOS.rect, grass.rect))
            onGround = true;
    state.onGround = onGround;
    if (prevOnGround && !state.onGround)
    {
        Velocity.x *= -1;
        lineOfSight.rect.w *= -1;
    }
    if (state.walking)
        lastDirection = (Velocity.x > 0) ? Direction::Right : Direction::Left;
    healthBar.handle(dt);
}

void Enemy::render(Vector2D Camera)
{
    if (hasDied)
        return;
    Sprite::render(Camera);
    healthBar.render(Camera);
}

void Enemy::damage(int byPoints)
{
    if (hasDied)
        return;
    healthPoints -= byPoints;
}