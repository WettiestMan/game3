#include "include/entities/EnemyMissile.hpp"
#include <raymath.h>
#include "include/entities/Background.hpp"

std::expected<EnemyMissile, int> EnemyMissile::create(Vector2 pos, Texture sprite, const Player* target) noexcept {
    EnemyMissile missile;
    missile.position = pos;
    missile.sprite = sprite;
    missile.target = target;
    missile.alive = true;

    if (target) {
        Rectangle target_pos = target->get_bounding_box();
        Vector2 target_center = {target_pos.x + target_pos.width / 2.0f, target_pos.y + target_pos.height / 2.0f};
        missile.direction = Vector2Normalize(Vector2Subtract(target_center, pos));
    } else {
        return std::unexpected(2); // Target not set
    }

    return missile;
}

void EnemyMissile::update() noexcept {
    const auto delta = GetFrameTime();
    if (!alive) return;
    if (target) {
        // Obtener el centro del jugador
        Rectangle target_box = target->get_bounding_box();
        Vector2 target_center = {target_box.x + target_box.width / 2.0f, target_box.y + target_box.height / 2.0f};
        Vector2 to_target = Vector2Normalize(Vector2Subtract(target_center, position));
        // Interpolación suave de dirección (ajuste el factor para más o menos "giro")
        const float homing_strength = 1.0f; // 0 = no gira, 1 = sigue instantáneamente
        direction = Vector2Normalize(Vector2Lerp(direction, to_target, homing_strength * delta));
    }
    // Mover el misil
    position.x += direction.x * speed * delta;
    position.y += direction.y * speed * delta;

    if (position.y > Background::real_height || position.y < 0 - height ||
        position.x > Background::real_width || position.x < 0 - width)
        alive = false;
}

void EnemyMissile::draw() noexcept {
    if (alive) {
        const auto display_pos = position * Background::scale;
        DrawTextureEx(sprite, display_pos, 0.0f, Background::scale, WHITE);
    }
}