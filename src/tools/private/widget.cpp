#include "../widget.h"

Widget::Widget(SDL_Renderer *renderer, float x, float y,
               UIFunction callback, string type)
    : renderer(renderer), onCallback(std::move(callback)), type(type)
{
    rect = SDL_FRect{x, y, 0, 0};
}