#include "include/Game.hpp"
#include "include/ResourceManager.hpp"
#include "include/CollisionManager.hpp"

#include <ctime>

int Game::oops = 0;

Game::Game() noexcept : over(false), player{nullptr}, boss{nullptr},
    projectile_manager{nullptr}, boss_spawned{false}, points{0} {
}

int Game::run() noexcept {
    InitWindow(Background::display_width, Background::display_height, "Game 3");

    SetRandomSeed(std::time(nullptr));

    if (!ResourceManager::allocate())
        return 2;

    int rve = create_entities();
    if (rve != 0) {
        CloseWindow();
        return rve;
    }

    while(!WindowShouldClose()) {
        if (!over) {
            if (!boss_spawned && points >= boss_spawn_points) {
                auto boss_creation = Boss::create().value();
                entities.push_back(std::move(boss_creation));
                boss = std::get_if<Boss>(&entities.back());
                boss_spawned = true;
                ufo_manager->halt();
                fighter_manager->halt();
            }

            for (auto& entity : entities) {
                update_entity(entity);
                CollisionManager::check_collisions(*player,
                    boss,
                    player->get_projectiles(),
                    ufo_manager->get_enemies(),
                    fighter_manager->get_enemies(),
                    projectile_manager->get_projectiles()
                );
                BeginDrawing();
                draw_entity(entity);
            }

            if (!player->is_alive()) {
                over = true;
            }

            if (boss && !boss->is_alive()) {
                entities.pop_back();
                boss = nullptr;
                ufo_manager->halt();
                fighter_manager->halt();
                ufo_manager->resume();
                fighter_manager->resume();
            }

            EndDrawing();
        } else {
            int keep = show_game_over();
            EndDrawing();
            if (keep == 0) {
                reset();
            } else if (keep == 1) {
                break;
            } else {
                continue;
            }
        }
    }

    ResourceManager::close();
    CloseWindow();
    return 0;
}

int Game::create_entities() noexcept {
    entities.reserve(20);
    auto background = Background::create();
    if (!background) {
        TraceLog(LOG_FATAL, "Couldn't create background");
        oops = background.error();
        return oops;
    }
    entities.push_back(std::move(background.value()));

    auto player = Player::create(Vector2{
        Background::real_width / 2 - Player::width / 2,
        Background::real_height - Player::height - 8
    });
    if (!player) {
        TraceLog(LOG_FATAL, "Couldn't create player");
        oops = player.error();
        return oops;
    }
    entities.push_back(std::move(player.value()));

    auto pmanager = EnemyProjectileManager::create();
    if (!pmanager) {
        TraceLog(LOG_FATAL, "Couldn't create enemy projectile manager");
        oops = pmanager.error();
        return oops;
    }
    entities.push_back(std::move(pmanager.value()));

    auto ufo_manager = EnemyUfoManager::create();
    if (!ufo_manager) {
        TraceLog(LOG_FATAL, "Couldn't create UFO manager");
        oops = ufo_manager.error();
        return oops;
    }
    entities.push_back(std::move(ufo_manager.value()));
    
    auto fighter_manager = EnemyFighterManager::create();
    if (!fighter_manager) {
        TraceLog(LOG_FATAL, "Couldn't create fighter manager");
        oops = fighter_manager.error();
        return oops;
    }
    entities.push_back(std::move(fighter_manager.value()));

    auto score = Score::create(&points);
    entities.push_back(std::move(score));

    this->player = std::get_if<Player>(&entities[1]);
    this->projectile_manager = std::get_if<EnemyProjectileManager>(&entities[2]);
    this->ufo_manager = std::get_if<EnemyUfoManager>(&entities[3]);
    this->fighter_manager = std::get_if<EnemyFighterManager>(&entities[4]);
    return 0;
}

void Game::reset() noexcept {
    over = false;
    points = 0;
    for (auto& entity : entities) {
        reset_entity(entity);
    }

    if (boss) {
        entities.pop_back();
        boss = nullptr;
    }

    boss_spawned = false;
}

int Game::show_game_over() noexcept {
    for (auto& entity : entities) {
        update_entity(entity);
        BeginDrawing();
        draw_entity(entity);
    }

    DrawText("Perdiste", Background::display_width / 2 - MeasureText("Perdiste", 20) / 2, Background::display_height / 2 - 10, 20, WHITE);
    DrawText("Pulse R para reiniciar o Q para salir", Background::display_width / 2 - MeasureText("Pulse R para reiniciar o Q para salir", 20) / 2, Background::display_height / 2 + 20, 20, WHITE);

    if (IsKeyPressed(KEY_R)) {
        return 0; // Restart
    } else if (IsKeyPressed(KEY_Q)) {
        return 1; // Quit
    }

    return -1; // Keep showing game over
}