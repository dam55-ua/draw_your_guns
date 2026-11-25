#include <GameOverState.hpp>
#include <MainGameState.hpp>
#include <MainMenuState.hpp>
#include <StateMachine.hpp>
#include <raylib.h>

// Evita que la misma pulsacion que mato al jugador reinicie al instante.
static constexpr float MIN_TIME_BEFORE_RESTART = 0.5f;

GameOverState::GameOverState(Result result): result(result), elapsed(0.0f)
{
}

void GameOverState::init()
{
    elapsed = 0.0f;
}

void GameOverState::handleInput()
{
    if (elapsed < MIN_TIME_BEFORE_RESTART) return;

    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
    {
        // Reemplaza este estado con una partida nueva (init() regenera el mapa).
        state_machine->add_state(std::make_unique<MainGameState>(), true);
    }
    else if (IsKeyPressed(KEY_ESCAPE))
    {
        state_machine->add_state(std::make_unique<MainMenuState>(), true);
    }
}

void GameOverState::update(float deltaTime)
{
    elapsed += deltaTime;
}

void GameOverState::render()
{
    BeginDrawing();

        ClearBackground(DARKGRAY);

        const char* title = "GAME OVER";
        DrawText(title,
                 (GetScreenWidth() - MeasureText(title, 80)) / 2,
                 GetScreenHeight() / 2 - 120, 80, RAYWHITE);

        if (result == Result::Draw)
        {
            const char* drawMsg = "EMPATE";
            DrawText(drawMsg,
                     (GetScreenWidth() - MeasureText(drawMsg, 40)) / 2,
                     GetScreenHeight() / 2 - 20, 40, RAYWHITE);
        }
        else
        {
            const char* teamName = "DERECHA";
            Color teamColor = RED;

            if (result == Result::LeftWins)
            {
                teamName = "IZQUIERDA";
                teamColor = BLUE;
            }

            const char* winnerMsg = TextFormat("Ha ganado el equipo %s", teamName);
            DrawText(winnerMsg,
                     (GetScreenWidth() - MeasureText(winnerMsg, 40)) / 2,
                     GetScreenHeight() / 2 - 20, 40, teamColor);
        }

        const char* restartMsg = "Pulsa INTRO o ESPACIO para volver a jugar";
        DrawText(restartMsg,
                 (GetScreenWidth() - MeasureText(restartMsg, 20)) / 2,
                 GetScreenHeight() / 2 + 60, 20, LIGHTGRAY);

        const char* menuMsg = "ESC: menu principal";
        DrawText(menuMsg,
                 (GetScreenWidth() - MeasureText(menuMsg, 20)) / 2,
                 GetScreenHeight() / 2 + 95, 20, LIGHTGRAY);

    EndDrawing();
}
