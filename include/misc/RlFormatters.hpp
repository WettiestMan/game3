#include "raylib.h"
#include <string_view>
#include <format>

namespace std{
    template <>
    struct formatter<Vector2> {
        bool display_canonical_vectors = false;
        constexpr auto parse(format_parse_context& ctx) {
            auto start = ctx.begin();
            if (start == ctx.end())
                return start;
            
            if (*start == 'i') {
                display_canonical_vectors = true;
                ++start;
            }

            if (start != ctx.end() && *start != '}')
                throw format_error("Invalid format for Vector2, expected 'i' or '}'");
            
            return start;
        }

        auto format(Vector2 vec, format_context& ctx) const {
            if (display_canonical_vectors) 
                return format_to(ctx.out(), "{}i{:+f}j", vec.x, vec.y);
                
            return format_to(ctx.out(), "({}, {})", vec.x, vec.y);
        }
    };
}