#include "ui/MainMenu.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

MainMenu::MainMenu() {}
MainMenu::~MainMenu() {}

void MainMenu::Initialize() {
    m_UIQuad.Initialize();
    
    if (!m_UIShader.Compile("assets/shaders/ui.vs", "assets/shaders/ui.fs")) {
        std::cerr << "[MainMenu] Failed to compile UI shaders." << std::endl;
    }

    m_TexTitle = std::make_shared<Texture>("assets/textures/ui/title.png");
    m_TexButton = std::make_shared<Texture>("assets/textures/ui/button.png");
    m_TexButtonHover = std::make_shared<Texture>("assets/textures/ui/button_hover.png");
    m_TexSingleplayer = std::make_shared<Texture>("assets/textures/ui/singleplayer_text.png");
    m_TexQuit = std::make_shared<Texture>("assets/textures/ui/quit_text.png");
    m_TexBackground = std::make_shared<Texture>("assets/textures/ui/dirt.png");
}

void MainMenu::Shutdown() {
    m_UIShader.Destroy();
    m_UIQuad.Shutdown();
}

MainMenuAction MainMenu::Update(int mouseX, int mouseY, bool isMouseClicked, int screenWidth, int screenHeight) {
    if (!isMouseClicked) return MainMenuAction::None;

    float sw = static_cast<float>(screenWidth);
    float sh = static_cast<float>(screenHeight);

    float btnW = 400.0f;
    float btnH = 50.0f;
    float btnX = (sw - btnW) / 2.0f;
    float singleBtnY = sh / 2.0f;
    float quitBtnY = singleBtnY + btnH + 20.0f;

    if (mouseX >= btnX && mouseX <= btnX + btnW && mouseY >= singleBtnY && mouseY <= singleBtnY + btnH) {
        return MainMenuAction::Play;
    }
    else if (mouseX >= btnX && mouseX <= btnX + btnW && mouseY >= quitBtnY && mouseY <= quitBtnY + btnH) {
        return MainMenuAction::Quit;
    }

    return MainMenuAction::None;
}

void MainMenu::Render(int mouseX, int mouseY, int screenWidth, int screenHeight) {
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float sw = static_cast<float>(screenWidth);
    float sh = static_cast<float>(screenHeight);
    glm::mat4 projection = glm::ortho(0.0f, sw, sh, 0.0f, -1.0f, 1.0f);

    // Draw tiled background
    float bgTileSize = 128.0f;
    for (float y = 0; y < sh; y += bgTileSize) {
        for (float x = 0; x < sw; x += bgTileSize) {
            m_UIQuad.Draw(m_UIShader, projection, x, y, bgTileSize, bgTileSize, m_TexBackground);
        }
    }

    // Darken the background slightly
    m_UIQuad.Draw(m_UIShader, projection, 0, 0, sw, sh, nullptr, glm::vec4(0.0f, 0.0f, 0.0f, 0.5f));

    // Logo (Diperbaiki aspect ratio agar tidak gepeng)
    float logoW = sw * 0.8f;
    if (logoW > 600.0f) logoW = 600.0f;
    
    // Gunakan aspect ratio asli dari gambar
    float logoH = logoW * 0.25f; // Fallback jika texture null
    if (m_TexTitle && m_TexTitle->IsValid()) {
        float aspect = static_cast<float>(m_TexTitle->GetWidth()) / static_cast<float>(m_TexTitle->GetHeight());
        logoH = logoW / aspect;
    }
    
    float logoX = (sw - logoW) / 2.0f;
    float logoY = sh * 0.15f;
    m_UIQuad.Draw(m_UIShader, projection, logoX, logoY, logoW, logoH, m_TexTitle, glm::vec4(1.0f), true);

    // Buttons
    float btnW = 400.0f;
    float btnH = 50.0f;
    float btnX = (sw - btnW) / 2.0f;
    float singleBtnY = sh / 2.0f;
    float quitBtnY = singleBtnY + btnH + 20.0f;
    
    bool hoverSingle = (mouseX >= btnX && mouseX <= btnX + btnW && mouseY >= singleBtnY && mouseY <= singleBtnY + btnH);
    bool hoverQuit = (mouseX >= btnX && mouseX <= btnX + btnW && mouseY >= quitBtnY && mouseY <= quitBtnY + btnH);

    // Draw button backgrounds
    m_UIQuad.Draw(m_UIShader, projection, btnX, singleBtnY, btnW, btnH, hoverSingle ? m_TexButtonHover : m_TexButton);
    m_UIQuad.Draw(m_UIShader, projection, btnX, quitBtnY, btnW, btnH, hoverQuit ? m_TexButtonHover : m_TexButton);

    // Draw button texts
    float textW = btnW * 0.8f;
    float textH = btnH * 0.6f;
    
    // Perbaiki aspect ratio text juga (opsional, tapi lebih baik)
    if (m_TexSingleplayer && m_TexSingleplayer->IsValid()) {
        float aspect = static_cast<float>(m_TexSingleplayer->GetWidth()) / static_cast<float>(m_TexSingleplayer->GetHeight());
        float targetH = textW / aspect;
        if (targetH < textH) { textH = targetH; }
        else { textW = textH * aspect; } // Fit inside
    }
    float textX = btnX + (btnW - textW) / 2.0f;
    float textY = singleBtnY + (btnH - textH) / 2.0f;
    m_UIQuad.Draw(m_UIShader, projection, textX, textY, textW, textH, m_TexSingleplayer, glm::vec4(1.0f), true);
    
    textW = btnW * 0.8f;
    textH = btnH * 0.6f;
    if (m_TexQuit && m_TexQuit->IsValid()) {
        float aspect = static_cast<float>(m_TexQuit->GetWidth()) / static_cast<float>(m_TexQuit->GetHeight());
        float targetH = textW / aspect;
        if (targetH < textH) { textH = targetH; }
        else { textW = textH * aspect; }
    }
    textX = btnX + (btnW - textW) / 2.0f;
    textY = quitBtnY + (btnH - textH) / 2.0f;
    m_UIQuad.Draw(m_UIShader, projection, textX, textY, textW, textH, m_TexQuit, glm::vec4(1.0f), true);

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}
