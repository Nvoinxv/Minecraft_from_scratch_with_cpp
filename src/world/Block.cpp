#include "world/Block.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "json/json.hpp"
#include "stb/stb_image.h"

namespace fs = std::filesystem;
using json = nlohmann::json;

BlockRegistry& BlockRegistry::Get()
{
    static BlockRegistry instance;
    return instance;
}

BlockRegistry::BlockRegistry()
    : m_NextTileIndex(0)
{
    m_AirBlock.ID = 0;
    m_AirBlock.Name = "air";
    m_AirBlock.DisplayName = "Air";
    m_AirBlock.Hardness = 0.0f;
    m_AirBlock.BreakBehavior = BlockBreakBehavior::Instant;
    m_AirBlock.PreferredTool = ToolType::None;
    m_AirBlock.EffectiveTools = {};
    m_AirBlock.CanBreakByHand = true;
    m_AirBlock.HarvestLevel = 0;
    m_AirBlock.Drop = "";
    m_AirBlock.BreakSound = "";
    m_AirBlock.StepSound = "";
    m_AirBlock.IsSolid = false;
    m_AirBlock.IsTransparent = true;
    m_AirBlock.IsLightSource = false;
    m_AirBlock.LightLevel = 0;
    m_AirBlock.Flammable = false;
    m_AirBlock.IsInteractive = false;
    m_AirBlock.RenderType = BlockRenderType::Cube; // irrelevant for air, never meshed
    m_AirBlock.FaceTextureIndex = { 0, 0, 0, 0, 0, 0 };

    m_BlocksByID[0] = m_AirBlock;
    m_IDsByName["air"] = 0;
}

BlockRegistry::~BlockRegistry()
{
    Shutdown();
}

void BlockRegistry::Shutdown()
{
    m_BlocksByID.clear();
    m_IDsByName.clear();
    m_TextureToTileIndex.clear();
    m_AtlasTexture.Destroy();
}

BlockRenderType BlockRegistry::ParseRenderType(const std::string& value)
{
    if (value == "sprite" || value == "cross")
    {
        return BlockRenderType::Sprite;
    }
    if (value != "cube" && !value.empty())
    {
        std::cerr << "[BlockRegistry] WARNING: Unknown render_type '" << value
                   << "', defaulting to 'cube'\n";
    }
    return BlockRenderType::Cube;
}

BlockBreakBehavior BlockRegistry::ParseBreakBehavior(const std::string& value)
{
    if (value == "instant")
    {
        return BlockBreakBehavior::Instant;
    }
    if (value == "unbreakable")
    {
        return BlockBreakBehavior::Unbreakable;
    }
    if (value != "timed" && !value.empty())
    {
        std::cerr << "[BlockRegistry] WARNING: Unknown break_behavior '" << value
                   << "', defaulting to 'timed'\n";
    }
    return BlockBreakBehavior::Timed;
}

ToolType BlockRegistry::ParseToolType(const std::string& value)
{
    if (value == "hand")    return ToolType::Hand;
    if (value == "pickaxe") return ToolType::Pickaxe;
    if (value == "axe")     return ToolType::Axe;
    if (value == "shovel")  return ToolType::Shovel;
    if (value == "hoe")     return ToolType::Hoe;
    if (value == "sword")   return ToolType::Sword;
    if (value == "shears")  return ToolType::Shears;

    if (value != "none" && !value.empty())
    {
        std::cerr << "[BlockRegistry] WARNING: Unknown tool_type '" << value
                   << "', defaulting to 'none'\n";
    }
    return ToolType::None;
}

