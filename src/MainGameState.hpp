#pragma once
#include <GameState.hpp>
#include <Player.hpp>
#include <Projectile.hpp>
#include <Explosion.hpp>
#include <vector>
#include <optional>
#include <array>
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

        void pause() override {};
        void resume() override {};

    private:
        float GetGroundY() const;
        void NextTurn();
        void ResolveImpact(Vector2 impactPosition);
        void CheckGameOver();
        bool CheckPlayerImpact(Vector2 impactPosition) const;
        void ClampPlayerBounds(Player& player, float& minX, float& maxX) const;
        bool HasWallAhead(const Player& player, float direction, float deltaTime) const;
        void RenderHud() const;

        std::vector<Player> players;
        Projectile projectile;
        Explosion explosion;
        int currentPlayerIndex;
        std::optional<ShotData> pendingShot;
        // Cada tramo ocupa todo el espacio desde su superficie hasta el fondo de la ventana.
        Map map;  

        char entered_key;
};
