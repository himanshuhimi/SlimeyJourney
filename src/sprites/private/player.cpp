#include "../player.h"

Player::Player(SDL_Renderer *renderer, float x, float y)
    : Sprite(renderer, "player/idle.png", x, y, true)
{
    string animBeginPath = "assets/images/player/anims/";
    string audioBeginPath = "assets/audios/player/";
    anims = {
        {"jump", new Animation(renderer, rect, animBeginPath + "jump.png")}};
    audios = {
        {"jump", new Audio(audioBeginPath + "jump.wav")},
        {"walking", new Audio(audioBeginPath + "walking.wav")}};
}

void Player::handle(double dt, const vector<Grass> &grasses)
{
    const bool *keys = SDL_GetKeyboardState(NULL);
    Velocity.x = -((int)keys[SDL_SCANCODE_A] - (int)keys[SDL_SCANCODE_D]);
    Velocity.x *= speed;
    Sprite::handle(dt, grasses);
    if (!state.jumping && keys[SDL_SCANCODE_SPACE])
        Velocity.y -= jumpStrength;
    if (state.jumping || !state.onGround)
        Velocity.y += constants.gravity * dt;
    throwCooldown.handle(dt);
    if (keys[SDL_SCANCODE_K] && throwCooldown.available)
    {
        balls.emplace_back(Ball(renderer, rect.x + rect.w / 2, rect.y + rect.h / 2));
        throwCooldown.available = false;
        throwCooldown.timeElapsed = 0;
    }
    if (!state.jumping && keys[SDL_SCANCODE_SPACE])
    {
        audios["jump"]->play();
        anims["jump"]->index = 0;
    }
    if (state.jumping)
    {
        anims["jump"]->handle(dt);
    }
    if (state.walking)
    {
        audios["walking"]->play();
        lastDirection = (Velocity.x > 0) ? Direction::Right : Direction::Left;
    }
}

void Player::render(Vector2D Camera)
{
    dst = rect;
    for (auto &[key, anim] : anims)
        anim->dst = dst;
    if (state.jumping)
        anims["jump"]->render(Camera);
    // else if (state.walking)
    //     anims["walking"]->render(Camera);
    else
        Sprite::render(Camera);
}