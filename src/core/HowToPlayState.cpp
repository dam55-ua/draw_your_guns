#include <HowToPlayState.hpp>
#include <StateMachine.hpp>
#include <MenuBackground.hpp>
#include <raylib.h>

void HowToPlayState::handleInput()
{
    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
        state_machine->remove_state(false);
}

void HowToPlayState::update(float deltaTime)
{
    (void)deltaTime;
}

void HowToPlayState::render()
{
    BeginDrawing();

        DrawMenuBackground();

        const int width  = GetScreenWidth();
        const int height = GetScreenHeight();

        const char* title = "COMO JUGAR";
        DrawText(title, (width - MeasureText(title, 52)) / 2, height / 6, 52,
                 Color{ 46, 19, 5, 255 });

        const char* lines[] = {
            "A / D o flechas: moverse",
            "W o flecha arriba: saltar",
            "Raton: apuntar",
            "Clic izquierdo: disparar",
            "INTRO: pasar turno",
            "ESC en partida: volver al menu"
        };

        int y = height / 2 - 130;
        for (const char* line : lines)
        {
            DrawText(line, (width - MeasureText(line, 26)) / 2, y, 26,
                     Color{ 46, 19, 5, 255 });
            y += 48;
        }

        const char* back = "ESC o INTRO: volver";
        DrawText(back, (width - MeasureText(back, 22)) / 2, height - 90, 22,
                 Color{ 90, 60, 36, 255 });

    EndDrawing();
}