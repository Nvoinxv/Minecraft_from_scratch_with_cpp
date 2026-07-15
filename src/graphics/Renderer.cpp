#include "graphics/Renderer.h"

#include "graphics/Mesh.h"
#include "graphics/Shader.h"
#include "world/World.h"

Renderer::Renderer()
{
    m_ClearColor[0] = 0.53f;
    m_ClearColor[1] = 0.81f;
    m_ClearColor[2] = 0.92f;
    m_ClearColor[3] = 1.0f;
}

Renderer::~Renderer()
{
    Shutdown();
}

bool Renderer::Initialize()
{
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glFrontFace(GL_CCW);

    glEnable(GL_BLEND);
    glBlendFunc(
        GL_SRC_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA
    );

    glClearColor(
        m_ClearColor[0],
        m_ClearColor[1],
        m_ClearColor[2],
        m_ClearColor[3]
    );

    return true;
}

void Renderer::Shutdown()
{
}

void Renderer::BeginFrame()
{
    glClearColor(
        m_ClearColor[0],
        m_ClearColor[1],
        m_ClearColor[2],
        m_ClearColor[3]
    );

    glClear(
        GL_COLOR_BUFFER_BIT |
        GL_DEPTH_BUFFER_BIT
    );
}

void Renderer::EndFrame()
{
}

void Renderer::SetViewport(
    int width,
    int height)
{
    glViewport(
        0,
        0,
        width,
        height
    );
}

void Renderer::SetClearColor(
    float r,
    float g,
    float b,
    float a)
{
    m_ClearColor[0] = r;
    m_ClearColor[1] = g;
    m_ClearColor[2] = b;
    m_ClearColor[3] = a;
}

void Renderer::DrawMesh(
    const Mesh& mesh) const
{
    mesh.Draw();
}

void Renderer::DrawWorld(const World& world, const Shader& shader) const
{
    shader.Use();

    // Fog color HARUS identik dengan clear color, biar chunk jauh menyatu ke langit
    shader.SetVec3("u_FogColor", glm::vec3(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2]));

    // Fog mulai di 60% render distance, full di 90% — beri jarak transisi sebelum cutoff
    shader.SetFloat("u_FogStart", m_RenderDistance * 0.6f);
    shader.SetFloat("u_FogEnd",   m_RenderDistance * 0.9f);

    world.Render(shader);
}