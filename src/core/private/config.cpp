#include "../config.h"

const string TITLE = "Puddle Pebble";
const int SPRITE_SIZE = 32;
int WIDTH = 640, HEIGHT = 360;
void log(string message)
{
    std::cout << "[LOG] " << message << std::endl;
}

void log(int number)
{
    std::cout << "[LOG] " << number << std::endl;
}

Image::Image(SDL_Renderer *renderer, string source) : renderer(renderer)
{
    texture = IMG_LoadTexture(renderer, source.c_str());
    if (!texture)
        log("Unloaded Texture: " + source);
}

void Image::render(const SDL_FRect *src, const SDL_FRect *dst)
{
    SDL_RenderTexture(renderer, texture, src, dst);
}

Text::Text(
    SDL_Renderer *renderer,
    float x,
    float y,
    string data,
    string fontSource,
    SDL_Color color,
    int pixelSize
)
: renderer(renderer), x(x), y(y),
    pixelSize(pixelSize), color(color)
{
    font = TTF_OpenFont(fontSource.c_str(), pixelSize);
    if (!font)
        log("Font Uninitialized: " + fontSource);
    surface = TTF_RenderText_Blended(
        font,
        data.c_str(),
        data.size(),
        color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    SDL_GetTextureSize(texture, &rect.w, &rect.h);
    rect.x = x - rect.w / 2;
    rect.y = y;
}

void Text::render()
{
    SDL_RenderTexture(renderer, texture, nullptr, &rect);
}

void Text::updateData(string newData)
{
    if (texture)
        SDL_DestroyTexture(texture);
    surface = TTF_RenderText_Blended(
        font,
        newData.c_str(),
        newData.size(),
        color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
}

void Text::updateAlpha(int newAlpha)
{
    alpha = newAlpha;
    if (texture)
        SDL_SetTextureAlphaMod(texture, alpha);
}