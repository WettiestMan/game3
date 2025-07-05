#include "include/ResourceManager.hpp"
#include "include/entities/Background.hpp"

std::expected<Background, int> Background::create() noexcept {
    Background bg;
    bg.bgtexture = ResourceManager::get_resource(ResourceElements::background);
    
    if (bg.bgtexture.id <= 0) {
        TraceLog(LOG_FATAL, "Couldn't load background asset");
        return std::unexpected(1);
    }

    return bg;
}

void Background::update() noexcept {
    // No update logic for the background
}

void Background::draw() noexcept {
    DrawTextureEx(bgtexture, Vector2{0, 0}, 0.0f, static_cast<float>(scale), WHITE);
}

void Background::reset() noexcept {
    // No reset logic for the background
}