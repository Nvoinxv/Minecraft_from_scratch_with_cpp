#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "graphics/Shader.h"
#include "graphics/Texture.h"
#include <memory>
#include <string>

class UIRenderer
{
public:
    UIRenderer();
    ~UIRenderer();

    void Initialize(int screenWidth, int screenHeight);
    void Shutdown();

    void UpdateScreenSize(int screenWidth, int screenHeight);

    // Render a colored rectangle
    void DrawRect(float x, float y, float width, float height, glm::vec4 color);
    
    // Render a textured rectangle
    void DrawTexture(std::shared_ptr<Texture> texture, float x, float y, float width, float height, glm::vec4 color = glm::vec4(1.0f), bool isText = false);

private:
    Shader m_UIShader;
    GLuint m_QuadVAO, m_QuadVBO;
    glm::mat4 m_Projection;
};
