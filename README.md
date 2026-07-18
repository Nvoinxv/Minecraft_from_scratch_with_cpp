<div align="center">

# ⛏️ Minecraft From Scratch By Nvoin With CPP

**A voxel-based sandbox game engine built from scratch in modern C++ & OpenGL**

![C++](https://img.shields.io/badge/C%2B%2B-17-00599C?style=for-the-badge&logo=cplusplus)
![OpenGL](https://img.shields.io/badge/OpenGL-4.x-5586A4?style=for-the-badge&logo=opengl)
![CMake](https://img.shields.io/badge/Build-CMake-064F8C?style=for-the-badge&logo=cmake)
![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-Work%20In%20Progress-yellow?style=for-the-badge)

</div>

---

## 📖 Tentang Proyek

**MinecraftCPP** adalah implementasi sederhana dari mekanisme voxel-world ala Minecraft, dibangun menggunakan **C++** dan **OpenGL**. Proyek ini dirancang sebagai eksplorasi arsitektur game engine: mulai dari chunk generation, block registry, rendering pipeline, hingga sistem save/load dunia.

> Cocok untuk siapa pun yang ingin mempelajari cara kerja voxel engine dari nol — tanpa framework besar, hanya library inti seperti GLFW, GLAD, dan GLM.

---

## ✨ Fitur Utama

- 🎮 **Modular Gameplay & Game Modes** — Arsitektur gameplay modular dengan dukungan penuh untuk **Creative Mode**, **Survival Mode**, sistem **Inventory**, serta manajemen **Player Controller** (`PlayerController`, `GameMode`, `CreativeMode`, `SurvivalMode`, `Inventory`)
- 🌍 **Procedural Voxel World** — Pembuatan dunia berbasis voxel procedural mulai dari manajemen **Block**, **Chunk generation**, **ChunkManager**, **ChunkMesh**, hingga **WorldGenerator**
- 📊 **Real-time Telemetry & Debug HUD** — HUD dinamis pada Window Title serta log telemetri interaktif (`F3`) untuk memantau FPS, waktu frame (ms), rotasi kamera/player, koordinat (XYZ), dan status input pergerakan secara presisi
- 🖼️ **Custom Rendering Pipeline** — Sistem rendering modular (Renderer, Camera, Mesh, Shader, Texture) yang dibangun di atas OpenGL 3.3+ & GLM
- ⚡ **Core Engine Architecture** — Application loop modern (`Application`), manajemen windowing (`Window` via GLFW), handling input akurat (`Input`), dan pemantauan waktu (`Time`)

---

## 📁 Struktur Proyek

Struktur folder telah diperbarui dan dirapikan dengan pemisahan modular antara header (`include/`) dan implementasi (`src/`), serta penambahan modul **gameplay**:

```
MinecraftCPP/
│
├── assets/                          # Game assets (textures, shaders, fonts, sounds)
│   ├── blocks/
│   ├── fonts/
│   ├── shaders/
│   ├── sounds/
│   └── textures/
│
├── data/                            # Game data & world configuration
│   ├── blocks/
│   └── worlds/
│
├── include/                         # Header files
│   ├── core/                        # Core engine
│   ├── gameplay/                    # Gameplay systems
│   │   ├── controller/
│   │   ├── gamemode/
│   │   ├── interaction/
│   │   └── player/
│   ├── graphics/                    # Rendering system
│   ├── world/                       # Voxel world system
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
│   │   └── player/                  # Player systems implementation
│   ├── graphics/                    # Rendering implementation
│   ├── world/                       # World generation & chunk system
│   └── glad.c                       # OpenGL loader implementation
│
├── main.cpp                         # Application entry point
├── dockerfile                       # Docker build configuration
├── docker-compose.yml               # Docker Compose configuration
├── README.md                        # Project documentation
└── minecraft                        # Compiled executable (generated after build)
```

---

## 🖼️ Cuplikan Konsep

<div align="center">

![Voxel World Concept](https://images.unsplash.com/photo-1560419015-7c427e8ae5ba?w=800&q=60)

*Ilustrasi konsep voxel world — kolaborasi modular antara core engine, rendering pipeline, world generation, dan sistem gameplay.*

</div>

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
- [x] Modular Gameplay (`CreativeMode`, `SurvivalMode`, `Inventory`)
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
