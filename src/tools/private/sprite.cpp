#include "../sprite.h"

Sprite::Sprite(SDL_Renderer *renderer, string imgSource, float x, float y)
    : renderer(renderer), image(renderer, "assets/images/" + imgSource),
      gravityLOS(renderer, 0, 0, 1, 0), lineOfSight(renderer, 0, 0, 0, 1)
{
    Position = Vector2D{x - image.width / 2, y};
    rect.x = Position.x;
    rect.y = Position.y;
    rect.w = image.width;
    rect.h = image.height;
    jumpStrength = 100.0f;
    speed = 180;
    lineOfSight.rect.w = rect.w / 2;
    gravityLOS.rect.h = rect.h / 2;
}

void Sprite::handle(double dt, const vector<Grass> &grasses)
{
    handleMovement(dt);
    handleGravity(dt, grasses);
    handleLOS();
}

void Sprite::render(Vector2D Camera)
{
    Sprite::Camera = Camera;
    dst = rect;
    dst.x -= Camera.x;
    dst.y -= Camera.y;
    image.render(nullptr, &dst);
    // lineOfSight.render(Camera);
}

void Sprite::handleMovement(double dt)
{
    if (!movable)
        return;
    Position.x += Velocity.x * dt;
    Position.y += Velocity.y * dt;
    rect.x = Position.x;
    rect.y = Position.y;
    Center.x = rect.x + rect.w / 2;
    Center.y = rect.y + rect.h / 2;
}

void Sprite::handleLOS()
{
    lineOfSight.matchPosition(rect);
    gravityLOS.matchPosition(rect);
    if (state.prevOnGround && !state.onGround)
        lineOfSight.rect.w *= -1;
}

void Sprite::handleGravity(double dt, const vector<Grass> &grasses)
{
    bool onGround = false;
    bool prevOnGround = state.onGround;
    for (auto &grass : grasses)
        if (checkCollision(gravityLOS.rect, grass.rect))
        {
            onGround = true;
            float bottom = Position.y + rect.h;
            if (Velocity.y > 0 && bottom >= grass.rect.y)
            {
                Position.y = grass.rect.y - rect.h;
                rect.y = Position.y;
                Velocity.y = 0;
            }
        }
    handleStates(onGround, prevOnGround);
}

void Sprite::handleStates(bool onGround, bool prevOnGround)
{
    state.prevOnGround = prevOnGround;
    state.walking = (bool)Velocity.x;
    state.onGround = onGround;
    state.jumping = !onGround;
    if (state.walking)
        lastDirection = (Velocity.x > 0) ? Direction::Right : Direction::Left;
}