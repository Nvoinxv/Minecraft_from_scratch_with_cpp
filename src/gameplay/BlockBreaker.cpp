#include "gameplay/BlockBreaker.h"
#include "world/Block.h" // Assuming BlockRegistry is here
#include <iostream>
#include <algorithm>

BlockBreaker::BlockBreaker()
    : m_IsBreaking(false)
    , m_TargetX(0)
    , m_TargetY(0)
    , m_TargetZ(0)
    , m_CurrentBreakTime(0.0f)
    , m_TargetBreakTime(0.0f)
{
}

void BlockBreaker::StartBreaking(int x, int y, int z, uint8_t blockID, float toolMultiplier)
{
    // Jika block tidak bisa dihancurkan (misal bedrock), batalkan
    if (IsUnbreakable(blockID)) {
        CancelBreaking();
        return;
    }

    m_IsBreaking = true;
    m_TargetX = x;
    m_TargetY = y;
    m_TargetZ = z;
    m_CurrentBreakTime = 0.0f;
    
    // [SLOT UNTUK UPDATE LOGIC TOOL]
    // Saat ini toolMultiplier selalu 1.0f (tangan kosong).
    // Nanti bisa dimodifikasi agar tool yang tepat memperbesar multiplier.
    float baseHardness = GetBlockHardness(blockID);
    m_TargetBreakTime = baseHardness / toolMultiplier;
}

BreakState BlockBreaker::Update(float deltaTime)
{
    if (!m_IsBreaking)
        return BreakState::Idle;

    m_CurrentBreakTime += deltaTime;

    if (m_CurrentBreakTime >= m_TargetBreakTime)
    {
        return BreakState::Finished;
    }

    return BreakState::InProgress;
}

void BlockBreaker::CancelBreaking()
{
    m_IsBreaking = false;
    m_CurrentBreakTime = 0.0f;
}

float BlockBreaker::GetBreakProgress() const
{
    if (!m_IsBreaking || m_TargetBreakTime <= 0.0f)
        return 0.0f;

    return std::clamp(m_CurrentBreakTime / m_TargetBreakTime, 0.0f, 1.0f);
}

bool BlockBreaker::GetTargetBlock(int& outX, int& outY, int& outZ) const
{
    if (!m_IsBreaking)
        return false;
        
    outX = m_TargetX;
    outY = m_TargetY;
    outZ = m_TargetZ;
    return true;
}

bool BlockBreaker::IsUnbreakable(uint8_t blockID) const
{
    uint8_t bedrockID = BlockRegistry::Get().GetBlockByName("bedrock").ID;
    return (blockID == bedrockID);
}

float BlockBreaker::GetBlockHardness(uint8_t blockID) const
{
    // Caching ID (idealnya ini dilakukan sekali saat inisialisasi, 
    // tapi karena BlockRegistry singleton, ini cukup efisien)
    const auto& reg = BlockRegistry::Get();
    uint8_t dirtID   = reg.GetBlockByName("dirt").ID;
    uint8_t stoneID  = reg.GetBlockByName("stone").ID;
    uint8_t cobbleID = reg.GetBlockByName("cobblestone").ID;
    uint8_t woodID   = reg.GetBlockByName("wood").ID;
    uint8_t leavesID = reg.GetBlockByName("leafs").ID; // Sesuai file leafs.json yang ada
    uint8_t chestID  = reg.GetBlockByName("chest").ID;
    uint8_t torchID  = reg.GetBlockByName("torch").ID;

    if (blockID == dirtID)   return 0.75f;
    if (blockID == stoneID)  return 7.5f;
    if (blockID == cobbleID) return 7.5f;
    if (blockID == woodID)   return 3.0f;
    if (blockID == leavesID) return 0.3f;
    if (blockID == chestID)  return 2.5f;
    if (blockID == torchID)  return 0.0f;

    // Default untuk block yang tidak terdaftar
    return 1.0f;
}
