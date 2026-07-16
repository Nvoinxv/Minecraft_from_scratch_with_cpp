#include "gameplay/SurvivalMode.h"
#include "graphics/Camera.h"

SurvivalMode::SurvivalMode()
    : m_Health(20.0f),
      m_MaxHealth(20.0f),
      m_IsDead(false)
{
}

void SurvivalMode::Enter(Camera* camera)
{
    if (camera == nullptr)
        return;

    camera->IsCreative = false;
}

void SurvivalMode::Exit(Camera* camera)
{
    (void)camera;
}

void SurvivalMode::Update(float deltaTime)
{
    (void)deltaTime;

    // Future:
    // - Fall damage
    // - Drowning
    // - Fire damage
    // - Poison
}

bool SurvivalMode::CanFly() const
{
    return false;
}

bool SurvivalMode::HasGravity() const
{
    return true;
}

bool SurvivalMode::HasFallDamage() const
{
    return true;
}

bool SurvivalMode::HasInfiniteBlocks() const
{
    return false;
}

bool SurvivalMode::CanInstantBreak() const
{
    return false;
}

void SurvivalMode::Damage(float amount)
{
    if (m_IsDead)
        return;

    m_Health -= amount;

    if (m_Health <= 0.0f)
    {
        m_Health = 0.0f;
        m_IsDead = true;
    }
}

void SurvivalMode::Heal(float amount)
{
    if (m_IsDead)
        return;

    m_Health += amount;

    if (m_Health > m_MaxHealth)
        m_Health = m_MaxHealth;
}

void SurvivalMode::Respawn()
{
    m_IsDead = false;
    m_Health = m_MaxHealth;
}

bool SurvivalMode::IsDead() const
{
    return m_IsDead;
}

float SurvivalMode::GetHealth() const
{
    return m_Health;
}

float SurvivalMode::GetMaxHealth() const
{
    return m_MaxHealth;
}