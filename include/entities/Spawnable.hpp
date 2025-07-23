#ifndef SPAWNABLE_HPP_GAM3
#define SPAWNABLE_HPP_GAM3

#include <variant>
#include <concepts>
#include "EnemyUfo.hpp"
#include "EnemyFighter.hpp"

template <class T>
concept MeasurableSpawn = requires(T t) {
    { t.get_spawn_time() } -> std::same_as<float>;
    { t.reduce_spawn_time_by_interval() } -> std::same_as<void>;
    { t.get_spawn_interval() } -> std::same_as<float>;
    { t.get_min_spawn_time() } -> std::same_as<float>;
    { t.get_max_spawn_time() } -> std::same_as<float>;
};

template <MeasurableSpawn... Ts>
using SpawnableList = std::variant<Ts...>;

using Spawnable = SpawnableList<EnemyUfo, EnemyFighter>;

#endif