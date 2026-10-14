#pragma once
#include <GameState.hpp>
#include <Projectile.hpp>

class MainGameState : public GameState
{
    public:
        MainGameState();
        Projectile projectile;
        ~MainGameState() = default;

        void init() override;
        void handleInput() override;
        void update(float deltaTime) override;
        void render() override;

        void pause(){};
        void resume(){};

    
    private:
        char entered_key;
};