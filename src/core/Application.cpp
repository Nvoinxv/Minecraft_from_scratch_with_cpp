#include "core/Application.h"

#include <GLFW/glfw3.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

Application::Application()
    :
    m_Window(
        1280,
        720,
        "Minecraft Alpha"),
    m_Camera(glm::vec3(
        0.0f,
        36.0f,
        8.0f)),
    m_IsRunning(false),
    m_TelemetryTimer(0.0f)
{
}

Application::~Application()
{
    std::cout
        << "[Application] Destructor called."
        << std::endl;

    Shutdown();
}

bool Application::Initialize()
{
    std::cout << "\n================================================================================\n";
    std::cout << "                 MINECRAFT ALPHA ENGINE - INITIALIZATION                        \n";
    std::cout << "================================================================================\n";
    std::cout << "[Application] 1/4 Creating GLFW Window (1280x720)..." << std::endl;

    if (!m_Window.Create())
    {
        std::cerr << "[Application] ERROR: Failed to create GLFW window!" << std::endl;
        return false;
    }
    m_Window.PollEvents();
    std::cout << "[Application]     -> Window successfully created and context active." << std::endl;

    std::cout << "[Application] 2/4 Initializing Time System..." << std::endl;
    Time::Init();
    std::cout << "[Application]     -> Time initialized (FPS & DeltaTime tracking active)." << std::endl;

    std::cout << "[Application] 3/4 Initializing Input Handlers..." << std::endl;
    Input::Initialize(
        m_Window.GetNativeWindow());
    std::cout << "[Application]     -> Keyboard, Mouse, and Scroll callbacks registered." << std::endl;

    std::cout << "[Application] 4/4 Initializing Renderer..." << std::endl;
    if (!m_Renderer.Initialize())
    {
        std::cerr << "[Application] ERROR: Renderer initialization failed!" << std::endl;
        return false;
    }
    std::cout << "[Application]     -> Renderer initialized successfully." << std::endl;

    std::cout << "[Application] 5/6 Initializing Block Registry & Loading JSON definitions..." << std::endl;
    if (!BlockRegistry::Get().Initialize("data/blocks"))
    {
        std::cerr << "[Application] WARNING: BlockRegistry initialization encountered issues or used fallbacks." << std::endl;
    }

    std::cout << "[Application] 6/6 Loading 3D Voxel Shaders & Generating World..." << std::endl;
    if (!m_Shader.Load("assets/shaders/block.vert", "assets/shaders/block.frag"))
    {
        std::cerr << "[Application] ERROR: Failed to load block shaders!" << std::endl;
        return false;
    }
    m_World.Initialize();

    m_IsRunning = true;
    m_TelemetryTimer = 0.0f;

    std::cout << "================================================================================\n";
    std::cout << "[Application] ALL SYSTEMS READY! Entering game loop...\n";
    std::cout << "[Info Kontrol] W/A/S/D = Gerak | Mouse = Lihat | TAB = Toggle Kursor | F3 = Debug | ESC = Keluar\n";
    std::cout << "================================================================================\n\n";

    return true;
}

void Application::Run()
{
    std::cout << "[Application] Main loop started. Enjoy Minecraft Alpha!" << std::endl;

    while (m_IsRunning &&
           !m_Window.ShouldClose())
    {
        m_Window.PollEvents();

        Time::Update();

        Input::Update();

        Update();

        Render();

        m_Window.SwapBuffers();
    }

    std::cout << "\n[Application] Main loop finished." << std::endl;
}

