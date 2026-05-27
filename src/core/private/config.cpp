#include "../config.h"

const string TITLE = "Puddle Pebble";
const float SPRITE_SIZE = 32.0f;
int WIDTH = 640, HEIGHT = 360;
const int CAMERA_X = WIDTH / 2, CAMERA_Y = HEIGHT / 2;
const unordered_map<string, double> juiceStrengths = {
    {"apple", 0.1},
    {"melon", 0.2},
    {"orange", 0.3}
};

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
    SDL_GetTextureSize(texture, &width, &height);
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
    int pixelSize)
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

Animation::Animation(SDL_Renderer *renderer, SDL_FRect dst, string source)
    : renderer(renderer), dst(dst)
{
    imageSet = new Image(renderer, source);
}

void Animation::handle(double dt)
{
    timer += dt;
    if (timer >= frameTime)
    {
        timer = 0.0f;
        index++;
        int maxFrames = imageSet->width / SPRITE_SIZE;
        if (index >= maxFrames)
            index = 0;
    }
}

void Animation::render(Vector2D Camera)
{
    SDL_FRect renderDst = dst;
    renderDst.x -= Camera.x;
    renderDst.y -= Camera.y;
    src.x = index * SPRITE_SIZE;
    src.y = 0;
    src.w = (float)SPRITE_SIZE;
    src.h = (float)SPRITE_SIZE;
    imageSet->render(&src, &renderDst);
}

int _Random_::randint(int begin, int end)
{
    static bool seeded = false;
    if (!seeded)
    {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        seeded = true;
    }
    return begin + std::rand() % (end - begin + 1);
};

_Random_ Random;
_Colors_ colors;