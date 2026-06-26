#include "../sprite.h"

Sprite::Sprite(SDL_Renderer *renderer, string imgSource, float x, float y)
    : renderer(renderer), Original(x, y), image(renderer, "images/" + imgSource),
      gravityLOS(renderer, 0, 0, 1, 0), lineOfSight(renderer, 0, 0, 0, 1)
{
    Position = Vector2D{x - image.width / 2, y - image.height / 2};
    rect.x = Position.x;
    rect.y = Position.y;
    rect.w = image.width;
    rect.h = image.height;
    hitbox.w = image.width;
    hitbox.h = image.height;
    lineOfSight.rect.w = rect.w / 2;
    gravityLOS.rect.h = rect.h / 2;
}

void Sprite::handle(double dt, const vector<Object> &objects)
{
    handleMovement(dt);
    handleGravity(dt, objects);
    handleLOS();
}

void Sprite::render(Vector2D Camera)
{
    Sprite::Camera = Camera;
    dst = rect;
    dst.x -= Camera.x;
    dst.y -= Camera.y;
    image.render(nullptr, &dst);
    if (lineOfSight.visible)
        lineOfSight.render(Camera);
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
    hitbox.x = Center.x;
    hitbox.y = Center.y;
}

void Sprite::handleLOS()
{
    lineOfSight.matchPosition(rect);
    gravityLOS.matchPosition(rect);
    if (states.prevOnGround && !states.onGround)
        lineOfSight.rect.w *= -1;
}

void Sprite::handleGravity(double dt, const vector<Object> &objects)
{
    bool onGround = false;
    bool prevOnGround = states.onGround;
    for (auto &grass : objects)
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
    if (states.inAir || !states.onGround)
        Velocity.y += constants.gravity * dt;
    handleStates(onGround, prevOnGround);
}

void Sprite::handleStates(bool onGround, bool prevOnGround)
{
    states.prevOnGround = prevOnGround;
    states.walking = (bool)Velocity.x;
    states.onGround = onGround;
    states.inAir = !onGround;
    if (states.walking)
        lastDirection = (Velocity.x > 0) ? Direction::Right : Direction::Left;
}