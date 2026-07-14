#include "world/Chunk.h"
#include "world/Block.h"
#include "world/World.h"
#include <cstring>

Chunk::Chunk()
    : m_ChunkX(0), m_ChunkZ(0), m_WorldPosition(0.0f)
{
    std::memset(m_Blocks, 0, sizeof(m_Blocks));
}

Chunk::~Chunk()
{
    m_Mesh.Destroy();
}

void Chunk::Initialize(int chunkX, int chunkZ)
{
    SetPosition(chunkX, chunkZ);
    std::memset(m_Blocks, 0, sizeof(m_Blocks));
}

void Chunk::SetPosition(int chunkX, int chunkZ)
{
    m_ChunkX = chunkX;
    m_ChunkZ = chunkZ;
    m_WorldPosition = glm::vec3(chunkX * CHUNK_WIDTH, 0.0f, chunkZ * CHUNK_DEPTH);
}

void Chunk::SetBlock(int x, int y, int z, uint8_t blockID)
{
    if (x >= 0 && x < CHUNK_WIDTH && y >= 0 && y < CHUNK_HEIGHT && z >= 0 && z < CHUNK_DEPTH)
    {
        m_Blocks[x][y][z] = blockID;
    }
}

uint8_t Chunk::GetBlock(int x, int y, int z) const
{
    if (x >= 0 && x < CHUNK_WIDTH && y >= 0 && y < CHUNK_HEIGHT && z >= 0 && z < CHUNK_DEPTH)
    {
        return m_Blocks[x][y][z];
    }
    return 0; // Air if out of bounds
}

bool Chunk::IsTransparentOrAir(int x, int y, int z, const World* world) const
{
    if (y < 0) return false;             // Bedrock / under world
    if (y >= CHUNK_HEIGHT) return true;  // Open sky above

    if (x < 0 || x >= CHUNK_WIDTH || z < 0 || z >= CHUNK_DEPTH)
    {
        if (world != nullptr)
        {
            int globalX = m_ChunkX * CHUNK_WIDTH + x;
            int globalZ = m_ChunkZ * CHUNK_DEPTH + z;
            return world->IsBlockTransparent(globalX, y, globalZ);
        }
        return true; // Draw border faces if world reference is unavailable
    }

    uint8_t id = m_Blocks[x][y][z];
    if (id == 0) return true;

    const BlockData& blockData = BlockRegistry::Get().GetBlock(id);
    return blockData.IsTransparent;
}

void Chunk::AddFace(
    std::vector<Vertex>& vertices,
    std::vector<uint32_t>& indices,
    const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3,
    const glm::vec2& uvMin, const glm::vec2& uvMax,
    const glm::vec3& normal)
{
    uint32_t baseIdx = static_cast<uint32_t>(vertices.size());

    Vertex v0, v1, v2, v3;
    
    v0.Position[0] = p0.x; v0.Position[1] = p0.y; v0.Position[2] = p0.z;
    v0.TexCoord[0] = uvMin.x; v0.TexCoord[1] = uvMin.y;
    v0.Normal[0] = normal.x; v0.Normal[1] = normal.y; v0.Normal[2] = normal.z;

    v1.Position[0] = p1.x; v1.Position[1] = p1.y; v1.Position[2] = p1.z;
    v1.TexCoord[0] = uvMax.x; v1.TexCoord[1] = uvMin.y;
    v1.Normal[0] = normal.x; v1.Normal[1] = normal.y; v1.Normal[2] = normal.z;

    v2.Position[0] = p2.x; v2.Position[1] = p2.y; v2.Position[2] = p2.z;
    v2.TexCoord[0] = uvMax.x; v2.TexCoord[1] = uvMax.y;
    v2.Normal[0] = normal.x; v2.Normal[1] = normal.y; v2.Normal[2] = normal.z;

    v3.Position[0] = p3.x; v3.Position[1] = p3.y; v3.Position[2] = p3.z;
    v3.TexCoord[0] = uvMin.x; v3.TexCoord[1] = uvMax.y;
    v3.Normal[0] = normal.x; v3.Normal[1] = normal.y; v3.Normal[2] = normal.z;

    vertices.push_back(v0);
    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);

    indices.push_back(baseIdx + 0);
    indices.push_back(baseIdx + 1);
    indices.push_back(baseIdx + 2);
    indices.push_back(baseIdx + 2);
    indices.push_back(baseIdx + 3);
    indices.push_back(baseIdx + 0);
}

