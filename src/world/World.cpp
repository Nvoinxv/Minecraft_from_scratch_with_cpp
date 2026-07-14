#include "world/World.h"
#include "world/Block.h"
#include <cmath>
#include <iostream>
#include <algorithm>

World::World()
{
}

World::~World()
{
    Shutdown();
}

void World::Initialize()
{
    std::cout << "[World] Initializing 3D Voxel World...\n";
    GenerateTerrain();
}

void World::Shutdown()
{
    m_Chunks.clear();
}

void World::Update(float deltaTime, const glm::vec3& playerPos)
{
    int playerChunkX = static_cast<int>(std::floor(playerPos.x / Chunk::CHUNK_WIDTH));
    int playerChunkZ = static_cast<int>(std::floor(playerPos.z / Chunk::CHUNK_DEPTH));

    if (playerChunkX != m_LastPlayerChunkX || playerChunkZ != m_LastPlayerChunkZ)
    {
        LoadChunksAroundPlayer(playerChunkX, playerChunkZ);
        m_LastPlayerChunkX = playerChunkX;
        m_LastPlayerChunkZ = playerChunkZ;
    }

    // Time-Slicing: Proses maksimal 1 mesh generation per frame untuk mencegah stutter
    if (!m_MeshUpdateQueue.empty())
    {
        auto coords = m_MeshUpdateQueue.front();
        m_MeshUpdateQueue.erase(m_MeshUpdateQueue.begin());

        Chunk* c = GetChunk(coords.first, coords.second);
        if (c)
        {
            c->GenerateMesh(this);
        }
    }
}

void World::LoadChunksAroundPlayer(int playerChunkX, int playerChunkZ)
{
    std::vector<std::pair<int, int>> chunksToMesh;

    for (int cz = playerChunkZ - RENDER_DISTANCE; cz <= playerChunkZ + RENDER_DISTANCE; ++cz)
    {
        for (int cx = playerChunkX - RENDER_DISTANCE; cx <= playerChunkX + RENDER_DISTANCE; ++cx)
        {
            if (m_Chunks.find({cx, cz}) == m_Chunks.end())
            {
                GenerateChunk(cx, cz);
                chunksToMesh.push_back({cx, cz});
                
                // Also trigger mesh update for neighbors to prevent gaps
                chunksToMesh.push_back({cx-1, cz});
                chunksToMesh.push_back({cx+1, cz});
                chunksToMesh.push_back({cx, cz-1});
                chunksToMesh.push_back({cx, cz+1});
            }
        }
    }

    // Unload chunks outside RENDER_DISTANCE + 2
    for (auto it = m_Chunks.begin(); it != m_Chunks.end(); )
    {
        int dx = std::abs(it->first.first - playerChunkX);
        int dz = std::abs(it->first.second - playerChunkZ);
        if (dx > RENDER_DISTANCE + 2 || dz > RENDER_DISTANCE + 2)
        {
            it = m_Chunks.erase(it);
        }
        else
        {
            ++it;
        }
    }

    // Masukkan ke antrian (queue) agar diproses secara bertahap (Time-Slicing)
    for (auto coords : chunksToMesh)
    {
        // Hanya masukkan jika belum ada di antrian
        if (std::find(m_MeshUpdateQueue.begin(), m_MeshUpdateQueue.end(), coords) == m_MeshUpdateQueue.end())
        {
            m_MeshUpdateQueue.push_back(coords);
        }
    }
}

Chunk* World::GetChunk(int chunkX, int chunkZ) const
{
    auto it = m_Chunks.find({chunkX, chunkZ});
    if (it != m_Chunks.end())
    {
        return it->second.get();
    }
    return nullptr;
}

void World::SetBlockGlobal(int x, int y, int z, uint8_t blockID)
{
    int chunkX = static_cast<int>(std::floor(static_cast<float>(x) / Chunk::CHUNK_WIDTH));
    int chunkZ = static_cast<int>(std::floor(static_cast<float>(z) / Chunk::CHUNK_DEPTH));
    int localX = x - chunkX * Chunk::CHUNK_WIDTH;
    int localZ = z - chunkZ * Chunk::CHUNK_DEPTH;

    Chunk* chunk = GetChunk(chunkX, chunkZ);
    if (chunk)
    {
        chunk->SetBlock(localX, y, localZ, blockID);
    }
}

uint8_t World::GetBlockGlobal(int x, int y, int z) const
{
    int chunkX = static_cast<int>(std::floor(static_cast<float>(x) / Chunk::CHUNK_WIDTH));
    int chunkZ = static_cast<int>(std::floor(static_cast<float>(z) / Chunk::CHUNK_DEPTH));
    int localX = x - chunkX * Chunk::CHUNK_WIDTH;
    int localZ = z - chunkZ * Chunk::CHUNK_DEPTH;

    Chunk* chunk = GetChunk(chunkX, chunkZ);
    if (chunk)
    {
        return chunk->GetBlock(localX, y, localZ);
    }
    return 0; // Air if unloaded
}

bool World::IsBlockTransparent(int x, int y, int z) const
{
    if (y < 0) return false;
    if (y >= Chunk::CHUNK_HEIGHT) return true;

    uint8_t id = GetBlockGlobal(x, y, z);
    if (id == 0) return true;

    return BlockRegistry::Get().GetBlock(id).IsTransparent;
}

