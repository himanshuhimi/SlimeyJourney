#include "../../objects/pad.h"

Pad::Pad(SDL_Renderer *renderer, float x, float y)
    : Sprite(renderer, "objects/pad.png", x, y) {}