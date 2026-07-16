#pragma once

#include <array>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "graphics/Texture.h"

//
// ============================================================================
// Block Face
// ============================================================================
//

enum class BlockFace : uint8_t
{
    Top = 0,
    Bottom,
    Front,
    Back,
    Left,
    Right
};

//
// ============================================================================
// Block Render Type
// ============================================================================
//

enum class BlockRenderType : uint8_t
{
    Cube = 0,
    Sprite
};

//
// ============================================================================
// Block Break Behavior
// ============================================================================
//

enum class BlockBreakBehavior : uint8_t
{
    Instant = 0,      // Langsung hancur (contoh: Torch)
    Timed,            // Menggunakan progress menghancurkan
    Unbreakable       // Tidak bisa dihancurkan (Bedrock)
};

//
// ============================================================================
// Tool Type
// ============================================================================
//

enum class ToolType : uint8_t
{
    None = 0,

    Hand,

    Pickaxe,
    Axe,
    Shovel,
    Hoe,
    Sword,
    Shears
};

//
// ============================================================================
// Block Data
// ============================================================================
//

struct BlockData
{
    //----------------------------------------------------------
    // Basic Information
    //----------------------------------------------------------

    uint8_t ID;

    std::string Name;

    std::string DisplayName;

    //----------------------------------------------------------
    // Mining
    //----------------------------------------------------------

    float Hardness;

    BlockBreakBehavior BreakBehavior;

    ToolType PreferredTool;

    std::vector<ToolType> EffectiveTools;

    bool CanBreakByHand;

    uint8_t HarvestLevel;

    //----------------------------------------------------------
    // Drop
    //----------------------------------------------------------

    std::string Drop;

    //----------------------------------------------------------
    // Audio
    //----------------------------------------------------------

    std::string BreakSound;

    std::string StepSound;

    //----------------------------------------------------------
    // World
    //----------------------------------------------------------

    bool IsSolid;

    bool IsTransparent;

    bool IsLightSource;

    int LightLevel;

    bool Flammable;

    bool IsInteractive;

    //----------------------------------------------------------
    // Rendering
    //----------------------------------------------------------

    BlockRenderType RenderType;

    std::array<int, 6> FaceTextureIndex;
};

//
// ============================================================================
// Block Registry
// ============================================================================
//

class BlockRegistry
{
public:

    static BlockRegistry& Get();

    bool Initialize(const std::string& blocksDirectory);

    void Shutdown();

    const BlockData& GetBlock(uint8_t id) const;

    const BlockData& GetBlockByName(
        const std::string& name) const;

    void BindAtlas(unsigned int slot = 0) const;

    void GetFaceUVs(
        int tileIndex,
        glm::vec2& uvMin,
        glm::vec2& uvMax) const;

    const Texture& GetAtlasTexture() const;

private:

    BlockRegistry();

    ~BlockRegistry();

    BlockRegistry(const BlockRegistry&) = delete;

    BlockRegistry& operator=(const BlockRegistry&) = delete;

private:

    int GetOrRegisterTextureTile(
        const std::string& texturePath,
        int explicitTileIndex = -1);

    static BlockRenderType ParseRenderType(
        const std::string& value);

    static BlockBreakBehavior ParseBreakBehavior(
        const std::string& value);

    static ToolType ParseToolType(
        const std::string& value);

private:

    std::map<uint8_t, BlockData> m_BlocksByID;

    std::map<std::string, uint8_t> m_IDsByName;

    BlockData m_AirBlock;

    Texture m_AtlasTexture;

    std::map<std::string, int> m_TextureToTileIndex;

    int m_NextTileIndex;
};