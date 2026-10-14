#pragma once
#include <GameState.hpp>
#include <Projectile.hpp>
#include "Explosion.hpp"

class MainGameState : public GameState
{
    public:
        MainGameState();
        Projectile projectile;
        Explosion explosion;

        float temporaryGroundY = 550.0f;

        Vector2 dummyTargetPosition = {700.0f, 530.0f};
        int dummyTargetHealth = 100;
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