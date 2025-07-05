#include "include/entities/Player.hpp"
#include "include/ResourceManager.hpp"
#include "include/ResourceElemets.hpp"
#include "include/entities/Background.hpp"
#include "include/Game.hpp"

#include <raylib.h>
#include <algorithm>
#include <cmath>

std::expected<Player, int> Player::create(Vector2 pos) noexcept {
    Player player;
    player.original_position = pos;
    player.position = pos;
    player.projectile_sprite = ResourceManager::get_resource(ResourceElements::projectile);
    player.sprites = ResourceManager::get_resource(ResourceElements::ship);
    
    if (player.sprites.id <= 0 || player.projectile_sprite.id <= 0) {
        TraceLog(LOG_FATAL, "Couldn't load player asset");
        return std::unexpected(1);
    }

    player.alive = true;
    player.health = max_health;
    player.shoot_timer = shoot_delay;
    player.animation_timer = 0.0f;
    player.current_sprite_index = 0;

    return player;
}

void Player::update() noexcept {
    auto delta = GetFrameTime();
    const float speed = static_cast<float>(Player::speed) * delta;
    animation_timer += delta;

    if (alive) {
        if (animation_timer >= Game::animation_interval) {
            animation_timer = 0.0f;
            current_sprite_index ^= 1; // 0 y 1
        }

        shoot_timer += delta;

        if (IsKeyDown(KEY_D)) {
            position.x += speed;
        }
        if (IsKeyDown(KEY_A)) {
            position.x -= speed;
        }
        if (IsKeyDown(KEY_W)) {
            position.y -= speed;
        }
        if (IsKeyDown(KEY_S)) {
            position.y += speed;
        }
        /*pos_accumulator.x += (IsKeyDown(KEY_D) - IsKeyDown(KEY_A)) * speed;
        pos_accumulator.y += (IsKeyDown(KEY_S) - IsKeyDown(KEY_W)) * speed;

        int move_x = static_cast<int>(std::round(pos_accumulator.x));
        int move_y = static_cast<int>(std::round(pos_accumulator.y));

        if (move_x != 0) {
            position.x += move_x;
            pos_accumulator.x -= move_x;
        }
        if (move_y != 0) {
            position.y += move_y;
            pos_accumulator.y -= move_y;
        }*/

        if (shoot_timer >= shoot_delay && IsKeyDown(KEY_SPACE)) {
            const auto proj_pos = Vector2{
                position.x + (width / 2.0f) - (PlayerProjectile::width / 2.0f),
                position.y - PlayerProjectile::height
            };
            auto projectile = PlayerProjectile::create(proj_pos, projectile_sprite);
            if (projectile) {
                projectiles.push_back(*projectile);
                shoot_timer = 0.0f;
            } else {
                TraceLog(LOG_FATAL, "Couldn't create player projectile");
                Game::oops = projectile.error();
                return;
            }
        }

        // Clamp position to screen bounds
        position.x = std::clamp(position.x, 0.0f, static_cast<float>(Background::real_width - width));
        position.y = std::clamp(position.y, 0.0f, static_cast<float>(Background::real_height - height));
    }
    else {
        if (animation_timer >= Game::animation_interval) {
            animation_timer = 0.0f;
            current_sprite_index++;
            if (current_sprite_index > sizeof(sprites_offsets) / sizeof(sprites_offsets[0]) - 1) {
                current_sprite_index--; // Stay on the last defeated sprite to avoid eventual integer overflow
            }
        }
    }

    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(),
            [](const PlayerProjectile& projectile) { return !projectile.is_alive(); }),
        projectiles.end());

    for (auto& projectile : projectiles)
        projectile.update();
}

void Player::draw() noexcept {
    const float draw_x = position.x * Background::scale;
    const float draw_y = position.y * Background::scale;
    if (alive) {
        DrawTexturePro(sprites,
                Rectangle{sprites_offsets[current_sprite_index], 0, static_cast<float>(width), static_cast<float>(height)},
                Rectangle{draw_x, draw_y
                    , static_cast<float>(width) * Background::scale, static_cast<float>(height) * Background::scale},
                Vector2{0, 0}, 0.0f, WHITE);
    } else {
        if (current_sprite_index < sizeof(sprites_offsets) / sizeof(sprites_offsets[0]))
                DrawTexturePro(sprites,
                    Rectangle{sprites_offsets[current_sprite_index], 0, static_cast<float>(width), static_cast<float>(height)},
                    Rectangle{draw_x, draw_y,
                         static_cast<float>(width) * Background::scale, static_cast<float>(height) * Background::scale},
                    Vector2{0, 0}, 0.0f, WHITE);
    }

    display_health();

    for (auto& projectile : projectiles) {
        if (projectile.is_alive()) {
            projectile.draw();
        }
    }
}

void Player::reset() noexcept {
    alive = true;
    health = max_health;
    position = original_position;
    shoot_timer = shoot_delay;
    animation_timer = 0.0f;
    current_sprite_index = 0;
    projectiles.clear();
}

std::vector<PlayerProjectile>& Player::get_projectiles() noexcept {
    return projectiles;
}

void Player::damage(int amount) noexcept {
    if (alive) {
        health -= amount;
        if (health <= 0) {
            health = 0;
            kill();
        }
    }
}

void Player::display_health() const noexcept {
    const int bar_width = 64;
    const int bar_height = 16;
    const int x = Background::real_width - bar_width - 8;
    const int y = 8;

    DrawRectangle(x * Background::scale, y * Background::scale, bar_width * Background::scale,
         bar_height * Background::scale, RED);
    DrawRectangle(x * Background::scale, y * Background::scale,
         static_cast<int>(bar_width * (static_cast<float>(health) / max_health)) * Background::scale,
          bar_height * Background::scale, GREEN);
}

void Player::kill() noexcept {
    alive = false;
    current_sprite_index = first_defeated_sprite_index;
}