#include "../game.h"

Game::Game()
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
        log("SDL Unloaded: " + (string) SDL_GetError());
    else
        log("SDL Loaded");
    if (!TTF_Init())
        log("TTF Unloaded: " + (string)SDL_GetError());
    else
        log("TTF Loaded");
    if (!SDL_CreateWindowAndRenderer(TITLE.c_str(), WIDTH, HEIGHT, 0, &window, &renderer))
        log("Display Unloaded: " + (string)SDL_GetError());
    else
        log("Display Loaded");
    active = true;
}

void Game::launch()
{
    while (active)
    {
        handle();
        render();
    }
}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer, 100, 198, 243, 255);
    SDL_RenderClear(renderer);
    for (auto grass : grasses)
        grass->render();
    player->render();
    SDL_RenderPresent(renderer);
}

void Game::handle()
{
    updateDeltaTime();
    while (SDL_PollEvent(&event))
        if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
            terminate();
    player->handle(dt, grasses);
}

void Game::terminate()
{
    active = false;
    SDL_Quit();
}

void Game::updateDeltaTime()
{
    NOW = SDL_GetPerformanceCounter();
    dt = (double)(NOW - LAST) / SDL_GetPerformanceFrequency();
    LAST = NOW;
}