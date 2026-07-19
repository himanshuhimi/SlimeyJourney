#include "../util.h"

Audio::Audio(string audioSource)
{
    string path = "data/assets/audios/" + audioSource;
    mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    if (!mixer)
        std::cout << "Mixer Unloaded";
    audio = MIX_LoadAudio(mixer, path.c_str(), true);
    if (!audio)
        std::cout << "Audio Unloaded:" + path;
    track = MIX_CreateTrack(mixer);
    if (!track)
        std::cout << "Track Unloaded";
    MIX_SetTrackAudio(track, audio);
}

void Audio::play(int volume, int times)
{
    MIX_SetTrackGain(track, (float)volume / 100);
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

void Cooldown::reset()
{
    timeElapsed = 0;
    available = false;
}

Timer::Timer(SDL_Renderer *renderer, float x, float y, double duration)
    : duration(duration * 60), currentTime(duration * 60),
      text(renderer, x, y, "00:00", colors.white) {}

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

string capitalize(string s)
{
    if (!s.empty())
        s[0] = std::toupper(static_cast<unsigned char>(s[0]));
    return s;
}

string toUppercase(string s)
{
    if (!s.empty())
        for (int i = 0; i < s.size(); i++)
            s[i] = std::toupper(static_cast<unsigned char>(s[i]));
    return s;
}