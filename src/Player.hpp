#pragma once
#include <GameTypes.hpp>

class Player
{
    public:
        enum class Team { Left, Right };

        Player(Team team, Vector2 feetPosition);

        void ResetTurn();
        void Move(float direction, float dt, float minX, float maxX);
        void Jump();
        void Update(float dt, float groundY);
        void UpdateAim(Vector2 mousePosition);
        ShotData BuildShot() const;

        void Render() const;
        void RenderAim() const;
        void RenderTrajectory(float groundY) const;

        Team GetTeam() const { return team; }
        Vector2 GetPosition() const { return position; }
        Rectangle GetBounds() const;
        float GetHp() const { return hp; }
        void ApplyDamage(float amount);
        float GetRemainingMove() const { return turnMoveBudget; }
        float GetAimAngle() const { return aimAngleDeg; }
        float GetAimPower() const { return aimPower; }
        bool IsAiming() const { return aimPower > 0.0f; }

    private:
        Vector2 GetMuzzle() const;
        bool IsValidAim(Vector2 mousePosition) const;

        Team team;
        Vector2 position;      // pies del personaje
        Vector2 aimPoint;
        float velocityY;
        float aimAngleDeg;
        float aimPower;
        float turnMoveBudget;
        float hp;
        bool grounded;
};
