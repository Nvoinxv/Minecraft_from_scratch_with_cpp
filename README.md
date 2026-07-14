<div align="center">

# ⛏️ MinecraftCPP

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

- 🌍 **Procedural World Generation** menggunakan FastNoiseLite
- 🧱 **Block Registry System** — mudah menambahkan tipe block baru
- 🎮 **Player Controller** lengkap dengan collision & raycast untuk block interaction
- 🖼️ **Custom Rendering Pipeline** (shader, texture atlas, chunk meshing)
- 💾 **Save/Load System** untuk menyimpan progres dunia
- 🧩 **UI Dasar** — crosshair, hotbar, dan inventory

---

## 📁 Struktur Proyek

```
MinecraftCPP/
│
├── assets/                 # Aset visual & audio
│   ├── textures/
│   │   ├── atlas.png
│   │   └── blocks/
│   ├── shaders/
│   │   ├── block.vert
│   │   └── block.frag
│   ├── fonts/
│   └── sounds/
│
├── data/                    # Data konfigurasi block & world
│   ├── blocks/
│   │   ├── grass.json
│   │   ├── dirt.json
│   │   └── stone.json
│   └── worlds/
│
├── docs/                     # Dokumentasi tambahan
├── include/                   # Header files
│
├── external/                   # Third-party libraries
│   ├── glad/
│   ├── glfw/
│   ├── glm/
│   ├── stb/
│   └── FastNoiseLite/
│
├── src/
│   ├── core/                    # Application loop, window, input, time
│   ├── graphics/                 # Renderer, shader, texture, camera, mesh
│   ├── world/                     # World, chunk, chunk mesh & manager, worldgen
│   ├── blocks/                     # Block registry & tipe-tipe block
│   ├── player/                      # Player, inventory, camera controller
│   ├── physics/                      # Collision & raycasting
│   ├── ui/                            # Crosshair, inventory UI, hotbar
│   ├── save/                           # Save manager & world serializer
│   ├── utils/                           # Math, logger, random helper
│   └── main.cpp
│
├── tests/                      # Unit test
├── CMakeLists.txt
└── README.md
```

---

## 🖼️ Cuplikan Konsep

<div align="center">

![Voxel World Concept](https://images.unsplash.com/photo-1560419015-7c427e8ae5ba?w=800&q=60)

*Ilustrasi konsep voxel world — texture atlas dan chunk mesh menjadi fondasi utama rendering.*

</div>

---

## ⚙️ Requirements

| Dependency | Keterangan |
|---|---|
| **Compiler** | GCC / Clang dengan dukungan C++17 |
| **GLFW** | Window & input handling |
| **GLAD** | OpenGL function loader |
| **GLM** | Math library untuk transformasi 3D |
| **OpenGL** | Minimal versi 3.3+ |

---

## 🚀 Cara Menjalankan

Clone repository ini terlebih dahulu, lalu compile menggunakan `g++`:

```bash
g++ \
main.cpp \
src/glad.c \
src/core/*.cpp \
src/graphics/*.cpp \
src/world/*.cpp \
-Iinclude \
-lglfw \
-lGL \
-o minecraft
```

Kemudian jalankan hasil build:

```bash
./minecraft
```

> 💡 Pastikan seluruh dependency (GLFW, GLAD, GLM) sudah terpasang dan path include-nya sesuai dengan struktur folder `external/`.

---

## 🗺️ Roadmap

- [x] Core rendering pipeline
- [x] Chunk generation & meshing
- [x] Player movement & collision
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