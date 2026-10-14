#include <MainGameState.hpp>
#include <raylib.h>

MainGameState::MainGameState()
    : players{}, currentPlayerIndex(0), pendingShot(std::nullopt)
{
}

float MainGameState::GetGroundY() const
{
    return static_cast<float>(GetScreenHeight()) - GameConfig::GROUND_HEIGHT;
}

void MainGameState::init()
{
    const float groundY = GetGroundY();

    players.clear();
    players.emplace_back(Player::Team::Left,
        Vector2{ static_cast<float>(GetScreenWidth()) * 0.15f, groundY });
    players.emplace_back(Player::Team::Right,
        Vector2{ static_cast<float>(GetScreenWidth()) * 0.85f, groundY });

    currentPlayerIndex = 0;
    pendingShot = std::nullopt;
    players[currentPlayerIndex].ResetTurn();
}

void MainGameState::ClampPlayerBounds(Player& player, float& minX, float& maxX) const
{
    minX = GameConfig::SCREEN_MARGIN + GameConfig::PLAYER_WIDTH * 0.5f;
    maxX = static_cast<float>(GetScreenWidth()) - GameConfig::SCREEN_MARGIN - GameConfig::PLAYER_WIDTH * 0.5f;

    // Ningun jugador puede cruzar la zona de separacion minima con su enemigo.
    const float minSeparation = static_cast<float>(GetScreenWidth()) * GameConfig::MIN_SEPARATION_RATIO;
    for (const Player& other : players)
    {
        if (&other == &player) continue;

        if (player.GetTeam() == Player::Team::Left)
        {
            const float enemyMinX = other.GetPosition().x - minSeparation;
            if (enemyMinX < maxX) maxX = enemyMinX;
        }
        else
        {
            const float enemyMaxX = other.GetPosition().x + minSeparation;
            if (enemyMaxX > minX) minX = enemyMaxX;
        }
    }

    if (minX > maxX) minX = maxX;
}

void MainGameState::handleInput()
{
    Player& current = players[currentPlayerIndex];

    if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) current.Jump();

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && current.IsAiming())
    {
        pendingShot = current.BuildShot();
        NextTurn();
    }
    else if (IsKeyPressed(KEY_ENTER))
    {
        NextTurn();
    }
}

void MainGameState::update(float deltaTime)
{
    Player& current = players[currentPlayerIndex];

    float direction = 0.0f;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  direction -= 1.0f;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) direction += 1.0f;

    if (direction != 0.0f)
    {
        float minX = 0.0f;
        float maxX = 0.0f;
        ClampPlayerBounds(current, minX, maxX);
        current.Move(direction, deltaTime, minX, maxX);
    }

    current.UpdateAim(GetMousePosition());
    current.Update(deltaTime, GetGroundY());

    for (Player& player : players)
    {
        if (&player != &current) player.Update(deltaTime, GetGroundY());
    }
}

void MainGameState::NextTurn()
{
    currentPlayerIndex = (currentPlayerIndex + 1) % static_cast<int>(players.size());
    players[currentPlayerIndex].ResetTurn();
}

void MainGameState::RenderGround() const
{
    const float groundY = GetGroundY();
    const float grassHeight = GameConfig::GRASS_HEIGHT;

    DrawRectangle(0, static_cast<int>(groundY), GetScreenWidth(),
                  static_cast<int>(GameConfig::GROUND_HEIGHT), BROWN);
    DrawRectangle(0, static_cast<int>(groundY), GetScreenWidth(),
                  static_cast<int>(grassHeight), GREEN);
}

void MainGameState::RenderHud() const
{
    const Player& current = players[currentPlayerIndex];
    const char* teamName = (current.GetTeam() == Player::Team::Left) ? "IZQUIERDA" : "DERECHA";

    DrawText(TextFormat("Turno: equipo %s", teamName), 20, 20, 24, DARKGRAY);
    DrawText(TextFormat("Movimiento restante: %.0f px", current.GetRemainingMove()), 20, 50, 18, GRAY);

    if (pendingShot.has_value())
    {
        const ShotData& shot = *pendingShot;
        DrawText(TextFormat("Cola disparo [feat/proyectile]: origen (%.0f, %.0f), angulo %.0f, potencia %.0f",
                            shot.origin.x, shot.origin.y, shot.angleDeg, shot.power),
                 20, 76, 18, MAROON);
    }

    DrawText("Raton: apuntar | Click: guardar disparo y pasar turno | ENTER: pasar turno",
             20, GetScreenHeight() - 56, 18, GRAY);
    DrawText("A/D o Flechas: mover | W: saltar | ESC: salir",
             20, GetScreenHeight() - 30, 18, DARKGRAY);
}

void MainGameState::render()
{
    BeginDrawing();

        ClearBackground(SKYBLUE);

        RenderGround();

        for (const Player& player : players)
        {
            player.Render();
        }

        const Player& current = players[currentPlayerIndex];
        current.RenderAim();
        current.RenderTrajectory(GetGroundY());

        RenderHud();

    EndDrawing();
}
