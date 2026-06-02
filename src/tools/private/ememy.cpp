#include "../enemy.h"

Enemy::Enemy(SDL_Renderer *renderer, string foldertype, float x, float y, int speed)
    : Sprite(renderer, "enemies/" + foldertype + "/idle.png", x, y),
        foldertype(foldertype)
{
    Velocity.x = speed;
    Position.y -= rect.h;
}

void Enemy::handle(double dt, const vector<Grass> &grasses)
{
    state.walking = (bool)Velocity.x;
    Position.x += Velocity.x * dt;
    Position.y += Velocity.y * dt;
    rect.x = Position.x;
    rect.y = Position.y;
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
}

void Enemy::render(Vector2D Camera)
{
    Sprite::render(Camera);
}