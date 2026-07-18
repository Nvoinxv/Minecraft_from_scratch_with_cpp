#pragma once

#include <cstdint>

enum class BreakState
{
    Idle,
    InProgress,
    Finished
};

class BlockBreaker
{
public:
    BlockBreaker();

    // Memulai proses penghancuran pada koordinat tertentu
    void StartBreaking(int x, int y, int z, uint8_t blockID, float toolMultiplier = 1.0f);

    // Update progress tiap frame. Mengembalikan status proses saat ini.
    BreakState Update(float deltaTime);

    // Membatalkan proses (misal: player melepas klik kiri atau target di luar jangkauan)
    void CancelBreaking();

    // Mengembalikan progress 0.0f - 1.0f
    float GetBreakProgress() const;

    // Mendapatkan koordinat block yang sedang dihancurkan
    bool GetTargetBlock(int& outX, int& outY, int& outZ) const;

    // Cek apakah block tidak bisa dihancurkan (bedrock)
    bool IsUnbreakable(uint8_t blockID) const;

private:
    float GetBlockHardness(uint8_t blockID) const;

private:
    bool m_IsBreaking;
    int m_TargetX;
    int m_TargetY;
    int m_TargetZ;
    float m_CurrentBreakTime;
    float m_TargetBreakTime;
};
