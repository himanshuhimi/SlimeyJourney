#include "../game.h"

Game::Game()
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
        log("SDL Unloaded: " + (string)SDL_GetError());
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
    level = new Level(renderer, 1);
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
    level->render();
    SDL_RenderPresent(renderer);
}

void Game::handle()
{
    updateDeltaTime();
    while (SDL_PollEvent(&event))
        if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
            terminate();
    level->handle(dt);
    for (
        auto fruitIt = level->fruits.begin(); 
        fruitIt != level->fruits.end(); 
    )
    {
        if (!fruitIt->taken 
            && SDL_HasRectIntersectionFloat(&fruitIt->rect, &level->player.rect))
        {
            fruitIt->taken = true;
            level->points += 1;
        }
        else
            fruitIt++;
    }
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