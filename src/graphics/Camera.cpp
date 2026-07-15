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

    /// -------------------------------------------------------
    // Gravitasi & Lompat
    // -------------------------------------------------------
    // Cek apakah ada tanah TEPAT DI BAWAH KAKI (bukan sepanjang seluruh badan).
    // Bug sebelumnya: box yang dicek terentang dari kaki sampai hampir ke level
    // mata (karena maxY = pos.y + 0.2 selalu dekat mata, feetEpsilon default 0
    // hanya menggeser minY dari kaki). Di tangga/step yang tidak rata, box
    // selebar itu gampang salah baca sehingga OnGround gagal balik true
    // setelah naik/turun blok, dan lompat berikutnya jadi terkunci.
    //
    // Perbaikan: pindahkan titik cek ke level kaki (Position.y - PLAYER_H),
    // lalu pakai feetEpsilon supaya box yang dicek jadi TIPIS (~0.25 unit)
    // dan benar-benar hanya menyentuh area kaki, bukan seluruh badan.
    glm::vec3 feetPos = Position;
    feetPos.y -= PLAYER_H;
    OnGround = CheckCollision(feetPos, world, PLAYER_H - 0.05f) && (VelocityY <= 0.0f);

    if (OnGround)
    {
        // Reset jatah lompat tambahan tiap kali benar-benar menapak tanah
        m_AirJumpsUsed = 0;
    }

    if (jump)
    {
        if (OnGround)
        {
            // Lompat dari tanah - bisa berkali-kali selama masih menapak,
            // tidak dibatasi jumlah total (survival maupun creative).
            VelocityY = JUMP_FORCE;
            OnGround = false;
        }
        else if (IsCreative && m_AirJumpsUsed < MAX_AIR_JUMPS)
        {
            // Khusus Creative: lompat kedua di udara (melayang/double jump).
            // Survival tidak masuk cabang ini sama sekali -> tidak bisa lompat di udara.
            VelocityY = JUMP_FORCE;
            m_AirJumpsUsed++;
        }
        // Survival + sudah di udara, atau creative tapi jatah air-jump habis -> diabaikan
    }

    // Terapkan gravitasi ke velocity vertikal
    if (!OnGround)
        VelocityY += GRAVITY * deltaTime;

    // Cap kecepatan jatuh (terminal velocity). Tanpa ini, VelocityY membesar
    // tanpa batas selama jatuh -> dy per frame makin besar -> gap collision
    // residual (lihat di bawah) ikut membesar tanpa batas -> OnGround bisa
    // gagal terdeteksi permanen setelah jatuh dari ketinggian.
    if (VelocityY < TERMINAL_VELOCITY)
        VelocityY = TERMINAL_VELOCITY;

    // Terapkan gerakan vertikal secara BERTAHAP (substep), bukan sekaligus.
    // Alasan: rollback "all-or-nothing" sebelumnya membatalkan SELURUH dy
    // saat kena collision, menyisakan gap antara kaki dan lantai sebesar
    // |dy| penuh — bisa besar kalau VelocityY tinggi. Substep kecil (maks
    // 0.05 unit per langkah) membuat gap residual selalu kecil dan konstan,
    // independen dari seberapa cepat pemain jatuh.
    float dyTotal = VelocityY * deltaTime;
    const float maxStep = 0.05f;
    int steps = static_cast<int>(std::ceil(std::abs(dyTotal) / maxStep));
    if (steps < 1) steps = 1;
    if (steps > 64) steps = 64; // Jaga-jaga kalau deltaTime melonjak (lag spike)

    float dyStep = dyTotal / static_cast<float>(steps);
    for (int i = 0; i < steps; ++i)
    {
        Position.y += dyStep;
        if (CheckCollision(Position, world)) {
            Position.y -= dyStep;
            VelocityY = 0.0f;
            break;
        }
    }
}