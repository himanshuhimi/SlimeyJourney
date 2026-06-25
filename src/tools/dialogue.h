#pragma once

#include "core/config.h"

class Dialogue
{
public:
    SDL_Renderer *renderer = nullptr;
    Image box;
    SDL_FRect rect;
    Text *text;
    Vector2D Position;
    Dialogue(
        SDL_Renderer *renderer,
        float x, float y,
        string initialText = ""
    );
    void updateText(string newData);
    void handle(double dt);
    void render(Vector2D Camera);
};