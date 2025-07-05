#include "include/ResourceManager.hpp"
#include "include/entities/PlayerProjectile.hpp"
#include "include/entities/Background.hpp"

std::expected<PlayerProjectile, int> PlayerProjectile::create(Vector2 pos, Texture sprite) noexcept {
    PlayerProjectile projectile;
    projectile.position = pos;
    projectile.sprite = sprite;
    projectile.alive = true;

    return projectile;
}

void PlayerProjectile::update() noexcept {
    if (alive) {
        position.y -= speed * GetFrameTime();
        if (position.y < -height) {
            alive = false;
        }
    }
}

void PlayerProjectile::draw() noexcept {
    if (alive) {
        auto display_pos = Vector2{
            position.x * Background::scale,
            position.y * Background::scale
        };

        DrawTextureEx(sprite, display_pos, 0.0f, Background::scale, WHITE);
    }
}

void PlayerProjectile::reset() noexcept {}