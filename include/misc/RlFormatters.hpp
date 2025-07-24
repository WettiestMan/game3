#include "raylib.h"
#include <string_view>
#include <format>

namespace std{
    template <>
    struct formatter<Vector2> {
        constexpr auto parse(format_parse_context& ctx) {
            return ctx.begin();
        }

        auto format(Vector2 vec, format_context& ctx) const {
            return format_to(ctx.out(), "({}, {})", vec.x, vec.y);
        }
    };
}