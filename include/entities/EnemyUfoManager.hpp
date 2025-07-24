#ifndef ENEMY_UFO_MANAGER_HPP_GAM3
#define ENEMY_UFO_MANAGER_HPP_GAM3

#include <raylib.h>
#include <expected>
#include <vector>
#include "EnemyUfo.hpp"
#include "include/ResourceManager.hpp"
#include "Spawnable.hpp"

class EnemyUfoManager {
private:
    const int* curr_points;
    std::vector<EnemyUfo> ufos;
    Texture ufo_sprites;
    Texture explosion_sprites;
    float spawn_timer;
    float spawn_interval = 3.0f; // seconds
    bool halted = false;
    constexpr static float max_spawn_interval = 3.0f;
    constexpr static float min_spawn_interval = 0.75f;
    constexpr static float spawn_interval_decrease = 0.25f; // seconds
    constexpr static int spawn_interval_rate = 5;

    EnemyUfoManager() = default;

public:

    EnemyUfoManager(const EnemyUfoManager&) = default;
    EnemyUfoManager(EnemyUfoManager&&) = default;
    EnemyUfoManager& operator=(const EnemyUfoManager&) = default;
    EnemyUfoManager& operator=(EnemyUfoManager&&) = default;

    static std::expected<EnemyUfoManager, int> create() noexcept;

    void update() noexcept;
    void draw() noexcept;
    void reset() noexcept;

    std::vector<EnemyUfo>& get_enemies() noexcept {
        return ufos;
    }

    void spawn() noexcept;
    void halt() noexcept {
        halted = true;
        for (auto& ufo : ufos) {
            ufo.kill();
        }
    }

    void resume() noexcept {
        halted = false;
    }

private:
    void increase_spawn_frequency() noexcept {
        int times = *curr_points / spawn_interval_rate;
        float new_interval = max_spawn_interval - times * spawn_interval_decrease;
        if (new_interval < min_spawn_interval) {
            new_interval = min_spawn_interval;
        }
        spawn_interval = new_interval;
    }
};

#endif