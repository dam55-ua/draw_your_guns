#pragma once
#include <GameState.hpp>

class MainGameState;

class PauseState : public GameState
{
    public:
        explicit PauseState(MainGameState& gameState);

        void init() override {}
        void handleInput() override;
        void update(float deltaTime) override;
        void render() override;

        void pause() override {}
        void resume() override {}

    private:
        enum class Confirm { None, Abandon, Quit };

        MainGameState& game;
        int selected = 0;
        Confirm confirm = Confirm::None;
};