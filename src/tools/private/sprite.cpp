#include "../sprite.h"

Sprite::Sprite(SDL_Renderer *renderer, string imgSource, float x, float y, double imgAngle)
    : renderer(renderer), Original(x, y), image(renderer, "images/" + imgSource), imgAngle(imgAngle)
{
    Position = Vector2D{x - image.width / 2, y - image.height / 2};
    rect.x = Position.x;
    rect.y = Position.y;
    rect.w = image.width;
    rect.h = image.height;
    hitbox.w = image.width;
    hitbox.h = image.height;
}

void Sprite::handle(double dt, const vector<Object> &objects)
{
    handleGravity(dt);
    handleMovement(dt, objects);
}

void Sprite::render(Vector2D Camera)
{
    Sprite::Camera = Camera;
    dst = rect;
    dst.x -= Camera.x;
    dst.y -= Camera.y;
    image.render(nullptr, &dst);
}

void Sprite::moveX(double dt, const vector<Object> &objects) 
{
    Position.x += Velocity.x * static_cast<float>(dt);
    rect.x = Position.x;
    for (const auto &obj : objects)
    {
        if (!checkCollision(rect, obj.rect) || !obj.visible)
            continue;
        if (Velocity.x > 0 && Position.x + rect.w <= obj.rect.x)
            Position.x = (obj.rect.x - obj.rect.w) - rect.w;
        else if (Velocity.x < 0 && Position.x >= obj.rect.x + obj.rect.w)
            Position.x = obj.rect.x + obj.rect.w;
        rect.x = Position.x;
        Velocity.x = 0;
    }
    states.walking = Original.x != Position.x && movable;
    if (states.walking)
        lastDirection = Velocity.x > 0 ? Direction::Right : Direction::Left;
};

void Sprite::moveY(double dt, const vector<Object> &objects) 
{
    states.prevOnGround = states.onGround;
    states.onGround = false;
    Position.y += Velocity.y * static_cast<float>(dt);
    rect.y = Position.y;
    for (const auto &obj : objects)
    {
        if (!checkCollision(rect, obj.rect) || !obj.visible)
            continue;
        if (Velocity.y < 0)
            Position.y = obj.rect.y + obj.rect.h;
        else if (Velocity.y > 0)
        {
            Position.y = obj.rect.y - rect.h;
            states.onGround = true;
        }
        rect.y = Position.y;
        Velocity.y = 0;
    }
    states.inAir = !states.onGround;
};

void Sprite::handleMovement(double dt, const vector<Object> &objects)
{
    if (!movable)
        return;
    moveX(dt, objects);
    moveY(dt, objects);
    handlePosition();
}

void Sprite::handlePosition()
{
    rect.x = Position.x;
    rect.y = Position.y;
    Center.x = rect.x + rect.w / 2;
    Center.y = rect.y + rect.h / 2;
    hitbox.x = Center.x;
    hitbox.y = Center.y;
}

void Sprite::handleGravity(double dt)
{
    if (states.inAir || !states.onGround)
        Velocity.y += constants.gravity * dt;
}