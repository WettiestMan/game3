#include "include/entities/EnemyUfoManager.hpp"
#include "include/ResourceManager.hpp"
#include "include/Game.hpp"
#include <raylib.h>

std::expected<EnemyUfoManager, int> EnemyUfoManager::create() noexcept {
    EnemyUfoManager manager;
    manager.ufos.reserve(30); // Reserve space for 30 UFOs initially
    manager.ufo_sprites = ResourceManager::get_resource(ResourceElements::enemy_ufo);
    manager.explosion_sprites = ResourceManager::get_resource(ResourceElements::boom);

    if (manager.ufo_sprites.id <= 0 || manager.explosion_sprites.id <= 0) {
        TraceLog(LOG_FATAL, "Couldn't load UFO assets");
        return std::unexpected(1);
    }

    manager.spawn_timer = 0.0f;
    manager.curr_points = &Game::get_instance()->get_points();
    return manager;
}

void EnemyUfoManager::update() noexcept {
    static int points_snapshot = *curr_points;
    spawn_timer += GetFrameTime();
    if (spawn_timer >= spawn_interval) {
        spawn();
        spawn_timer = 0.0f;
    }

    for (auto& ufo : ufos) {
        ufo.update();
    }

    ufos.erase(std::remove_if(ufos.begin(), ufos.end(),
        [](const EnemyUfo& ufo) { return !ufo.is_alive(); }), ufos.end());

    if (*curr_points % spawn_interval_rate == 0 && (points_snapshot != *curr_points)) {
        increase_spawn_frequency();
    }

    if (points_snapshot != *curr_points)
        points_snapshot = *curr_points;
}

void EnemyUfoManager::draw() noexcept {
    for (auto& ufo : ufos) {
        ufo.draw();
    }
}

void EnemyUfoManager::reset() noexcept {
    ufos.clear();
    spawn_timer = 0.0f;
    spawn_interval = max_spawn_interval; // Reset to initial spawn interval
}

void EnemyUfoManager::spawn() noexcept {
    Vector2 spawn_position;
    if (GetRandomValue(0, 1) == 0) {
        spawn_position = Vector2{ -EnemyUfo::width, (float)GetRandomValue(0, Background::real_height - EnemyUfo::height) };
    } else {
        spawn_position = Vector2{ Background::real_width, (float)GetRandomValue(0, Background::real_height - EnemyUfo::height) };
    }

    auto ufo_result = EnemyUfo::create(spawn_position, ufo_sprites, explosion_sprites, 
                                       (spawn_position.x < 0) ? EnemyUfo::Appeareance::left : EnemyUfo::Appeareance::right);
    if (ufo_result) {
        ufos.push_back(std::move(ufo_result.value()));
    } else {
        TraceLog(LOG_FATAL, "Couldn't create UFO: %d", ufo_result.error());
    }
}