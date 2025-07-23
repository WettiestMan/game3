#ifndef PLAYER_HPP_GAM3
#define PLAYER_HPP_GAM3

#include <raylib.h>
#include <expected>
#include <vector>
#include "PlayerProjectile.hpp"

class Player {
private:
    Texture sprites;
    Texture projectile_sprite;
    bool alive;
    int health;
    Vector2 position;
    Vector2 original_position;
    float shoot_timer;
    float animation_timer;
    std::vector<PlayerProjectile> projectiles;
    constexpr static float sprites_offsets[] = {
        0, 32, 64, 96, 128, 160, 192, 224 
    };
    int current_sprite_index = 0;
    constexpr static int first_defeated_sprite_index = 2;
    constexpr static int speed = 100;
    constexpr static float shoot_delay = 0.3f; // seconds

public:
    constexpr static int width = 32;
    constexpr static int height = 32;
    constexpr static int max_health = 20;
    
    Player() = default;
    Player(const Player&) = default;
    Player(Player&&) = default;

    Player& operator=(const Player&) = default;
    Player& operator=(Player&&) = default;

    static std::expected<Player, int> create(Vector2 pos) noexcept;
    
    void update() noexcept;
    void draw() noexcept;
    void reset() noexcept;

    std::vector<PlayerProjectile>& get_projectiles() noexcept;

    bool is_alive() const noexcept {
        return alive;
    }

    void damage(int amount) noexcept;

    void kill() noexcept;

    Rectangle get_bounding_box() const noexcept {
        return Rectangle{position.x, position.y, static_cast<float>(width), static_cast<float>(height)};
    }

    Vector2 get_position() const noexcept {
        return position;
    }

private:
    void display_health() const noexcept;
};

#endif