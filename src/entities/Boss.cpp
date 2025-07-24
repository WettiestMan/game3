#include "include/entities/Boss.hpp"
#include "include/entities/Background.hpp"
#include "include/Game.hpp"
#include "include/ResourceManager.hpp"
#include "include/ResourceElements.hpp"
#include <raymath.h>
#include <cassert>
#include <cmath>

// [0] = primer set (0°), [1] = segundo set (15°)
const Vector2 Boss::projectile_directions[2][num_directions] = {
    {   // Primer array: empieza en 0°
        { std::cosf(0.0f * 2 * PI / num_directions), std::sinf(0.0f * 2 * PI / num_directions) },
        { std::cosf(1.0f * 2 * PI / num_directions), std::sinf(1.0f * 2 * PI / num_directions) },
        { std::cosf(2.0f * 2 * PI / num_directions), std::sinf(2.0f * 2 * PI / num_directions) },
        { std::cosf(3.0f * 2 * PI / num_directions), std::sinf(3.0f * 2 * PI / num_directions) },
        { std::cosf(4.0f * 2 * PI / num_directions), std::sinf(4.0f * 2 * PI / num_directions) },
        { std::cosf(5.0f * 2 * PI / num_directions), std::sinf(5.0f * 2 * PI / num_directions) },
        { std::cosf(6.0f * 2 * PI / num_directions), std::sinf(6.0f * 2 * PI / num_directions) },
        { std::cosf(7.0f * 2 * PI / num_directions), std::sinf(7.0f * 2 * PI / num_directions) },
        { std::cosf(8.0f * 2 * PI / num_directions), std::sinf(8.0f * 2 * PI / num_directions) },
        { std::cosf(9.0f * 2 * PI / num_directions), std::sinf(9.0f * 2 * PI / num_directions) },
        { std::cosf(10.0f * 2 * PI / num_directions), std::sinf(10.0f * 2 * PI / num_directions) },
        { std::cosf(11.0f * 2 * PI / num_directions), std::sinf(11.0f * 2 * PI / num_directions) }
    },
    {   // Segundo array: empieza en 15°
        { std::cosf((0.0f + 0.5f) * 2 * PI / num_directions), std::sinf((0.0f + 0.5f) * 2 * PI / num_directions) },
        { std::cosf((1.0f + 0.5f) * 2 * PI / num_directions), std::sinf((1.0f + 0.5f) * 2 * PI / num_directions) },
        { std::cosf((2.0f + 0.5f) * 2 * PI / num_directions), std::sinf((2.0f + 0.5f) * 2 * PI / num_directions) },
        { std::cosf((3.0f + 0.5f) * 2 * PI / num_directions), std::sinf((3.0f + 0.5f) * 2 * PI / num_directions) },
        { std::cosf((4.0f + 0.5f) * 2 * PI / num_directions), std::sinf((4.0f + 0.5f) * 2 * PI / num_directions) },
        { std::cosf((5.0f + 0.5f) * 2 * PI / num_directions), std::sinf((5.0f + 0.5f) * 2 * PI / num_directions) },
        { std::cosf((6.0f + 0.5f) * 2 * PI / num_directions), std::sinf((6.0f + 0.5f) * 2 * PI / num_directions) },
        { std::cosf((7.0f + 0.5f) * 2 * PI / num_directions), std::sinf((7.0f + 0.5f) * 2 * PI / num_directions) },
        { std::cosf((8.0f + 0.5f) * 2 * PI / num_directions), std::sinf((8.0f + 0.5f) * 2 * PI / num_directions) },
        { std::cosf((9.0f + 0.5f) * 2 * PI / num_directions), std::sinf((9.0f + 0.5f) * 2 * PI / num_directions) },
        { std::cosf((10.0f + 0.5f) * 2 * PI / num_directions), std::sinf((10.0f + 0.5f) * 2 * PI / num_directions) },
        { std::cosf((11.0f + 0.5f) * 2 * PI / num_directions), std::sinf((11.0f + 0.5f) * 2 * PI / num_directions) }
    }
};

