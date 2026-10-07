#pragma once
#include <MainGameState.hpp>

#define GRAVITY_PLAYER 6.8f
// Variables for player position
#define MIN_PLAYER_POSITION 5
#define MAX_PLAYER_POSITION 20

class Player
{
    public:
        Player();
        ~Player() = default;

        void InitPlayers(int maxPlayers);
        bool UpdatePlayer(int playerTurn);
    
    private:
        Vector2 position;
        Vector2 size;
        float heal_points;

        bool isLeftTeam;
        bool isPlayer;
        bool isAlive;
};
