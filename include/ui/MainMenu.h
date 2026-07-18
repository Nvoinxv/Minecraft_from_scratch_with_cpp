#pragma once
#include <memory>
#include "../graphics/Texture.h"
#include "../graphics/Shader.h"
#include "UIQuad.h"

enum class MainMenuAction {
    None,
    Play,
    Quit
};

class MainMenu {
public:
    MainMenu();
    ~MainMenu();
    
    void Initialize();
    void Shutdown();
    
    // Returns action if button clicked
    MainMenuAction Update(int mouseX, int mouseY, bool isMouseClicked, int screenWidth, int screenHeight);
    
    void Render(int mouseX, int mouseY, int screenWidth, int screenHeight);
private:
    UIQuad m_UIQuad;
    Shader m_UIShader;
    std::shared_ptr<Texture> m_TexTitle;
    std::shared_ptr<Texture> m_TexButton;
    std::shared_ptr<Texture> m_TexButtonHover;
    std::shared_ptr<Texture> m_TexSingleplayer;
    std::shared_ptr<Texture> m_TexQuit;
    std::shared_ptr<Texture> m_TexBackground;
};
