#include "graphics/Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader()
    : m_ID(0)
{
}

Shader::~Shader()
{
    Destroy();
}

bool Shader::Load(const std::string& vertexPath, const std::string& fragmentPath)
{
    Destroy();

    std::string vertexCodeStr = ReadFile(vertexPath);
    std::string fragmentCodeStr = ReadFile(fragmentPath);

    if (vertexCodeStr.empty() || fragmentCodeStr.empty())
    {
        std::cerr << "[Shader] ERROR: Failed to read shader files: " << vertexPath << " or " << fragmentPath << std::endl;
        return false;
    }

    const char* vShaderCode = vertexCodeStr.c_str();
    const char* fShaderCode = fragmentCodeStr.c_str();

    // 1. Compile Vertex Shader
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    CheckCompileErrors(vertex, "VERTEX");

    // 2. Compile Fragment Shader
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    CheckCompileErrors(fragment, "FRAGMENT");

    // 3. Shader Program Link
    m_ID = glCreateProgram();
    glAttachShader(m_ID, vertex);
    glAttachShader(m_ID, fragment);
    glLinkProgram(m_ID);
    CheckCompileErrors(m_ID, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return m_ID != 0;
}

void Shader::Use() const
{
    if (m_ID != 0)
    {
        glUseProgram(m_ID);
    }
}

void Shader::Destroy()
{
    if (m_ID != 0)
    {
        glDeleteProgram(m_ID);
        m_ID = 0;
    }
}

std::string Shader::ReadFile(const std::string& filepath) const
{
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        shaderFile.open(filepath);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        return shaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cerr << "[Shader] ERROR: File not successfully read: " << filepath << " -> " << e.what() << std::endl;
        return "";
    }
}

void Shader::CheckCompileErrors(GLuint shader, const std::string& type) const
{
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "[Shader] ERROR: Shader compilation error of type: " << type << "\n" << infoLog << "\n-----------------------------------------------------\n";
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "[Shader] ERROR: Program linking error of type: " << type << "\n" << infoLog << "\n-----------------------------------------------------\n";
        }
    }
}

void Shader::SetInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}
