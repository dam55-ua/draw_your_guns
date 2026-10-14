#pragma once

#include "raylib.h"

class Projectile {
private:
    Vector2 position;
    Vector2 velocity;

    float radius;
    float gravity;
    bool active;

public:
    Projectile();

    void Launch(Vector2 origin, float angle, float power);
    void Update(float deltaTime);
    void Render() const;

    bool IsActive() const;
    Vector2 GetPosition() const;
};