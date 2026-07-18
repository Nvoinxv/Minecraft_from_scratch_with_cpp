#include "graphics/UIRenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

UIRenderer::UIRenderer() : m_QuadVAO(0), m_QuadVBO(0) {}

UIRenderer::~UIRenderer() {}

void UIRenderer::Initialize(int screenWidth, int screenHeight)
{
    // Compile UI Shader
    if (!m_UIShader.Load("assets/shaders/ui.vs", "assets/shaders/ui.fs"))
    {
        std::cerr << "[UIRenderer] Failed to compile UI shaders." << std::endl;
    }

    UpdateScreenSize(screenWidth, screenHeight);

    // Configure VAO/VBO
    float vertices[] = { 
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 
    
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &m_QuadVAO);
    glGenBuffers(1, &m_QuadVBO);

    glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(m_QuadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void UIRenderer::Shutdown()
{
    m_UIShader.Destroy();
    if (m_QuadVAO) glDeleteVertexArrays(1, &m_QuadVAO);
    if (m_QuadVBO) glDeleteBuffers(1, &m_QuadVBO);
}

void UIRenderer::UpdateScreenSize(int screenWidth, int screenHeight)
{
    m_Projection = glm::ortho(0.0f, static_cast<float>(screenWidth), static_cast<float>(screenHeight), 0.0f, -1.0f, 1.0f);
}

void UIRenderer::DrawRect(float x, float y, float width, float height, glm::vec4 color)
{
    m_UIShader.Use();
    m_UIShader.SetMat4("u_Projection", m_Projection);
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(x, y, 0.0f));  
    model = glm::scale(model, glm::vec3(width, height, 1.0f));
    
    m_UIShader.SetMat4("u_Model", model);
    m_UIShader.SetVec4("u_Color", color);
    m_UIShader.SetBool("u_HasTexture", false);
    m_UIShader.SetBool("u_IsText", false);

    glBindVertexArray(m_QuadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void UIRenderer::DrawTexture(std::shared_ptr<Texture> texture, float x, float y, float width, float height, glm::vec4 color, bool isText)
{
    if (!texture) return;

    m_UIShader.Use();
    m_UIShader.SetMat4("u_Projection", m_Projection);
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(x, y, 0.0f));  
    model = glm::scale(model, glm::vec3(width, height, 1.0f));
    
    m_UIShader.SetMat4("u_Model", model);
    m_UIShader.SetVec4("u_Color", color);
    m_UIShader.SetBool("u_HasTexture", true);
    m_UIShader.SetBool("u_IsText", isText);

    glActiveTexture(GL_TEXTURE0);
    texture->Bind();
    m_UIShader.SetInt("image", 0);

    glBindVertexArray(m_QuadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
