#pragma once

#include <map>
#include <memory>
#include <utility>
#include <glm/glm.hpp>
#include "world/Chunk.h"
#include "graphics/Shader.h"

class World
{
public:
    World();
    ~World();

    World(const World&) = delete;
    World& operator=(const World&) = delete;

    void Initialize();
    void Shutdown();

    void Update(float deltaTime);
    void Render(const Shader& shader) const;

    Chunk* GetChunk(int chunkX, int chunkZ) const;
    void SetBlockGlobal(int x, int y, int z, uint8_t blockID);
    uint8_t GetBlockGlobal(int x, int y, int z) const;
    bool IsBlockTransparent(int x, int y, int z) const;

    void GenerateTerrain();
    void GenerateTree(int globalX, int groundY, int globalZ);

private:
    std::map<std::pair<int, int>, std::unique_ptr<Chunk>> m_Chunks;
};
