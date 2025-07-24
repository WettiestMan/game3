#include "include/entities/EnemyFighterManager.hpp"
#include "include/ResourceManager.hpp"
#include "include/ResourceElements.hpp"
#include "include/Game.hpp"

std::expected<EnemyFighterManager, int> EnemyFighterManager::create() noexcept {
    EnemyFighterManager manager;

    manager.fighters.reserve(20);
    
    manager.fighter_sprites = ResourceManager::get_resource(ResourceElements::enemy_fighter);
    manager.explosion_sprites = ResourceManager::get_resource(ResourceElements::boom);

    if (manager.fighter_sprites.id <= 0 || manager.explosion_sprites.id <= 0) {
        TraceLog(LOG_FATAL, "Couldn't load enemy fighter assets");
        return std::unexpected(1);
    }

    manager.spawn_timer = 0.0f;
    manager.curr_points = &Game::get_instance()->get_points();
    return manager;
}

void EnemyFighterManager::update() noexcept {

    static int points_snapshot = *curr_points;
    if (!halted) {
        spawn_timer += GetFrameTime();
        if (spawn_timer >= current_spawn_time) {
            spawn();
            spawn_timer = 0.0f;
        }
    }

    for (auto& fighter : fighters) {
        fighter.update();
    }

    fighters.erase(std::remove_if(fighters.begin(), fighters.end(),
        [](const auto& fighter) { return !fighter.is_alive();}), fighters.end());

    if (*curr_points % spawn_interval_rate == 0 && (points_snapshot != *curr_points)) {
        increase_spawn_frequency();
    }

    if (points_snapshot != *curr_points)
        points_snapshot = *curr_points;
}

void EnemyFighterManager::draw() noexcept {
    for (auto& fighter : fighters) {
        fighter.draw();
    }
}

void EnemyFighterManager::reset() noexcept {
    fighters.clear();
    spawn_timer = 0.0f;
    current_spawn_time = max_spawn_time;
    halted = false;
}

void EnemyFighterManager::spawn() noexcept {
    auto spawn_position = Vector2 {
        static_cast<float>(GetRandomValue(0, Background::real_width - EnemyFighter::width)),
        -EnemyFighter::height
    };

    EnemyFighter::create(spawn_position, fighter_sprites, explosion_sprites)
        .transform([this](auto&& fighter) {
            fighters.push_back(std::move(fighter));
            return fighter;
        })
        .or_else([this](int error_code)->std::expected<EnemyFighter, int> {
            TraceLog(LOG_ERROR, "Failed to spawn enemy fighter: %d", error_code);
            return std::unexpected(error_code);
        });
}