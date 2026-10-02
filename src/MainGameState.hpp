#pragma once
#include <GameState.hpp>
#include <array>
#include <vector>

class MainGameState : public GameState
{
    public:
        MainGameState();
        ~MainGameState() = default;

        void init() override;
        void handleInput() override;
        void update(float deltaTime) override;
        void render() override;

        Vector2 getPlayerSpawnPosition(int playerIndex) const;

        void pause(){};
        void resume(){};

    
    private:
        // Cada tramo ocupa todo el espacio desde su superficie hasta el fondo de la ventana.
        struct TerrainSegment
        {
            Rectangle rectangle;
        };

        struct SpawnBase
        {
            Rectangle platform;
            Vector2 spawnPosition;
        };

        std::vector<TerrainSegment> terrain;
        std::array<SpawnBase, 2> spawnBases;

        void generateTerrain();

        char entered_key;
};