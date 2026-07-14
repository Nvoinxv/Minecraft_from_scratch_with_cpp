#include "graphics/Mesh.h"

Mesh::Mesh()
    :
    m_VAO(0),
    m_VBO(0),
    m_EBO(0),
    m_IndexCount(0)
{
}

Mesh::~Mesh()
{
    Destroy();
}

void Mesh::Create(
    const std::vector<Vertex>& vertices,
    const std::vector<uint32_t>& indices)
{
    Destroy();

    m_IndexCount =
        static_cast<uint32_t>(indices.size());

    glGenVertexArrays(
        1,
        &m_VAO);

    glGenBuffers(
        1,
        &m_VBO);

    glGenBuffers(
        1,
        &m_EBO);

    glBindVertexArray(m_VAO);

    //-------------------------------------
    // Vertex Buffer
    //-------------------------------------

    glBindBuffer(
        GL_ARRAY_BUFFER,
        m_VBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(Vertex),
        vertices.data(),
        GL_STATIC_DRAW);

    //-------------------------------------
    // Index Buffer
    //-------------------------------------

    glBindBuffer(
        GL_ELEMENT_ARRAY_BUFFER,
        m_EBO);

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(uint32_t),
        indices.data(),
        GL_STATIC_DRAW);

    //-------------------------------------
    // Position
    //-------------------------------------

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, Position));

    //-------------------------------------
    // Texture Coordinate
    //-------------------------------------

    glEnableVertexAttribArray(1);

    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, TexCoord));

    //-------------------------------------
    // Normal
    //-------------------------------------

    glEnableVertexAttribArray(2);

    glVertexAttribPointer(
        2,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, Normal));

    glBindVertexArray(0);
}

void Mesh::Destroy()
{
    if (m_EBO != 0)
    {
        glDeleteBuffers(
            1,
            &m_EBO);

        m_EBO = 0;
    }

    if (m_VBO != 0)
    {
        glDeleteBuffers(
            1,
            &m_VBO);

        m_VBO = 0;
    }

    if (m_VAO != 0)
    {
        glDeleteVertexArrays(
            1,
            &m_VAO);

        m_VAO = 0;
    }

    m_IndexCount = 0;
}

void Mesh::Draw() const
{
    if (!IsValid())
    {
        return;
    }

    glBindVertexArray(m_VAO);

    glDrawElements(
        GL_TRIANGLES,
        m_IndexCount,
        GL_UNSIGNED_INT,
        nullptr);

    glBindVertexArray(0);
}

bool Mesh::IsValid() const
{
    return
        m_VAO != 0 &&
        m_VBO != 0 &&
        m_EBO != 0 &&
        m_IndexCount > 0;
}

uint32_t Mesh::GetIndexCount() const
{
    return m_IndexCount;
}