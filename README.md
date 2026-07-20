<div align="center">

# ⛏️ Minecraft From Scratch By Nvoin With CPP

**A voxel-based sandbox game engine built from scratch in modern C++ & OpenGL**

`C++17` · `OpenGL 4.x` · `CMake / g++` · `MIT License` · `Status: Work In Progress`

</div>

---

## 📖 Tentang Proyek

**MinecraftCPP** adalah implementasi sederhana dari mekanisme voxel-world ala Minecraft, dibangun menggunakan **C++** dan **OpenGL**. Proyek ini dirancang sebagai eksplorasi arsitektur game engine: mulai dari chunk generation, block registry, rendering pipeline, UI, hingga sistem save/load dunia.

> Cocok untuk siapa pun yang ingin mempelajari cara kerja voxel engine dari nol — tanpa framework besar, hanya library inti seperti GLFW, GLAD, dan GLM.

---

## ✨ Fitur Utama

- 🎮 **Modular Gameplay & Game Modes** — Arsitektur gameplay modular dengan dukungan penuh untuk **Creative Mode**, **Survival Mode**, sistem **Inventory** & **Health**, serta manajemen **Player Controller** (`PlayerController`, `GameMode`, `CreativeMode`, `SurvivalMode`, `Inventory`, `Player`, `Health`)
- 🧱 **Block Interaction System** — Sistem interaksi block modular (`BlockBreaker`, `BlockInteraction`) untuk placing & breaking block secara real-time
- 🌍 **Procedural Voxel World** — Pembuatan dunia berbasis voxel procedural mulai dari manajemen **Block**, **Chunk**, **ChunkManager**, **ChunkMesh**, hingga **WorldGenerator**
- 🖥️ **UI System** — Main menu & elemen UI modular (`MainMenu`, `UIQuad`) dengan shader terpisah untuk UI dan crosshair
- 📊 **Real-time Telemetry & Debug HUD** — HUD dinamis pada Window Title serta log telemetri interaktif (`F3`) untuk memantau FPS, waktu frame (ms), rotasi kamera/player, koordinat (XYZ), dan status input pergerakan secara presisi
- 🖼️ **Custom Rendering Pipeline** — Sistem rendering modular (`Renderer`, `Camera`, `Mesh`, `Shader`, `Texture`) yang dibangun di atas OpenGL 3.3+ & GLM, dengan shader terpisah untuk block, crosshair, dan UI
- ⚡ **Core Engine Architecture** — Application loop modern (`Application`), manajemen windowing (`Window` via GLFW), handling input akurat (`Input`), dan pemantauan waktu (`Time`)
- 📦 **Data-Driven Block Registry** — Definisi block berbasis JSON (`data/blocks/*.json`) untuk bedrock, chest, cobblestone, dirt, grass, leafs, stone, torch, dan wood

---

## 📁 Struktur Proyek

Struktur folder telah diperbarui dan dirapikan dengan pemisahan modular antara header (`include/`) dan implementasi (`src/`), termasuk penambahan modul **gameplay** dan **ui**:

