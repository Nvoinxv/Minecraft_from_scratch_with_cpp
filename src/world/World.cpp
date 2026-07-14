#include "world/World.h"
#include "world/Block.h"
#include <cmath>
#include <iostream>

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

void World::Update(float deltaTime)
{
    // Future chunk loading/unloading or block animations can go here
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

void World::GenerateTerrain()
{
    uint8_t stoneID = BlockRegistry::Get().GetBlockByName("stone").ID;
    uint8_t dirtID = BlockRegistry::Get().GetBlockByName("dirt").ID;
    uint8_t grassID = BlockRegistry::Get().GetBlockByName("grass").ID;
    uint8_t cobbleID = BlockRegistry::Get().GetBlockByName("cobblestone").ID;
    uint8_t torchID = BlockRegistry::Get().GetBlockByName("torch").ID;
    uint8_t chestID = BlockRegistry::Get().GetBlockByName("chest").ID;

    // Create 4x4 chunks around spawn (-2..1, -2..1) -> 64x64 blocks area
    for (int cz = -2; cz <= 1; ++cz)
    {
        for (int cx = -2; cx <= 1; ++cx)
        {
            auto chunk = std::make_unique<Chunk>();
            chunk->Initialize(cx, cz);
            m_Chunks[{cx, cz}] = std::move(chunk);
        }
    }

    std::cout << "[World] Generating terrain hills across 16 chunks...\n";

    for (int cz = -2; cz <= 1; ++cz)
    {
        for (int cx = -2; cx <= 1; ++cx)
        {
            for (int z = 0; z < Chunk::CHUNK_DEPTH; ++z)
            {
                for (int x = 0; x < Chunk::CHUNK_WIDTH; ++x)
                {
                    int globalX = cx * Chunk::CHUNK_WIDTH + x;
                    int globalZ = cz * Chunk::CHUNK_DEPTH + z;

                    // Rolling terrain height using gentle sin waves
                    float wave = std::sin(globalX * 0.08f) * 3.0f + std::cos(globalZ * 0.08f) * 3.0f;
                    int groundY = 30 + static_cast<int>(wave);
                    if (groundY < 5) groundY = 5;
                    if (groundY >= Chunk::CHUNK_HEIGHT - 10) groundY = Chunk::CHUNK_HEIGHT - 10;

                    for (int y = 0; y <= groundY; ++y)
                    {
                        if (y == groundY)
                        {
                            SetBlockGlobal(globalX, y, globalZ, grassID);
                        }
                        else if (y >= groundY - 3)
                        {
                            SetBlockGlobal(globalX, y, globalZ, dirtID);
                        }
                        else
                        {
                            SetBlockGlobal(globalX, y, globalZ, stoneID);
                        }
                    }

                    // Procedural tree planting (pseudo-random based on coords)
                    if ((globalX * 11 + globalZ * 17) % 67 == 0 && globalX != 0 && globalZ != -5)
                    {
                        GenerateTree(globalX, groundY + 1, globalZ);
                    }
                }
            }
        }
    }

    // Place a showcase monument right in front of spawn (0, groundY+1, -5)
    int spawnGroundY = 30 + static_cast<int>(std::sin(0.0f) * 3.0f + std::cos(-5 * 0.08f) * 3.0f);
    SetBlockGlobal(-1, spawnGroundY + 1, -5, cobbleID);
    SetBlockGlobal(0, spawnGroundY + 1, -5, cobbleID);
    SetBlockGlobal(1, spawnGroundY + 1, -5, cobbleID);
    SetBlockGlobal(-1, spawnGroundY + 2, -5, torchID);
    SetBlockGlobal(0, spawnGroundY + 2, -5, chestID);
    SetBlockGlobal(1, spawnGroundY + 2, -5, torchID);

    std::cout << "[World] Building chunk meshes (Face Culling)...\n";
    for (auto& [coords, chunk] : m_Chunks)
    {
        chunk->GenerateMesh(this);
    }
    std::cout << "[World] World generation and meshing completed!\n";
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
