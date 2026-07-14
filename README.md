MinecraftCPP/
│
├── assets/
│   ├── textures/
│   │      atlas.png
│   │      blocks/
│   │
│   ├── shaders/
│   │      block.vert
│   │      block.frag
│   │
│   ├── fonts/
│   │
│   └── sounds/
│
├── data/
│   ├── blocks/
│   │      grass.json
│   │      dirt.json
│   │      stone.json
│   │
│   └── worlds/
│
├── docs/
│
├── include/
│
├── external/
│   ├── glad/
│   ├── glfw/
│   ├── glm/
│   ├── stb/
│   └── FastNoiseLite/
│
├── src/
│
│   ├── core/
│   │      Application.cpp
│   │      Window.cpp
│   │      Input.cpp
│   │      Time.cpp
│   │
│   ├── graphics/
│   │      Renderer.cpp
│   │      Shader.cpp
│   │      Texture.cpp
│   │      Camera.cpp
│   │      Mesh.cpp
│   │
│   ├── world/
│   │      World.cpp
│   │      Chunk.cpp
│   │      ChunkMesh.cpp
│   │      ChunkManager.cpp
│   │      WorldGenerator.cpp
│   │      Block.cpp
│   │
│   ├── blocks/
│   │      BlockRegistry.cpp
│   │      GrassBlock.cpp
│   │      StoneBlock.cpp
│   │      TorchBlock.cpp
│   │      WaterBlock.cpp
│   │
│   ├── player/
│   │      Player.cpp
│   │      Inventory.cpp
│   │      CameraController.cpp
│   │
│   ├── physics/
│   │      Collision.cpp
│   │      Raycast.cpp
│   │
│   ├── ui/
│   │      Crosshair.cpp
│   │      InventoryUI.cpp
│   │      Hotbar.cpp
│   │
│   ├── save/
│   │      SaveManager.cpp
│   │      WorldSerializer.cpp
│   │
│   ├── utils/
│   │      Math.cpp
│   │      Logger.cpp
│   │      Random.cpp
│   │
│   └── main.cpp
│
├── tests/
│
├── CMakeLists.txt
│
└── README.md

Jika mau melakukan running untuk eksekusi kodenya bisa melakukan seperti ini: 
g++ \
main.cpp \
src/glad.c \
src/core/*.cpp \
src/graphics/*.cpp \
world/*.cpp \
-Iinclude \
-lglfw \
-lGL \
-o minecraft