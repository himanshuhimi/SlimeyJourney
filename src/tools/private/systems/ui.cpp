#include "../../systems/ui.h"

UIScreen::UIScreen(Game &game) : game(game) {}

void UIScreen::render(Vector2D Camera)
{
    for (auto &[category, widgets] : ctgWidgets)
        for (auto &[name, widget] : widgets)
        {
            if (!widget)
                continue;
            widget->render(Camera);
        }
}

void UIScreen::handle(double dt)
{
    for (auto &[category, widgets] : ctgWidgets)
        for (auto &[name, widget] : widgets)
        {
            if (!widget)
                continue;
            widget->handle(dt);
        }
}

void UIScreen::update(SDL_Event event)
{
    for (auto &[category, widgets] : ctgWidgets)
        for (auto &[name, widget] : widgets)
        {
            if (!widget)
                continue;
            widget->update(event);
        }
}

Hearts::Hearts(Game &game)
    : game(game), brokenHeart{game.renderer, "images/hearts/broken.png"},
      normalHeart(game.renderer, "images/hearts/normal.png")
{
    if (game.crntLvl == nullptr)
        return;
    load();
}

void Hearts::render()
{
    if (game.crntLvl == nullptr)
        return;
    if (images.empty())
        load();
    for (int i = 0; i < images.size(); i++)
        images[i].render(nullptr, &rects[i]);
}

void Hearts::handle()
{
    if (game.crntLvl == nullptr)
        return;
    if (game.crntLvl->player.HP != game.crntLvl->player.maxHP)
        for (int i = 0; i < game.crntLvl->player.maxHP; i++)
            images[i] = (i < game.crntLvl->player.HP) ? normalHeart : brokenHeart;
}

void Hearts::clear()
{
    if (game.crntLvl == nullptr)
        return;
    images.clear();
    rects.clear();
}

void Hearts::load()
{
    float startX = (WIDTH / 2 - (game.crntLvl->player.maxHP / 2 * SPRITE_SIZE));
    if (images.empty())
        for (int i = 0; i < game.crntLvl->player.maxHP; i++)
        {
            images.emplace_back(normalHeart);
            rects.emplace_back(
                SDL_FRect{
                    startX + (i * SPRITE_SIZE),
                    HEIGHT - SPRITE_SIZE,
                    images[i].width,
                    images[i].height});
        };
}

HomeScreen::HomeScreen(Game &game)
    : UIScreen(game), titleImage(game.renderer, "images/title.png")
{
    ctgWidgets["btns"];
    vector<pair<string, UIFunction>> funcs = {
        {"PLAY", [this]
         { this->game.setScene(Scenes::SELECTION); }},
        {"SETTINGS", [this]
         { this->game.setScene(Scenes::SETTINGS); }},
        {"QUIT", [this]
         { this->game.terminate(); }}};
    int i = 0;
    for (auto &[label, func] : funcs)
    {
        float padding = i++ * SPRITE_SIZE * 2;
        ctgWidgets.at("btns").emplace_back(label, make_unique<Button>(
                                                      game.renderer,
                                                      WIDTH / 4 + padding,
                                                      HEIGHT / 2 + padding,
                                                      func,
                                                      label,
                                                      colors.yellow));
    }
    titleRect = SDL_FRect{
        (float)WIDTH, 
        SPRITE_SIZE * 3, 
        titleImage.width, 
        titleImage.height};
    titleRect.x -= titleRect.w + SPRITE_SIZE;
}

void HomeScreen::render(Vector2D Camera)
{
    titleImage.render(nullptr, &titleRect);
    UIScreen::render(Camera);
}

LoadingScreen::LoadingScreen(Game &game) : UIScreen(game)
{
    ctgWidgets["progs"];
    ctgWidgets.at("progs").emplace_back(
        "loading",
        make_unique<Progress>(
            game.renderer,
            SPRITE_SIZE,
            HEIGHT - SPRITE_SIZE,
            [this]
            { this->game.setScene(this->game.nextScene, false); }));
}

