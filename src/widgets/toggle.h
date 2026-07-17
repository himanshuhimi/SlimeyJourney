#pragma once

#include "tools/widget.h"

class Toggle : public Widget
{
public:
    Image checkedImg, uncheckedImg;
    Image image;
    bool value = false;
    Toggle(SDL_Renderer *renderer, float x, float y);
    void render(Vector2D Camera = {}) override;
    void handle(double dt) override;
    void update(SDL_Event event) override;
    bool hovered();
    bool clicked(SDL_Event event);
};