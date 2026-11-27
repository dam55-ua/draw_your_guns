#include <MainMenuState.hpp>
#include <HowToPlayState.hpp>
#include <OptionsState.hpp>
#include <MainGameState.hpp>
#include <StateMachine.hpp>
#include <MenuBackground.hpp>
#include <ResourceManager.hpp>
#include <raylib.h>
#include <memory>
#include <vector>

MainMenuState::MainMenuState()
    : buttons{
        MenuButton("JUGAR",      { 760.0f, 130.0f, 450.0f, 86.0f }),
        MenuButton("COMO JUGAR", { 760.0f, 236.0f, 450.0f, 86.0f }),
        MenuButton("OPCIONES",   { 760.0f, 342.0f, 450.0f, 86.0f }),
        MenuButton("SALIR",      { 760.0f, 448.0f, 450.0f, 86.0f })
      }
{
}

void MainMenuState::init()
{
    LoadResources();
    layout.Update();

    focusIndex = 0;
    activateFocused = false;
}

void MainMenuState::LoadResources()
{
    font = ResourceManager::Get().GetFont(
        "assets/fonts/LilitaOne-Regular.ttf", 128);

    const Font defaultFont = GetFontDefault();

    if (font.texture.id != 0 &&
        font.texture.id != defaultFont.texture.id)
    {
        SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
    }

    logo = ResourceManager::Get().GetTexture(
        "assets/sprites/ui/menu.png");

    if (logo.id > 0)
    {
        SetTextureFilter(logo, TEXTURE_FILTER_BILINEAR);
    }
}

void MainMenuState::handleInput()
{
    const int count = static_cast<int>(buttons.size());

    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
        focusIndex = (focusIndex + 1) % count;
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
        focusIndex = (focusIndex + count - 1) % count;

    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
        activateFocused = true;

    if (IsKeyPressed(KEY_ESCAPE))
        state_machine->remove_state(true);
}

void MainMenuState::update(float deltaTime)
{
    (void)deltaTime;
    layout.Update();

    int pressed = -1;

    for (int i = 0; i < static_cast<int>(buttons.size()); ++i)
    {
        const bool focused = (i == focusIndex);
        const bool activated = activateFocused && focused;

        if (buttons[i].Update(layout, focused, activated))
            pressed = i;
    }

    activateFocused = false;

    switch (pressed)
    {
        case 0: state_machine->add_state(std::make_unique<MainGameState>(), true); break;
        case 1: state_machine->add_state(std::make_unique<HowToPlayState>(), false); break;
        case 2: state_machine->add_state(std::make_unique<OptionsState>(), false); break;
        case 3: state_machine->remove_state(true); break;
        default: break;
    }
}

void MainMenuState::DrawLogo() const
{
    if (logo.id == 0) return;   // no hay PNG, no se dibuja nada

    const Rectangle dst = layout.Rect(30.0f, 100.0f, 690.0f, 385.0f);
    const Rectangle src = { 0.0f, 0.0f,
                            static_cast<float>(logo.width),
                            static_cast<float>(logo.height) };

    DrawTexturePro(logo, src, dst, { 0.0f, 0.0f }, 0.0f, WHITE);
}

void MainMenuState::render()
{
    BeginDrawing();

        DrawMenuBackground();
        DrawLogo();

        for (const MenuButton& button : buttons)
            button.Draw(layout, font);

    EndDrawing();
}