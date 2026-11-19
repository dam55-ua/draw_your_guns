#pragma once
#include <GameState.hpp>

class HowToPlayState : public GameState
{
    public:
        void init() override {}
        void handleInput() override;
        void update(float deltaTime) override;
        void render() override;

        void pause() override {}
        void resume() override {}
};