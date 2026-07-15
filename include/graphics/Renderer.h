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

// Render distance in blocks (world units), NOT chunks.
// Set this from wherever chunk load radius is decided (e.g. ChunkManager),
// so fog distance never drifts out of sync with actual chunk cutoff.
void SetRenderDistance(float blocks) { m_RenderDistance = blocks; }
float GetRenderDistance() const { return m_RenderDistance; }

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
float m_RenderDistance = 128.0f; // fallback default until ChunkManager sets a real value
};