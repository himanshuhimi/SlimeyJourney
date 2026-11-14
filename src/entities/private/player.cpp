#include "../player.h"

Player::Player(SDL_Renderer *renderer, float x, float y)
    : Sprite(renderer, "player/idle.png", x, y)
{
    anims = {
        {"jump", Anim(renderer, "player/jump.gif", 0.2)},
        {"damage", Anim(renderer, "player/damage.gif", 0.1)}};
    audios = {
        {"jump", Audio("player/jump.wav")},
        {"shoot", Audio("player/shoot.wav")},
        {"pickup", Audio("player/pickup.wav")},
        {"hurt", Audio("player/hurt.wav")}};
}

void Player::handle(double dt, const vector<Object> &objects)
{
    dead = HP <= 0;
    if (dead)
        return;
    if (anims.at("damage").active)
        anims.at("damage").handle(dt);
    if (anims.at("damage").complete)
    {
        movable = true;
        immune = false;
    }
    handleMovement(dt);
    Sprite::handle(dt, objects);
    handleAttacking(dt);
    for (auto &ball : balls)
        ball.handle(dt, objects);
}

void Player::render(Vector2D Camera)
{
    dst = rect;
    if (anims.at("damage").active)
        anims.at("damage").render(Camera, dst);
    else if (anims.at("jump").active)
        anims.at("jump").render(Camera, dst);
    else
        Sprite::render(Camera);
    for (auto &ball : balls)
        ball.render(Camera);
}

void Player::damage(int byPoints)
{
    if (dead || immune)
        return;
    if (movable)
        HP -= byPoints;
    movable = false;
    immune = true;
    anims.at("damage").restart();
    audios.at("hurt").play(Random.randint(50, 80));
}

void Player::attack()
{
    SDL_FRect mouseWin = getMousePosition();
    Vector2D mapMouse = {mouseWin.x + Camera.x, mouseWin.y + Camera.y};
    Vector2D Direction = {mapMouse.x - Center.x, mapMouse.y - Center.y};
    Direction.normalise();
    balls.emplace_back(renderer, Center.x, Center.y, "player", Direction);
    audios.at("shoot").play(Random.randint(50, 80));
}

void Player::resetPos(bool previous)
{
    Position = (previous) ? prevJumpPos : Original;
    rect.x = Position.x;
    rect.y = Position.y;
    damage();
}

void Player::handleMovement(double dt)
{
    if (!movable)
        return;
    const bool *keys = SDL_GetKeyboardState(NULL);
    Velocity.x = -((int)keys[SDL_SCANCODE_A] - (int)keys[SDL_SCANCODE_D]) * speed;
    if (!states.inAir && keys[SDL_SCANCODE_SPACE])
    {
        Velocity.y = -jumpStrength;
        prevJumpPos = Position;
        audios.at("jump").play(Random.randint(50, 80));
        anims.at("jump").restart();
    }
    if (states.inAir || !states.onGround)
    {
        if (!anims.at("jump").active)
            anims.at("jump").restart();
        anims.at("jump").handle(dt);
    }
    else
        anims.at("jump").active = false;
}

void Player::handleAttacking(double dt)
{
    throwCooldown.handle(dt);
    if (mouseClicked && throwCooldown.available)
    {
        attack();
        throwCooldown.reset();
        mouseClicked = false;
    }
}