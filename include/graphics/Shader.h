#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader
{
public:
    Shader();
    ~Shader();

    bool Load(const std::string& vertexPath, const std::string& fragmentPath);
    void Use() const;
    void Destroy();

    GLuint GetID() const { return m_ID; }
    bool IsValid() const { return m_ID != 0; }

    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetVec3(const std::string& name, const glm::vec3& value) const;
    void SetMat4(const std::string& name, const glm::mat4& mat) const;

private:
    std::string ReadFile(const std::string& filepath) const;
    void CheckCompileErrors(GLuint shader, const std::string& type) const;

private:
    GLuint m_ID;
};
