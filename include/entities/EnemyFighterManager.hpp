#ifndef ENEMY_FIGHTER_MANAGER_HPP
#define ENEMY_FIGHTER_MANAGER_HPP

#include "EnemyFighter.hpp"
#include <expected>
#include <vector>

// Hasta que encuentre una manera de unificar la generación de enemigos
// voy a tener que hacer un manager por cada tipo de enemigo :(
class EnemyFighterManager {
public:
    const int* curr_points;
    std::vector<EnemyFighter> fighters;
    Texture fighter_sprites;
    Texture explosion_sprites;

    constexpr static float max_spawn_time = EnemyFighter::get_max_spawn_time();
    constexpr static float min_spawn_time = EnemyFighter::get_min_spawn_time();
    constexpr static float spawn_interval_decrease = EnemyFighter::get_spawn_interval();
    constexpr static int spawn_interval_rate = 5;

    float spawn_timer;
    float current_spawn_time = max_spawn_time;
    bool halted = false;

    EnemyFighterManager() = default;

public:
    EnemyFighterManager(const EnemyFighterManager&) = default;
    EnemyFighterManager(EnemyFighterManager&&) = default;
    EnemyFighterManager& operator=(const EnemyFighterManager&) = default;
    EnemyFighterManager& operator=(EnemyFighterManager&&) = default;

    static std::expected<EnemyFighterManager, int> create() noexcept;

    void draw() noexcept;
    void update() noexcept;
    void reset() noexcept;

    std::vector<EnemyFighter>& get_enemies() noexcept {
        return fighters;
    }

    void spawn() noexcept;
    void halt() noexcept {
        halted = true;
        for (auto& fighter : fighters) {
            fighter.kill();
        }
    }

    void resume() noexcept {
        halted = false;
    }

private:
    void increase_spawn_frequency() noexcept {
        int times = *curr_points / spawn_interval_rate;
        float new_interval = max_spawn_time - times * spawn_interval_decrease;
        if (new_interval < min_spawn_time) {
            new_interval = min_spawn_time;
        }
        current_spawn_time = new_interval;
    }
};

#endif