void LoadingScreen::handle(double dt)
{
    getWidget<Progress>("progs", "loading").advance(0.0025);
    UIScreen::handle(dt);
}

SettingsScreen::SettingsScreen(Game &game)
    : UIScreen(game)
{
    ctgWidgets["toggles"];
    ctgWidgets["carousels"];
    int i = 0;
    for (auto &[category, data] : game.settings->data)
    {
        Text title(
            game.renderer,
            WIDTH / 12,
            16 + (SPRITE_SIZE * 3 * i++),
            capitalize(category),
            colors.white,
            18,
            1
        );
        texts.emplace_back(title);
        int count = game.settings->allowedData.at(category).size();
        const float padding = SPRITE_SIZE;
        SDL_FRect container = SDL_FRect{
            title.rect.x + (SPRITE_SIZE / 2),
            title.rect.y + SPRITE_SIZE,
            WIDTH / 2 + (SPRITE_SIZE * 6),
            (count) * (SPRITE_SIZE)
        };
        containers.emplace_back(container);
        int j = 0;
        for (auto &[name, options] : game.settings->allowedData.at(category))
        {
            Text text(
                game.renderer,
                container.x + (SPRITE_SIZE / 2),
                container.y + (SPRITE_SIZE / 2),
                toUppercase(name),
                colors.white,
                12, 1);
            text.rect.y += j++ * text.rect.h;
            texts.emplace_back(text);
            float widgetX = WIDTH / 2 + text.rect.x + (SPRITE_SIZE * 3);
            float widgetY = text.rect.y;
            if (options == SettingBool)
            {
                auto prevVal = this->game.settings->get(category, name);
                unique_ptr<Toggle> toggle = make_unique<Toggle>(
                    game.renderer,
                    widgetX,
                    widgetY,
                    [this]{},
                    (prevVal != "0") ? true : false);
                Toggle *togglePtr = toggle.get();
                toggle->onCallback = [this, category, name, togglePtr]
                {
                    auto crntVal = togglePtr->value;
                    this->game.settings->update(category, name, (crntVal) ? "1" : "0");
                };
                ctgWidgets.at("toggles").emplace_back(name, std::move(toggle));
            }
            else
            {
                unique_ptr<Carousel> carousel = make_unique<Carousel>(
                    game.renderer,
                    widgetX,
                    widgetY,
                    [this] {},
                    options,
                    this->game.settings->get(category, name));
                Carousel *carouselPtr = carousel.get();
                carousel->onCallback = [this, category, name, carouselPtr]
                {
                    auto crntVal = carouselPtr->data.at(carouselPtr->index);
                    this->game.settings->update(category, name, crntVal);
                };
                ctgWidgets.at("carousels").emplace_back(name, std::move(carousel));
            }
        }
    }
}

void SettingsScreen::render(Vector2D Camera)
{
    for (auto &text : texts)
        text.render(Camera);
    for (auto &container : containers)
    {
        SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);
        SDL_RenderRect(game.renderer, &container);
    }
    UIScreen::render(Camera);
}

SelectionScreen::SelectionScreen(Game &game) : UIScreen(game)
{
    ctgWidgets["cards"];
    vector<pair<string, UIFunction>> funcs = {};
    string directory = "data/maps";
    if (!fs::exists(directory))
        return;
    for (const auto &rgnEntry : fs::directory_iterator(directory))
    {
        auto rgnPath = rgnEntry.path();
        if (fs::is_directory(rgnPath))
        {
            string rgnFilename = rgnPath.filename().string();
            if (rgnFilename == "tilesets")
                continue;
            string region = rgnPath.stem().string();
            funcs.emplace_back(
                region,
                [this, region]
                {
                    this->game.setLevel(region, 0);
                    this->game.setScene(Scenes::PLAYING);
                });
        }
    }
    int i = 1;
    for (auto &[name, func] : funcs)
        ctgWidgets.at("cards").emplace_back(
            name,
            make_unique<Card>(
                game.renderer,
                WIDTH / 2,
                SPRITE_SIZE + SPRITE_SIZE * (2 * i++),
                capitalize(name),
                func));
}

