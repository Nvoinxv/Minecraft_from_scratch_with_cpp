#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>
#include "../graphics/Shader.h"
#include "../graphics/Texture.h"

class UIQuad {
public:
    UIQuad();
    ~UIQuad();
    void Initialize();
    void Shutdown();
    void Draw(Shader& shader, const glm::mat4& projection, float x, float y, float width, float height, std::shared_ptr<Texture> texture = nullptr, glm::vec4 color = glm::vec4(1.0f), bool isText = false);
private:
    GLuint m_VAO, m_VBO;
};
