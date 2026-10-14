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
        Vector2 position = projectile.GetPosition();

        if (position.y >= temporaryGroundY) {
            Vector2 impactPosition = {
                position.x,
                temporaryGroundY
            };

            projectile.Deactivate();
            explosion.Activate(impactPosition);

            // Apply damage once when the explosion starts
            if (explosion.Contains(dummyTargetPosition)) {
                dummyTargetHealth -= 30;

                if (dummyTargetHealth < 0) {
                    dummyTargetHealth = 0;
                }
            }
        }

        // Deactivate if projectile leaves screen
        else if (position.x < 0.0f ||
                 position.x > GetScreenWidth() ||
                 position.y > GetScreenHeight()) {
            projectile.Deactivate();
        }
    }

    // Update explosion lifetime
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