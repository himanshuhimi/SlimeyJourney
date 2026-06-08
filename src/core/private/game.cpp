#include "../game.h"

Game::Game()
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
        print("SDL Unloaded: " + (string)SDL_GetError());
    if (!TTF_Init())
        print("TTF Unloaded: " + (string)SDL_GetError());
    if (!MIX_Init())
        print("Mix Unloaded: " + (string)SDL_GetError());
    if (!SDL_CreateWindowAndRenderer(TITLE.c_str(), CHANGED_WIDTH, CHANGED_HEIGHT,
                                     0, &window, &renderer))
        print("Display Unloaded: " + (string)SDL_GetError());
    SDL_SetRenderLogicalPresentation(renderer, WIDTH, HEIGHT,
                                     SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);
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
    if (!uiInit)
    {
        ui = new UI(this);
        uiInit = true;
    }
    updateDeltaTime();
    while (SDL_PollEvent(&event))
        switch (event.type)
        {
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            terminate();
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
        {
            if (ui != nullptr)
                ui->handle(event);
            currentLevel->player.mouseClicked = (event.button.button == SDL_BUTTON_LEFT);
            break;
        }
        }
    switch (state)
    {
    // case States::LOADING:
    //     ui->loading.handle(dt);
    //     break;
    case States::PLAYING:
        currentLevel->handle(dt);
        break;
    }
    if (ui != nullptr)
    {
        ui->update(dt);
        if (ui->loading.complete)
            state = nextState;
    }
    if (state == States::PLAYING && SDL_GetKeyboardState(NULL)[SDL_SCANCODE_ESCAPE])
        update(States::PAUSED);
    updateStateTexts();
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
    // case States::LOADING:
    //     ui->loading.update();
    //     ui->loading.render();
    //     break;
    case States::PLAYING:
        currentLevel->render();
        break;
    }
    if (ui != nullptr)
        ui->render();
    for (auto &text : texts)
        text.render();
    SDL_RenderPresent(renderer);
}

void Game::update(States newState, bool loading)
{
    nextState = newState;
    state = nextState;
    ui->loadButtons();
    loadLevels();
}

void Game::nextLevel()
{
    if (levelNum >= levels.size())
    {
        update(States::HOME);
        return;
    }
    update(States::PLAYING);
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

void Game::loadLevels()
{
    if (!levels.empty())
        levels.clear();
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
        levels.emplace_back(new Level(renderer, number));
    }
    updateLevel();
}

void Game::updateLevel() 
{ 
    if (levelNum < levels.size())
        currentLevel = levels.at(levelNum);
}

void Game::manageUpdation()
{
    if (state != States::PLAYING)
        return;
    bool complete = currentLevel->fruitBar.percentage >= 0.8;
    bool collided = checkCollision(currentLevel->player.rect, currentLevel->flag.rect);
    if (complete && collided)
    {
        update(States::PROGRESSING);
        levelNum += 1;
    }
    else if (currentLevel->player.dead)
        update(States::OVER);
}

void Game::updateStateTexts()
{
    if (!texts.empty())
        texts.clear();
    if (titles.find(state) == titles.end())
        return;
    texts.emplace_back(renderer, WIDTH / 2, 50,
                       titles.at(state), titleColors.at(state),
                       48, "assets/fonts/molle.ttf");
}