bool BlockRegistry::Initialize(const std::string& blocksDirectory)
{
    std::cout << "[BlockRegistry] Loading block definitions from: " << blocksDirectory << " ...\n";

    if (!fs::exists(blocksDirectory))
    {
        std::cerr << "[BlockRegistry] ERROR: Directory does not exist -> " << blocksDirectory << "\n";
        return false;
    }

    // 1. Scan and parse all JSON block files
    for (const auto& entry : fs::directory_iterator(blocksDirectory))
    {
        if (entry.path().extension() == ".json")
        {
            try
            {
                std::ifstream file(entry.path());
                if (!file.is_open())
                {
                    std::cerr << "[BlockRegistry] WARNING: Could not open file: " << entry.path() << "\n";
                    continue;
                }

                json j;
                file >> j;
                file.close();

                BlockData block;
                block.ID = j.value("id", (uint8_t)0);
                if (block.ID == 0) continue; // skip ID 0 if invalid

                block.Name = j.value("name", "Unknown");
                block.DisplayName = j.value("display_name", block.Name);

                //----------------------------------------------------------
                // Mining
                //----------------------------------------------------------

                block.Hardness = j.value("hardness", 1.0f);
                block.BreakBehavior = ParseBreakBehavior(j.value("break_behavior", std::string("timed")));
                block.PreferredTool = ParseToolType(j.value("preferred_tool", std::string("none")));

                block.EffectiveTools.clear();
                if (j.contains("effective_tools") && j["effective_tools"].is_array())
                {
                    for (const auto& toolVal : j["effective_tools"])
                    {
                        if (toolVal.is_string())
                        {
                            block.EffectiveTools.push_back(ParseToolType(toolVal.get<std::string>()));
                        }
                    }
                }

                block.CanBreakByHand = j.value("can_break_by_hand", true);
                block.HarvestLevel = j.value("harvest_level", (uint8_t)0);

                // Unbreakable blocks (e.g. bedrock) can never be broken by hand,
                // regardless of what the JSON declares - avoid inconsistent data.
                if (block.BreakBehavior == BlockBreakBehavior::Unbreakable)
                {
                    block.CanBreakByHand = false;
                }

                //----------------------------------------------------------
                // Drop
                //----------------------------------------------------------

                block.Drop = j.value("drop", block.Name);

                //----------------------------------------------------------
                // Audio
                //----------------------------------------------------------

                block.BreakSound = j.value("break_sound", "");
                block.StepSound = j.value("step_sound", "");

                //----------------------------------------------------------
                // World
                //----------------------------------------------------------

                block.IsSolid = j.value("is_solid", true);
                block.IsTransparent = j.value("is_transparent", false);
                block.IsLightSource = j.value("is_light_source", false);
                block.LightLevel = j.value("light_level", 0);
                block.Flammable = j.value("flammable", false);
                block.IsInteractive = j.value("is_interactive", false);

                //----------------------------------------------------------
                // Rendering
                //----------------------------------------------------------

                block.RenderType = ParseRenderType(j.value("render_type", std::string("cube")));

                if (j.contains("textures") && j["textures"].is_object())
                {
                    auto& tex = j["textures"];
                    std::string top = tex.value("top", "");
                    std::string bottom = tex.value("bottom", top);
                    std::string front = tex.value("front", top);
                    std::string back = tex.value("back", front);
                    std::string left = tex.value("left", front);
                    std::string right = tex.value("right", front);

                    // "atlas_index" (if present) is treated as a manual/fixed starting
                    // tile index for this block. A block can reference more than one
                    // distinct texture file across its faces (e.g. top vs side texture),
                    // so each *newly encountered* unique texture path within this block
                    // consumes one sequential slot starting at atlas_index:
                    //   1st new texture in this block -> atlas_index
                    //   2nd new texture in this block -> atlas_index + 1, etc.
                    // Textures already registered by a previous block keep their
                    // existing index (dedup is preserved). See GetOrRegisterTextureTile.
                    bool hasExplicitBase = j.contains("atlas_index") && j["atlas_index"].is_number_integer();
                    int explicitBase = hasExplicitBase ? j["atlas_index"].get<int>() : -1;
                    int explicitOffset = 0;

                    auto registerFace = [&](const std::string& path) -> int
                    {
                        if (path.empty()) return 0;

                        // Already registered globally (by this block or a previous one)?
                        auto existing = m_TextureToTileIndex.find(path);
                        if (existing != m_TextureToTileIndex.end())
                        {
                            return existing->second;
                        }

                        if (hasExplicitBase)
                        {
                            int assigned = GetOrRegisterTextureTile(path, explicitBase + explicitOffset);
                            explicitOffset++;
                            return assigned;
                        }

                        return GetOrRegisterTextureTile(path);
                    };

                    block.FaceTextureIndex[(int)BlockFace::Top] = registerFace(top);
                    block.FaceTextureIndex[(int)BlockFace::Bottom] = registerFace(bottom);
                    block.FaceTextureIndex[(int)BlockFace::Front] = registerFace(front);
                    block.FaceTextureIndex[(int)BlockFace::Back] = registerFace(back);
                    block.FaceTextureIndex[(int)BlockFace::Left] = registerFace(left);
                    block.FaceTextureIndex[(int)BlockFace::Right] = registerFace(right);
                }
                else
                {
                    block.FaceTextureIndex = { 0, 0, 0, 0, 0, 0 };
                }

                m_BlocksByID[block.ID] = block;
                m_IDsByName[block.Name] = block.ID;
                std::string lowerName = block.Name;
                for (auto& c : lowerName) c = (char)tolower(c);
                m_IDsByName[lowerName] = block.ID;

                std::cout << "[BlockRegistry] Registered Block ID " << (int)block.ID
                          << " (" << block.DisplayName << ") | Solid: " << (block.IsSolid ? "Yes" : "No")
                          << " | RenderType: " << (block.RenderType == BlockRenderType::Sprite ? "Sprite" : "Cube")
                          << " | BreakBehavior: " << (int)block.BreakBehavior
                          << " | PreferredTool: " << (int)block.PreferredTool
                          << " | AtlasIndexBase: " << (j.contains("atlas_index") ? std::to_string(j.value("atlas_index", -1)) : "auto")
                          << "\n";
            }
            catch (const std::exception& e)
            {
                std::cerr << "[BlockRegistry] ERROR parsing " << entry.path() << ": " << e.what() << "\n";
            }
        }
    }

    // 2. Build 2D Texture Atlas (256x256 image holding 8x8 tiles of 32x32 px)
    const int atlasSize = 256;
    const int tileSize = 32;
    const int tilesPerRow = atlasSize / tileSize;
    std::vector<unsigned char> atlasBuffer(atlasSize * atlasSize * 4, 0);

    std::cout << "[BlockRegistry] Building 2D Texture Atlas (" << atlasSize << "x" << atlasSize << ")... across "
              << m_TextureToTileIndex.size() << " unique texture tiles.\n";

    for (const auto& [texPath, tileIndex] : m_TextureToTileIndex)
    {
        if (tileIndex >= tilesPerRow * tilesPerRow)
        {
            std::cerr << "[BlockRegistry] WARNING: Atlas full! Skipping tile index " << tileIndex << "\n";
            continue;
        }

        std::string fullPath = texPath;
        if (!fs::exists(fullPath))
        {
            if (fs::exists("assets/" + texPath))
                fullPath = "assets/" + texPath;
            else if (fs::exists("assets/textures/blocks/" + texPath))
                fullPath = "assets/textures/blocks/" + texPath;
        }

        int w, h, channels;
        stbi_set_flip_vertically_on_load(false); // keep standard 2D top-to-bottom for atlas placement
        unsigned char* imgData = stbi_load(fullPath.c_str(), &w, &h, &channels, 4);
        if (!imgData)
        {
            std::cerr << "[BlockRegistry] WARNING: Could not load texture tile image: " << fullPath << "\n";
            continue;
        }

        int tileX = (tileIndex % tilesPerRow) * tileSize;
        int tileY = (tileIndex / tilesPerRow) * tileSize;

        // Downsample/resample to 32x32 crisp point-sampling
        for (int y = 0; y < tileSize; ++y)
        {
            for (int x = 0; x < tileSize; ++x)
            {
                int srcX = (x * w) / tileSize;
                int srcY = (y * h) / tileSize;
                int srcIdx = (srcY * w + srcX) * 4;
                int dstIdx = ((tileY + y) * atlasSize + (tileX + x)) * 4;

                atlasBuffer[dstIdx + 0] = imgData[srcIdx + 0];
                atlasBuffer[dstIdx + 1] = imgData[srcIdx + 1];
                atlasBuffer[dstIdx + 2] = imgData[srcIdx + 2];
                atlasBuffer[dstIdx + 3] = imgData[srcIdx + 3];
            }
        }

        stbi_image_free(imgData);
    }

    // Upload atlas to OpenGL texture
    bool atlasSuccess = m_AtlasTexture.CreateFromData(atlasBuffer.data(), atlasSize, atlasSize, 4);
    if (atlasSuccess)
    {
        std::cout << "[BlockRegistry] Texture Atlas successfully created and uploaded to OpenGL (ID: "
                  << m_AtlasTexture.GetID() << ")\n";
    }
    return atlasSuccess;
}

