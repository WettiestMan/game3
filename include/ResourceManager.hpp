#ifndef RESOURCE_MANAGER_HPP_GAM3
#define RESOURCE_MANAGER_HPP_GAM3

#include <raylib.h>
#include <cstddef>
#include "magic_enum/magic_enum.hpp"
#include "ResourceElements.hpp"

class ResourceManager
{
public:
    constexpr static size_t no_resources = magic_enum::enum_count<ResourceElements>();
private:
    
    /* data */
    static Texture resources[no_resources];
public:
    static bool allocate() noexcept;

    static Texture get_resource(ResourceElements rd) noexcept;

    static void close() noexcept;
    
};

#endif