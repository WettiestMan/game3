#include "include/entities/EnemyProjectile.hpp"
#include "include/ResourceManager.hpp"
#include "include/entities/Background.hpp"
#include <raymath.h>

std::expected<EnemyProjectile, int> EnemyProjectile::create(Vector2 pos, Texture sprite, Vector2 direction) noexcept {
    EnemyProjectile proj;
    proj.position = pos;
    proj.sprite = sprite;
    proj.alive = true;
    proj.direction = Vector2Normalize(direction);

    return proj;
}

void EnemyProjectile::update() noexcept {
    const auto delta = GetFrameTime();

    if (alive) {
        position.x += direction.x * speed * delta;
        position.y += direction.y * speed * delta;

        if (position.y > Background::real_height || position.y < 0 || 
            position.x > Background::real_width || position.x < 0) {
            alive = false; // Out of bounds
        }
    }
}

void EnemyProjectile::draw() noexcept {
    if (alive) {
        auto display_pos = position * Background::scale;
        DrawTextureEx(sprite, display_pos, 0.0f, Background::scale, WHITE);
    }
}

void EnemyProjectile::reset() noexcept {
    alive = true;
}