int BlockRegistry::GetOrRegisterTextureTile(const std::string& texturePath, int explicitTileIndex)
{
    if (texturePath.empty()) return 0;

    auto it = m_TextureToTileIndex.find(texturePath);
    if (it != m_TextureToTileIndex.end())
    {
        // Already registered (by this block or an earlier one). If a different
        // explicit index was requested for it now, that's a data conflict in the
        // JSON files - warn, but keep the index that is already in use so we don't
        // silently break whichever block/texture registered it first.
        if (explicitTileIndex >= 0 && explicitTileIndex != it->second)
        {
            std::cerr << "[BlockRegistry] WARNING: texture '" << texturePath
                      << "' already registered at tile " << it->second
                      << ", ignoring conflicting atlas_index " << explicitTileIndex << "\n";
        }
        return it->second;
    }

    if (explicitTileIndex >= 0)
    {
        // Check whether this manual index is already occupied by a *different*
        // texture path. If so, honoring it would silently overwrite that texture's
        // slot in the atlas, so fall back to auto-assignment instead.
        bool collision = false;
        for (const auto& [otherPath, otherIdx] : m_TextureToTileIndex)
        {
            if (otherIdx == explicitTileIndex)
            {
                collision = true;
                std::cerr << "[BlockRegistry] WARNING: atlas_index " << explicitTileIndex
                          << " for texture '" << texturePath << "' collides with texture '"
                          << otherPath << "' already using that tile. Falling back to auto-assigned index.\n";
                break;
            }
        }

        if (!collision)
        {
            m_TextureToTileIndex[texturePath] = explicitTileIndex;
            // Keep the auto-increment counter ahead of any manually-assigned index so
            // future auto-assigned textures never collide with it.
            if (explicitTileIndex >= m_NextTileIndex)
            {
                m_NextTileIndex = explicitTileIndex + 1;
            }
            return explicitTileIndex;
        }
        // fall through to auto-assignment below
    }

    int tileIdx = m_NextTileIndex++;
    m_TextureToTileIndex[texturePath] = tileIdx;
    return tileIdx;
}

