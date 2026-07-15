#pragma once

#include <vector>
#include <cstdint>

#include <glm/glm.hpp>

#include "graphics/Mesh.h"


class Chunk;
class World;

class ChunkMesh
{
public:

    ChunkMesh();

    ~ChunkMesh();

    ChunkMesh(const ChunkMesh&) = delete;
    ChunkMesh& operator=(const ChunkMesh&) = delete;

public:

    void Build(
        const Chunk& chunk,
        const World* world = nullptr
    );

    void Destroy();

    void Draw() const;

public:

    bool IsValid() const;

    uint32_t GetIndexCount() const;

private:

    bool IsFaceVisible(
        const Chunk& chunk,
        int x,
        int y,
        int z,
        const World* world
    ) const;

    void AddFace(
        std::vector<Vertex>& vertices,
        std::vector<uint32_t>& indices,
        const glm::vec3& p0,
        const glm::vec3& p1,
        const glm::vec3& p2,
        const glm::vec3& p3,
        const glm::vec2& uvMin,
        const glm::vec2& uvMax,
        const glm::vec3& normal
    );

private:

    Mesh m_Mesh;
};