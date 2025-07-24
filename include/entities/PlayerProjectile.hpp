#ifndef PLAYER_PROYECTILE_HPP_GAM3
#define PLAYER_PROYECTILE_HPP_GAM3

#include <raylib.h>
#include <expected>

class PlayerProjectile {
private:
    Texture sprite;
    Vector2 position;
    bool alive;
    PlayerProjectile() = default;

public:
    constexpr static float speed = 320.0f;
    constexpr static int width = 8;
    constexpr static int height = 8;

    PlayerProjectile(const PlayerProjectile&) = default;
    PlayerProjectile(PlayerProjectile&&) = default;
    PlayerProjectile& operator=(const PlayerProjectile&) = default;
    PlayerProjectile& operator=(PlayerProjectile&&) = default;

    static std::expected<PlayerProjectile, int> create(Vector2 pos, Texture sprite) noexcept;

    void update() noexcept;
    void draw() noexcept;
    void reset() noexcept;

    Rectangle get_bounding_box() const noexcept {
        return Rectangle{position.x, position.y, static_cast<float>(width), static_cast<float>(height)};
    };

    constexpr int get_damage() const noexcept {
        return 1;
    }

    bool is_alive() const noexcept { return alive; }
    void kill() noexcept { alive = false; }
};

#endif