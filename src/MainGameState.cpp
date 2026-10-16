#include <MainGameState.hpp>
#include <raylib.h>


MainGameState::MainGameState()
    : players{}, projectile(), explosion(), currentPlayerIndex(0), pendingShot(std::nullopt),map(GetScreenWidth(), GetScreenHeight())
{
}

float MainGameState::GetGroundY() const
{
    return static_cast<float>(GetScreenHeight()) - GameConfig::GROUND_HEIGHT;
}

void MainGameState::init()
{
	map.generateTerrain();
    const float groundY = GetGroundY();

    players.clear();
    players.emplace_back(Player::Team::Left,
        Vector2{ static_cast<float>(GetScreenWidth()) * 0.15f, groundY });
    players.emplace_back(Player::Team::Right,
        Vector2{ static_cast<float>(GetScreenWidth()) * 0.85f, groundY });

    projectile = Projectile();
    explosion = Explosion();

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
    if (projectile.IsActive()) return;

    Player& current = players[currentPlayerIndex];

    if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) current.Jump();

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && current.IsAiming())
    {
        pendingShot = current.BuildShot();
        projectile.Launch(*pendingShot);
    }
    else if (IsKeyPressed(KEY_ENTER))
    {
        NextTurn();
    }
}

void MainGameState::update(float deltaTime)
{
    Player& current = players[currentPlayerIndex];

    if (!projectile.IsActive())
    {
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
    }

    for (Player& player : players)
    {
        player.Update(deltaTime, GetGroundY());
    }

    projectile.Update(deltaTime);

    if (projectile.IsActive())
    {
        Vector2 positionNow = projectile.GetPosition();
        Vector2 previous = projectile.GetPreviousPosition();
        const float radius = projectile.GetRadius();
        const float groundY = GetGroundY();

        // Interpolacion del punto de impacto contra el suelo (portada de feat/proyectile)
        const float previousBottom = previous.y + radius;
        const float currentBottom = positionNow.y + radius;

        if (previousBottom < groundY && currentBottom >= groundY)
        {
            const float t = (groundY - previousBottom) / (currentBottom - previousBottom);
            const float impactX = previous.x + (positionNow.x - previous.x) * t;
            ResolveImpact({ impactX, groundY });
        }
        else if (CheckPlayerImpact(positionNow))
        {
            ResolveImpact(positionNow);
        }
        else if (positionNow.x < -radius ||
                 positionNow.x > GetScreenWidth() + radius ||
                 positionNow.y > GetScreenHeight() + radius)
        {
            projectile.Deactivate();
            NextTurn();
        }
    }

    explosion.Update(deltaTime);
}

bool MainGameState::CheckPlayerImpact(Vector2 impactPosition) const
{
    const float radius = projectile.GetRadius();

    for (int i = 0; i < static_cast<int>(players.size()); ++i)
    {
        // El proyectil no impacta contra el jugador que lo ha disparado.
        if (i == currentPlayerIndex) continue;

        if (CheckCollisionCircleRec(impactPosition, radius, players[i].GetBounds()))
        {
            return true;
        }
    }

    return false;
}

void MainGameState::ResolveImpact(Vector2 impactPosition)
{
    projectile.Deactivate();
    explosion.Activate(impactPosition);

    for (Player& player : players)
    {
        const Rectangle bounds = player.GetBounds();
        const Vector2 center = { bounds.x + bounds.width * 0.5f,
                                 bounds.y + bounds.height * 0.5f };
        const int damage = explosion.CalculateDamage(center, GameConfig::EXPLOSION_MAX_DAMAGE);
        if (damage > 0) player.ApplyDamage(static_cast<float>(damage));
    }

    NextTurn();
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

    for (int i = 0; i < static_cast<int>(players.size()); ++i)
    {
        const char* name = (players[i].GetTeam() == Player::Team::Left) ? "IZQ" : "DER";
        const Color color = (players[i].GetTeam() == Player::Team::Left) ? BLUE : RED;
        DrawText(TextFormat("%s: %.0f HP", name, players[i].GetHp()), 20, 76 + i * 24, 18, color);
    }

    if (pendingShot.has_value())
    {
        const ShotData& shot = *pendingShot;
        DrawText(TextFormat("Ultimo disparo [feat/proyectile]: origen (%.0f, %.0f), angulo %.0f, potencia %.0f",
                            shot.origin.x, shot.origin.y, shot.angleDeg, shot.power),
                 20, 128, 18, MAROON);
    }

    DrawText("Raton: apuntar | Click: disparar | ENTER: pasar turno",
             20, GetScreenHeight() - 56, 18, GRAY);
    DrawText("A/D o Flechas: mover | W: saltar | ESC: salir",
             20, GetScreenHeight() - 30, 18, DARKGRAY);
}

void MainGameState::render()
{
    BeginDrawing();

       ClearBackground(SKYBLUE);
       map.draw();

        for (const Player& player : players)
        {
            player.Render();
        }

        if (!projectile.IsActive())
        {
            const Player& current = players[currentPlayerIndex];
            current.RenderAim();
            current.RenderTrajectory(GetGroundY());
        }

        projectile.Render();
        explosion.Render();

        RenderHud();

    EndDrawing();
}
