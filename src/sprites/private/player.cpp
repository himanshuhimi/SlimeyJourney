#include "../player.h"

Player::Player(SDL_Renderer *renderer, float x, float y)
    : Sprite(renderer, "player/idle.png", x, y)
{
    jumpStrength = 100.0f;
    speed = 180;
    anims = {
        {"jump", Animation(renderer, "player/jump.png")},
        {"damage", Animation(renderer, "player/damage.png", 0.1)},
        {"walking", Animation(renderer, "player/walking.png", 0.1)}};
    audios = {
        {"jump", Audio("audios/player/jump.wav")},
        {"walking", Audio("audios/player/walking.wav")},
        {"shoot", Audio("audios/player/shoot.wav")},
        {"pickup", Audio("audios/player/pickup.wav")},
        {"hurt", Audio("audios/hurt.wav")}};
}

void Player::handle(double dt, const vector<Object> &grasses)
{
    dead = HP <= 0;
    if (dead)
        return;
    if (anims.at("damage").active)
        anims.at("damage").handle(dt);
    else if (anims.at("walking").active)
        anims.at("walking").handle(dt);
    if (anims.at("damage").complete)
    {
        movable = true;
        immune = false;
    }
    handleMovement(dt);
    Sprite::handle(dt, grasses);
    handleShooting(dt);
}

void Player::render(Vector2D Camera)
{
    dst = rect;
    for (auto &[key, anim] : anims)
        anim.dst = dst;
    if (states.inAir)
        anims.at("jump").render(Camera, rect);
    else if (anims.at("walking").active)
        anims.at("walking").render(Camera, rect);
    else
        Sprite::render(Camera);
    if (anims.at("damage").active)
    {
        for (auto &[key, anim]: anims)
            if (key != "damage")
                anim.active = false;
        anims.at("damage").render(Camera, rect);
    }
}

void Player::damage(Progress healthBar, int byPoints)
{
    healthBar.update(-(double)1 / maxHP);
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
    balls.emplace_back(renderer, rect.x, rect.y, "player", Direction);
    audios.at("shoot").play(Random.randint(50, 80));
}

void Player::resetPos(Progress healthBar, bool previous)
{
    Position = (previous) ? prevPos : Original;
    rect.x = Position.x;
    rect.y = Position.y;
    damage(healthBar);
}

void Player::handleMovement(double dt)
{
    if (!movable)
        return;
    const bool *keys = SDL_GetKeyboardState(NULL);
    Velocity.x = -((int)keys[SDL_SCANCODE_A] - (int)keys[SDL_SCANCODE_D]) * speed;
    anims.at("walking").active = states.walking;
    if (!states.inAir && keys[SDL_SCANCODE_SPACE])
    {
        Velocity.y -= jumpStrength;
        prevPos = Position;
        audios.at("jump").play(Random.randint(50, 80));
        anims.at("jump").restart();
    }
    if (states.inAir || !states.onGround)
        anims.at("jump").handle(dt);
    if (anims.at("walking").active)
        anims.at("walking").handle(dt);
}

void Player::handleShooting(double dt)
{
    throwCooldown.handle(dt);
    if (mouseClicked && throwCooldown.available)
    {
        attack();
        mouseClicked = false;
        throwCooldown.timeElapsed = 0;
        throwCooldown.available = false;
    }
}