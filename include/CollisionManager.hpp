#ifndef COLLISION_MANAGER_HPP_GAM3
#define COLLISION_MANAGER_HPP_GAM3

#include <raylib.h>
#include <vector>
#include "entities/Player.hpp"
#include "entities/EnemyUfo.hpp"
#include "entities/EnemyProjectileManager.hpp"
#include "entities/EnemyFighter.hpp"

class CollisionManager {
public:
    static void check_collisions(
        Player& player,
        std::vector<PlayerProjectile>& player_projectiles,
        std::vector<EnemyUfo>& ufos,
        std::vector<EnemyFighter>& fighters,
        std::vector<EnemyProjectileEntity>& enemy_projectiles
    ) noexcept;
};

#endif