#pragma once

#include "tools/widget.h"

class Carousel : public Widget
{
public:
    Text text;
    Image rightArrow, leftArrow, rightHovered, rightUnhovered, leftHovered, leftUnhovered;
    SDL_FRect rightRect, leftRect;
    vector<string> data = {};
    int index = 0, maxIdx = 0;
    string maxElem = "";
    Carousel(SDL_Renderer *renderer, float x, float y,
        UIFunction callback, vector<string> &data, string defaultVal = "");
    void render(Vector2D Camera = {}) override;
    void handle(double dt) override;
    void update(SDL_Event event) override;
    bool hovered(SDL_FRect rect);
    bool clicked(SDL_FRect rect, SDL_Event event);
};