#pragma once
#include <GameState.hpp>
#include <Player.hpp>

class GameOverState : public GameState
{
    public:

        enum class Result
        {
            LeftWins,
            RightWins,
            Draw
        };

        explicit GameOverState(Result result);
        ~GameOverState() = default;

        void init() override;
        void handleInput() override;
        void update(float deltaTime) override;
        void render() override;

        void pause() override {};
        void resume() override {};

    private:
        Result result;
        float elapsed;
};
