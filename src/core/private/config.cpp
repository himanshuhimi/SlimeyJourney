#include "../config.h"

const string TITLE = "Slimey Journey";
const float SPRITE_SIZE = 32.0f;
int DEFAULT_WIDTH = 640, DEFAULT_HEIGHT = 360;
int WIDTH = 640, HEIGHT = 360;
int scaleX = WIDTH / DEFAULT_WIDTH;
int scaleY = HEIGHT / DEFAULT_HEIGHT;
const int CAMERA_X = WIDTH / 2, CAMERA_Y = HEIGHT / 2;

bool checkCollision(SDL_FRect A, SDL_FRect B)
{
    return SDL_HasRectIntersectionFloat(&A, &B);
}

SDL_FRect GetMousePosition()
{
    SDL_FRect result = {0, 0, 0, 0};
    SDL_GetMouseState(&result.x, &result.y);
    return result;
}

Image::Image(SDL_Renderer *renderer, string source) : renderer(renderer)
{
    if (!renderer)
        return;
    surface = IMG_Load(source.c_str());
    if (!surface)
        log("Unloaded Image: " + (string)SDL_GetError());
    texture = SDL_CreateTextureFromSurface(renderer, surface);
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
    SDL_Color color,
    int pixelSize,
    string fontSource)
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
    SDL_GetTextureSize(texture, &rect.w, &rect.h);
}

void Text::updateAlpha(int newAlpha)
{
    alpha = newAlpha;
    if (texture)
        SDL_SetTextureAlphaMod(texture, alpha);
}

Animation::Animation(SDL_Renderer *renderer, string source)
    : renderer(renderer)
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
        complete = true;
    }
}

void Animation::render(Vector2D Camera, SDL_FRect dst)
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

Audio::Audio(string audioSource)
{
    mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    if (!mixer)
        log("Mixer Unloaded");
    audio = MIX_LoadAudio(mixer, audioSource.c_str(), true);
    if (!audio)
        log("Audio Unloaded: " + audioSource);
    track = MIX_CreateTrack(mixer);
    if (!track)
        log("Track Unloaded");
    MIX_SetTrackAudio(track, audio);
}

void Audio::play(int times)
{
    MIX_PlayTrack(track, times);
}

Cooldown::Cooldown(double duration) : duration(duration) {}

void Cooldown::handle(double dt)
{
    if (!available)
    {
        timeElapsed += dt;
        if (timeElapsed >= duration)
        {
            available = true;
            timeElapsed = 0;
        }
    }
}

_Random_ Random;
_Colors_ colors;
_Consts_ constants;