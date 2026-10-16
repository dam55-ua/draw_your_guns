#pragma once

#include "raylib.h"
#include "GameTypes.hpp"

class Projectile {
private:
    Vector2 position;
    Vector2 velocity;
    Vector2 previousPosition;

    float radius;
    float gravity;
    bool active;

public:
    Projectile();

    // Contrato con el turno del jugador: usa exactamente la fisica de GameTypes
    // (StepShot) para que la trayectoria previsualizada y la bala real coincidan.
    void Launch(const ShotData& shot);
    void Launch(Vector2 origin, float angle, float power);
    void Update(float deltaTime);
    void Render() const;
    void Deactivate();

    float GetRadius() const;
    Vector2 GetPreviousPosition() const;

    bool IsActive() const;
    Vector2 GetPosition() const;
};