#include "include/entities/EnemyFighter.hpp"
#include "include/Game.hpp"
#include "raymath.h"
#include "include/misc/RlFormatters.hpp"
#include <print>

float EnemyFighter::current_spawn_time = EnemyFighter::spawn_time_max;

static Vector2 get_player_center() noexcept {
    Rectangle rec = Game::get_instance()->get_player()->get_bounding_box();
    Vector2 center = {rec.x + rec.width / 2.0f, rec.y + rec.height / 2.0f};
    return center;
}

std::expected<EnemyFighter, int> EnemyFighter::create(Vector2 position, Texture normal, Texture boom) noexcept {
    EnemyFighter ef;
    ef.position = position;
    ef.sprites = normal;
    ef.explosion_sprites = boom;
    ef.alive = true;
    ef.present = true;
    ef.current_sprite_index = 0;
    ef.ammo = max_ammo;
    ef.projectile_manager = Game::get_instance()->get_projectile_manager();
    ef.anchor_point = ef.position + Vector2{0, -ef.position.y + height / 2};
    ef.destination = Vector2Normalize(get_player_center() - ef.anchor_point);
    ef.animation_timer = 0.0f;
    ef.current_spawn_time = 0.0f;
    ef.shoot_timer = shoot_interval;

    return ef;
}

void EnemyFighter::update() noexcept {
    const auto delta = GetFrameTime();
    const float current_speed = static_cast<float>(EnemyFighter::speed) * delta;
    animation_timer += delta;

    if (present) {
        if (animation_timer >= Game::animation_interval) {
            animation_timer = 0.0f;
            current_sprite_index ^= 1;
        }
        if (ammo <= 0) {
            Vector2 target_center = get_player_center();
            Vector2 to_target = Vector2Normalize(target_center - position);

            const float homing_strength = 1.4f;
            destination = Vector2Normalize(Vector2Lerp(destination, to_target, homing_strength * delta));

            position.x += destination.x * speed * delta;
            position.y += destination.y * speed * delta;
        } else {
            shoot_timer += delta;

            Vector2 direction = Vector2Normalize(anchor_point - position);
            position.x += direction.x * speed * delta;
            position.y += direction.y * speed * delta;

            if (Vector2Distance(position, anchor_point) < 1.0f) {
                position = anchor_point;
            }

            if (shoot_timer >= shoot_interval) {
                shoot_timer = 0.0f;
                Vector2 position_for_shooting = Vector2{
                    position.x + width / 2.0f,
                    position.y + height / 2.0f
                };

                projectile_manager->create_projectile(
                    EnemyProjectileManager::MISSILE, position_for_shooting, destination
                );
                ammo--;
            }
        }
    } else {
        if (animation_timer >= Game::animation_interval) {
            animation_timer = 0.0f;
            current_sprite_index++;
            if (current_sprite_index >= sizeof(explosion_sprites_offsets) / sizeof(explosion_sprites_offsets[0])) {
                alive = false;
            }
        }
    }

    //std::println("EnemyFighter.anchor_point @ {}", anchor_point);
}

void EnemyFighter::draw() noexcept {
    if (!alive) {
        return;
    }

    auto display_pos = Vector2{
        position.x * Background::scale,
        position.y * Background::scale
    };

	DrawTexturePro(
			(present) ? sprites : explosion_sprites,
			Rectangle{(present) ? normal_sprites_offsets[current_sprite_index] : explosion_sprites_offsets[current_sprite_index], 0.0f,
			static_cast<float>(width), static_cast<float>(height)},
            Rectangle{display_pos.x, display_pos.y,
            static_cast<float>(width) * Background::scale, static_cast<float>(height) * Background::scale},
            Vector2{0, 0}, 0.0f, WHITE
    );
}
