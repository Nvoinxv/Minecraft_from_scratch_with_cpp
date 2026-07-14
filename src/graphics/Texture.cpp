#include "graphics/Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <iostream>

Texture::Texture()
    : m_ID(0), m_Width(0), m_Height(0), m_Channels(0)
{
}

Texture::~Texture()
{
    Destroy();
}

bool Texture::Load(const std::string& filepath)
{
    Destroy();

    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_Channels, 0);
    if (!data)
    {
        std::cerr << "[Texture] ERROR: Failed to load image from path: " << filepath << std::endl;
        return false;
    }

    bool result = CreateFromData(data, m_Width, m_Height, m_Channels);
    stbi_image_free(data);

    if (result)
    {
        std::cout << "[Texture] Successfully loaded: " << filepath << " (" << m_Width << "x" << m_Height << ", " << m_Channels << " channels)\n";
    }
    return result;
}

bool Texture::CreateFromData(const unsigned char* data, int width, int height, int channels)
{
    m_Width = width;
    m_Height = height;
    m_Channels = channels;

    glGenTextures(1, &m_ID);
    glBindTexture(GL_TEXTURE_2D, m_ID);

    // Retro Minecraft Alpha texture filtering: GL_NEAREST ensures crisp pixel art
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    GLenum format = GL_RGBA;
    if (channels == 1)
        format = GL_RED;
    else if (channels == 3)
        format = GL_RGB;
    else if (channels == 4)
        format = GL_RGBA;

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    return m_ID != 0;
}

void Texture::Bind(unsigned int slot) const
{
    if (m_ID != 0)
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_ID);
    }
}

void Texture::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Destroy()
{
    if (m_ID != 0)
    {
        glDeleteTextures(1, &m_ID);
        m_ID = 0;
    }
}
