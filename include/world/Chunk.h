#pragma once

#include <memory>
#include <cstdint>
#include <glm/glm.hpp>

class ChunkMesh;

class Chunk
{
public:

    static constexpr int CHUNK_WIDTH  = 16;
    static constexpr int CHUNK_HEIGHT = 128;
    static constexpr int CHUNK_DEPTH  = 16;

public:

    Chunk();
    ~Chunk();

    Chunk(const Chunk&) = delete;
    Chunk& operator=(const Chunk&) = delete;

public:

    void Initialize(
        int chunkX,
        int chunkZ
    );

    void SetPosition(
        int chunkX,
        int chunkZ
    );

    void SetBlock(
        int x,
        int y,
        int z,
        uint8_t blockID
    );

    uint8_t GetBlock(
        int x,
        int y,
        int z
    ) const;

public:

    int GetChunkX() const;

    int GetChunkZ() const;

    glm::vec3 GetWorldPosition() const;

    ChunkMesh* GetMesh() const;

private:

    uint8_t m_Blocks
        [CHUNK_WIDTH]
        [CHUNK_HEIGHT]
        [CHUNK_DEPTH];

    int m_ChunkX;

    int m_ChunkZ;

    glm::vec3 m_WorldPosition;

    std::unique_ptr<ChunkMesh> m_ChunkMesh;
};