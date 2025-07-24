#ifndef ENEMY_FIGHTER_HPP_GAM3
#define ENEMY_FIGHTER_HPP_GAM3

#include <raylib.h>
#include <expected>
#include "EnemyProjectileManager.hpp"
#include <cmath>

class EnemyFighter {
private:
    Texture sprites;
    Texture explosion_sprites;

    Vector2 position;
    Vector2 anchor_point;
    Vector2 destination;

    int ammo;

    float shoot_timer;
    float animation_timer;

    EnemyProjectileManager* projectile_manager;

    bool alive;
    bool present;

    int current_sprite_index = 0;

    constexpr static float normal_sprites_offsets[] = {
        0, 32
    };

    constexpr static float explosion_sprites_offsets[] = {
        0, 32, 64, 96, 128, 160
    };

    constexpr static float shoot_interval = 3.0f;
    constexpr static int max_ammo = 8;
    constexpr static int fighter_damage = 6;

    constexpr static float speed = 200.0f;

    constexpr static float spawn_time_max = 5.0f;
    constexpr static float spawn_time_min = 2.75f;
    constexpr static float spawn_interval_decrease = 0.25f;
    static float current_spawn_time;

    EnemyFighter() = default;
public:
    constexpr static int width = 32;
    constexpr static int height = 32;

    EnemyFighter(const EnemyFighter&) = default;
    EnemyFighter(EnemyFighter&&) = default;
    EnemyFighter& operator=(const EnemyFighter&) = default;
    EnemyFighter& operator=(EnemyFighter&&) = default;

    static std::expected<EnemyFighter, int> create(Vector2 position, Texture normal, Texture boom) noexcept;

    void update() noexcept;
    void draw() noexcept;

    Rectangle get_bounding_box() const noexcept {
        return Rectangle{position.x, position.y, static_cast<float>(width), static_cast<float>(height)};
    }

    bool is_alive() const noexcept { return alive; }
    bool is_present() const noexcept { return present; }
    void kill() noexcept { present = false; current_sprite_index = 0; }

    int get_damage() const noexcept { return fighter_damage; }

    static float get_spawn_time() noexcept { return current_spawn_time; }
    constexpr static float get_spawn_interval() noexcept { return spawn_interval_decrease; }
    static void reduce_spawn_time_by_interval() noexcept { current_spawn_time -= spawn_interval_decrease; }
    constexpr static float get_max_spawn_time() noexcept { return spawn_time_max; }
    constexpr static float get_min_spawn_time() noexcept { return spawn_time_min; }
};

#endif