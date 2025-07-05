#ifndef ENEMY_PROJECTILE_MANAGER_HPP_GAM3
#define ENEMY_PROJECTILE_MANAGER_HPP_GAM3

#include <raylib.h>
#include <vector>
#include <variant>
#include <concepts>

#include "EnemyProjectile.hpp"
#include "EnemyMissile.hpp"

template<typename T>
concept RenderableEnemyProjectile = requires(T ep) {
    { ep.update() } -> std::same_as<void>;
    { ep.draw() } -> std::same_as<void>;
    { ep.is_alive() } -> std::same_as<bool>;
    { ep.kill() } -> std::same_as<void>;
    { ep.get_bounding_box() } -> std::same_as<Rectangle>;
    { ep.get_damage() } -> std::same_as<int>;
};

template<RenderableEnemyProjectile... Ts>
using EnemyProjectileList = std::variant<Ts...>;

using EnemyProjectileEntity = EnemyProjectileList<
            EnemyProjectile,
            EnemyMissile>;

inline void update_enemy_projectile(EnemyProjectileEntity& projectile) noexcept {
    std::visit([](RenderableEnemyProjectile auto& ep) { ep.update(); }, projectile);
}

inline void draw_enemy_projectile(EnemyProjectileEntity& projectile) noexcept {
    std::visit([](RenderableEnemyProjectile auto& ep) { ep.draw(); }, projectile);
}

inline bool is_enemy_projectile_alive(EnemyProjectileEntity& projectile) noexcept {
    return std::visit([](RenderableEnemyProjectile auto& ep) { return ep.is_alive(); }, projectile);
}

inline void kill_enemy_projectile(EnemyProjectileEntity& projectile) noexcept {
    std::visit([](RenderableEnemyProjectile auto& ep) { ep.kill(); }, projectile);
}

inline Rectangle get_enemy_projectile_bounding_box(EnemyProjectileEntity& projectile) noexcept {
    return std::visit([](RenderableEnemyProjectile auto& ep) { return ep.get_bounding_box(); }, projectile);
}

inline int get_enemy_projectile_damage(EnemyProjectileEntity& projectile) noexcept {
    return std::visit([](RenderableEnemyProjectile auto& ep) { return ep.get_damage(); }, projectile);
}


class EnemyProjectileManager {
private:
    std::vector<EnemyProjectileEntity> projectiles;
    Texture projectile_sprite;
    Texture missile_sprite;

    EnemyProjectileManager() = default;
public:

    constexpr static int PROJECTILE = 0;
    constexpr static int MISSILE = 1;
    static std::expected<EnemyProjectileManager, int> create() noexcept;
    void update() noexcept;
    void draw() noexcept;
    void reset() noexcept;

    std::expected<void, int> create_projectile(int type, Vector2 pos, Vector2 direction) noexcept;

    std::vector<EnemyProjectileEntity>& get_projectiles() noexcept {
        return projectiles;
    }
};

#endif