#pragma once

#include <string>
#include <glad/glad.h>

class Texture
{
public:
    Texture();
    ~Texture();

    bool Load(const std::string& filepath);
    bool CreateFromData(const unsigned char* data, int width, int height, int channels);
    
    void Bind(unsigned int slot = 0) const;
    void Unbind() const;
    void Destroy();

    GLuint GetID() const { return m_ID; }
    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }
    bool IsValid() const { return m_ID != 0; }

private:
    GLuint m_ID;
    int m_Width;
    int m_Height;
    int m_Channels;
};
