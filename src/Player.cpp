#include <Player.hpp>
#include <cmath>
#include "map/Map.hpp" 


namespace
{
    Vector2 Normalize(Vector2 v)
    {
        const float length = std::sqrt(v.x * v.x + v.y * v.y);
        if (length <= 0.0001f) return { 0.0f, 0.0f };
        return { v.x / length, v.y / length };
    }
}

Player::Player(Team team, Vector2 feetPosition)
    : team(team),
      position(feetPosition),
      aimPoint{ 0.0f, 0.0f },
      velocityY(0.0f),
      aimAngleDeg(0.0f),
      aimPower(0.0f),
      turnMoveBudget(0.0f),
      hp(GameConfig::PLAYER_MAX_HP),
      grounded(true)
{
}

void Player::ResetTurn()
{
    turnMoveBudget = GameConfig::TURN_MOVE_BUDGET;
    aimAngleDeg = 0.0f;
    aimPower = 0.0f;
    aimPoint = { 0.0f, 0.0f };
}

void Player::Move(float direction, float dt, float minX, float maxX)
{
    if (direction == 0.0f || turnMoveBudget <= 0.0f) return;

    float step = direction * GameConfig::MOVE_SPEED * dt;
    if (std::fabs(step) > turnMoveBudget) step = (step > 0.0f) ? turnMoveBudget : -turnMoveBudget;

    float newX = position.x + step;
    if (newX < minX) newX = minX;
    if (newX > maxX) newX = maxX;

    turnMoveBudget -= std::fabs(newX - position.x);
    position.x = newX;
}

void Player::Jump()
{
    if (!grounded) return;
    velocityY = -GameConfig::JUMP_SPEED;
    grounded = false;
}

void Player::Update(float dt, float groundY)
{

    if (grounded)
    {
        if (groundY > position.y + 1.0f)
        {
            grounded = false;      // ya no hay suelo debajo
        }
        else
        {
            position.y = groundY;  // está andando sobre superficie
        }
    }

    if (grounded) return;

    velocityY += GameConfig::GRAVITY * dt;
    position.y += velocityY * dt;

    if (position.y >= groundY)
    {
        position.y = groundY;
        velocityY = 0.0f;
        grounded = true;
    }
}

bool Player::IsValidAim(Vector2 mousePosition) const
{
    const Vector2 muzzle = GetMuzzle();
    const bool inFront = (team == Team::Left) ? (mousePosition.x >= muzzle.x)
                                              : (mousePosition.x <= muzzle.x);
    const bool above = mousePosition.y <= muzzle.y;
    return inFront && above;
}

void Player::UpdateAim(Vector2 mousePosition)
{
    if (!IsValidAim(mousePosition))
    {
        aimPower = 0.0f;
        aimAngleDeg = 0.0f;
        return;
    }

    const Vector2 muzzle = GetMuzzle();
    const Vector2 toMouse = { mousePosition.x - muzzle.x, mousePosition.y - muzzle.y };
    const float distance = std::sqrt(toMouse.x * toMouse.x + toMouse.y * toMouse.y);
    if (distance < 1.0f)
    {
        aimPower = 0.0f;
        return;
    }

    aimPower = (distance > GameConfig::MAX_POWER) ? GameConfig::MAX_POWER : distance;
    aimPoint = mousePosition;

    const Vector2 direction = Normalize(toMouse);
    aimAngleDeg = std::atan2(-direction.y, direction.x) * RAD2DEG;
    if (aimAngleDeg < 0.0f) aimAngleDeg += 360.0f;
}

ShotData Player::BuildShot() const
{
    ShotData shot;
    if (!IsAiming()) return shot;

    const Vector2 muzzle = GetMuzzle();
    const Vector2 toMouse = { aimPoint.x - muzzle.x, aimPoint.y - muzzle.y };
    const Vector2 direction = Normalize(toMouse);

    shot.origin = muzzle;
    shot.velocity = { direction.x * aimPower * GameConfig::SHOT_SPEED_SCALE,
                      direction.y * aimPower * GameConfig::SHOT_SPEED_SCALE };
    shot.angleDeg = aimAngleDeg;
    shot.power = aimPower;
    return shot;
}