```
MinecraftCPP/
│
├── assets/                          # Game assets (textures, shaders, fonts, sounds)
│   ├── atlas.png
│   ├── blocks/
│   ├── fonts/
│   ├── shaders/
│   │   ├── block.frag / block.vert
│   │   ├── crosshair.frag / crosshair.vert
│   │   └── ui.frag / ui.fs / ui.vert / ui.vs
│   ├── sounds/
│   └── textures/
│       ├── blocks/                  # bedrock, chest, cobblestone, dirt, grass, leafs, stone, torch, wood
│       └── ui/                      # button, button_hover, dirt, title, quit_text, singleplayer_text
│
├── data/                            # Game data & world configuration
│   ├── blocks/                      # bedrock.json, chest.json, dirt.json, grass.json, dll.
│   └── worlds/
│
├── include/                         # Header files
│   ├── core/                        # Application.h, Input.h, Time.h, Window.h
│   ├── gameplay/                    # Gameplay systems
│   │   ├── controller/              # PlayerController.h
│   │   ├── gamemode/                # GameMode.h, CreativeMode.h, SurvivalMode.h
│   │   ├── interaction/             # BlockBreaker.h, BlockInteraction.h
│   │   └── player/                  # Player.h, Inventory.h, health.h
│   ├── graphics/                    # Camera.h, Mesh.h, Renderer.h, Shader.h, Texture.h
│   ├── ui/                          # MainMenu.h, UIQuad.h
│   ├── world/                       # Block.h, Chunk.h, ChunkMesh.h, World.h
│   ├── glad/                        # OpenGL loader headers
│   ├── KHR/                         # Khronos platform headers
│   ├── json/                        # JSON library
│   └── stb/                         # Image loading library
│
├── src/                             # Source files
│   ├── core/                        # Engine implementation
│   ├── gameplay/
│   │   ├── controller/              # Player controller implementation
│   │   ├── gamemode/                # Game mode implementation
│   │   ├── interaction/             # Block interaction implementation
│   │   └── player/                  # Health.cpp, Inventory.cpp, Player.cpp
│   ├── graphics/                    # Rendering implementation
│   ├── ui/                          # MainMenu.cpp, UIQuad.cpp
│   ├── world/                       # Block, Chunk, ChunkManager, ChunkMesh, World, WorldGenerator
│   └── glad.c                       # OpenGL loader implementation
│
├── main.cpp                         # Application entry point
├── dockerfile                       # Docker build configuration
├── docker-compose.yml               # Docker Compose configuration
└── README.md                        # Project documentation
```

---

## ⚙️ Requirements

| Dependency | Keterangan |
|---|---|
| **Compiler** | GCC / Clang / MSVC dengan dukungan C++17 |
| **GLFW** | Window & input handling |
| **GLAD** | OpenGL function loader |
| **GLM** | Math library untuk transformasi & vektor 3D |
| **OpenGL** | Minimal versi 3.3+ |

---

## 🚀 Cara Menjalankan

Clone repository ini terlebih dahulu, lalu compile menggunakan `g++` (atau compiler C++ pilihanmu):

```bash
g++ main.cpp \
src/glad.c \
src/core/*.cpp \
src/gameplay/controller/*.cpp \
src/gameplay/gamemode/*.cpp \
src/gameplay/interaction/*.cpp \
src/gameplay/player/*.cpp \
src/graphics/*.cpp \
src/world/*.cpp \
src/ui/*.cpp \
-Iinclude \
-lglfw \
-lGL \
-ldl \
-lpthread \
-o minecraft
```

Kemudian jalankan hasil build:

```bash
./minecraft
```

> 💡 Pastikan seluruh dependency (GLFW, GLAD, GLM) sudah terpasang di sistem dan parameter `-Iinclude` digunakan agar struktur modul header terbaca dengan benar.

---

## 🗺️ Roadmap

- [x] Core rendering pipeline
- [x] Chunk generation & meshing
- [x] Player movement & collision
- [x] Modular Gameplay (`CreativeMode`, `SurvivalMode`, `Inventory`, `Health`)
- [x] Block Interaction System (`BlockBreaker`, `BlockInteraction`)
- [x] UI System (`MainMenu`, `UIQuad`)
- [x] Real-time HUD & Diagnostic Telemetry (`F3` & Window Title)
- [ ] Multiplayer support
- [ ] Lighting engine (ambient occlusion)
- [ ] Mod/plugin system

---

## 🤝 Kontribusi

Kontribusi sangat terbuka! Silakan buat *pull request* atau buka *issue* untuk melaporkan bug maupun mengusulkan fitur baru.

## 📄 Lisensi

Proyek ini dilisensikan di bawah **MIT License** — bebas digunakan, dimodifikasi, dan didistribusikan.

---

<div align="center">

Dibuat dengan ❤️ menggunakan C++ & OpenGL

</div>