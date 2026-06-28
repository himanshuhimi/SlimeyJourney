#pragma once

#include "tools/widget.h"

class Button : public Widget
{
public:
    SDL_Color color;
    Image image;
    Text text;
    vector<Image> images = {};
    string label;
    bool resized = false;
    Button(SDL_Renderer *renderer, float x, float y, std::function<void()> callback, 
        string label, SDL_Color color);
    void handle(double dt);
    void render(Vector2D Camera = {0.0f, 0.0f});
    bool hovered();
    bool clicked(SDL_Event event);
    void update(SDL_Event event);
};