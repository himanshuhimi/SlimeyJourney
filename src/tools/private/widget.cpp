#include "../widget.h"

Widget::Widget(SDL_Renderer *renderer, float x, float y,
               UIFunction callback)
    : renderer(renderer), onCallback(std::move(callback))
{
    rect = SDL_FRect{x, y, 0, 0};
}