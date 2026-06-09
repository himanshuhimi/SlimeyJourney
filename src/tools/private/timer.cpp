#include "../timer.h"

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