void Chunk::GenerateMesh(const World* world)
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    vertices.reserve(CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH * 2);
    indices.reserve(CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH * 3);

    for (int y = 0; y < CHUNK_HEIGHT; ++y)
    {
        for (int z = 0; z < CHUNK_DEPTH; ++z)
        {
            for (int x = 0; x < CHUNK_WIDTH; ++x)
            {
                uint8_t id = m_Blocks[x][y][z];
                if (id == 0) continue; // Air

                const BlockData& blockData = BlockRegistry::Get().GetBlock(id);
                glm::vec3 pos = m_WorldPosition + glm::vec3(x, y, z);
                glm::vec2 uvMin, uvMax;

                // 1. Top Face (+Y)
                if (IsTransparentOrAir(x, y + 1, z, world))
                {
                    BlockRegistry::Get().GetFaceUVs(blockData.FaceTextureIndex[(int)BlockFace::Top], uvMin, uvMax);
                    AddFace(vertices, indices,
                            pos + glm::vec3(0, 1, 1), pos + glm::vec3(1, 1, 1), pos + glm::vec3(1, 1, 0), pos + glm::vec3(0, 1, 0),
                            uvMin, uvMax, glm::vec3(0, 1, 0));
                }

                // 2. Bottom Face (-Y)
                if (IsTransparentOrAir(x, y - 1, z, world))
                {
                    BlockRegistry::Get().GetFaceUVs(blockData.FaceTextureIndex[(int)BlockFace::Bottom], uvMin, uvMax);
                    AddFace(vertices, indices,
                            pos + glm::vec3(0, 0, 0), pos + glm::vec3(1, 0, 0), pos + glm::vec3(1, 0, 1), pos + glm::vec3(0, 0, 1),
                            uvMin, uvMax, glm::vec3(0, -1, 0));
                }

                // 3. Front Face (+Z)
                if (IsTransparentOrAir(x, y, z + 1, world))
                {
                    BlockRegistry::Get().GetFaceUVs(blockData.FaceTextureIndex[(int)BlockFace::Front], uvMin, uvMax);
                    AddFace(vertices, indices,
                            pos + glm::vec3(0, 0, 1), pos + glm::vec3(1, 0, 1), pos + glm::vec3(1, 1, 1), pos + glm::vec3(0, 1, 1),
                            uvMin, uvMax, glm::vec3(0, 0, 1));
                }

                // 4. Back Face (-Z)
                if (IsTransparentOrAir(x, y, z - 1, world))
                {
                    BlockRegistry::Get().GetFaceUVs(blockData.FaceTextureIndex[(int)BlockFace::Back], uvMin, uvMax);
                    AddFace(vertices, indices,
                            pos + glm::vec3(1, 0, 0), pos + glm::vec3(0, 0, 0), pos + glm::vec3(0, 1, 0), pos + glm::vec3(1, 1, 0),
                            uvMin, uvMax, glm::vec3(0, 0, -1));
                }

                // 5. Left Face (-X)
                if (IsTransparentOrAir(x - 1, y, z, world))
                {
                    BlockRegistry::Get().GetFaceUVs(blockData.FaceTextureIndex[(int)BlockFace::Left], uvMin, uvMax);
                    AddFace(vertices, indices,
                            pos + glm::vec3(0, 0, 0), pos + glm::vec3(0, 0, 1), pos + glm::vec3(0, 1, 1), pos + glm::vec3(0, 1, 0),
                            uvMin, uvMax, glm::vec3(-1, 0, 0));
                }

                // 6. Right Face (+X)
                if (IsTransparentOrAir(x + 1, y, z, world))
                {
                    BlockRegistry::Get().GetFaceUVs(blockData.FaceTextureIndex[(int)BlockFace::Right], uvMin, uvMax);
                    AddFace(vertices, indices,
                            pos + glm::vec3(1, 0, 1), pos + glm::vec3(1, 0, 0), pos + glm::vec3(1, 1, 0), pos + glm::vec3(1, 1, 1),
                            uvMin, uvMax, glm::vec3(1, 0, 0));
                }
            }
        }
    }

    m_Mesh.Create(vertices, indices);
}

void Chunk::Draw() const
{
    m_Mesh.Draw();
}
