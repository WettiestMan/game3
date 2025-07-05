#ifndef BACKGROUND_HPP_GAM3
#define BACKGROUND_HPP_GAM3

#include <raylib.h>
#include <expected>

class Background {
private:
    Texture bgtexture;
    Background() = default;

public:
    constexpr static int scale = 4;
    constexpr static int real_width = 480;
    constexpr static int real_height = 270;
    constexpr static int display_width = real_width * scale;
    constexpr static int display_height = real_height * scale;

    Background(const Background&) = default;
    Background(Background&&) = default;
    Background& operator=(const Background&) = default;
    Background& operator=(Background&&) = default;

    static std::expected<Background, int> create() noexcept;

    void update() noexcept;
    void draw() noexcept;
    void reset() noexcept;
};

#endif