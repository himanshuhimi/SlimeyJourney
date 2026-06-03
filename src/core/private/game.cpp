#include "../game.h"

Game::Game()
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
        log("SDL Unloaded: " + (string)SDL_GetError());
    if (!TTF_Init())
        log("TTF Unloaded: " + (string)SDL_GetError());
    if (!MIX_Init())
        log("Mix Unloaded: " + (string)SDL_GetError());
    if (!SDL_CreateWindowAndRenderer(TITLE.c_str(), WIDTH, HEIGHT, 0, &window, &renderer))
        log("Display Unloaded: " + (string)SDL_GetError());
    level = new Level(renderer, 1);
    audios = {
        {"pickup", new Audio("assets/audios/pickup.wav")},
        {"hurt", new Audio("assets/audios/hurt.wav")}
    };
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

void Game::handle()
{
    updateDeltaTime();
    while (SDL_PollEvent(&event))
        switch (event.type)
        {
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            terminate();
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
            level->player.mouseClicked = (event.button.button == SDL_BUTTON_LEFT);
            break;
        }
    collision();
    level->handle(dt);
}

void Game::render()
{
    SDL_SetRenderDrawColor(
        renderer,
        colors.skyblue.r,
        colors.skyblue.g,
        colors.skyblue.b,
        colors.skyblue.a);
    SDL_RenderClear(renderer);
    level->render();
    SDL_RenderPresent(renderer);
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

void Game::collision()
{
    for (auto fruitIt = level->fruits.begin(); fruitIt != level->fruits.end();)
        if (!fruitIt->picked && checkCollision(fruitIt->rect, level->player.rect))
        {
            fruitIt->picked = true;
            level->points += 1;
            audios["pickup"]->play();
            level->fruitBar.update(level->increment);
        }
        else
            fruitIt++;
    for (auto ballIt = level->player.balls.begin(); ballIt != level->player.balls.end(); ballIt++)
        for (auto enemyIt = level->enemies.begin(); enemyIt != level->enemies.end();)
        {
            if (!ballIt->used && checkCollision(ballIt->rect, enemyIt->rect))
            {
                enemyIt->damage();
                enemyIt->healthBar.update(-(double)1 / enemyIt->maxHP);
                audios["hurt"]->play();
                ballIt->used = true;
            }
            else
                enemyIt++;
            enemyIt->drop<Fruit>(level->fruits);
        }
}