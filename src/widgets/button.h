#pragma once

#include "tools/widget.h"

class Button : public Widget
{
public:
    SDL_FRect shadow;
    SDL_Color color;
    Text text;
    string label;
    float scaleX{1}, scaleY{1};
    float maxScale = 1.1;
    bool resized = false;
    Button(SDL_Renderer *renderer, float x, float y, std::function<void()> callback, 
        string label, SDL_Color color);
    void handle(double dt) override;
    void render(Vector2D Camera = {}) override;
    void update(SDL_Event event) override;
    bool hovered();
    bool clicked(SDL_Event event);
};