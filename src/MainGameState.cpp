#include <MainGameState.hpp>
#include "Projectile.hpp"
#include <iostream>
#include <cmath>

MainGameState::MainGameState()
{
}

void MainGameState::init()
{

}

void MainGameState::handleInput()
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
        && !projectile.IsActive())
    {
        Vector2 origin = {200.0f, 500.0f};
        Vector2 mouse = GetMousePosition();

        float dx = mouse.x - origin.x;
        float dy = origin.y - mouse.y;

        float angle = std::atan2(dy, dx) * RAD2DEG;
        float power = std::sqrt(dx * dx + dy * dy);

        if (power > 600.0f) {
            power = 600.0f;
        }

        if (power > 0.0f) {
            projectile.Launch(origin, angle, power);
        }
    }
}

void MainGameState::update(float deltaTime)
{
    // Update projectile physics
    projectile.Update(deltaTime);

    // Check collision with temporary floor
if (projectile.IsActive()) {
    Vector2 current = projectile.GetPosition();
    Vector2 previous = projectile.GetPreviousPosition();
    float radius = projectile.GetRadius();

    float previousBottom = previous.y + radius;
    float currentBottom = current.y + radius;

    if (previousBottom < temporaryGroundY &&
        currentBottom >= temporaryGroundY) {

        float denominator = currentBottom - previousBottom;

        float t = (temporaryGroundY - previousBottom) / denominator;

        float impactX = previous.x +
                        (current.x - previous.x) * t;

        Vector2 impactPosition = {
            impactX,
            temporaryGroundY
        };

        projectile.Deactivate();
        explosion.Activate(impactPosition);

        int damage = explosion.CalculateDamage(
            dummyTargetPosition,
            30
        );

        dummyTargetHealth -= damage;

        if (dummyTargetHealth < 0) {
            dummyTargetHealth = 0;
        }

    }
    else if (current.x < -radius ||
             current.x > GetScreenWidth() + radius ||
             current.y > GetScreenHeight() + radius) {
        projectile.Deactivate();
    }
}

explosion.Update(deltaTime);
}

void MainGameState::render()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Temporary floor
    DrawLine(
        0,
        static_cast<int>(temporaryGroundY),
        GetScreenWidth(),
        static_cast<int>(temporaryGroundY),
        DARKGREEN
    );


    // Dummy target
    DrawCircleV(dummyTargetPosition, 12.0f, BLUE);

    // Display dummy target health
    DrawText(
        TextFormat("Target HP: %d", dummyTargetHealth),
        20, 20, 24, DARKGRAY
    );

    // Render game objects
    projectile.Render();
    explosion.Render();
    EndDrawing();
}