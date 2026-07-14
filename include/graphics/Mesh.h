#pragma once

#include <vector>
#include <cstdint>

#include <glad/glad.h>

struct Vertex
{
    float Position[3];
    float TexCoord[2];
    float Normal[3];
};

class Mesh
{
public:

    Mesh();

    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

public:

    void Create(
        const std::vector<Vertex>& vertices,
        const std::vector<uint32_t>& indices
    );

    void Destroy();

    void Draw() const;

public:

    bool IsValid() const;

    uint32_t GetIndexCount() const;

private:

    GLuint m_VAO;
    GLuint m_VBO;
    GLuint m_EBO;

    uint32_t m_IndexCount;
};