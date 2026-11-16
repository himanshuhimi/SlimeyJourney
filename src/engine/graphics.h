#pragma once

#include <iostream>
#include <vector>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "vector.h"

using std::string, std::vector;

struct Image
{
    SDL_Renderer *renderer = nullptr;
    SDL_Surface *surface = nullptr;
    SDL_Texture *texture = nullptr;
    float width = 0, height = 0;
    double angle = 0;
    string source = "";
    Image(SDL_Renderer *renderer, string source, double angle = 0);
    void render(const SDL_FRect *srcrect, const SDL_FRect *dstrect);
};

struct Anim
{
    SDL_Renderer *renderer = nullptr;
    vector<SDL_Texture *> textures = {};
    IMG_Animation *animation = nullptr;
    string source = "";
    float x, y;
    double delay, timeElapsed;
    bool active = false, complete = false;
    int i = 0;
    Anim(SDL_Renderer *renderer, string source, double delay = 1.0);
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
    Image *img;
    int id;
    bool visible = true;
    Object(SDL_Renderer *renderer,
        float x, float y,
        int objID,
        float width = 32, float height = 32,
        string imgSrc = "images/empty.png");
    void render(Vector2D Camera);
};