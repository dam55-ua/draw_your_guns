#include "Explosion.hpp"

Explosion::Explosion()
    : position{0.0f, 0.0f},
      radius(50.0f),
      duration(0.5f),
      elapsedTime(0.0f),
      active(false)
{
}

void Explosion::Activate(Vector2 impactPosition)
{
    position = impactPosition;
    elapsedTime = 0.0f;
    active = true;
}

void Explosion::Update(float deltaTime)
{
    if (!active) {
        return;
    }

    elapsedTime += deltaTime;

    if (elapsedTime >= duration) {
        active = false;
    }
}

void Explosion::Render() const
{
    if (!active) {
        return;
    }

    DrawCircleV(position, radius, Fade(ORANGE, 0.5f));
    DrawCircleLines(
        static_cast<int>(position.x),
        static_cast<int>(position.y),
        radius,
        RED
    );
}

bool Explosion::Contains(Vector2 point) const
{
    if (!active) {
        return false;
    }

    return CheckCollisionPointCircle(point, position, radius);
}

bool Explosion::IsActive() const
{
    return active;
}

float Explosion::GetRadius() const
{
    return radius;
}