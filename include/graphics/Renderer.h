#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Mesh;
class World;
class Shader;

class Renderer
{
public:

    Renderer();

    ~Renderer();

public:

    bool Initialize();

    void Shutdown();

public:

    void BeginFrame();

    void EndFrame();

public:

    void SetViewport(
        int width,
        int height
    );

    void SetClearColor(
        float r,
        float g,
        float b,
        float a = 1.0f
    );

public:

    void DrawMesh(
        const Mesh& mesh
    ) const;

    void DrawWorld(
        const World& world,
        const Shader& shader
    ) const;

private:

    float m_ClearColor[4];
};