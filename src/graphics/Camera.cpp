#include "graphics/Camera.h"

// Ini titik spawn kamera di dunia game
Camera::Camera(glm::vec3 spawnPosition)
{
    // Set posisi awal
    // Gw buat rada acak
    Position = spawnPosition;
    
    // Set arah kamera menghadap ke depan
    Front = glm::vec3(0.0f, 0.0f, -1.0f);
    
    // Set arah atas kamera
    WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    Yaw = -90.0f;
    Pitch = 0.0f;

    Speed = 8.0f;
    Sensitivity = 0.1f;

    UpdateVectors();
}

// Render kamera berdasarkan input keyboard
glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(
        Position,
        Position + Front,
        Up
    );
}

// Bagian posisi mouse 
void Camera::ProcessMouse(float xoffset, float yoffset)
{
    xoffset *= Sensitivity;
    yoffset *= Sensitivity;

    Yaw += xoffset;

    Pitch += yoffset;

    if (Pitch > 89.0f)
        Pitch = 89.0f;

    if (Pitch < -89.0f)
        Pitch = -89.0f;

    UpdateVectors();
}

// Mengubah posisi kamera berdasarkan pemain lakukan
void Camera::UpdateVectors()
{
    glm::vec3 front;

    front.x =
        cos(glm::radians(Yaw))
        * cos(glm::radians(Pitch));

    front.y =
        sin(glm::radians(Pitch));

    front.z =
        sin(glm::radians(Yaw))
        * cos(glm::radians(Pitch));

    Front = glm::normalize(front);

    Right = glm::normalize(glm::cross(
        Front,
        WorldUp));

    Up = glm::normalize(glm::cross(
        Right,
        Front));
}

#include "world/World.h"
#include <cmath>

// Helper: cek tabrakan AABB di posisi pos.
// feetEpsilon: offset kecil dari bawah AABB — dipakai saat cek horizontal
// agar blok lantai yang sedang dipijak tidak ikut dihitung sebagai collision.
bool Camera::CheckCollision(const glm::vec3& pos, World* world, float feetEpsilon) const
{
    float minX = pos.x - PLAYER_R;
    float maxX = pos.x + PLAYER_R;
    float minY = pos.y - PLAYER_H + feetEpsilon;  // geser sedikit ke atas saat cek horizontal
    float maxY = pos.y + 0.2f;
    float minZ = pos.z - PLAYER_R;
    float maxZ = pos.z + PLAYER_R;

    int x0 = static_cast<int>(std::floor(minX));
    int x1 = static_cast<int>(std::floor(maxX));
    int y0 = static_cast<int>(std::floor(minY));
    int y1 = static_cast<int>(std::floor(maxY));
    int z0 = static_cast<int>(std::floor(minZ));
    int z1 = static_cast<int>(std::floor(maxZ));

    for (int x = x0; x <= x1; ++x)
        for (int y = y0; y <= y1; ++y)
            for (int z = z0; z <= z1; ++z)
                if (world->GetBlockGlobal(x, y, z) != 0)
                    return true;
    return false;
}

// Bagian melakukan pergerakan kamera berdasarkan input keyboard
void Camera::ProcessKeyboard(
    bool forward,
    bool backward,
    bool left,
    bool right,
    bool jump,
    float deltaTime,
    World* world)
{
    if (!world) {
        // Fallback: free-fly tanpa fisika
        float vel = Speed * deltaTime;
        if (forward)  Position += Front * vel;
        if (backward) Position -= Front * vel;
        if (left)     Position -= Right * vel;
        if (right)    Position += Right * vel;
        return;
    }

    // -------------------------------------------------------
    // Gerakan HORIZONTAL: gunakan flat front (tanpa komponen Y)
    // agar WASD selalu sejajar dengan bidang XZ, tidak terpengaruh
    // ke mana kamera menghadap secara vertikal (pitch).
    // -------------------------------------------------------
    glm::vec3 flatFront = glm::normalize(glm::vec3(Front.x, 0.0f, Front.z));
    glm::vec3 flatRight = glm::normalize(glm::vec3(Right.x, 0.0f, Right.z));

    float vel = Speed * deltaTime;
    glm::vec3 hMove(0.0f);
    if (forward)  hMove += flatFront * vel;
    if (backward) hMove -= flatFront * vel;
    if (left)     hMove -= flatRight * vel;
    if (right)    hMove += flatRight * vel;

    // Terapkan horizontal X dulu, rollback kalau nabrak
    // feetEpsilon=1.0f: geser bottom AABB 1 unit ke atas agar blok lantai
    // yang dipijak tidak dihitung — menghindari "frozen on ground" bug.
    Position.x += hMove.x;
    if (CheckCollision(Position, world, 1.0f))
        Position.x -= hMove.x;

    // Terapkan horizontal Z
    Position.z += hMove.z;
    if (CheckCollision(Position, world, 1.0f))
        Position.z -= hMove.z;

    // -------------------------------------------------------
    // Gravitasi & Lompat
    // -------------------------------------------------------
    // Cek apakah ada tanah tepat di bawah kaki pemain
    glm::vec3 belowPos = Position;
    belowPos.y -= 0.05f;
    OnGround = CheckCollision(belowPos, world) && (VelocityY <= 0.0f);

    // Lompat hanya bisa kalau di tanah
    if (jump && OnGround) {
        VelocityY = JUMP_FORCE;
        OnGround = false;
    }

    // Terapkan gravitasi ke velocity vertikal
    if (!OnGround)
        VelocityY += GRAVITY * deltaTime;

    // Terapkan gerakan vertikal, rollback jika nabrak
    float dy = VelocityY * deltaTime;
    Position.y += dy;
    if (CheckCollision(Position, world)) {
        Position.y -= dy;
        VelocityY = 0.0f;   // Reset velocity (baik mendarat maupun menabrak langit-langit)
    }
}