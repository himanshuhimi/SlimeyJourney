#include "../game.h"

Game::Game() : player(nullptr, 32, 64)
{
    settings = new Settings();
    string size = settings->get("graphics", "size");
    vector<string> partitioned = partition(size, "x");
    CHANGED_WIDTH = std::stoi(partitioned.at(0));
    CHANGED_HEIGHT = std::stoi(partitioned.at(partitioned.size() - 1));
    updateScale();
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
        print("SDL Unloaded: " + (string)SDL_GetError());
    if (!TTF_Init())
        print("TTF Unloaded: " + (string)SDL_GetError());
    if (!MIX_Init())
        print("Mix Unloaded: " + (string)SDL_GetError());
    float width = (scaled) ? CHANGED_WIDTH : WIDTH;
    float height = (scaled) ? CHANGED_HEIGHT : HEIGHT;
    if (!SDL_CreateWindowAndRenderer(
            TITLE.c_str(),
            width, height,
            0,
            &window, &renderer))
        print("Display Unloaded: " + (string)SDL_GetError());
    if (scaled)
        SDL_SetRenderLogicalPresentation(renderer, WIDTH, HEIGHT, logicalPresentation);
    SDL_SetRenderVSync(renderer, std::stoi(settings->get("graphics", "vsync")));
    ui = new UI(*this);
    parallax = new Parallax(renderer);
    active = true;
    player = Player(renderer, 32, 64);
    audios = {
        {"success", Audio("success.wav")}};
    setScene(Scenes::HOME, false);
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
    {
        switch (event.type)
        {
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            terminate();
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            if (scene == Scenes::PLAYING && (event.button.button == SDL_BUTTON_LEFT))
                player.mouseClicked = true;
            break;
        case SDL_EVENT_KEY_UP:
            if (event.key.key == SDLK_ESCAPE)
            {
                if (scene != Scenes::PLAYING &&
                    scene != Scenes::LOADING &&
                    scene != Scenes::HOME)
                    setScene(Scenes::HOME);
                if (scene == Scenes::PLAYING)
                    setScene(Scenes::PAUSED, false);
            }
            break;
        }
        ui->update(event);
    }
    for (auto &cloud : clouds)
        cloud.handle(dt);
    for (auto &petal : petals)
        petal.handle(dt);
    ui->handle(dt);
    switch (scene)
    {
    case Scenes::PLAYING:
        world->handle(dt);
        collision();
        break;
    }
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
    parallax->render((scene == Scenes::PLAYING) ? world->Camera : Vector2D{});
    if (scene == Scenes::PLAYING)
    {
        world->render();
        player.render(world->Camera);
    }
    for (auto &cloud : clouds)
        cloud.render((scene == Scenes::PLAYING) ? world->Camera : Vector2D{});
    for (auto &petal : petals)
        petal.render((scene == Scenes::PLAYING) ? world->Camera : Vector2D{});
    ui->render();
    SDL_RenderPresent(renderer);
}

void Game::setScene(Scenes newScene, bool loading)
{
    prevScene = scene;
    nextScene = newScene;
    scene = (loading) ? Scenes::LOADING : nextScene;
    ui->updateScreen(scene);
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

void Game::loadWorld(string region)
{
    vector<int> levels = {};
    string directory = "data/maps/" + region;
    if (!fs::exists(directory))
        return;
    if (fs::is_directory(fs::path(directory)))
    {
        for (auto &tmx : fs::directory_iterator(directory))
        {
            auto tmxPath = tmx.path();
            string tmxFilename = tmxPath.filename().string();
            int number = std::stoi(tmxPath.stem().string());
            levels.emplace_back(number);
        }
    }
    int maxLevel = *std::max_element(levels.begin(), levels.end());
    for (int i = 0; i <= maxLevel; i++)
        worldSeed += std::to_string(Random.randint(0, maxLevel));
    world = new World(renderer, &player, region, worldSeed);
}

void Game::unloadWorld()
{
    world = nullptr;
}

void Game::collision()
{
    if (world->map.stones.size() > 0)
        for (auto stoneIt = world->map.stones.begin();
             stoneIt != world->map.stones.end(); stoneIt++)
        {
            auto stone = stoneIt->get();
            for (auto bIt = player.balls.begin();
                 bIt != player.balls.end();)
            {
                auto ball = *bIt;
                if (checkCollision(stone->rect, ball.rect))
                {
                    stone->destruct();
                    bIt = player.balls.erase(bIt);
                }
                else
                    bIt++;
            }
        }
    if (world->map.spikes.size() > 0)
        for (auto &spike : world->map.spikes)
            if (checkCollision(player.rect, spike->rect))
                player.resetPos();
    if (world->map.pads.size() > 0)
    {
        bool onPad = false;
        for (auto &pad : world->map.pads)
        {
            onPad = checkCollision(player.rect, pad->rect) && !player.buffed;
            break;
        }
        if (onPad && !player.buffed)
        {
            player.jumpStrength += 0.2 * player.baseJumpStrength;
            player.buffed = true;
        }
    }
    if (world->map.fruits.size() > 0)
        for (auto fruitIt = world->map.fruits.begin();
             fruitIt != world->map.fruits.end();)
        {
            Fruit *fruit = dynamic_cast<Fruit *>(fruitIt->get());
            if (!fruit->picked && checkCollision(fruit->rect, player.rect))
            {
                player.audios.at("pickup").play();
                ui->getWidget<Progress>("progs", "fruit").advance(world->increment);
                fruitIt = world->map.fruits.sprites.erase(fruitIt);
            }
            else
                ++fruitIt;
        }
    if (world->map.enemies.size() > 0)
        for (auto eIt = world->map.enemies.begin(); eIt != world->map.enemies.end();)
        {
            auto &enemy = *eIt;
            auto rawSlime = enemy.get();
            if (enemy->dead)
            {
                // enemy->drop(world->map.fruits.sprites);
                eIt = world->map.enemies.sprites.erase(eIt);
                player.enemiesKilled++;
                if (player.HP != player.maxHP)
                    player.HP++;
                continue;
            }
            if (checkCollision(player.rect, enemy->range))
            {
                enemy->actions.alert = true;
                enemy->actions.attacking = true;
                player.inCombat = true;
                player.combatEnemy = rawSlime;
            }
            else if (player.inCombat && player.combatEnemy == rawSlime)
            {
                player.inCombat = false;
                player.combatEnemy = nullptr;
                enemy->actions.attacking = false;
                enemy->actions.alert = false;
            }
            if (enemy->actions.attacking)
                enemy->attack((player.Position - enemy->Position).normalise());
            for (auto wIt = enemy->throws.begin(); wIt != enemy->throws.end();)
            {
                auto &weapon = *wIt->get();
                if (!weapon.used && checkCollision(player.rect, weapon.rect))
                {
                    player.damage();
                    wIt = enemy->throws.erase(wIt);
                }
                else
                    wIt++;
            }
            for (auto bIt = player.balls.begin();
                 bIt != player.balls.end();)
                if (!bIt->used && checkCollision(enemy->rect, bIt->rect))
                {
                    enemy->damage();
                    bIt = player.balls.erase(bIt);
                }
                else
                    bIt++;
            eIt++;
        }
    // ending needs to be changed
    if (checkCollision(player.rect, world->checkpoint.rect))
    {
        
    }
    if (player.dead)
        setScene(Scenes::OVER, false);
}