#include "include/CollisionManager.hpp"
#include "include/Game.hpp"

void CollisionManager::check_collisions(
    Player& player,
    Boss* boss,
    std::vector<PlayerProjectile>& player_projectiles,
    std::vector<EnemyUfo>& ufos,
    std::vector<EnemyFighter>& fighters,
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

            for (auto& fighter : fighters) {
                if (!fighter.is_present()) continue;
                if (CheckCollisionRecs(proj.get_bounding_box(), fighter.get_bounding_box())) {
                    Game::get_instance()->get_points()++;
                    fighter.kill();
                    proj.kill();
                }
            }

            if (boss && boss->is_present() && CheckCollisionRecs(proj.get_bounding_box(), boss->get_bounding_box())) {
                boss->damage(proj.get_damage());
                proj.kill();
            }
        }


        // Player vs UFOs
        for (size_t i = 0; i < ufos.size(); ++i) {
            if (!ufos[i].is_present()) continue;
            if (CheckCollisionRecs(player.get_bounding_box(), ufos[i].get_bounding_box())) {
                player.damage(ufos[i].get_damage());
                ufos[i].kill();
            }
        }

        // Player vs Fighters
        for (size_t i = 0; i < fighters.size(); ++i) {
            if (!fighters[i].is_present()) continue;
            if (CheckCollisionRecs(player.get_bounding_box(), fighters[i].get_bounding_box())) {
                player.damage(fighters[i].get_damage());
                fighters[i].kill();
            }
        }

        // Player vs Boss
        if (boss && boss->is_present() && CheckCollisionRecs(player.get_bounding_box(), boss->get_bounding_box())) {
            player.damage(boss->get_damage());
        }
    }