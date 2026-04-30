#include "../player.h"

Player::Player(SDL_Renderer *renderer, float x, float y)
    : Sprite(renderer, "assets/images/jagannathji.png", x, y) 
{
    jumpStrength = 490.0f;
    speed = 205;
}

void Player::handle(double dt, const vector<Grass *> &grasses)
{
    const bool *keys = SDL_GetKeyboardState(NULL);
    Velocity.x = -((int)keys[SDL_SCANCODE_A] - (int)keys[SDL_SCANCODE_D]);
    Velocity.x *= speed;
    colGrasses.clear();
    for (auto grass : grasses)
        if (SDL_HasRectIntersectionFloat(&rect, &grass->rect))
            colGrasses.push_back(grass);
    state.jumping = colGrasses.empty();
    for (auto collided : colGrasses)
        if (collided->rect.y > Position.y && Velocity.y > 0)
        {
            Position.y = collided->rect.y - rect.h;
            Velocity.y = 0;
            state.jumping = false;
        }
    if (!state.jumping && keys[SDL_SCANCODE_SPACE])
    {
        Velocity.y -= jumpStrength;
        state.jumping = true;
    }
    if (state.jumping)
        Velocity.y += constants::gravity * dt;
    state.walking = (bool)Velocity.x;
    state.jumping = (bool)Velocity.y;
    Sprite::handle(dt);
}