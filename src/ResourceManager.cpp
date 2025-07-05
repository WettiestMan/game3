#include "include/ResourceManager.hpp"

Texture ResourceManager::resources[ResourceManager::no_resources] = {};

bool ResourceManager::allocate() noexcept {
    bool ok = true;
    Texture* begin = resources;
    for (const auto rd : magic_enum::enum_values<ResourceElements>()) {
        std::string path = get_resource_path(rd);
        *begin = LoadTexture(path.c_str());

        if (begin->id <= 0) {
            TraceLog(LOG_FATAL, "Couldn't load asset: %s", path.c_str());
            ok = false;
            break;
        }

        SetTextureFilter(*begin, TEXTURE_FILTER_POINT);

        begin++;
    }

    return ok;
}

Texture ResourceManager::get_resource(ResourceElements rd) noexcept {
    return resources[static_cast<size_t>(rd)];
}

void ResourceManager::close() noexcept {
    for (auto& resource : resources) {
        if (resource.id > 0) {
            UnloadTexture(resource);
        }
    }
}