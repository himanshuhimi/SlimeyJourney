#include "../enemy.h"

Enemy::Enemy(SDL_Renderer *renderer, string foldertype, float x, float y, int speed)
: 
    Sprite(renderer, "enemies/" + foldertype + "/idle.png", x, y),
    foldertype(foldertype),
    speed(speed)
{
    Velocity.x = speed;
    Position.y -= rect.h;
    LOSRect.w = 1;
    LOSRect.h = 20;
}

void Enemy::handle(double dt, const vector<Grass> &grasses)
{
    Sprite::handle(dt, grasses);
    LOSRect.x = Position.x + rect.w / 2;
    LOSRect.y = Position.y + (rect.h / 2);
    bool onGround = false;
    bool wasOnGround = state.onGround;
    for (auto &grass : grasses)
        if (SDL_HasRectIntersectionFloat(&LOSRect, &grass.rect))
            onGround = true;
    state.onGround = onGround;
    if (wasOnGround && !state.onGround)
        Velocity.x *= -1;
}

void Enemy::render(Vector2D Camera)
{
    Sprite::render(Camera);
    LOSdst = LOSRect;
    LOSdst.x -= Camera.x;
    LOSdst.y -= Camera.y;
    // SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    // SDL_RenderFillRect(renderer, &LOSdst);
}