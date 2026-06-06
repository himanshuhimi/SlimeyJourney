#include "../player.h"

Player::Player(SDL_Renderer *renderer, float x, float y)
    : Sprite(renderer, "player/idle.png", x, y),
      healthBar(renderer, 5, HEIGHT / 16.0f, colors.red,
                Image(renderer, "assets/ui/heart.png"), 1.0, 150)
{
    jumpStrength = 100.0f;
    speed = 180;
    string animPath = "assets/anims/player/";
    string audioPath = "assets/audios/player/";
    anims = {
        {"jump", Animation(renderer, animPath + "jump.png")},
        {"damage", Animation(renderer, animPath + "damage.png", 0.1)},
        {"walking", Animation(renderer, animPath + "walking.png", 0.1)}};
    audios = {
        {"jump", Audio(audioPath + "jump.wav")},
        {"walking", Audio(audioPath + "walking.wav")},
        {"shoot", Audio(audioPath + "shoot.wav")},
        {"pickup", Audio(audioPath + "pickup.wav")},
        {"hurt", Audio("assets/audios/hurt.wav")}};
}

void Player::handle(double dt, const vector<Object> &grasses)
{
    healthBar.handle(dt);
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
    healthBar.render();
    dst = rect;
    for (auto &[key, anim] : anims)
        anim.dst = dst;
    if (state.jumping)
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

void Player::damage(int byPoints)
{
    healthBar.update(-(double)1 / maxHP);
    if (dead || immune)
        return;
    if (movable)
        HP -= byPoints;
    movable = false;
    immune = true;
    anims.at("damage").restart();
    audios.at("hurt").play();
}

void Player::resetPos(bool previous)
{
    Position = (previous) ? prevPos : Original;
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
    anims.at("walking").active = state.walking;
    if (!state.jumping && keys[SDL_SCANCODE_SPACE])
    {
        Velocity.y -= jumpStrength;
        prevPos = Position;
        audios.at("jump").play();
        anims.at("jump").restart();
    }
    if (state.jumping || !state.onGround)
    {
        Velocity.y += constants.gravity * dt;
        anims.at("jump").handle(dt);
    }
    if (anims.at("walking").active)
        anims.at("walking").handle(dt);
    if (rect.y > HEIGHT)
        resetPos();
}

void Player::handleShooting(double dt)
{
    SDL_FRect mouseWin = getMousePosition();
    Vector2D mapMouse = {mouseWin.x + Camera.x, mouseWin.y + Camera.y};
    Vector2D Direction = {mapMouse.x - Center.x, mapMouse.y - Center.y};
    Direction.normalise();
    throwCooldown.handle(dt);
    if (mouseClicked && throwCooldown.available)
    {
        balls.emplace_back(renderer, rect.x, rect.y, "player", Direction);
        mouseClicked = false;
        throwCooldown.timeElapsed = 0;
        throwCooldown.available = false;
        audios.at("shoot").play();
    }
    if (inCombat && movable && combatEnemy->throwCooldown.available)
    {
        float x = combatEnemy->rect.x;
        float y = combatEnemy->rect.y;
        Vector2D Direction = {Center.x - combatEnemy->rect.x, Center.y - combatEnemy->rect.y};
        Direction.normalise();
        combatEnemy->balls.emplace_back(renderer, x, y, "enemies/slime", Direction);
        combatEnemy->throwCooldown.timeElapsed = 0;
        combatEnemy->throwCooldown.available = false;
    }
}