#include "../../systems/region.h"

Region::Region(SDL_Renderer *renderer, string name)
    : renderer(renderer), name(name),
      display(renderer, WIDTH / 2, 60, name, colors.white, 48) {};

void Region::handle(double dt) {};

void Region::render(Vector2D Camera) 
{
    display.render(Camera);
}