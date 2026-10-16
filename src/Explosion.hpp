#pragma once

#include "raylib.h"

class Explosion {
private:
    Vector2 position;
    float radius;
    float duration;
    float elapsedTime;
    bool active;

public:
    Explosion();

    void Activate(Vector2 impactPosition);
    void Update(float deltaTime);
    void Render() const;

    bool Contains(Vector2 point) const;

    //Calculate damage based on distance from explosion
    int CalculateDamage(Vector2 targetPosition, int maxDamage) const;

    bool IsActive() const;
    float GetRadius() const;
};

