#include "../config.h"

const string TITLE = "Slimey Journey";
const float SPRITE_SIZE = 32.0f;
int WIDTH{640}, HEIGHT{360};
int CHANGED_WIDTH{WIDTH * 3}, CHANGED_HEIGHT{HEIGHT* 3};
int scaleX{CHANGED_WIDTH / WIDTH}, scaleY{CHANGED_HEIGHT / HEIGHT};
int CAMERA_X{WIDTH / 2}, CAMERA_Y{HEIGHT / 2};
bool scaled = true;
const vector<string> fruits = {
    "apple",
    "blueberry",
    "mango",
    "melon",
    "orange",
    "strawberry"
};
SDL_RendererLogicalPresentation logicalPresentation = SDL_LOGICAL_PRESENTATION_INTEGER_SCALE;

bool checkCollision(SDL_FRect A, SDL_FRect B) { return SDL_HasRectIntersectionFloat(&A, &B); }

SDL_FRect getMousePosition()
{
    SDL_FRect result = {0, 0, 0, 0};
    SDL_GetMouseState(&result.x, &result.y);
    if (scaled)
        result.x /= scaleX;
        result.y /= scaleY;
    return result;
}

Vector2D::Vector2D(float x, float y) : x(x), y(y) {};

float Vector2D::distanceFromVec(Vector2D secVector)
{
    return sqrt(pow(secVector.x - x, 2) + pow(secVector.y - y, 2));
}

float Vector2D::getLength() { return sqrt(pow(x, 2) + pow(y, 2)); }

Vector2D Vector2D::normalise()
{
    float length = getLength();
    if (length == 0.0)
        return Vector2D{0, 0};
    x /= length;
    y /= length;
    return Vector2D{x, y};
}

Image::Image(SDL_Renderer *renderer, string source) : renderer(renderer)
{
    string path = "data/assets/" + source;
    if (!renderer)
        return;
    surface = IMG_Load(path.c_str());
    if (!surface)
        print("Unloaded Image: " + (string)SDL_GetError());
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_GetTextureSize(texture, &width, &height);
}

void Image::render(const SDL_FRect *src, const SDL_FRect *dst)
{
    SDL_RenderTexture(renderer, texture, src, dst);
}

Text::Text(
    SDL_Renderer *renderer,
    float x, float y, string data, SDL_Color color,
    int pixelSize, string fontSource)
    : renderer(renderer), x(x), y(y), pixelSize(pixelSize), color(color)
{
    string path = "data/assets/" + fontSource;
    font = TTF_OpenFont(path.c_str(), pixelSize);
    if (!font)
        print("Font Uninitialized: " + path);
    surface = TTF_RenderText_Blended(font, data.c_str(), data.size(), color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    SDL_GetTextureSize(texture, &rect.w, &rect.h);
    rect.x = x - (rect.w / 2);
    rect.y = y - (rect.h / 2);
}

void Text::render() { SDL_RenderTexture(renderer, texture, nullptr, &rect); }

void Text::updateData(string newData)
{
    if (texture)
        SDL_DestroyTexture(texture);
    surface = TTF_RenderText_Blended(
        font, newData.c_str(),
        newData.size(), color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_GetTextureSize(texture, &rect.w, &rect.h);
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_PIXELART);
}

void Text::updateAlpha(int newAlpha)
{
    alpha = newAlpha;
    if (texture)
        SDL_SetTextureAlphaMod(texture, alpha);
}

Animation::Animation(SDL_Renderer *renderer, string source, float frameTime)
    : renderer(renderer), frameTime(frameTime), imageSet(renderer, "anims/" + source), 
      maxFrames(imageSet.width / SPRITE_SIZE) {}

void Animation::handle(double dt)
{
    if (!active)
        return;
    timer += (float)dt;
    if (timer >= frameTime)
    {
        timer = 0.0f;
        index++;
        if (index >= maxFrames)
        {
            index = 0;
            complete = true;
            active = false;
        }
    };
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
    imageSet.render(&src, &renderDst);
}

void Animation::restart()
{
    index = 0;
    timer = 0;
    complete = false;
    active = true;
}

Audio::Audio(string audioSource)
{
    string path = "data/assets/" + audioSource;
    mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    if (!mixer)
        print("Mixer Unloaded");
    audio = MIX_LoadAudio(mixer, path.c_str(), true);
    if (!audio)
        print("Audio Unloaded: " + path);
    track = MIX_CreateTrack(mixer);
    if (!track)
        print("Track Unloaded");
    MIX_SetTrackAudio(track, audio);
}

void Audio::play(int times) { MIX_PlayTrack(track, times); }

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

LineOfSight::LineOfSight(SDL_Renderer *renderer, float x, float y, float width, float height)
    : renderer(renderer)
{
    rect.x = x;
    rect.y = y;
    rect.w = width;
    rect.h = height;
}

void LineOfSight::matchPosition(SDL_FRect matchRect)
{
    rect.x = matchRect.x + (matchRect.w / 2);
    rect.y = matchRect.y + (matchRect.h / 2);
}

void LineOfSight::render(Vector2D Camera)
{
    dst = rect;
    dst.x -= Camera.x;
    dst.y -= Camera.y;
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &dst);
}

Timer::Timer(SDL_Renderer *renderer, double duration)
    : duration(duration * 60), currentTime(duration * 60),
      text(renderer, WIDTH / 2, 20, "00:00", colors.white) {}

void Timer::handle(double dt)
{
    if (currentTime > 0.0)
        currentTime -= dt;
}

void Timer::render()
{
    int minutes = static_cast<int>(currentTime) / 60;
    int seconds = static_cast<int>(currentTime) % 60;
    char buffer[10];
    sprintf(buffer, "%02d:%02d", minutes, seconds);
    text.updateData(buffer);
    text.render();
}

int _Random_::randint(int begin, int end)
{
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(begin, end);
    return dist(rng);
};

map<int, double> durations = {
    {0, 10.0},
    {1, 0.5},
    {2, 0.5},
    {3, 0.5},
    {4, 0.5},
    {5, 1.0}
};

_Random_ Random;
_Colors_ colors;
_Consts_ constants;