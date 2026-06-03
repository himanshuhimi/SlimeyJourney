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

void Player::handle(
    double dt, 
    const vector<Grass> &grasses
)
{
    const bool *keys = SDL_GetKeyboardState(NULL);
    Velocity.x = -((int)keys[SDL_SCANCODE_A] - (int)keys[SDL_SCANCODE_D]) * speed;
    handleJump(dt);
    Sprite::handle(dt, grasses);
    handleShooting(dt);
}

void Player::render(Vector2D Camera)
{
    dst = rect;
    for (auto &[key, anim] : anims)
        anim->dst = dst;
    if (state.jumping)
        anims["jump"]->render(Camera);
    else
        Sprite::render(Camera);
}

void Player::handleJump(double dt)
{
    const bool *keys = SDL_GetKeyboardState(NULL);
    if (!state.jumping && keys[SDL_SCANCODE_SPACE])
    {
        prevPos = Position;
        Velocity.y -= jumpStrength;
        audios["jump"]->play();
        anims["jump"]->index = 0;
    }
    if (state.jumping || !state.onGround)
        Velocity.y += constants.gravity * dt;
}

void Player::handleShooting(double dt)
{
    SDL_FRect mouseWin = GetMousePosition();
    Vector2D mapMouse = {
        mouseWin.x + Camera.x,
        mouseWin.y + Camera.y
    };
    Vector2D Direction = {mapMouse.x - Center.x, mapMouse.y - Center.y};
    Direction.normalise();
    throwCooldown.handle(dt);
    if (mouseClicked && throwCooldown.available)
    {
        balls.emplace_back(Ball(
            renderer,
            Center.x,
            Center.y,
            Direction
        ));
        throwCooldown.available = false;
        throwCooldown.timeElapsed = 0;
        mouseClicked = false;
    }
}