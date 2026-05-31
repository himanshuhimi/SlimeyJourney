#include "../sprite.h"

Sprite::Sprite(
    SDL_Renderer *renderer,
    string imgSource,
    float x,
    float y,
    bool isGravitational)
    : renderer(renderer), isGravitational(isGravitational),
      image(renderer, "assets/images/" + imgSource)
{
    jumpStrength = 150.0f;
    speed = 205;
    SDL_GetTextureSize(image.texture, &rect.w, &rect.h);
    Position = Vector2D{x - rect.w / 2, y};
    rect.x = Position.x;
    rect.y = Position.y;
}

void Sprite::handle(double dt, const vector<Grass> &grasses)
{
    if (isGravitational)
    {
        bool onGround = false;
        const bool *keys = SDL_GetKeyboardState(NULL);
        for (auto grass : grasses)
        {
            bool collided = SDL_HasRectIntersectionFloat(&rect, &grass.rect);
            if (collided)
            {
                onGround = true;
                float bottom = Position.y + rect.h;
                if (Velocity.y > 0 && bottom >= grass.rect.y)
                {
                    Position.y = grass.rect.y - rect.h;
                    Velocity.y = 0;
                }
            }
        }
        state.jumping = !onGround;
        state.onGround = onGround;
        if (!state.jumping && keys[SDL_SCANCODE_SPACE])
            Velocity.y -= jumpStrength;
        if (state.jumping || !state.onGround)
            Velocity.y += constants.gravity * dt;
    }
    state.walking = (bool)Velocity.x;
    Position.x += Velocity.x * dt;
    Position.y += Velocity.y * dt;
    rect.x = Position.x;
    rect.y = Position.y;
}

void Sprite::render(Vector2D Camera)
{
    dst = rect;
    dst.x -= Camera.x;
    dst.y -= Camera.y;
    image.render(nullptr, &dst);
}