#pragma once
#include <GameState.hpp>
#include <array>
#include <vector>
#include "map/Map.hpp"

class MainGameState : public GameState
{
    public:
        MainGameState();
        ~MainGameState() = default;

        void init() override;
        void handleInput() override;
        void update(float deltaTime) override;
        void render() override;

        void pause(){};
        void resume(){};

    
    private:
        // Cada tramo ocupa todo el espacio desde su superficie hasta el fondo de la ventana.
        Map map;  

        char entered_key;
};