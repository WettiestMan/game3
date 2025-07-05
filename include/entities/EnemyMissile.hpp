#ifndef ENEMY_MISSILE_HPP_GAM3
#define ENEMY_MISSILE_HPP_GAM3

#include <raylib.h>
#include <expected>

#include "Player.hpp"

class EnemyMissile {
private:
    Texture sprite;
    Vector2 position;
    Vector2 direction;
    const Player* target;
    bool alive;
    EnemyMissile() = default;

public:
    constexpr static float speed = 200.0f;
    constexpr static int width = 16;
    constexpr static int height = 26;

    EnemyMissile(const EnemyMissile&) = default;
    EnemyMissile(EnemyMissile&&) = default;
    EnemyMissile& operator=(const EnemyMissile&) = default;
    EnemyMissile& operator=(EnemyMissile&&) = default;

    static std::expected<EnemyMissile, int> create(Vector2 pos, Texture sprite, const Player* target) noexcept;

    void update() noexcept;
    void draw() noexcept;
    void reset() noexcept;

    bool is_alive() const noexcept { return alive; }
    void kill() noexcept { alive = false; }
    int get_damage() const noexcept { return 4; } // Assuming a fixed damage value
    Rectangle get_bounding_box() const noexcept {
        return Rectangle{position.x, position.y, static_cast<float>(width), static_cast<float>(height)};
    }
};

#endif