#include "world/Chunk.h"

#include <cstring>

Chunk::Chunk()
    :
    m_ChunkX(0),
    m_ChunkZ(0),
    m_WorldPosition(0.0f)
{
    std::memset(
        m_Blocks,
        0,
        sizeof(m_Blocks));
}

Chunk::~Chunk()
{
}

void Chunk::Initialize(
    int chunkX,
    int chunkZ)
{
    SetPosition(
        chunkX,
        chunkZ);

    std::memset(
        m_Blocks,
        0,
        sizeof(m_Blocks));
}

void Chunk::SetPosition(
    int chunkX,
    int chunkZ)
{
    m_ChunkX = chunkX;
    m_ChunkZ = chunkZ;

    m_WorldPosition =
        glm::vec3(
            chunkX * CHUNK_WIDTH,
            0.0f,
            chunkZ * CHUNK_DEPTH);
}

void Chunk::SetBlock(
    int x,
    int y,
    int z,
    uint8_t blockID)
{
    if (
        x >= 0 &&
        x < CHUNK_WIDTH &&
        y >= 0 &&
        y < CHUNK_HEIGHT &&
        z >= 0 &&
        z < CHUNK_DEPTH)
    {
        m_Blocks[x][y][z] = blockID;
    }
}

uint8_t Chunk::GetBlock(
    int x,
    int y,
    int z) const
{
    if (
        x >= 0 &&
        x < CHUNK_WIDTH &&
        y >= 0 &&
        y < CHUNK_HEIGHT &&
        z >= 0 &&
        z < CHUNK_DEPTH)
    {
        return m_Blocks[x][y][z];
    }

    return 0;
}

int Chunk::GetChunkX() const
{
    return m_ChunkX;
}

int Chunk::GetChunkZ() const
{
    return m_ChunkZ;
}

glm::vec3 Chunk::GetWorldPosition() const
{
    return m_WorldPosition;
}