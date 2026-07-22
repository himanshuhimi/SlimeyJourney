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
    states.prevOnGround = states.onGround;
    states.onGround = false;
    handleGravity(dt);
    handleMovement(dt, objects);
    states.inAir = !states.onGround;
    states.walking = Velocity.x != 0;
    if (states.walking)
        lastDirection = Velocity.x > 0 ? Direction::Right : Direction::Left;
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

void Sprite::moveX(double dt, const vector<Object> &objects) 
{
    float oldX = Position.x;
    Position.x += Velocity.x * dt;
    rect.x = Position.x;
    for (const auto &obj : objects)
    {
        if (!checkCollision(rect, obj.rect))
            continue;
        if (Velocity.x > 0 && oldX + rect.w <= obj.rect.x)
            Position.x = obj.rect.x - rect.w;
        else if (Velocity.x < 0 && oldX >= obj.rect.x + obj.rect.w)
            Position.x = obj.rect.x + obj.rect.w;
        rect.x = Position.x;
        Velocity.x = 0;
        break;
    }
};

void Sprite::moveY(double dt, const vector<Object> &objects) 
{
    Position.y += Velocity.y * dt;
    rect.y = Position.y;
    for (const auto &obj : objects)
    {
        if (!checkCollision(rect, obj.rect))
            continue;
        if (Velocity.y > 0)
        {
            Position.y = obj.rect.y - rect.h;
            states.onGround = true;
        }
        else if (Velocity.y < 0)
            Position.y = obj.rect.y + obj.rect.h;
        rect.y = Position.y;
        Velocity.y = 0;
        break;
    }
};

void Sprite::handleMovement(double dt, const vector<Object> &objects)
{
    if (!movable)
        return;
    moveX(dt, objects);
    moveY(dt, objects);
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

void Sprite::handleGravity(double dt)
{
    if (states.inAir || !states.onGround)
        Velocity.y += constants.gravity * dt;
}