PlayingScreen::PlayingScreen(Game &game) : UIScreen(game), hearts(game)
{
    ctgWidgets["progs"];
    ctgWidgets.at("progs").emplace_back(
        "fruit",
        make_unique<Progress>(
            game.renderer,
            WIDTH - SPRITE_SIZE,
            SPRITE_SIZE,
            [this]
            { this->game.crntLvl->quests.at("fruit").completed = true; },
            colors.violet,
            Image(game.renderer, "ui/bottle.png")));
    auto &fruit = getWidget<Progress>("progs", "fruit");
    fruit.rect.x -= fruit.rect.w + fruit.attachmentRect.w;
}

void PlayingScreen::render(Vector2D Camera)
{
    hearts.render();
    UIScreen::render(Camera);
}

void PlayingScreen::handle(double dt)
{
    hearts.handle();
    UIScreen::handle(dt);
}

PausedScreen::PausedScreen(Game &game)
    : UIScreen(game),
      title(game.renderer, SPRITE_SIZE * 2, HEIGHT / 2, "PAUSED!", colors.white, 48)
{
    ctgWidgets["btns"];
    vector<pair<string, UIFunction>> funcs = {
        {"CONTINUE", [this]
         { this->game.setScene(Scenes::PLAYING); }},
        {"HOME", [this]
         { this->game.setScene(Scenes::HOME); }},
        {"QUIT", [this]
         { this->game.terminate(); }}};
    int i = 0;
    for (auto &[name, function] : funcs)
        ctgWidgets.at("btns").emplace_back(
            name,
            make_unique<Button>(
                game.renderer,
                (WIDTH / 4) + SPRITE_SIZE * (i++),
                HEIGHT / 2,
                function,
                name,
                colors.yellow));
}

void PausedScreen::render(Vector2D Camera)
{
    title.render(Camera);
    UIScreen::render(Camera);
}

OverScreen::OverScreen(Game &game)
    : UIScreen(game),
      title(game.renderer, WIDTH / 2, 128, "Game Over!", colors.red, 48)
{
    ctgWidgets["btns"];
    vector<pair<string, UIFunction>> funcs = {
        {"TRY AGAIN!", [this]
         { this->game.setScene(Scenes::PLAYING); }},
        {"HOME", [this]
         { this->game.setScene(Scenes::HOME); }},
        {"QUIT", [this]
         { this->game.terminate(); }}};
    int i = 0;
    for (auto &[name, function] : funcs)
        ctgWidgets.at("btns").emplace_back(
            name,
            make_unique<Button>(
                game.renderer,
                (WIDTH / 2 - 196) + SPRITE_SIZE * 6 * (i++),
                HEIGHT / 2 + 32,
                function,
                name,
                colors.yellow));
}

void OverScreen::render(Vector2D Camera)
{
    title.render(Camera);
    UIScreen::render(Camera);
}

UI::UI(Game &game) : game(game)
{
    screens.insert({Scenes::HOME, make_unique<HomeScreen>(game)});
    screens.insert({Scenes::LOADING, make_unique<LoadingScreen>(game)});
    screens.insert({Scenes::SETTINGS, make_unique<SettingsScreen>(game)});
    screens.insert({Scenes::SELECTION, make_unique<SelectionScreen>(game)});
    screens.insert({Scenes::PLAYING, make_unique<PlayingScreen>(game)});
    screens.insert({Scenes::PAUSED, make_unique<PausedScreen>(game)});
    screens.insert({Scenes::OVER, make_unique<OverScreen>(game)});
}

void UI::render(Vector2D Camera)
{
    if (activeScreen == nullptr)
        return;
    activeScreen->render(Camera);
}

void UI::handle(double dt)
{
    if (activeScreen == nullptr)
        return;
    activeScreen->handle(dt);
}

void UI::update(SDL_Event event)
{
    if (activeScreen == nullptr)
        return;
    activeScreen->update(event);
}

void UI::updateScreen(Scenes scene)
{
    try
    {
        activeScreen = screens.at(scene).get();
    }
    catch (std::exception &e)
    {
        activeScreen = nullptr;
    }
}