#include <GameOverState.hpp>
#include <MainGameState.hpp>
#include <StateMachine.hpp>
#include <raylib.h>

// Evita que la misma pulsacion que mato al jugador reinicie al instante.
static constexpr float MIN_TIME_BEFORE_RESTART = 0.5f;

GameOverState::GameOverState(Player::Team winnerTeam): winnerTeam(winnerTeam), elapsed(0.0f)
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
}

void GameOverState::update(float deltaTime)
{
    elapsed += deltaTime;
}

void GameOverState::render()
{
    BeginDrawing();

        ClearBackground(DARKGRAY);

        const char* teamName = (winnerTeam == Player::Team::Left) ? "IZQUIERDA" : "DERECHA";
        const Color teamColor = (winnerTeam == Player::Team::Left) ? BLUE : RED;

        const char* title = "GAME OVER";
        DrawText(title,
                 (GetScreenWidth() - MeasureText(title, 80)) / 2,
                 GetScreenHeight() / 2 - 120, 80, RAYWHITE);

        const char* winnerMsg = TextFormat("Ha ganado el equipo %s", teamName);
        DrawText(winnerMsg,
                 (GetScreenWidth() - MeasureText(winnerMsg, 40)) / 2,
                 GetScreenHeight() / 2 - 20, 40, teamColor);

        const char* restartMsg = "Pulsa INTRO o ESPACIO para volver a jugar";
        DrawText(restartMsg,
                 (GetScreenWidth() - MeasureText(restartMsg, 20)) / 2,
                 GetScreenHeight() / 2 + 60, 20, LIGHTGRAY);

    EndDrawing();
}
