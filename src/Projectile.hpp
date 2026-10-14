#pragma once

#include "raylib.h"

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

    void Launch(Vector2 origin, float angle, float power);
    void Update(float deltaTime);
    void Render() const;
    void Deactivate();

    float GetRadius() const;
    Vector2 GetPreviousPosition() const;

    bool IsActive() const;
    Vector2 GetPosition() const;
};