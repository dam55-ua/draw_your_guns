#pragma once
#include <GameState.hpp>
#include <Player.hpp>
#include <vector>
#include <optional>

class MainGameState : public GameState
{
    public:
        MainGameState();
        ~MainGameState() = default;

        void init() override;
        void handleInput() override;
        void update(float deltaTime) override;
        void render() override;

        void pause() override {};
        void resume() override {};

    private:
        float GetGroundY() const;
        void NextTurn();
        void ClampPlayerBounds(Player& player, float& minX, float& maxX) const;
        void RenderGround() const;
        void RenderHud() const;

        std::vector<Player> players;
        int currentPlayerIndex;
        std::optional<ShotData> pendingShot;
};