const BlockData& BlockRegistry::GetBlock(uint8_t id) const
{
    auto it = m_BlocksByID.find(id);
    if (it != m_BlocksByID.end())
    {
        return it->second;
    }
    return m_AirBlock;
}

const BlockData& BlockRegistry::GetBlockByName(const std::string& name) const
{
    auto it = m_IDsByName.find(name);
    if (it != m_IDsByName.end())
    {
        return GetBlock(it->second);
    }
    std::string lowerName = name;
    for (auto& c : lowerName) c = (char)tolower(c);
    it = m_IDsByName.find(lowerName);
    if (it != m_IDsByName.end())
    {
        return GetBlock(it->second);
    }
    return m_AirBlock;
}

void BlockRegistry::BindAtlas(unsigned int slot) const
{
    m_AtlasTexture.Bind(slot);
}

const Texture& BlockRegistry::GetAtlasTexture() const
{
    return m_AtlasTexture;
}

void BlockRegistry::GetFaceUVs(int tileIndex, glm::vec2& uvMin, glm::vec2& uvMax) const
{
    const float tilesPerRow = 8.0f; // 256 / 32 = 8
    const float tileSizeUV = 1.0f / tilesPerRow;
    // Add tiny half-pixel inset (0.002f) to prevent texture bleeding from neighboring atlas tiles
    const float pad = 0.002f;

    int tileX = tileIndex % (int)tilesPerRow;
    int tileY = tileIndex / (int)tilesPerRow;

    uvMin.x = tileX * tileSizeUV + pad;
    uvMin.y = tileY * tileSizeUV + pad;
    uvMax.x = (tileX + 1) * tileSizeUV - pad;
    uvMax.y = (tileY + 1) * tileSizeUV - pad;
}