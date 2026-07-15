#ifndef GAME_MODE_H
#define GAME_MODE_H

class Camera;
class World;

class GameMode
{
public:

    virtual ~GameMode() = default;

    // Dipanggil ketika mode mulai digunakan.
    virtual void Enter(Camera* camera, World* world) = 0;

    // Dipanggil ketika mode diganti.
    virtual void Exit(Camera* camera, World* world) = 0;

    // Dipanggil setiap frame.
    virtual void Update(float deltaTime) = 0;

    // Capability
    virtual bool CanFly() const = 0;
    virtual bool HasGravity() const = 0;
    virtual bool HasFallDamage() const = 0;
    virtual bool HasInfiniteBlocks() const = 0;
    virtual bool CanInstantBreak() const = 0;
};

#endif