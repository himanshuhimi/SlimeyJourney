#include "../flag.h"

Flag::Flag(SDL_Renderer *renderer, float x, float y)
    : Sprite(renderer, "empty.png", x, y),
      anim(renderer, "assets/anims/flag.png", 0.1)
{
    anim.active = true;
}

void Flag::handle(double dt)
{
    if (anim.complete)
        anim.restart();
    if (anim.active)
        anim.handle(dt);
}

void Flag::render(Vector2D Camera)
{
    if (anim.active)
        anim.render(Camera, rect);
}