void Application::Update()
{
    // Periksa tombol ESC untuk keluar game dengan cepat
    if (Input::IsKeyPressed(GLFW_KEY_ESCAPE))
    {
        std::cout << "[Input Event] Tombol ESC ditekan. Menutup aplikasi..." << std::endl;
        m_IsRunning = false;
        glfwSetWindowShouldClose(m_Window.GetNativeWindow(), true);
        return;
    }

    // Tombol TAB untuk membebaskan / mengunci kembali kursor mouse
    if (Input::IsKeyPressed(GLFW_KEY_TAB))
    {
        int currentMode = glfwGetInputMode(m_Window.GetNativeWindow(), GLFW_CURSOR);
        if (currentMode == GLFW_CURSOR_DISABLED)
        {
            glfwSetInputMode(m_Window.GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            std::cout << "[Input Event] Tombol TAB: Kursor dibebaskan (Normal Mode)." << std::endl;
        }
        else
        {
            glfwSetInputMode(m_Window.GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            std::cout << "[Input Event] Tombol TAB: Kursor dikunci (FPS Look Mode)." << std::endl;
        }
    }

    // Pergerakan kamera/player berdasarkan input keyboard (W, S, A, D)
    m_Camera.ProcessKeyboard(
        Input::IsKeyDown(GLFW_KEY_W),
        Input::IsKeyDown(GLFW_KEY_S),
        Input::IsKeyDown(GLFW_KEY_A),
        Input::IsKeyDown(GLFW_KEY_D),
        Time::GetDeltaTime()
    );

    // Pergerakan sudut pandang kamera berdasarkan rotasi mouse (hanya saat kursor dikunci)
    if (glfwGetInputMode(m_Window.GetNativeWindow(), GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
    {
        m_Camera.ProcessMouse(
            static_cast<float>(Input::GetDeltaX()),
            static_cast<float>(Input::GetDeltaY())
        );

        // Interaksi: Menghancurkan (Kiri) atau Menaruh (Kanan) Blok
        bool leftClick = Input::IsMousePressed(GLFW_MOUSE_BUTTON_LEFT);
        bool rightClick = Input::IsMousePressed(GLFW_MOUSE_BUTTON_RIGHT);

        if (leftClick || rightClick)
        {
            // Sederhana: Raycast maju sejauh 5 blok
            glm::vec3 rayPos = m_Camera.Position;
            glm::vec3 rayDir = m_Camera.Front;
            float maxReach = 5.0f;
            float stepSize = 0.05f;
            
            glm::vec3 previousPoint = rayPos;

            for (float t = 0; t < maxReach; t += stepSize)
            {
                glm::vec3 currentPoint = rayPos + rayDir * t;
                int bx = static_cast<int>(std::floor(currentPoint.x));
                int by = static_cast<int>(std::floor(currentPoint.y));
                int bz = static_cast<int>(std::floor(currentPoint.z));

                if (m_World.GetBlockGlobal(bx, by, bz) != 0) // Menabrak blok solid
                {
                    if (leftClick)
                    {
                        // Lindungi Bedrock agar tidak bisa dihancurkan
                        uint8_t targetID = m_World.GetBlockGlobal(bx, by, bz);
                        uint8_t bedrockID = BlockRegistry::Get().GetBlockByName("bedrock").ID;
                        if (targetID == bedrockID)
                        {
                            std::cout << "[Gameplay] Bedrock terlalu keras untuk dihancurkan!" << std::endl;
                            break;
                        }

                        // 1. Hancurkan blok tersebut (set id = 0)
                        m_World.SetBlockGlobal(bx, by, bz, 0);
                        std::cout << "[Gameplay] Blok dihancurkan pada koordinat X:" << bx << " Y:" << by << " Z:" << bz << std::endl;
                    }
                    else if (rightClick)
                    {
                        // 1. Taruh blok di ruang kosong tepat SEBELUM menabrak blok (memakai previousPoint)
                        int px = static_cast<int>(std::floor(previousPoint.x));
                        int py = static_cast<int>(std::floor(previousPoint.y));
                        int pz = static_cast<int>(std::floor(previousPoint.z));
                        
                        // Jangan taruh balok jika pemain sedang berdiri di dalam koordinat tersebut (kasar checking)
                        if (py != static_cast<int>(std::floor(m_Camera.Position.y))) 
                        {
                            // Taruh balok Cobblestone (ID 4)
                            m_World.SetBlockGlobal(px, py, pz, 4);
                            std::cout << "[Gameplay] Blok ditaruh pada koordinat X:" << px << " Y:" << py << " Z:" << pz << std::endl;
                            
                            // Ganti target koordinat mesh update ke koordinat blok yang baru ditaruh
                            bx = px;
                            bz = pz;
                        }
                        else
                        {
                            break; // Pemain menghalangi
                        }
                    }

                    // 2. Perbarui mesh chunk utama tempat perubahan terjadi
                    int chunkX = static_cast<int>(std::floor(static_cast<float>(bx) / Chunk::CHUNK_WIDTH));
                    int chunkZ = static_cast<int>(std::floor(static_cast<float>(bz) / Chunk::CHUNK_DEPTH));
                    Chunk* targetChunk = m_World.GetChunk(chunkX, chunkZ);
                    if (targetChunk) targetChunk->GenerateMesh(&m_World);

                    // 3. Perbarui mesh chunk tetangga jika blok berada di perbatasan chunk (agar culling face sinkron)
                    int localX = bx - chunkX * Chunk::CHUNK_WIDTH;
                    int localZ = bz - chunkZ * Chunk::CHUNK_DEPTH;

                    if (localX == 0) {
                        Chunk* neighbor = m_World.GetChunk(chunkX - 1, chunkZ);
                        if (neighbor) neighbor->GenerateMesh(&m_World);
                    } else if (localX == Chunk::CHUNK_WIDTH - 1) {
                        Chunk* neighbor = m_World.GetChunk(chunkX + 1, chunkZ);
                        if (neighbor) neighbor->GenerateMesh(&m_World);
                    }

                    if (localZ == 0) {
                        Chunk* neighbor = m_World.GetChunk(chunkX, chunkZ - 1);
                        if (neighbor) neighbor->GenerateMesh(&m_World);
                    } else if (localZ == Chunk::CHUNK_DEPTH - 1) {
                        Chunk* neighbor = m_World.GetChunk(chunkX, chunkZ + 1);
                        if (neighbor) neighbor->GenerateMesh(&m_World);
                    }

                    break; // Berhenti mengecek setelah satu blok berhasil dihancurkan/ditaruh
                }
                
                previousPoint = currentPoint; // Simpan titik history untuk deteksi ruang kosong
            }
        }
    }

    // Perbarui HUD Telemetri (Window Title dan Console Debug)
    UpdateTelemetry();

    //---------------------------------------
    // World
    //---------------------------------------

    m_World.Update(Time::GetDeltaTime(), m_Camera.Position);

    //---------------------------------------
    // Entity
    //---------------------------------------

    // m_Player.Update();
}

void Application::UpdateTelemetry()
{
    // 1. Deteksi event input interaktif (Mouse Click & Scroll & F3) untuk log langsung di terminal
    if (Input::IsMousePressed(GLFW_MOUSE_BUTTON_LEFT))
    {
        std::cout << "[Input Event] Klik Kiri Mouse pada koordinat (" 
                  << Input::GetMouseX() << ", " << Input::GetMouseY() 
                  << ") | Posisi Player: [" << m_Camera.Position.x << ", " 
                  << m_Camera.Position.y << ", " << m_Camera.Position.z << "]\n";
    }

    if (Input::IsMousePressed(GLFW_MOUSE_BUTTON_RIGHT))
    {
        std::cout << "[Input Event] Klik Kanan Mouse pada koordinat (" 
                  << Input::GetMouseX() << ", " << Input::GetMouseY() 
                  << ") | Posisi Player: [" << m_Camera.Position.x << ", " 
                  << m_Camera.Position.y << ", " << m_Camera.Position.z << "]\n";
    }

    if (Input::GetScrollY() != 0.0)
    {
        std::cout << "[Input Event] Mouse Scroll terdeteksi: OffsetY = " << Input::GetScrollY() << "\n";
    }

    if (Input::IsKeyPressed(GLFW_KEY_F3))
    {
        std::cout << "[Debug Event] Tombol F3 Ditekan -> Snapshot Telemetri:\n";
        PrintTelemetryLog(true);
    }

    // 2. Update Window Title secara real-time setiap frame dengan informasi FPS dan Posisi Player
    std::stringstream titleStream;
    titleStream << std::fixed << std::setprecision(1);
    titleStream << "Minecraft Alpha | FPS: " << static_cast<int>(Time::GetFPS())
                << " (" << std::setprecision(2) << (Time::GetDeltaTime() * 1000.0f) << " ms) | "
                << std::setprecision(1)
                << "Pos: [" << m_Camera.Position.x << ", " << m_Camera.Position.y << ", " << m_Camera.Position.z << "] | "
                << "Rot: [Yaw: " << m_Camera.Yaw << "° Pitch: " << m_Camera.Pitch << "°]";
    m_Window.SetTitle(titleStream.str());

    // 3. Update Log Telemetri periodik ke Terminal / Console setiap 1.0 detik agar tidak membanjiri terminal
    m_TelemetryTimer += Time::GetDeltaTime();
    if (m_TelemetryTimer >= 1.0f)
    {
        PrintTelemetryLog(false);
        m_TelemetryTimer = 0.0f;
    }
}

void Application::PrintTelemetryLog(bool forceImmediate)
{
    float fps = Time::GetFPS();
    float dtMs = Time::GetDeltaTime() * 1000.0f;
    unsigned long long frameCount = Time::GetFrameCount();
    float totalTime = Time::GetTime();

    glm::vec3 pos = m_Camera.Position;
    glm::vec3 front = m_Camera.Front;

    // Menentukan status input pergerakan yang sedang aktif
    std::string activeMovements = "";
    if (Input::IsKeyDown(GLFW_KEY_W)) activeMovements += "[W: Maju] ";
    if (Input::IsKeyDown(GLFW_KEY_S)) activeMovements += "[S: Mundur] ";
    if (Input::IsKeyDown(GLFW_KEY_A)) activeMovements += "[A: Kiri] ";
    if (Input::IsKeyDown(GLFW_KEY_D)) activeMovements += "[D: Kanan] ";
    if (Input::IsKeyDown(GLFW_KEY_SPACE)) activeMovements += "[SPACE: Naik/Jump] ";
    if (Input::IsKeyDown(GLFW_KEY_LEFT_SHIFT)) activeMovements += "[SHIFT: Turun/Sneak] ";
    if (activeMovements.empty()) activeMovements = "Idle (Diam / Tidak Ada Tombol Pergerakan)";

    std::cout << "\n+==============================================================================+\n";
    std::cout << "| [MINECRAFT ALPHA TELEMETRY] - " << (forceImmediate ? "MANUAL DUMP (F3)" : "PERIODIC LOG (1.0s)") << "\n";
    std::cout << "+==============================================================================+\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "| Waktu Game   : " << totalTime << " s | Total Frame Rendered : " << frameCount << "\n";
    std::cout << "| FPS / Time   : " << static_cast<int>(fps) << " FPS (" << dtMs << " ms per frame)\n";
    std::cout << "+------------------------------------------------------------------------------+\n";
    std::cout << "| [PLAYER / CAMERA POSITION & ROTATION]\n";
    std::cout << "| Posisi (XYZ) : X: " << pos.x << " | Y: " << pos.y << " | Z: " << pos.z << "\n";
    std::cout << "| Rotasi Sudut : Yaw: " << m_Camera.Yaw << "° | Pitch: " << m_Camera.Pitch << "°\n";
    std::cout << "| Vektor Front : [" << front.x << ", " << front.y << ", " << front.z << "]\n";
    std::cout << "+------------------------------------------------------------------------------+\n";
    std::cout << "| [INPUT STATUS & PERGERAKAN]\n";
    std::cout << "| Status Gerak : " << activeMovements << "\n";
    std::cout << "| Kursor Mouse : (" << Input::GetMouseX() << ", " << Input::GetMouseY() << ")\n";
    std::cout << "| Delta Mouse  : dX: " << Input::GetDeltaX() << " | dY: " << Input::GetDeltaY() << "\n";
    std::cout << "+==============================================================================+\n";
}

void Application::Render()
{
    m_Renderer.BeginFrame();

    //---------------------------------------
    // World
    //---------------------------------------

    m_Shader.Use();
    glm::mat4 projection = glm::perspective(
        glm::radians(70.0f),
        static_cast<float>(m_Window.GetWidth()) / static_cast<float>(m_Window.GetHeight()),
        0.1f,
        500.0f
    );
    glm::mat4 view = m_Camera.GetViewMatrix();
    m_Shader.SetMat4("u_Projection", projection);
    m_Shader.SetMat4("u_View", view);

    m_Renderer.DrawWorld(m_World, m_Shader);

    //---------------------------------------
    // Player
    //---------------------------------------

    //---------------------------------------
    // UI
    //---------------------------------------

    m_Renderer.EndFrame();
}

void Application::Shutdown()
{
    if (!m_IsRunning)
    {
        std::cout
            << "[Application] Shutdown skipped."
            << std::endl;

        return;
    }

    std::cout << "\n================================================================================\n";
    std::cout << "                 MINECRAFT ALPHA ENGINE - SHUTTING DOWN                         \n";
    std::cout << "================================================================================\n";

    m_IsRunning = false;

    std::cout << "[Application] Shutting down World, Shader & BlockRegistry..." << std::endl;
    m_World.Shutdown();
    m_Shader.Destroy();
    BlockRegistry::Get().Shutdown();

    std::cout << "[Application] Shutting down Renderer..." << std::endl;
    m_Renderer.Shutdown();

    std::cout << "[Application] Destroying Window..." << std::endl;
    m_Window.Destroy();

    std::cout << "[Application] Shutdown complete. Sampai jumpa!" << std::endl;
    std::cout << "================================================================================\n";
}