static Vector2 get_player_center() noexcept {
    Rectangle rec = Game::get_instance()->get_player()->get_bounding_box();
    Vector2 center = {rec.x + rec.width / 2.0f, rec.y + rec.height / 2.0f};
    return center;
}

auto Boss::create() noexcept -> std::expected<Boss, int> {
    Boss boss;
    boss.position = Vector2{Background::real_width / 2.0f - width / 2.0f, -height};
    boss.anchor_point = boss.position + Vector2{0, height + width / 4.0f};
    boss.present = boss.alive = true;
    boss.health = max_health;
    boss.shoot_timer = boss.animation_timer = 0.0f;
    boss.projectile_manager = Game::get_instance()->get_projectile_manager();
    boss.ufo_manager = Game::get_instance()->get_ufo_manager();
    boss.fighter_manager = Game::get_instance()->get_fighter_manager();
    boss.sprites = ResourceManager::get_resource(ResourceElements::boss);
    boss.defeated_sprites = ResourceManager::get_resource(ResourceElements::big_boom);
    boss.action_timer = 0.0f;
    
    boss.actions[0] = &Boss::shoot;
    boss.actions[1] = &Boss::charge_at_player;
    boss.actions[2] = &Boss::spawn_enemies;
    boss.actions[3] = &Boss::do_projectile_spam;

    return boss;
}

void Boss::update() noexcept {
    const auto delta = GetFrameTime();
    shoot_timer += delta;
    animation_timer += delta;

    if (present) {
        switch (status) {
            case Status::in_starting_animation: {
                if (animation_timer >= Game::animation_interval) {
                    animation_timer = 0.0f;
                    current_sprite_index ^= 1;
                }

                Vector2 direction = Vector2Normalize(anchor_point - position);
                position.x += direction.x * speed * delta;
                position.y += direction.y * speed * delta;

                if (Vector2Distance(position, anchor_point) < 1.0f) {
                    position = anchor_point;
                    status = Status::idle;
                    shoot_timer = 0.0f;
                }
            }
                break;
            case Status::idle: {
                if (animation_timer >= Game::animation_interval) {
                    animation_timer = 0.0f;
                    current_sprite_index ^= 1;
                }

                constexpr float amplitude = 10.0f; // altura máxima del movimiento
                constexpr float frequency = 1.0f;  // ciclos por segundo
                position.y = anchor_point.y + amplitude * std::sinf(action_timer * frequency * 2.0f * PI);

                action_timer += delta;

                if (action_timer >= action_interval) {
                    action_timer = 0.0f;
                    current_action = GetRandomValue(0, no_actions - 1);
                    status = Status::doing_action;
                }
            }
                break;
            case Status::doing_action: {
                assert(current_action >= 0 && current_action < no_actions && "Invalid action index. Contact the programmer to solve their skill issue.");
                (this->*actions[current_action])();
            }
                break;
            case Status::returning_to_idle: {
                if (animation_timer >= Game::animation_interval) {
                    animation_timer = 0.0f;
                    current_sprite_index ^= 1;
                }

                Vector2 direction = Vector2Normalize(anchor_point - position);
                position.x += direction.x * speed * delta;
                position.y += direction.y * speed * delta;

                if (Vector2Distance(position, anchor_point) < 1.0f) {
                    position = anchor_point;
                    status = Status::idle;
                    shoot_timer = 0.0f;
                }
            }
                break;
        }
    } else {
        if (animation_timer >= Game::animation_interval) {
            animation_timer = 0.0f;
            boss_idle_sprite_index_when_defeated ^= 1;
            if (in_defeated_animation_loop) {
                current_sprite_index++;
                if (current_sprite_index >= defeated_sprite_loop_index_max) {
                    current_sprite_index = defeated_sprite_loop_index_min;
                    in_defeated_animation_loop_counter++;
                    if (in_defeated_animation_loop_counter >= in_defeated_animation_loop_times) {
                        in_defeated_animation_loop = false;
                        current_sprite_index = defeated_sprite_loop_index_max;

                        assert(current_sprite_index + 1 <= sizeof(sprites_offsets) / sizeof(*sprites_offsets) &&
                               "Defeated sprite index out of bounds. Contact the programmer to solve their skill issue.");
                    }
                }
            } else {
                current_sprite_index++;
                if (current_sprite_index >= defeated_sprite_loop_index_min && current_sprite_index < defeated_sprite_loop_index_max) {
                    in_defeated_animation_loop = true;
                    in_defeated_animation_loop_counter = 0;
                }

                if (current_sprite_index >= defeated_sprite_loop_index_max) {
                    current_sprite_index = defeated_sprite_loop_index_max;
                    alive = false;
                    Game::get_instance()->get_points() += 50;
                }
            }
        }
    }
}

