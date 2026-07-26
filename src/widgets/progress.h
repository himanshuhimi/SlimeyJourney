#pragma once

#include "tools/widget.h"

class Progress : public Widget
{
public:
    SDL_FRect attachmentRect, fillRect;
    Image attachment;
    SDL_Color color;
    Vector2D Position;
    Text text;
    double startPercent = 0.0, animSpeed = 0.0, reachPercent = 0.0, percentage = 0.0;
    bool complete = false;
    Progress(SDL_Renderer *renderer, float x, float y, 
        std::function<void()> callback,  SDL_Color color = SDL_Color{255, 255, 255, 255},
        Image attachment = {nullptr, ""}, double startPercent = 0.0,
        float width = 128, double animSpeed = 1.5);
    void handle(double dt) override;
    void render(Vector2D Camera = {}) override;
    void advance(double increment = 0.2);
    void reset();
};