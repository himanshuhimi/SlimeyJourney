#include "../pad.h"

Pad::Pad(SDL_Renderer *renderer, float x, float y, int incPrecent)
    : Sprite(renderer, "pad.png", x, y), incPrecent(incPrecent) {}