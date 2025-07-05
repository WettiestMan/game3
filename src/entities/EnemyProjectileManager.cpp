#include "include/Game.hpp"
#include "include/entities/EnemyProjectileManager.hpp"
#include "include/ResourceManager.hpp"
#include "include/entities/Background.hpp"

std::expected<EnemyProjectileManager, int> EnemyProjectileManager::create() noexcept {
    EnemyProjectileManager manager;
    manager.projectiles.reserve(50);

    manager.projectile_sprite = ResourceManager::get_resource(ResourceElements::enemy_projectile);
    manager.missile_sprite = ResourceManager::get_resource(ResourceElements::missile);

    if (manager.projectile_sprite.id <= 0 || manager.missile_sprite.id <= 0) {
        TraceLog(LOG_FATAL, "Couldn't load enemy projectile assets");
        return std::unexpected(1);
    }

    return manager;
}

void EnemyProjectileManager::update() noexcept {
    for (auto& projectile : projectiles)
        update_enemy_projectile(projectile);

    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(),
            [](EnemyProjectileEntity& projectile){
                return !is_enemy_projectile_alive(projectile);
            }),
        projectiles.end()
    );
}

void EnemyProjectileManager::draw() noexcept {
    for (auto& projectile : projectiles)
        draw_enemy_projectile(projectile);
}

void EnemyProjectileManager::reset() noexcept {
    projectiles.clear();
}

std::expected<void, int> EnemyProjectileManager::create_projectile(int type, Vector2 pos, Vector2 direction) noexcept {
        
    if (type == PROJECTILE) {
        EnemyProjectileEntity projectile{*EnemyProjectile::create(pos, projectile_sprite, direction)};
        
        projectiles.push_back(std::move(projectile));
    } else if (type == MISSILE) {
        EnemyProjectileEntity projectile{*EnemyMissile::create(pos, missile_sprite, Game::get_instance()->get_player())};
           
        projectiles.push_back(std::move(projectile));
    } else {
        return std::unexpected(3); // Unsupported type
    }

    return {};
}