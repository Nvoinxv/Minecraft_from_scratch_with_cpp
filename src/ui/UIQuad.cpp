#include "ui/UIQuad.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

UIQuad::UIQuad() : m_VAO(0), m_VBO(0) {}

UIQuad::~UIQuad() {}

void UIQuad::Initialize() {
    float vertices[] = { 
        // pos (x, y) // tex (u, v)
        // Diperbaiki: V koordinat dibalik agar gambar tidak terbalik (upside-down)
        0.0f, 1.0f, 0.0f, 0.0f, // Bottom-Left
        1.0f, 0.0f, 1.0f, 1.0f, // Top-Right
        0.0f, 0.0f, 0.0f, 1.0f, // Top-Left
    
        0.0f, 1.0f, 0.0f, 0.0f, // Bottom-Left
        1.0f, 1.0f, 1.0f, 0.0f, // Bottom-Right
        1.0f, 0.0f, 1.0f, 1.0f  // Top-Right
    };

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(m_VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void UIQuad::Shutdown() {
    if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
    if (m_VBO) glDeleteBuffers(1, &m_VBO);
}

void UIQuad::Draw(Shader& shader, const glm::mat4& projection, float x, float y, float width, float height, std::shared_ptr<Texture> texture, glm::vec4 color, bool isText) {
    shader.Use();
    shader.SetMat4("u_Projection", projection);
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(x, y, 0.0f));  
    model = glm::scale(model, glm::vec3(width, height, 1.0f));
    
    shader.SetMat4("u_Model", model);
    shader.SetVec4("u_Color", color);
    shader.SetBool("u_HasTexture", texture != nullptr);
    shader.SetBool("u_IsText", isText);

    if (texture) {
        glActiveTexture(GL_TEXTURE0);
        texture->Bind();
        shader.SetInt("image", 0);
    }

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
