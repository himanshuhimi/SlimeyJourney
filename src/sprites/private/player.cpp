#include "../player.h"

Player::Player(SDL_Renderer *renderer, float x, float y)
    : Sprite(renderer, "player/idle.png", x, y),
      healthBar(renderer, WIDTH / 24.0f, HEIGHT / 16.0f,
          colors.red, Image(renderer, "assets/images/ui/heart.png"),
          1.0, 150)
{
    string animBeginPath = "assets/images/player/anims/";
    string audioBeginPath = "assets/audios/player/";
    anims = {
        {"jump", new Animation(renderer, animBeginPath + "jump.png")},
        {"damage", new Animation(renderer, animBeginPath + "damage.png", 0.2)}};
    audios = {
        {"jump", new Audio(audioBeginPath + "jump.wav")},
        {"walking", new Audio(audioBeginPath + "walking.wav")},
        {"shoot", new Audio(audioBeginPath + "shoot.wav")}};
}

void Player::handle(double dt, const vector<Grass> &grasses)
{
    dead = HP <= 0;
    if (!movable || dead)
        return;
    const bool *keys = SDL_GetKeyboardState(NULL);
    Velocity.x = -((int)keys[SDL_SCANCODE_A] - (int)keys[SDL_SCANCODE_D]) * speed;
    handleJump(dt, keys);
    if (anims["damage"]->active)
    {
        movable = true;
        immune = false;
        anims["damage"]->handle(dt);
    }
    Sprite::handle(dt, grasses);
    handleShooting(dt);
    healthBar.handle(dt);
}

void Player::render(Vector2D Camera)
{
    dst = rect;
    for (auto &[key, anim] : anims)
        anim->dst = dst;
    if (state.jumping)
        anims["jump"]->render(Camera, rect);
    else if (anims["damage"]->active)
        anims["damage"]->render(Camera, rect);
    else
        Sprite::render(Camera);
    healthBar.render();
}

void Player::damage(int byPoints)
{
    if (dead || immune)
        return;
    if (movable && !immune)
        HP -= byPoints;
    movable = false;
    immune = true;
    anims["damage"]->restart();
    healthBar.update(-(double)1 / HP);
}

void Player::resetPos(bool previous)
{
    Position = (previous) ? prevPos : original;
    damage();
}

void Player::handleJump(double dt, const bool *keys)
{
    if (!state.jumping && keys[SDL_SCANCODE_SPACE])
    {
        Velocity.y -= jumpStrength;
        prevPos = Position;
        audios["jump"]->play();
        anims["jump"]->restart();
    }
    if (state.jumping || !state.onGround)
    {
        Velocity.y += constants.gravity * dt;
        anims["jump"]->handle(dt);
    }
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
        balls.emplace_back(Ball(renderer, Center.x, Center.y, "player", Direction));
        mouseClicked = false;
        throwCooldown.timeElapsed = 0;
        throwCooldown.available = false;
        audios["shoot"]->play();
    }
}