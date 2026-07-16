#ifndef SURVIVAL_MODE_H
#define SURVIVAL_MODE_H

#include "GameMode.h"

class Camera;

class SurvivalMode : public GameMode
{
public:

    SurvivalMode();
    ~SurvivalMode() override = default;

    void Enter(Camera* camera) override;
    void Exit(Camera* camera) override;
    void Update(float deltaTime) override;

    bool CanFly() const override;
    bool HasGravity() const override;
    bool HasFallDamage() const override;
    bool HasInfiniteBlocks() const override;
    bool CanInstantBreak() const override;

public:

    void Damage(float amount);
    void Heal(float amount);

    void Respawn();

    bool IsDead() const;

    float GetHealth() const;
    float GetMaxHealth() const;

private:

    float m_Health;
    float m_MaxHealth;

    bool m_IsDead;
};

#endif