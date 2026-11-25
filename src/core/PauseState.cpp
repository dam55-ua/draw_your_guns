#include <PauseState.hpp>
#include <MainGameState.hpp>
#include <StateMachine.hpp>
#include <raylib.h>

namespace
{
    const char* OPTIONS[] = {
        "CONTINUAR",
        "REINICIAR PARTIDA",
        "ABANDONAR PARTIDA",
        "CERRAR JUEGO"
    };

    const int OPTION_COUNT = 4;
}

PauseState::PauseState(MainGameState& gameState)
    : game(gameState)
{
}

void PauseState::handleInput()
{
    // Confirmacion: S = si, N o ESC = no.
    if (confirm != Confirm::None)
    {
        if (IsKeyPressed(KEY_S))
        {
            const Confirm action = confirm;
            confirm = Confirm::None;

            if (action == Confirm::Abandon)
            {
                game.RequestAbandon();
                state_machine->remove_state(false);
            }
            else if (action == Confirm::Quit)
            {
                state_machine->remove_state(true);
            }

            return;
        }

        if (IsKeyPressed(KEY_N) || IsKeyPressed(KEY_ESCAPE))
            confirm = Confirm::None;

        return;
    }

    // ESC tambien reanuda.
    if (IsKeyPressed(KEY_ESCAPE))
    {
        state_machine->remove_state(false);
        return;
    }

    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
        selected = (selected + 1) % OPTION_COUNT;
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
        selected = (selected + OPTION_COUNT - 1) % OPTION_COUNT;

    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
    {
        switch (selected)
        {
            case 0:
                state_machine->remove_state(false);
                break;

            case 1:
                game.RequestRestart();
                state_machine->remove_state(false);
                break;

            case 2:
                confirm = Confirm::Abandon;
                break;

            case 3:
                confirm = Confirm::Quit;
                break;

            default:
                break;
        }
    }
}

void PauseState::update(float deltaTime)
{
    (void)deltaTime;
}

void PauseState::render()
{
    BeginDrawing();

    ClearBackground(BLACK);

    const int width  = GetScreenWidth();
    const int height = GetScreenHeight();

    // Escala sencilla, misma proporcion que el lienzo 1280x720.
    const float scale = static_cast<float>(height) / 720.0f;

    const char* title = "PAUSA";
    const int titleSize = static_cast<int>(52.0f * scale);
    DrawText(title, (width - MeasureText(title, titleSize)) / 2,
             static_cast<int>(height * 0.18f), titleSize, RAYWHITE);

    if (confirm != Confirm::None)
    {
        const char* line1 = "Cerrar el juego?";
        if (confirm == Confirm::Abandon){
            line1 = "El equipo rival ganara la partida.";
        }
        const char* line2 = "Seguro?   S / N";

        const int size1 = static_cast<int>(28.0f * scale);
        const int size2 = static_cast<int>(22.0f * scale);

        DrawText(line1, (width - MeasureText(line1, size1)) / 2,
                 static_cast<int>(height * 0.45f), size1, RAYWHITE);
        DrawText(line2, (width - MeasureText(line2, size2)) / 2,
                 static_cast<int>(height * 0.55f), size2, LIGHTGRAY);
    }
    else
    {
        const int size = static_cast<int>(30.0f * scale);
        const int startY = static_cast<int>(height * 0.40f);
        const int gap = static_cast<int>(56.0f * scale);

        for (int i = 0; i < OPTION_COUNT; ++i)
        {
            const int textWidth = MeasureText(OPTIONS[i], size);
            const int x = (width - textWidth) / 2;
            const int y = startY + i * gap;

            if (i == selected)
            {
                DrawText(">", x - static_cast<int>(40.0f * scale), y, size, YELLOW);
                DrawText(OPTIONS[i], x, y, size, RAYWHITE);
            }
            else
            {
                DrawText(OPTIONS[i], x, y, size, GRAY);
            }
        }

        const char* hint = "ESC: continuar";
        const int hintSize = static_cast<int>(18.0f * scale);
        DrawText(hint, (width - MeasureText(hint, hintSize)) / 2,
                 height - static_cast<int>(60.0f * scale), hintSize, DARKGRAY);
    }

    EndDrawing();
}