void Boss::draw() const noexcept {
    const auto display_pos = position * Background::scale;

    if (present) {
        DrawTexturePro(sprites,
            Rectangle{sprites_offsets[current_sprite_index], 0, static_cast<float>(width), static_cast<float>(height)},
            Rectangle{display_pos.x, display_pos.y,
                static_cast<float>(width) * Background::scale, static_cast<float>(height) * Background::scale},
            Vector2{}, 0.0f, WHITE);
    } else {

        DrawTexturePro(sprites,
            Rectangle{sprites_offsets[boss_idle_sprite_index_when_defeated], 0, static_cast<float>(width), static_cast<float>(height)},
            Rectangle{display_pos.x, display_pos.y,
                static_cast<float>(width) * Background::scale, static_cast<float>(height) * Background::scale},
            Vector2{}, 0.0f, WHITE);
        DrawTexturePro(defeated_sprites,
            Rectangle{sprites_offsets[current_sprite_index], 0, static_cast<float>(width), static_cast<float>(height)},
            Rectangle{display_pos.x, display_pos.y,
                static_cast<float>(width) * Background::scale, static_cast<float>(height) * Background::scale},
            Vector2{}, 0.0f, WHITE);
    }
    display_health();
}

void Boss::reset() noexcept {
    // The boss gets deleted and recreated when needed, so no need to reset its state here.
}
void Boss::display_health() const noexcept {
    constexpr int health_bar_width = 120;
    constexpr int health_bar_height = 16;
    constexpr int x = Background::real_width - health_bar_width - 8;
    constexpr int y = 16 + health_bar_height;

    constexpr auto health_bar = Rectangle{
        static_cast<float>(x * Background::scale),
        static_cast<float>(y * Background::scale),
        static_cast<float>(health_bar_width * Background::scale),
        static_cast<float>(health_bar_height * Background::scale)
    };

    DrawRectangleRec(health_bar, RED);

    DrawRectangleRec(Rectangle{
        health_bar.x,
        health_bar.y,
        static_cast<float>(health_bar_width * (static_cast<float>(health) / max_health)) * Background::scale,
        health_bar.height
    }, YELLOW);
}

void Boss::damage(int amount) noexcept {
    if (alive) {
        health -= amount;
        if (health <= 0) {
            health = 0;
            kill();
        }
    }
}

void Boss::kill() noexcept {
    present = false;
    current_sprite_index = 0;
    in_defeated_animation_loop = false;
    in_defeated_animation_loop_counter = 0;
}

