#include <MainGameState.hpp>
#include "Projectile.hpp"
#include <iostream>

MainGameState::MainGameState()
{
}

void MainGameState::init()
{

}

void MainGameState::handleInput()
{

}

void MainGameState::update(float deltaTime)
{
    if (IsKeyPressed(KEY_SPACE) && !projectile.IsActive()) {
        projectile.Launch({200.0f, 500.0f}, 45.0f, 600.0f);
    }

    projectile.Update(deltaTime);
}

void MainGameState::render()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    projectile.Render();
    EndDrawing();
}