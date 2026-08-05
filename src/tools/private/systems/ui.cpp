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
    : game(game), brokenHeart{game.renderer, "ui/hearts/broken.png"},
      normalHeart(game.renderer, "ui/hearts/normal.png")
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
            { this->game.setScene(this->game.nextScene, false); },
            colors.white,
            Image{nullptr, ""},
            0.0,
            WIDTH - 64));
}

void LoadingScreen::handle(double dt)
{
    getWidget<Progress>("progs", "loading").advance(0.001);
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
            WIDTH / 2,
            16 + (SPRITE_SIZE * 3 * i++),
            capitalize(category),
            colors.white,
            18);
        texts.emplace_back(title);
        int count = game.settings->allowedData.at(category).size();
        const float padding = SPRITE_SIZE;
        SDL_FRect container = SDL_FRect{
            WIDTH / 2.0f,
            title.rect.y + SPRITE_SIZE,
            (float)WIDTH - (SPRITE_SIZE * 2),
            (count) * (SPRITE_SIZE)};
        container.x -= container.w / 2;
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
            float widgetX = container.w - container.w / 8.0f;
            float widgetY = text.rect.y;
            if (options == SettingBool)
            {
                auto prevVal = this->game.settings->get(category, name);
                unique_ptr<Toggle> toggle = make_unique<Toggle>(
                    game.renderer,
                    widgetX,
                    widgetY,
                    [this] {},
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
        SDL_SetRenderDrawBlendMode(game.renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 32);
        SDL_RenderFillRect(game.renderer, &container);
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
    {
        Image image{game.renderer, "ui/cards/default.png"};
        string path = "data/assets/ui/cards/" + name + ".png";
        if (fs::exists(path))
            image = Image{game.renderer, path};
        else
            image = Image(game.renderer, "ui/cards/default.png");
        ctgWidgets.at("cards").emplace_back(
            name,
            make_unique<Card>(
                game.renderer,
                WIDTH / 2,
                HEIGHT / 2  - SPRITE_SIZE + SPRITE_SIZE * (1.5 * i++),
                capitalize(name),
                func,
                image));
    }
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
         { this->game.setScene(Scenes::PLAYING, false); }},
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
                HEIGHT / 2 + SPRITE_SIZE,
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
        {"TRY AGAIN", [this]
         { this->game.setScene(Scenes::SELECTION); }},
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
                HEIGHT / 2 + SPRITE_SIZE,
                function,
                name,
                colors.yellow));
}

void OverScreen::render(Vector2D Camera)
{
    title.render(Camera);
    UIScreen::render(Camera);
}

UI::UI(Game &game) : game(game) {}

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
    switch (scene)
    {
    case Scenes::HOME:
        activeScreen = make_unique<HomeScreen>(game);
        break;
    case Scenes::LOADING:
        activeScreen = make_unique<LoadingScreen>(game);
        break;
    case Scenes::SETTINGS:
        activeScreen = make_unique<SettingsScreen>(game);
        break;
    case Scenes::SELECTION:
        activeScreen = make_unique<SelectionScreen>(game);
        break;
    case Scenes::PLAYING:
        activeScreen = make_unique<PlayingScreen>(game);
        break;
    case Scenes::PAUSED:
        activeScreen = make_unique<PausedScreen>(game);
        break;
    case Scenes::OVER:
        activeScreen = make_unique<OverScreen>(game);
        break;
    default:
        activeScreen = nullptr;
        break;
    }
}