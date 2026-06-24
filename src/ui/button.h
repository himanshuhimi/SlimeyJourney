#pragma once

#include "core/config.h"

class Button
{
public:
    SDL_Renderer *renderer = nullptr;
    SDL_FRect rect;
    SDL_Color color;
    Image image;
    Text text;
    vector<Image> images = {};
    std::function<void()> callback;
    string label;
    bool resized = false;
    Button(
        SDL_Renderer *renderer, float x, float y,
        std::function<void()> callback, string label,
        SDL_Color color);
    void handle(double dt);
    void render();
    bool hovered();
    bool clicked(SDL_Event event);
    void update(SDL_Event event);
};