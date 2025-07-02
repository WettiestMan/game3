#include <raylib.h>
#include <expected>

class Player {
private:
    Texture sprites;
    bool alive;
    int health;
    Vector2 position;

public:
    Player() = default;
    Player(const Player&) = default;
    Player(Player&&) = default;
    static std::expected<Player> crear(Vector2 pos) noexcept;
    constexpr static int width = 32;
    constexpr static int height = 32;
    constexpr static int max_health = 20;
    
    void update() noexcept;
    void draw() noexcept;
    void reset() noexcept;
};
