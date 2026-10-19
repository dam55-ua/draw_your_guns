#pragma once
#include <raylib.h>

namespace GameConfig
{
    // Fisica (unidades por segundo para poder usar delta time)
    const float GRAVITY = 1400.0f;
    const float SHOT_SPEED_SCALE = 4.0f;
    const float MOVE_SPEED = 220.0f;
    const float JUMP_SPEED = 520.0f;

    // Turnos
    const float TURN_MOVE_BUDGET = 140.0f;
    const float MIN_SEPARATION_RATIO = 0.18f;

    // Jugador
    const float PLAYER_WIDTH = 22.0f;
    const float PLAYER_HEIGHT = 32.0f;
    const float PLAYER_MAX_HP = 100.0f;
    const float SCREEN_MARGIN = 10.0f;
    const float MAX_STEP_HEIGHT = 12.0f;

    // Escenario
    const float GROUND_HEIGHT = 48.0f;
    

    // Apuntado y trayectoria
    const float MAX_POWER = 700.0f;
    const float TRAJECTORY_STEP = 0.04f;
    const int   TRAJECTORY_POINTS = 40;
    const int   TRAJECTORY_DOT_RADIUS = 3;

    // Combate
    const float EXPLOSION_MAX_DAMAGE = 30.0f;
}

// Informacion completa de un disparo: es el contrato entre el turno actual
// (Player/MainGameState) y el sistema de proyectiles (feat/proyectile).
struct ShotData
{
    Vector2 origin   = { 0.0f, 0.0f };
    Vector2 velocity = { 0.0f, 0.0f };
    float   angleDeg = 0.0f;
    float   power    = 0.0f;
    float   gravity  = GameConfig::GRAVITY;

    bool IsValid() const { return power > 0.0f; }
};

// Integrador compartido: la trayectoria previsualizada y el proyectil real
// deben usar exactamente esta misma funcion.
inline Vector2 StepShot(const Vector2& position, Vector2& velocity, float dt, float gravity)
{
    velocity.y += gravity * dt;
    return { position.x + velocity.x * dt, position.y + velocity.y * dt };
}
