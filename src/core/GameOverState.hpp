#pragma once
#include <GameState.hpp>
#include <Player.hpp>

class GameOverState : public GameState
{
    public:
        explicit GameOverState(Player::Team winnerTeam);
        ~GameOverState() = default;

        void init() override;
        void handleInput() override;
        void update(float deltaTime) override;
        void render() override;

        void pause() override {};
        void resume() override {};

    private:
        Player::Team winnerTeam;
        float elapsed;
};