void World::GenerateTree(int globalX, int groundY, int globalZ)
{
    uint8_t woodID = BlockRegistry::Get().GetBlockByName("wood").ID;
    uint8_t leafsID = BlockRegistry::Get().GetBlockByName("leafs").ID;

    int trunkHeight = 4;
    for (int y = 0; y < trunkHeight; ++y)
    {
        SetBlockGlobal(globalX, groundY + y, globalZ, woodID);
    }

    // Leaves canopy
    for (int ly = groundY + trunkHeight - 1; ly <= groundY + trunkHeight + 1; ++ly)
    {
        int radius = (ly == groundY + trunkHeight + 1) ? 1 : 2;
        for (int lx = globalX - radius; lx <= globalX + radius; ++lx)
        {
            for (int lz = globalZ - radius; lz <= globalZ + radius; ++lz)
            {
                if (lx == globalX && lz == globalZ && ly < groundY + trunkHeight) continue; // don't replace trunk
                if (GetBlockGlobal(lx, ly, lz) == 0)
                {
                    SetBlockGlobal(lx, ly, lz, leafsID);
                }
            }
        }
    }
}

void World::GenerateChunk(int cx, int cz)
{
    auto chunk = std::make_unique<Chunk>();
    chunk->Initialize(cx, cz);
    m_Chunks[{cx, cz}] = std::move(chunk);

    uint8_t bedrockID = BlockRegistry::Get().GetBlockByName("bedrock").ID;
    uint8_t stoneID = BlockRegistry::Get().GetBlockByName("stone").ID;
    uint8_t dirtID = BlockRegistry::Get().GetBlockByName("dirt").ID;
    uint8_t grassID = BlockRegistry::Get().GetBlockByName("grass").ID;

    for (int z = 0; z < Chunk::CHUNK_DEPTH; ++z)
    {
        for (int x = 0; x < Chunk::CHUNK_WIDTH; ++x)
        {
            int globalX = cx * Chunk::CHUNK_WIDTH + x;
            int globalZ = cz * Chunk::CHUNK_DEPTH + z;

            // Pseudo-random noise for terrain height centered around Y=70
            float wave = std::sin(globalX * 0.05f) * 4.0f + std::cos(globalZ * 0.04f) * 5.0f;
            float detail = std::sin(globalX * 0.2f + globalZ * 0.2f) * 2.0f;
            int groundY = 70 + static_cast<int>(wave + detail);

            if (groundY < 10) groundY = 10;
            if (groundY >= Chunk::CHUNK_HEIGHT - 20) groundY = Chunk::CHUNK_HEIGHT - 20;

            for (int y = 0; y <= groundY; ++y)
            {
                if (y == 0)
                {
                    SetBlockGlobal(globalX, y, globalZ, bedrockID); // Dasar dunia
                }
                else if (y == groundY)
                {
                    SetBlockGlobal(globalX, y, globalZ, grassID); // Permukaan
                }
                else if (y >= groundY - 10)
                {
                    SetBlockGlobal(globalX, y, globalZ, dirtID); // 10 Lapis Dirt
                }
                else
                {
                    SetBlockGlobal(globalX, y, globalZ, stoneID); // Sisa bawahnya Stone
                }
            }

            // Procedural Random Trees
            // Gunakan seed sederhana dari koordinat X dan Z
            int treeHash = (globalX * 73856093 ^ globalZ * 19349663) % 1000;
            if (treeHash < 0) treeHash = -treeHash;
            
            // Peluang pohon muncul sekitar 1 dari 150 blok permukaan
            if (treeHash < 10 && globalX != 0 && globalZ != -5)
            {
                GenerateTree(globalX, groundY + 1, globalZ);
            }
        }
    }
}

void World::GenerateTerrain()
{
    // Monument di spawn
    uint8_t cobbleID = BlockRegistry::Get().GetBlockByName("cobblestone").ID;
    uint8_t torchID = BlockRegistry::Get().GetBlockByName("torch").ID;
    uint8_t chestID = BlockRegistry::Get().GetBlockByName("chest").ID;
    
    // Spawn ground Y is around 70 (0, 70, -5)
    float wave = std::sin(0.0f) * 4.0f + std::cos(-5 * 0.04f) * 5.0f;
    float detail = std::sin(0.0f + (-5 * 0.2f)) * 2.0f;
    int spawnGroundY = 70 + static_cast<int>(wave + detail);

    // Initial load for spawn point
    LoadChunksAroundPlayer(0, 0);

    SetBlockGlobal(-1, spawnGroundY + 1, -5, cobbleID);
    SetBlockGlobal(0, spawnGroundY + 1, -5, cobbleID);
    SetBlockGlobal(1, spawnGroundY + 1, -5, cobbleID);
    SetBlockGlobal(-1, spawnGroundY + 2, -5, torchID);
    SetBlockGlobal(0, spawnGroundY + 2, -5, chestID);
    SetBlockGlobal(1, spawnGroundY + 2, -5, torchID);
    
    // Rebuild mesh around spawn just in case monument cuts chunk border
    Chunk* c = GetChunk(0, 0);
    if(c) c->GenerateMesh(this);
}

void World::Render(const Shader& shader) const
{
    BlockRegistry::Get().BindAtlas(0);
    shader.SetInt("u_Texture", 0);

    glm::mat4 model = glm::mat4(1.0f);
    shader.SetMat4("u_Model", model);

    for (const auto& [coords, chunk] : m_Chunks)
    {
        chunk->Draw();
    }
}
