#include "../../objects/plate.h"

Plate::Plate(SDL_Renderer *renderer, float x, float y, int objID)
    : Sprite(renderer, "objects/plates/0.png", x, y), objID(objID)
{
}