bool Player::HasWallAhead(float direction, float deltaTime, const Map& map) const
{
    const Rectangle bounds = GetBounds();
    const float feetY = bounds.y + bounds.height;               // los pies
    const float limitY = feetY - GameConfig::MAX_STEP_HEIGHT;   // altura máxima que puedo subir sin saltar

    // El sonda cubre justo el próximo paso, para no meternos dentro de la pared.
    const float probeWidth = GameConfig::MOVE_SPEED * deltaTime + 2.0f;

    // Banda fina de 1 px: si el terreno la cruza, es que es más alto que un escalón.
    Rectangle band = { 0.0f, limitY, probeWidth, 1.0f };
    band.x = (direction > 0.0f) ? bounds.x + bounds.width : bounds.x - probeWidth;

    return map.checkCollision(band);
}


Vector2 Player::GetMuzzle() const
{
    return { position.x, position.y - GameConfig::PLAYER_HEIGHT };
}

Rectangle Player::GetBounds() const
{
    return { position.x - GameConfig::PLAYER_WIDTH * 0.5f,
             position.y - GameConfig::PLAYER_HEIGHT,
             GameConfig::PLAYER_WIDTH,
             GameConfig::PLAYER_HEIGHT };
}

void Player::ApplyDamage(float amount)
{
    hp -= amount;
    if (hp < 0.0f) hp = 0.0f;
}

void Player::Render() const
{
    const Rectangle bounds = GetBounds();
    const Color bodyColor = (team == Team::Left) ? BLUE : RED;

    DrawRectangleRec(bounds, bodyColor);
    DrawRectangleLinesEx(bounds, 2.0f, Fade(WHITE, 0.6f));

    DrawCircle(static_cast<int>(position.x), static_cast<int>(bounds.y - 10.0f), 10.0f, SKYBLUE);
    DrawCircleLines(static_cast<int>(position.x), static_cast<int>(bounds.y - 10.0f), 10.0f, Fade(WHITE, 0.6f));

    const Rectangle hpBack = { bounds.x, bounds.y - 26.0f, bounds.width, 5.0f };
    const Rectangle hpFill = { bounds.x, bounds.y - 26.0f, bounds.width * (hp / GameConfig::PLAYER_MAX_HP), 5.0f };
    DrawRectangleRec(hpBack, Fade(BLACK, 0.5f));
    DrawRectangleRec(hpFill, GREEN);
}

void Player::RenderAim() const
{
    if (!IsAiming()) return;

    const Vector2 muzzle = GetMuzzle();
    const Color color = (team == Team::Left) ? BLUE : RED;

    DrawLineEx(muzzle, aimPoint, 2.0f, Fade(color, 0.7f));
    DrawCircleV(aimPoint, 5.0f, Fade(color, 0.9f));
    DrawCircleV(muzzle, 4.0f, YELLOW);

    const char* label = TextFormat("%.0f deg | power %.0f", aimAngleDeg, aimPower);
    DrawText(label, static_cast<int>(aimPoint.x) + 10, static_cast<int>(aimPoint.y) - 10, 18, DARKGRAY);
}

void Player::RenderTrajectory(float groundY) const
{
    if (!IsAiming()) return;

    const ShotData shot = BuildShot();
    Vector2 positionStep = shot.origin;
    Vector2 velocityStep = shot.velocity;

    for (int i = 0; i < GameConfig::TRAJECTORY_POINTS; ++i)
    {
        positionStep = StepShot(positionStep, velocityStep, GameConfig::TRAJECTORY_STEP, shot.gravity);

        if (positionStep.y > groundY || positionStep.x < 0.0f ||
            positionStep.x > static_cast<float>(GetScreenWidth()))
        {
            break;
        }

        const float fade = 1.0f - static_cast<float>(i) / static_cast<float>(GameConfig::TRAJECTORY_POINTS);
        DrawCircleV(positionStep, static_cast<float>(GameConfig::TRAJECTORY_DOT_RADIUS),
                    Fade(YELLOW, 0.3f + 0.5f * fade));
    }
}
