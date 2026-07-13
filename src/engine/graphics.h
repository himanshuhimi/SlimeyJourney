#pragma once

#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "vector.h"

using std::string;

struct Image
{
    SDL_Renderer *renderer = nullptr;
    SDL_Surface *surface = nullptr;
    SDL_Texture *texture = nullptr;
    float width = 0, height = 0;
    string source = "";
    Image(SDL_Renderer *renderer, string source);
    void render(const SDL_FRect *srcrect, const SDL_FRect *dstrect);
};

struct Animation
{
    SDL_Renderer *renderer = nullptr;
    SDL_FRect src, renderDst, dst;
    Image imageSet;
    int maxFrames = 0.0, index = 0;
    bool active = false, complete = false;
    float timer = 0.0f, frameTime = 0.5f;
    Animation(SDL_Renderer *renderer, string source, float frameTime = 0.5f);
    void handle(double dt);
    void render(Vector2D Camera, SDL_FRect dst);
    void restart();
};

struct Text
{
    SDL_Renderer *renderer = nullptr;
    SDL_Surface *surface = nullptr;
    SDL_Texture *texture = nullptr;
    TTF_Font *font = nullptr;
    SDL_FRect rect, dst, attachRect;
    SDL_Color color;
    Image attachment;
    string data, fontSource;
    float x, y;
    int pixelSize, alpha;
    enum PositionModes
    {
        CENTERED,
        TOPLEFT
    } posMode;
    Text(SDL_Renderer *renderer, float x, float y,
         string data, SDL_Color color, int pixelSize = 24, int posMode = 0,
         Image attachment = {nullptr, ""},
         string fontSource = "fonts/sans.ttf");
    void render(Vector2D Camera = {});
    void updateData(string newData);
    void updateAlpha(int newAlpha);
    void updateAttach(Image newAttach);
};

struct Object
{
    SDL_Renderer *renderer = nullptr;
    SDL_FRect rect;
    Vector2D Position;
    Image image;
    bool rendering = false;
    Object(SDL_Renderer *renderer, float x, float y);
    void render(Vector2D Camera);
};