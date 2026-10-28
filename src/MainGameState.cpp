#include <MainGameState.hpp>
#include <GameOverState.hpp>
#include <StateMachine.hpp>
#include <raylib.h>
#include <memory>

MainGameState::MainGameState(): players{}, projectile(), explosion(), currentPlayerIndex(0), pendingShot(std::nullopt), 
								map(GetScreenWidth(), GetScreenHeight())
{
}
float MainGameState::GetGroundY() const
{
    return static_cast<float>(GetScreenHeight()) - GameConfig::GROUND_HEIGHT;
}

void MainGameState::init()
{

    // Generamos el mapa
    map.generateTerrain();

    // Spawnean los jugadores
    const std::vector<SpawnBase>& bases = map.getSpawnBases();
    const Rectangle b0 = bases[0].getBase();
    const Rectangle b1 = bases[1].getBase();

    players.clear();
    for(int i = 0; i < GameConfig::MAX_PLAYERS_PER_TEAM; ++i){
        float t = (i + 0.5f) / GameConfig::MAX_PLAYERS_PER_TEAM;

        players.emplace_back(Player::Team::Left,  Vector2{ b0.x + b0.width * t, b0.y });
        players.emplace_back(Player::Team::Right, Vector2{ b1.x + b1.width * t, b1.y });
    }


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
        if (other.GetTeam() == player.GetTeam()) continue;   // la separacion minima solo aplica contra el equipo enemigo

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

            if (!current.HasWallAhead(direction, deltaTime, map))
            {
                current.Move(direction, deltaTime, minX, maxX);
            }
        }

        current.UpdateAim(GetMousePosition());
    }

    for (Player& player : players)
    {
        Vector2 position = player.GetPosition();

        float groundY = static_cast<float>(
            map.getGroundYAt(position.x, position.y)
        );

        player.Update(deltaTime, groundY);
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

        if (map.checkCollisionProjectile(positionNow, radius))
        {
            ResolveImpact(positionNow);
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

    map.destroyTerrainAt(impactPosition, explosion.GetRadius());

    // Vida total de cada equipo antes de la explosion: sirve de criterio de desempate
    // si la misma explosion elimina a los dos equipos a la vez.
    float leftHpBefore = 0.0f;
    float rightHpBefore = 0.0f;
    for (const Player& player : players)
    {
        if (player.GetTeam() == Player::Team::Left){
            leftHpBefore += player.GetHp();
        } 
        else{
            rightHpBefore += player.GetHp(); 
        }
    }

    for (Player& player : players)
    {
        const Rectangle bounds = player.GetBounds();
        const Vector2 center = { bounds.x + bounds.width * 0.5f,
                                 bounds.y + bounds.height * 0.5f };
        const int damage = explosion.CalculateDamage(center, GameConfig::EXPLOSION_MAX_DAMAGE);
        if (damage > 0) player.ApplyDamage(static_cast<float>(damage));
    }

    CheckGameOver(leftHpBefore, rightHpBefore);
}

void MainGameState::CheckGameOver(float leftHpBefore, float rightHpBefore)
{
    bool leftAlive = false;
    bool rightAlive = false;

    for (const Player& player : players)
    {
        if (player.GetHp() <= 0.0f) continue;

        if (player.GetTeam() == Player::Team::Left){
            leftAlive = true;
        }
        else{
            rightAlive = true;
        }
    }

    // La partida continua mientras quede al menos un jugador vivo en cada equipo.
    if (leftAlive && rightAlive)
    {
        NextTurn();
        return;
    }

    // Doble KO: gana el equipo que mas vida tenia antes de la explosion.
    Player::Team winner = Player::Team::Left;
    if (!leftAlive && !rightAlive)
    {
        if(rightHpBefore > leftHpBefore)
        {
            winner = Player::Team::Right;
        }
        else
        {
            winner = Player::Team::Left;
        }
    }
    else
    {
        if(leftAlive)
        {
            winner = Player::Team::Left;
        }
        else
        {
            winner = Player::Team::Right;
        }
    }

    state_machine->add_state(std::make_unique<GameOverState>(winner), true);
}

void MainGameState::NextTurn()
{
    const int count = static_cast<int>(players.size());

    for (int step = 1; step <= count; ++step)
    {
        const int next = (currentPlayerIndex + step) % count;
        if (players[next].GetHp() > 0.0f)
        {
            currentPlayerIndex = next;
            players[currentPlayerIndex].ResetTurn();
            return;
        }
    }
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

    
    const float lastShotY = 76.0f + static_cast<float>(players.size()) * 24.0f + 4.0f;
    if (pendingShot.has_value())
    {
        const ShotData& shot = *pendingShot;
        DrawText(TextFormat("Ultimo disparo [feat/proyectile]: origen (%.0f, %.0f), angulo %.0f, potencia %.0f",
                            shot.origin.x, shot.origin.y, shot.angleDeg, shot.power),
                 20, lastShotY, 18, MAROON);
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

void MainGameState::unloadResources()
{
    map.unloadTerrain();
}
