#include <OptionsState.hpp>
#include <StateMachine.hpp>
#include <MenuBackground.hpp>
#include <raylib.h>

void OptionsState::handleInput()
{
    if (IsKeyPressed(KEY_ESCAPE))
    {
        state_machine->remove_state(false);
        return;
    }

    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
        ToggleFullscreen();
}

void OptionsState::update(float deltaTime)
{
    (void)deltaTime;
}

void OptionsState::render()
{
    BeginDrawing();

        DrawMenuBackground();

        const int width  = GetScreenWidth();
        const int height = GetScreenHeight();

        const char* title = "OPCIONES";
        DrawText(title, (width - MeasureText(title, 52)) / 2, height / 4, 52,
                 Color{ 46, 19, 5, 255 });

        const char* state = IsWindowFullscreen()
                          ? "Pantalla completa: SI"
                          : "Pantalla completa: NO";
        DrawText(state, (width - MeasureText(state, 30)) / 2, height / 2 - 40, 30,
                 Color{ 46, 19, 5, 255 });

        const char* hint = "INTRO o ESPACIO: cambiar";
        DrawText(hint, (width - MeasureText(hint, 22)) / 2, height / 2 + 30, 22,
                 Color{ 90, 60, 36, 255 });

        const char* back = "ESC: volver";
        DrawText(back, (width - MeasureText(back, 22)) / 2, height / 2 + 90, 22,
                 Color{ 90, 60, 36, 255 });

    EndDrawing();
}