void Boss::do_projectile_spam() noexcept {
    current_time_after_barrage += GetFrameTime();
    if (animation_timer >= Game::animation_interval) {
        animation_timer = 0.0f;
        current_sprite_index = 2 + ((current_sprite_index & 1) ^ 1);
    }

    if (current_time_after_barrage >= time_after_barrage_to_next) {
        current_time_after_barrage = 0.0f;
        projectile_directions_set_alternator ^= 1;

        for (const auto& dir : projectile_directions[projectile_directions_set_alternator]) {
            const Vector2 projectile_pos = position + Vector2{width / 2.0f, height / 2.0f};
            projectile_manager->create_projectile(
                EnemyProjectileManager::PROJECTILE, projectile_pos, dir
            );
        }

        barrages_counter++;
    }

    if (barrages_counter >= barrages_for_do_projectile_spam) {
        barrages_counter = 0;
        current_time_after_barrage = 0.0f;
        status = Status::returning_to_idle;
        action_timer = 0.0f;
        current_sprite_index = 0;
        projectile_directions_set_alternator = 0;
    }
}

void Boss::shoot() noexcept {
    const auto delta = GetFrameTime();
    current_time_after_shots += delta;
    alternative_animation_timer += delta;

    constexpr float amplitude_x = Background::real_width / 2 - 32; // amplitud lateral
    constexpr float frequency_x = 1.0f;  // frecuencia lateral

    position.x = anchor_point.x + amplitude_x * std::sinf(alternative_animation_timer * frequency_x * 2.0f * PI);

    if (animation_timer >= Game::animation_interval) {
        animation_timer = 0.0f;
        current_sprite_index ^= 1;
    }

    if (current_time_after_shots >= time_after_shots) {
        current_time_after_shots = 0.0f;

        const auto position_for_shooting = position + Vector2{width / 2.0f, height / 2.0f};
        
        const Vector2 target_center = get_player_center();
        const auto direction = Vector2Normalize(target_center - position_for_shooting);

        projectile_manager->create_projectile(
            EnemyProjectileManager::PROJECTILE, position_for_shooting, direction
        );

        shots_counter++;
    }

    if (shots_counter >= shots_ammo_for_shoot) {
        shots_counter = 0;
        current_time_after_shots = 0.0f;
        status = Status::returning_to_idle;
        action_timer = 0.0f;
        alternative_animation_timer = 0.0f;
    }
}

void Boss::spawn_enemies() noexcept {
    if (in_spawn_animation_startup) {
        current_sprite_index = 3;
        in_spawn_animation_startup = false;
    }

    if (animation_timer >= Game::animation_interval) {
        animation_timer = 0.0f;
        current_sprite_index++;
        if (current_sprite_index >= std::size(sprites_offsets)) {
            current_sprite_index = 0;
            status = Status::returning_to_idle;
            action_timer = 0.0f;
            in_spawn_animation_startup = true;

            bool spawn_selectors[enemies_amount_for_spawn] = {
                static_cast<bool>(GetRandomValue(0, 1)),
                static_cast<bool>(GetRandomValue(0, 1)),
                static_cast<bool>(GetRandomValue(0, 1)),
                static_cast<bool>(GetRandomValue(0, 1))
            };

            for (bool selector : spawn_selectors) {
                if (selector) {
                    ufo_manager->spawn();
                } else {
                    fighter_manager->spawn();
                }
            }
        }
    }
}

void Boss::charge_at_player() noexcept {
    if (in_spawn_animation_startup) {
        destination = Vector2Normalize(get_player_center() - position);
        in_spawn_animation_startup = false;
    }

    const auto delta = GetFrameTime();
    if (animation_timer >= Game::animation_interval) {
        animation_timer = 0.0f;
        current_sprite_index = 2 + ((current_sprite_index & 1) ^ 1);
        spins_counter++;
    }

    if (spins_counter >= no_spins_for_charge) {
        const auto player_center = get_player_center();
        const auto direction = Vector2Normalize(player_center - position);
        destination = Vector2Normalize(Vector2Lerp(destination, player_center, homing_strength));
        position.x += direction.x * charge_speed * delta;
        position.y += direction.y * charge_speed * delta;
    }

    if (position.x < -width || position.x > Background::real_width ||
        position.y > Background::real_height) {
        position = Vector2{Background::real_width / 2.0f - width / 2.0f, -height};
        status = Status::returning_to_idle;
        action_timer = 0.0f;
        spins_counter = 0;
    }
}