#pragma once

#include "tools/widget.h"

class Progress : public Widget
{
public:
    SDL_FRect attachmentRect, fillRect;
    Image attachment, image;
    SDL_Color color;
    Vector2D Position;
    double startPercent = 0.0, animSpeed = 0.0, reachPercent = 0.0, percentage = 0.0;
    bool complete = false;
    Progress(SDL_Renderer *renderer, float x, float y, 
        std::function<void()> callback,  SDL_Color color = SDL_Color{255, 255, 255, 255},
        Image attachment = {nullptr, ""}, double startPercent = 0.0,
        float width = 100, double animSpeed = 2.0);
    void handle(double dt) override;
    void render(Vector2D Camera = {}) override;
    void advance(double increment = 0.2);
    void reset();
};