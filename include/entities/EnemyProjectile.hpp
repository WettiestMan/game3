#ifndef ENEMY_PROJECTILE_HPP_GAM3
#define ENEMY_PROJECTILE_HPP_GAM3

#include <raylib.h>
#include <expected>

class EnemyProjectile {
private:
    Texture sprite;
    Vector2 position;
    Vector2 direction;
    bool alive;
    EnemyProjectile() = default;

public:
    constexpr static float speed = 200.0f;
    constexpr static int width = 8;
    constexpr static int height = 8;

    EnemyProjectile(const EnemyProjectile&) = default;
    EnemyProjectile(EnemyProjectile&&) = default;
    EnemyProjectile& operator=(const EnemyProjectile&) = default;
    EnemyProjectile& operator=(EnemyProjectile&&) = default;

    static std::expected<EnemyProjectile, int> create(Vector2 pos, Texture sprite, Vector2 direction) noexcept;

    void update() noexcept;
    void draw() noexcept;
    void reset() noexcept;

    bool is_alive() const noexcept { return alive; }
    void kill() noexcept { alive = false; }
    int get_damage() const noexcept { return 1; } // Assuming a fixed damage value
    Rectangle get_bounding_box() const noexcept {
        return Rectangle{position.x, position.y, static_cast<float>(width), static_cast<float>(height)};
    }
};


#endif