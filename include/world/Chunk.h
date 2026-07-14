#pragma once

#include <cstdint>
#include <vector>
#include <glm/glm.hpp>
#include "graphics/Mesh.h"

class World; // Forward declaration

class Chunk
{
public:
    static constexpr int CHUNK_WIDTH = 16;
    static constexpr int CHUNK_HEIGHT = 64;
    static constexpr int CHUNK_DEPTH = 16;

public:
    Chunk();
    ~Chunk();

    Chunk(const Chunk&) = delete;
    Chunk& operator=(const Chunk&) = delete;

    void Initialize(int chunkX, int chunkZ);
    void SetPosition(int chunkX, int chunkZ);

    void SetBlock(int x, int y, int z, uint8_t blockID);
    uint8_t GetBlock(int x, int y, int z) const;

    void GenerateMesh(const World* world = nullptr);
    void Draw() const;

    int GetChunkX() const { return m_ChunkX; }
    int GetChunkZ() const { return m_ChunkZ; }
    glm::vec3 GetWorldPosition() const { return m_WorldPosition; }
    uint32_t GetIndexCount() const { return m_Mesh.GetIndexCount(); }

private:
    bool IsTransparentOrAir(int x, int y, int z, const World* world) const;
    void AddFace(
        std::vector<Vertex>& vertices,
        std::vector<uint32_t>& indices,
        const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3,
        const glm::vec2& uvMin, const glm::vec2& uvMax,
        const glm::vec3& normal
    );

private:
    uint8_t m_Blocks[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_DEPTH];
    int m_ChunkX;
    int m_ChunkZ;
    glm::vec3 m_WorldPosition;
    Mesh m_Mesh;
};
