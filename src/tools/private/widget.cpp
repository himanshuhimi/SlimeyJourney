#include "../widget.h"

Widget::Widget(SDL_Renderer *renderer, float x, float y,
               std::function<void()> callback, string type)
    : renderer(renderer), onCallback(callback), type(type)
{
    rect = SDL_FRect{x, y, 0, 0};
}