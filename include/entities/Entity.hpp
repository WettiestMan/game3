#ifndef ENTITY_HPP_GAM3
#define ENTITY_HPP_GAM3

#include <concepts>
#include <variant>

#include "Player.hpp"
#include "Background.hpp"
#include "PlayerProjectile.hpp"
#include "EnemyProjectileManager.hpp"
#include "EnemyUfoManager.hpp"
#include "Score.hpp"

template<class T>
concept Renderable = requires(T t) {
    { t.update() } -> std::same_as<void>;
    { t.draw() } -> std::same_as<void>;
    { t.reset() } -> std::same_as<void>;
};

template<Renderable... Ts>
using EntityList = std::variant<Ts...>;

using Entity = EntityList<
    Player,
    Background,
    EnemyProjectileManager,
    EnemyUfoManager,
    Score
    >;

inline void update_entity(Entity& entity) noexcept {
    std::visit([](Renderable auto& e) { e.update(); }, entity);
}

inline void draw_entity(Entity& entity) noexcept {
    std::visit([](Renderable auto& e) { e.draw(); }, entity);
}

inline void reset_entity(Entity& entity) noexcept {
    std::visit([]( Renderable auto& e) { e.reset(); }, entity);
}

#endif