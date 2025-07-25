#ifndef BOSS_HPP_GAM3
#define BOSS_HPP_GAM3

#include "raylib.h"
#include <expected>
#include "EnemyProjectileManager.hpp"
#include "EnemyUfoManager.hpp"
#include "EnemyFighterManager.hpp"
#include <cmath>

class Boss {
private:
    Texture sprites;
    Texture defeated_sprites;
    Vector2 position;
    Vector2 destination;
    Vector2 anchor_point;
    EnemyProjectileManager* projectile_manager;
    EnemyUfoManager* ufo_manager;
    EnemyFighterManager* fighter_manager;

    constexpr static float sprites_offsets[] = {
        0, 64, 128, 192, 256, 320, 384, 448, 512, 576, 640, 704, 768, 832
    };

    float shoot_timer;
    float animation_timer;
    float alternative_animation_timer = 0.0f;
    int current_sprite_index = 0;
    int boss_idle_sprite_index_when_defeated = 1;
    bool alive;
    bool present;
    bool in_defeated_animation_loop;
    constexpr static int in_defeated_animation_loop_times = 10;
    int in_defeated_animation_loop_counter = 0;
    constexpr static ptrdiff_t defeated_sprite_loop_index_amount = 4;
    constexpr static ptrdiff_t defeated_sprite_loop_index_min = 7;
    constexpr static ptrdiff_t defeated_sprite_loop_index_max =
            defeated_sprite_loop_index_min + defeated_sprite_loop_index_amount;

    
    constexpr static int barrages_for_do_projectile_spam = 12;
    constexpr static float time_after_barrage_to_next = 0.5f;
    float current_time_after_barrage = 0;
    int barrages_counter = 0;

    constexpr static int num_directions = 12;

    // [0] = primer set (0°), [1] = segundo set (15°)
    const static Vector2 projectile_directions[2][num_directions];
    int projectile_directions_set_alternator = 0;

    constexpr static int shots_ammo_for_shoot = 16;
    constexpr static float time_after_shots = 0.3f;
    float current_time_after_shots = 0;
    int shots_counter = 0;

    constexpr static int enemies_amount_for_spawn = 4;
    bool in_spawn_animation_startup = true;

    constexpr static int no_spins_for_charge = 16;
    constexpr static float charge_speed = 120.0f;
    constexpr static float homing_strength = 0.4f;
    constexpr static float time_pursuit_limit = 3.0f;
    int spins_counter = 0;
    float time_pursuing = 0.0f;

    enum class Status {
        idle,
        in_starting_animation,
        doing_action,
        returning_to_idle
    };

    Status status = Status::in_starting_animation;

    int health;
    constexpr static int max_health = 150;

    constexpr static float speed = 80.0f;

    constexpr static size_t no_actions = 4;
    void(Boss::*actions[no_actions])();
    constexpr static float action_interval = 6.0f;

    float action_timer = 0.0f;

    ptrdiff_t current_action = -1;
    
    Boss() = default;
public:
    constexpr static int width = 64;
    constexpr static int height = 64;

    Boss(const Boss&) = default;
    Boss(Boss&&) = default;
    Boss& operator=(const Boss&) = default;
    Boss& operator=(Boss&&) = default;

    static std::expected<Boss, int> create() noexcept;

    void update() noexcept;
    void draw() const noexcept;
    void reset() noexcept;

    Rectangle get_bounding_box() const noexcept {
        return Rectangle{position.x, position.y,
        static_cast<float>(width), static_cast<float>(height)};
    }

    bool is_alive() const noexcept {
        return alive;
    }

    bool is_present() const noexcept {
        return present;
    }

    void kill() noexcept;

    void damage(int amount) noexcept;
    int get_health() const noexcept {
        return health;
    }

    constexpr int get_damage() const noexcept {
        return 6;
    }

private:
    
    void display_health() const noexcept;
    void shoot() noexcept;
    void charge_at_player() noexcept;
    void spawn_enemies() noexcept;
    void do_projectile_spam() noexcept;
};

#endif