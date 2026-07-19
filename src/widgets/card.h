#pragma once

#include "tools/widget.h"

class Card : public Widget
{
public:
    Image bgUnhovered, bgHovered;
    Text displayText;
    string label = "";
    Card(SDL_Renderer *renderer, float x, float y, string label, UIFunction callback);
    void handle(double dt) override;
    void render(Vector2D Camera = {}) override;
    void update(SDL_Event event) override;
    bool hovered();
    bool clicked(SDL_Event event);
};