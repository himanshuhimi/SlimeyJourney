#include "../game.h"

Game::Game()
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
        print("SDL Unloaded: " + (string)SDL_GetError());
    if (!TTF_Init())
        print("TTF Unloaded: " + (string)SDL_GetError());
    if (!MIX_Init())
        print("Mix Unloaded: " + (string)SDL_GetError());
    if (!SDL_CreateWindowAndRenderer(TITLE.c_str(), WIDTH, HEIGHT, 0, &window, &renderer))
        print("Display Unloaded: " + (string)SDL_GetError());
    loadLevels();
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
            currentLevel->player.mouseClicked = (event.button.button == SDL_BUTTON_LEFT);
            break;
        }
    switch (state)
    {
    case States::HOME:
        break;
    case States::SETTINGS:
        break;
    case States::PLAYING:
        currentLevel->handle(dt);
        break;
    case States::PAUSED:
        break;
    case States::PROGRESSING:
        break;
    case States::OVER:
        break;
    }
    manageUpdation();
}

void Game::render()
{
    SDL_SetRenderDrawColor(
        renderer, colors.skyblue.r,
        colors.skyblue.g, colors.skyblue.b,
        colors.skyblue.a);
    SDL_RenderClear(renderer);
    switch (state)
    {
    case States::HOME:
        break;
    case States::SETTINGS:
        break;
    case States::PLAYING:
        currentLevel->render();
        break;
    case States::PAUSED:
        break;
    case States::PROGRESSING:
        break;
    case States::OVER:
        break;
    }
    SDL_RenderPresent(renderer);
}

void Game::update(States newState) { state = newState; }

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

void Game::loadLevels()
{
    string directory = "maps";
    if (!fs::exists(directory) && !fs::is_directory(directory))
        return;
    for (const auto &entry : fs::directory_iterator(directory))
    {
        auto path = entry.path();
        if (path.filename().string() == "tilesets" || path.extension().string() != ".tmx")
            continue;
        string filename = path.stem().string();
        int number = std::stoi(filename);
        levels.emplace_back(new Level(renderer, number + 1));
    }
    updateLevel();
}

void Game::updateLevel() { currentLevel = levels.at(levelNum - 1); }

void Game::manageUpdation()
{
    bool complete = currentLevel->fruitBar.complete;
    bool collided = checkCollision(currentLevel->player.rect, currentLevel->flag.rect);
    if (complete && collided)
        update(States::PROGRESSING);
    if (currentLevel->player.dead)
        update(States::OVER);
}