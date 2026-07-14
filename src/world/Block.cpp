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
    m_AirBlock.IsSolid = false;
    m_AirBlock.IsTransparent = true;
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
                block.Hardness = j.value("hardness", 1.0f);
                block.IsSolid = j.value("is_solid", true);
                block.IsTransparent = j.value("is_transparent", false);
                block.IsLightSource = j.value("is_light_source", false);
                block.LightLevel = j.value("light_level", 0);

                if (j.contains("textures") && j["textures"].is_object())
                {
                    auto& tex = j["textures"];
                    std::string top = tex.value("top", "");
                    std::string bottom = tex.value("bottom", top);
                    std::string front = tex.value("front", top);
                    std::string back = tex.value("back", front);
                    std::string left = tex.value("left", front);
                    std::string right = tex.value("right", front);

                    block.FaceTextureIndex[(int)BlockFace::Top] = GetOrRegisterTextureTile(top);
                    block.FaceTextureIndex[(int)BlockFace::Bottom] = GetOrRegisterTextureTile(bottom);
                    block.FaceTextureIndex[(int)BlockFace::Front] = GetOrRegisterTextureTile(front);
                    block.FaceTextureIndex[(int)BlockFace::Back] = GetOrRegisterTextureTile(back);
                    block.FaceTextureIndex[(int)BlockFace::Left] = GetOrRegisterTextureTile(left);
                    block.FaceTextureIndex[(int)BlockFace::Right] = GetOrRegisterTextureTile(right);
                }

                m_BlocksByID[block.ID] = block;
                m_IDsByName[block.Name] = block.ID;
                std::string lowerName = block.Name;
                for (auto& c : lowerName) c = (char)tolower(c);
                m_IDsByName[lowerName] = block.ID;

                std::cout << "[BlockRegistry] Registered Block ID " << (int)block.ID 
                          << " (" << block.DisplayName << ") | Solid: " << (block.IsSolid ? "Yes" : "No") << "\n";
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

int BlockRegistry::GetOrRegisterTextureTile(const std::string& texturePath)
{
    if (texturePath.empty()) return 0;
    auto it = m_TextureToTileIndex.find(texturePath);
    if (it != m_TextureToTileIndex.end())
    {
        return it->second;
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

void BlockRegistry::GetFaceUVs(int tileIndex, glm::vec2& uvMin, glm::vec2& uvMax) const
{
    const float tilesPerRow = 8.0f; // 256 / 32 = 8
    const float tileSizeUV = 1.0f / tilesPerRow;
    // Add tiny half-pixel inset (0.001f) to prevent texture bleeding from neighboring atlas tiles
    const float pad = 0.002f;

    int tileX = tileIndex % (int)tilesPerRow;
    int tileY = tileIndex / (int)tilesPerRow;

    uvMin.x = tileX * tileSizeUV + pad;
    uvMin.y = tileY * tileSizeUV + pad;
    uvMax.x = (tileX + 1) * tileSizeUV - pad;
    uvMax.y = (tileY + 1) * tileSizeUV - pad;
}
