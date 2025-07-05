#ifndef GAME_HPP_GAM3
#define GAME_HPP_GAM3

#include <vector>
#include "include/entities/Entity.hpp"
#include "include/entities/Player.hpp"
#include "include/entities/EnemyProjectileManager.hpp"
#include "include/entities/EnemyUfoManager.hpp"

class EnemyProjectileManager;
class EnemyUfoManager;

class Game {
private:
    int points;
    bool over;
    std::vector<Entity> entities;
    Player* player;
    EnemyProjectileManager* projectile_manager;
    EnemyUfoManager* ufo_manager;
    int create_entities() noexcept;
    void reset() noexcept;
    int show_game_over() noexcept;

public:
    Game() noexcept;

    int run() noexcept;
    Player* get_player() noexcept {
        return player;
    }

    EnemyProjectileManager* get_projectile_manager() noexcept {
        return projectile_manager;
    }

    static Game* get_instance() noexcept {
        static Game instance;
        return &instance;
    }

    EnemyUfoManager* get_ufo_manager() noexcept {
        return ufo_manager;
    }

    int& get_points() noexcept {
        return points;
    }

    constexpr static float animation_interval = 1.0f / 15.0f;
    static int oops;
};

#endif