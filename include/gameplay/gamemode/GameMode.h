#ifndef GAME_MODE_H
#define GAME_MODE_H

class Camera;
class World;

class GameMode
{
public:

    virtual ~GameMode() = default;

    virtual void Enter(Camera* camera) = 0;

    virtual void Exit(Camera* camera) = 0;

    virtual void Update(float dt) = 0;

    virtual bool CanFly() const = 0;
    virtual bool HasGravity() const = 0;
    virtual bool HasFallDamage() const = 0;
};

#endif