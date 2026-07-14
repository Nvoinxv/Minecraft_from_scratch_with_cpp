#pragma once

#include <string>
#include <vector>
#include <map>
#include <cstdint>
#include <glm/glm.hpp>
#include "graphics/Texture.h"

enum class BlockFace : uint8_t
{
    Top = 0,
    Bottom = 1,
    Front = 2,
    Back = 3,
    Left = 4,
    Right = 5
};

struct BlockData
{
    uint8_t ID = 0;
    std::string Name = "Air";
    std::string DisplayName = "Air";
    float Hardness = 0.0f;
    bool IsSolid = false;
    bool IsTransparent = true;
    bool IsLightSource = false;
    int LightLevel = 0;
    
    // Atlas tile indices for each of the 6 faces: [Top, Bottom, Front, Back, Left, Right]
    int FaceTextureIndex[6] = {0, 0, 0, 0, 0, 0};
};

class BlockRegistry
{
public:
    static BlockRegistry& Get();

    bool Initialize(const std::string& blocksDirectory);
    void Shutdown();

    const BlockData& GetBlock(uint8_t id) const;
    const BlockData& GetBlockByName(const std::string& name) const;
    
    void BindAtlas(unsigned int slot = 0) const;
    void GetFaceUVs(int tileIndex, glm::vec2& uvMin, glm::vec2& uvMax) const;

    const Texture& GetAtlasTexture() const { return m_AtlasTexture; }

private:
    BlockRegistry();
    ~BlockRegistry();

    BlockRegistry(const BlockRegistry&) = delete;
    BlockRegistry& operator=(const BlockRegistry&) = delete;

    int GetOrRegisterTextureTile(const std::string& texturePath);

private:
    std::map<uint8_t, BlockData> m_BlocksByID;
    std::map<std::string, uint8_t> m_IDsByName;
    BlockData m_AirBlock;

    Texture m_AtlasTexture;
    std::map<std::string, int> m_TextureToTileIndex;
    int m_NextTileIndex;
};
