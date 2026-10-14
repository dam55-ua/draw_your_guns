#include "Projectile.hpp"

#include <cmath>

Projectile::Projectile()
    : position{0.0f, 0.0f},
      velocity{0.0f, 0.0f},
      radius(8.0f),
      gravity(500.0f),
      active(false)
{
}

void Projectile::Launch(Vector2 origin, float angle, float power)
{
    position = origin;

    float angleRadians = angle * DEG2RAD;

    velocity.x = std::cos(angleRadians) * power;
    velocity.y = -std::sin(angleRadians) * power;

    active = true;
}

void Projectile::Update(float deltaTime)
{
    if (!active) {
        return;
    }

    velocity.y += gravity * deltaTime;

    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
}

void Projectile::Render() const
{
    if (!active) {
        return;
    }

    DrawCircleV(position, radius, RED);
}

bool Projectile::IsActive() const
{
    return active;
}

Vector2 Projectile::GetPosition() const
{
    return position;
}