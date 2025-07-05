#ifndef RESOURCE_ELEMENTS_HPP_GAM3
#define RESOURCE_ELEMENTS_HPP_GAM3

#include <string>
#include "magic_enum/magic_enum.hpp"

enum class ResourceElements {
    background,
    ship,
    projectile,
    missile,
    enemy_fighter,
    enemy_projectile,
    enemy_ufo,
    boom
};

std::string get_resource_name(ResourceElements rd) noexcept;

std::string get_resource_path(ResourceElements rd) noexcept;

#endif