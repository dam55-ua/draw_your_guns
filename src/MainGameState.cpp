#include <MainGameState.hpp>
#include <iostream>


MainGameState::MainGameState(): map(GetScreenWidth(), GetScreenHeight())
{
}

void MainGameState::init()
{
	map.generateTerrain();
}

void MainGameState::handleInput()
{

}

void MainGameState::update(float deltaTime)
{

}

void MainGameState::render()
{
	BeginDrawing();
    ClearBackground(SKYBLUE);
    map.draw();
    EndDrawing();
	
}