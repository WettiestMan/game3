#ifndef ENEMY_UFO_HPP_GAM3
#define ENEMY_UFO_HPP_GAM3

#include <raylib.h>
#include <expected>
#include <array>
#include "EnemyProjectileManager.hpp"

class EnemyUfo {
private:
    Texture sprites;
    Texture explosion_sprite;

    Vector2 position;
    Vector2 destination;
    float destination_selection_timer = 0.0f;
    constexpr static float destination_selection_interval = 4.0f; // seconds
    constexpr static float destination_selection_distance = 60.0f; // pixels
    constexpr static float sprites_offsets[] = {
        0, 32, 64, 96, 128, 160
    };

    constexpr static auto shoot_patterns = std::array{
        Vector2{0.0f, 1.0f}, // Straight down
        Vector2{-1.0f, 1.0f}, // Diagonal left down
        Vector2{1.0f, 1.0f} // Diagonal right down
    };

    int shoot_pattern_option = 0;
    EnemyProjectileManager* projectile_manager;
    constexpr static float shoot_delay = 1.0f; // seconds
    float shoot_timer;
    float animation_timer;
    int current_sprite_index = 0;

    bool alive;
    bool present;
    EnemyUfo() = default;

public:
    enum class Appeareance : int {
        left,
        right
    };

    constexpr static float speed = 100.0f;
    constexpr static int width = 32;
    constexpr static int height = 32;

    EnemyUfo(const EnemyUfo&) = default;
    EnemyUfo(EnemyUfo&&) = default;
    EnemyUfo& operator=(const EnemyUfo&) = default;
    EnemyUfo& operator=(EnemyUfo&&) = default;

    static std::expected<EnemyUfo, int> create(Vector2 pos, Texture sprite, Texture boom, Appeareance ap) noexcept;

    void update() noexcept;
    void draw() noexcept;

    Rectangle get_bounding_box() const noexcept {
        return Rectangle{position.x, position.y, static_cast<float>(width), static_cast<float>(height)};
    }
    bool is_alive() const noexcept { return alive; }
    bool is_present() const noexcept { return present; }
    void kill() noexcept { present = false; current_sprite_index = 0; }
};

#endif