#include "../player.h"

Player::Player(SDL_Renderer *renderer, float x, float y)
    : Sprite(renderer, "player/idle.png", x, y)
{
    jumpStrength = 200.0f;
    speed = 205;
    string beginPath = "assets/images/player/anims/";
    anims = {
        {"jump", new Animation(renderer, rect, beginPath + "jump.png")}};
}

void Player::handle(double dt, const vector<Grass> &grasses)
{
    const bool *keys = SDL_GetKeyboardState(NULL);
    Velocity.x = -((int)keys[SDL_SCANCODE_A] - (int)keys[SDL_SCANCODE_D]);
    Velocity.x *= speed;
    for (auto grass : grasses)
    {
        bool collided = SDL_HasRectIntersectionFloat(&rect, &grass.rect);
        if (collided && grass.rect.y > Position.y && Velocity.y > 0)
        {
            Position.y = grass.rect.y - rect.h;
            Velocity.y = 0;
            state.jumping = false;
        }
    }
    if (!state.jumping && keys[SDL_SCANCODE_SPACE])
    {
        Velocity.y -= jumpStrength;
        state.jumping = true;
        anims["jump"]->index = 0;
    }
    if (state.jumping)
    {
        Velocity.y += constants::gravity * dt;
        anims["jump"]->handle(dt);
    }
    // state.walking = (bool)Velocity.x;
    // if (state.walking)
    //     lastDirection = (Velocity.x > 0) ? Direction::Right : Direction::Left;
    Sprite::handle(dt);
}

void Player::render(Vector2D Camera)
{
    dst = rect;
    for (auto &[key, anim] : anims)
        anim->dst = dst;
    if (state.jumping)
        anims["jump"]->render(Camera);
    else if (state.walking)
        anims["walking"]->render(Camera);
    else
        Sprite::render(Camera);
}