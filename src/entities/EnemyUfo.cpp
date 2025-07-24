#include "include/entities/EnemyUfo.hpp"
#include "include/Game.hpp"
#include "include/entities/Background.hpp"
#include <raymath.h>

float EnemyUfo::current_spawn_time = EnemyUfo::spawn_time_max;

std::expected<EnemyUfo, int> EnemyUfo::create(Vector2 pos, Texture sprite, Texture boom, Appeareance ap) noexcept {
    EnemyUfo ufo;
    ufo.position = pos;
    ufo.sprites = sprite;
    ufo.explosion_sprite = boom;
    ufo.alive = true;
    ufo.present = true;
    ufo.shoot_pattern_option = 0; // Start with the first shoot pattern
    ufo.destination = Vector2{0, 0};
    ufo.projectile_manager = Game::get_instance()->get_projectile_manager();
    ufo.shoot_timer = shoot_delay;
    ufo.animation_timer = 0.0f;
    ufo.current_spawn_time = 0.0f;

    if (ap == Appeareance::left) {
        ufo.position.x = -width; // Start off-screen to the left
        ufo.destination.x = Background::real_width + width; // Move to the right
    } else {
        ufo.position.x = Background::real_width; // Start off-screen to the right
        ufo.destination.x = -width; // Move to the left
    }

    return ufo;
}

void EnemyUfo::update() noexcept {
    const auto delta = GetFrameTime();
    const float speed = static_cast<float>(EnemyUfo::speed) * delta;
    destination_selection_timer += delta;
    animation_timer += delta;

    if (present) {
        if (animation_timer >= Game::animation_interval) {
            animation_timer = 0.0f;
            current_sprite_index++;
            if (current_sprite_index >= sizeof(sprites_offsets) / sizeof(sprites_offsets[0])) {
                current_sprite_index = 0; // Loop through the sprite frames
            }
        }

        if (destination_selection_timer >= destination_selection_interval) {
            destination_selection_timer = 0.0f;
            // Randomly select a new destination
            destination.x = position.x + (GetRandomValue(-static_cast<int>(destination_selection_distance), static_cast<int>(destination_selection_distance)));
            destination.y = position.y + (GetRandomValue(-static_cast<int>(destination_selection_distance), static_cast<int>(destination_selection_distance)));
        }

        // Move towards the destination
        Vector2 direction = Vector2Normalize(Vector2Subtract(destination, position));
        position.x += direction.x * speed;
        position.y += direction.y * speed;

        // Check if the UFO has reached the destination
        if (Vector2Distance(position, destination) < 1.0f) {
            position = destination; // Snap to the destination
        }

        shoot_timer += delta;
        if (shoot_timer >= shoot_delay) {
            shoot_timer = 0.0f;
            Vector2 position_for_shooting = Vector2{
                position.x + width / 2.0f,
                position.y + height / 2.0f
            };
            // Shoot a projectile in the current shoot pattern
            if (shoot_pattern_option == 0) {

                projectile_manager->create_projectile(
                    EnemyProjectileManager::PROJECTILE, position_for_shooting, shoot_patterns[0]
                );
            }
            else {
                for (const auto& pattern : shoot_patterns) {
                    projectile_manager->create_projectile(
                        EnemyProjectileManager::PROJECTILE, position_for_shooting, pattern
                    );
                }
            }

            shoot_pattern_option ^= 1;
        }
    } else {
        if (animation_timer >= Game::animation_interval) {
            animation_timer = 0.0f;
            current_sprite_index++;
            if (current_sprite_index >= sizeof(sprites_offsets) / sizeof(sprites_offsets[0])) {
                alive = false;
            }
        }
    }
}

void EnemyUfo::draw() noexcept {
    if (!alive) {
        return; // Do not draw if the UFO is not alive
    }

    if (present) {
        auto display_pos = Vector2{
            position.x * Background::scale,
            position.y * Background::scale
        };
        DrawTexturePro(sprites,
                       Rectangle{sprites_offsets[current_sprite_index], 0, static_cast<float>(width), static_cast<float>(height)},
                       Rectangle{display_pos.x, display_pos.y, static_cast<float>(width) * Background::scale, static_cast<float>(height) * Background::scale},
                       Vector2{0, 0}, 0.0f, WHITE);
    } else {
        // Draw explosion animation if needed
        auto display_pos = Vector2{
            position.x * Background::scale,
            position.y * Background::scale
        };
        DrawTexturePro(explosion_sprite,
                       Rectangle{sprites_offsets[current_sprite_index], 0, static_cast<float>(width), static_cast<float>(height)},
                       Rectangle{display_pos.x, display_pos.y, static_cast<float>(width) * Background::scale, static_cast<float>(height) * Background::scale},
                       Vector2{0, 0}, 0.0f, WHITE);
    }
}