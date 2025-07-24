#ifndef BOSS_HPP_GAM3
#define BOSS_HPP_GAM3

#include "raylib.h"
#include <expected>
#include "EnemyProjectileManager.hpp"

class Boss {
private:
    Texture sprites;
    Texture explosion_sprites;
    Vector2 position;
    Vector2 destination;
    Vector2 anchor_point;
    EnemyProjectileManager* projectile_manager;
    float shoot_timer;
    float animation_timer;
    int current_sprite_index = 0;
    bool alive;
    bool present;

    int health;
    constexpr static int max_health = 100;

    constexpr static float speed = 80.0f;

    void(Boss::*actions[])() = {
        &Boss::shoot,
        &Boss::charge_at_player,
        &Boss::spawn_enemies,
        &Boss::do_projectile_spam
    };

    constexpr static size_t no_actions = sizeof(this->actions) / sizeof(this->actions[0]);

    constexpr static ptrdiff_t current_action = -1;
    
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

    void kill() noexcept {
        present = false;
        current_sprite_index = 0;
    }

    void damage(int amount) noexcept;
    int get_health() const noexcept {
        return health;
    }

private:
    
    void display_health() const noexcept;
    void shoot() noexcept;
    void charge_at_player() noexcept;
    void spawn_enemies() noexcept;
    void do_projectile_spam() noexcept;
};

#endif