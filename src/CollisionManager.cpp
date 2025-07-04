#include "include/CollisionManager.hpp"
#include "include/Game.hpp"

void CollisionManager::check_collisions(
    Player& player,
    std::vector<PlayerProjectile>& player_projectiles,
    std::vector<EnemyUfo>& ufos,
    std::vector<EnemyProjectileEntity>& enemy_projectiles
) noexcept {
        // Player vs Enemy Projectiles
        for (auto& ep : enemy_projectiles) {
            if (!is_enemy_projectile_alive(ep)) continue;
            if (CheckCollisionRecs(player.get_bounding_box(), std::visit([](auto& e){ return e.get_bounding_box(); }, ep))) {
                player.damage(get_enemy_projectile_damage(ep));
                kill_enemy_projectile(ep);
            }
        }

        // Player Projectiles vs UFOs
        for (auto& proj : player_projectiles) {
            if (!proj.is_alive()) continue;
            for (auto& ufo : ufos) {
                if (!ufo.is_present()) continue;
                if (CheckCollisionRecs(proj.get_bounding_box(), ufo.get_bounding_box())) {
                    Game::get_instance()->get_points()++;
                    ufo.kill();
                    proj.kill();
                }
            }
        }

        // Player vs UFOs (opcional)
        for (size_t i = 0; i < ufos.size(); ++i) {
            if (!ufos[i].is_present()) continue;
            if (CheckCollisionRecs(player.get_bounding_box(), ufos[i].get_bounding_box())) {
                player.damage(1);
                ufos[i].kill();
            }
        }
    }