#include "../sprite.h"

Sprite::Sprite(
    SDL_Renderer *renderer,
    string imgSource,
    float x,
    float y,
    bool isGravitational)
    : renderer(renderer), isGravitational(isGravitational),
      image(renderer, "assets/images/" + imgSource),
      gravityLOS(renderer, 0, 0, 1, 0),
      lineOfSight(renderer, 0, 0, 0, 1)
{
    jumpStrength = 200.0f;
    speed = 180;
    SDL_GetTextureSize(image.texture, &rect.w, &rect.h);
    Position = Vector2D{x - rect.w / 2, y};
    rect.x = Position.x;
    rect.y = Position.y;
    lineOfSight.rect.w = rect.w / 2;
    gravityLOS.rect.h = rect.h / 2;
}

void Sprite::handle(double dt, const vector<Grass> &grasses)
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
    {
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
    }
    state.jumping = !onGround;
    state.onGround = onGround;
    if (state.walking)
        lastDirection = (Velocity.x > 0) ? Direction::Right : Direction::Left;
    if (prevOnGround && !state.onGround)
        lineOfSight.rect.w *= -1;
}

void Sprite::render(Vector2D Camera)
{
    dst = rect;
    dst.x -= Camera.x;
    dst.y -= Camera.y;
    image.render(nullptr, &dst);
    lineOfSight.render(Camera);
}