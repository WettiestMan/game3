#include "include/ResourceElemets.hpp"

std::string get_resource_name(ResourceElements rd) noexcept {
    std::string name{magic_enum::enum_name(rd)};
    name.append(".png");
    return name;
}

std::string get_resource_path(ResourceElements rd) noexcept {
    using namespace std::string_literals;
    const auto path = "assets/"s.append(get_resource_name(rd));
    return path;
}