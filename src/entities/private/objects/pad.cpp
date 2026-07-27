#include "../../objects/pad.h"

Pad::Pad(SDL_Renderer *renderer, float x, float y)
    : Sprite(renderer, "pad.png", x, y) {}