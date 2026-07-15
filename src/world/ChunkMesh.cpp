#include "world/ChunkMesh.h"

#include "world/Chunk.h"
#include "world/World.h"
#include "world/Block.h" 

ChunkMesh::ChunkMesh()
{
}

ChunkMesh::~ChunkMesh()
{
    Destroy();
}

void ChunkMesh::Build(const Chunk& chunk, const World* world)
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    int cx = chunk.GetChunkX();
    int cz = chunk.GetChunkZ();

    for (int x = 0; x < Chunk::CHUNK_WIDTH; ++x)
    {
        for (int z = 0; z < Chunk::CHUNK_DEPTH; ++z)
        {
            for (int y = 0; y < Chunk::CHUNK_HEIGHT; ++y)
            {
                uint8_t id = chunk.GetBlock(x, y, z);
                if (id == 0) continue;

                glm::vec3 base(
                    static_cast<float>(cx * Chunk::CHUNK_WIDTH + x),
                    static_cast<float>(y),
                    static_cast<float>(cz * Chunk::CHUNK_DEPTH + z)
                );

                const BlockData& blockDef = BlockRegistry::Get().GetBlock(id);
                glm::vec2 uvMin, uvMax;

                // +Y (atas / Top)
                if (IsFaceVisible(chunk, x, y + 1, z, world))
                {
                    BlockRegistry::Get().GetFaceUVs(blockDef.FaceTextureIndex[(int)BlockFace::Top], uvMin, uvMax);
                    AddFace(vertices, indices,
                        base + glm::vec3(0,1,1), base + glm::vec3(1,1,1),
                        base + glm::vec3(1,1,0), base + glm::vec3(0,1,0),
                        uvMin, uvMax, glm::vec3(0,1,0));
                }
                // -Y (bawah / Bottom)
                if (IsFaceVisible(chunk, x, y - 1, z, world))
                {
                    BlockRegistry::Get().GetFaceUVs(blockDef.FaceTextureIndex[(int)BlockFace::Bottom], uvMin, uvMax);
                    AddFace(vertices, indices,
                        base + glm::vec3(0,0,0), base + glm::vec3(1,0,0),
                        base + glm::vec3(1,0,1), base + glm::vec3(0,0,1),
                        uvMin, uvMax, glm::vec3(0,-1,0));
                }
                // +Z (Front)
                if (IsFaceVisible(chunk, x, y, z + 1, world))
                {
                    BlockRegistry::Get().GetFaceUVs(blockDef.FaceTextureIndex[(int)BlockFace::Front], uvMin, uvMax);
                    AddFace(vertices, indices,
                    base + glm::vec3(0,0,1), base + glm::vec3(1,0,1),
                    base + glm::vec3(1,1,1), base + glm::vec3(0,1,1),
                    uvMin, uvMax, glm::vec3(0,0,1));
                }
                // -Z (Back)
                if (IsFaceVisible(chunk, x, y, z - 1, world))
                {
                    BlockRegistry::Get().GetFaceUVs(blockDef.FaceTextureIndex[(int)BlockFace::Back], uvMin, uvMax);
                    AddFace(vertices, indices,
                    base + glm::vec3(1,0,0), base + glm::vec3(0,0,0),
                    base + glm::vec3(0,1,0), base + glm::vec3(1,1,0),
                    uvMin, uvMax, glm::vec3(0,0,-1));
                }
                // -X (Left)
                if (IsFaceVisible(chunk, x - 1, y, z, world))
                {
                    BlockRegistry::Get().GetFaceUVs(blockDef.FaceTextureIndex[(int)BlockFace::Left], uvMin, uvMax);
                    AddFace(vertices, indices,
                        base + glm::vec3(0,0,0), base + glm::vec3(0,0,1),
                        base + glm::vec3(0,1,1), base + glm::vec3(0,1,0),
                        uvMin, uvMax, glm::vec3(-1,0,0));
                }
                // +X (Right)
                if (IsFaceVisible(chunk, x + 1, y, z, world))
                {
                    BlockRegistry::Get().GetFaceUVs(blockDef.FaceTextureIndex[(int)BlockFace::Right], uvMin, uvMax);
                    AddFace(vertices, indices,
                        base + glm::vec3(1,0,1), base + glm::vec3(1,0,0),
                        base + glm::vec3(1,1,0), base + glm::vec3(1,1,1),
                        uvMin, uvMax, glm::vec3(1,0,0));
                }
            }
        }
    }

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
    int x, int y, int z,
    const World* world) const
{
    // Di luar chunk secara lokal -> tanya World (kalau ada) untuk cek chunk tetangga
    if (x < 0 || x >= Chunk::CHUNK_WIDTH ||
        z < 0 || z >= Chunk::CHUNK_DEPTH)
    {
        if (!world) return true; // tanpa world, asumsikan visible (aman untuk build awal)

        int globalX = chunk.GetChunkX() * Chunk::CHUNK_WIDTH + x;
        int globalZ = chunk.GetChunkZ() * Chunk::CHUNK_DEPTH + z;
        return world->IsBlockTransparent(globalX, y, globalZ);
    }

    if (y < 0) return false;             // dasar dunia, tidak usah gambar bawah bedrock
    if (y >= Chunk::CHUNK_HEIGHT) return true;

    uint8_t neighborID = chunk.GetBlock(x, y, z);
    if (neighborID == 0) return true;

    return BlockRegistry::Get().GetBlock(neighborID).IsTransparent;
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