#pragma once

#include "tools/widget.h"

class Carousel : public Widget
{
public:
    Text text;
    Image rightArrow, leftArrow;
    SDL_FRect rightRect, leftRect;
    vector<string> data = {};
    int index = 0, maxIdx = 0;
    Carousel(SDL_Renderer *renderer, float x, float y,
        UIFunction callback, vector<string> &data);
    void render(Vector2D Camera = {}) override;
    void handle(double dt) override;
    void update(SDL_Event event) override;
    bool hovered(SDL_FRect rect);
    bool clicked(SDL_FRect rect, SDL_Event event);
};