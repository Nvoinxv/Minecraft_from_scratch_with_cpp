#include "world/ChunkMesh.h"

#include "world/Chunk.h"
#include "world/World.h"

ChunkMesh::ChunkMesh()
{
}

ChunkMesh::~ChunkMesh()
{
    Destroy();
}

void ChunkMesh::Build(
    const Chunk& chunk,
    const World* world)
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    /*
        Nantinya:

        1. Loop seluruh block
        2. Cek apakah block kosong
        3. Cek keenam sisi
        4. Jika terlihat
             AddFace(...)
    */

    m_Mesh.Create(vertices, indices);
}

void ChunkMesh::Destroy()
{
    m_Mesh.Destroy();
}

void ChunkMesh::Draw() const
{
    m_Mesh.Draw();
}

bool ChunkMesh::IsValid() const
{
    return m_Mesh.IsValid();
}

uint32_t ChunkMesh::GetIndexCount() const
{
    return m_Mesh.GetIndexCount();
}

bool ChunkMesh::IsFaceVisible(
    const Chunk& chunk,
    int x,
    int y,
    int z,
    const World* world) const
{
    return true;
}

void ChunkMesh::AddFace(
    std::vector<Vertex>& vertices,
    std::vector<uint32_t>& indices,
    const glm::vec3& p0,
    const glm::vec3& p1,
    const glm::vec3& p2,
    const glm::vec3& p3,
    const glm::vec2& uvMin,
    const glm::vec2& uvMax,
    const glm::vec3& normal)
{
    uint32_t start = static_cast<uint32_t>(vertices.size());

    vertices.push_back(
    {
        { p0.x, p0.y, p0.z },
        { uvMin.x, uvMin.y },
        { normal.x, normal.y, normal.z }
    });

    vertices.push_back(
    {
        { p1.x, p1.y, p1.z },
        { uvMax.x, uvMin.y },
        { normal.x, normal.y, normal.z }
    });

    vertices.push_back(
    {
        { p2.x, p2.y, p2.z },
        { uvMax.x, uvMax.y },
        { normal.x, normal.y, normal.z }
    });

    vertices.push_back(
    {
        { p3.x, p3.y, p3.z },
        { uvMin.x, uvMax.y },
        { normal.x, normal.y, normal.z }
    });

    indices.push_back(start + 0);
    indices.push_back(start + 1);
    indices.push_back(start + 2);

    indices.push_back(start + 2);
    indices.push_back(start + 3);
    indices.push_back(start + 0);
}