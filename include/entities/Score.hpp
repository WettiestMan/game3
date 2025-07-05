#ifndef SCORE_HPP_GAM3
#define SCORE_HPP_GAM3

#include <raylib.h>

class Score {
private:
    const int* points;
    Score() = default;

public:
    Score(const Score&) = default;
    Score(Score&&) = default;
    Score& operator=(const Score&) = default;
    Score& operator=(Score&&) = default;

    static Score create(const int* initial_points) noexcept {
        Score score;
        score.points = initial_points;
        return score;
    }

    void update() noexcept {
        // Score update logic can be added here if needed
    }

    void draw() const noexcept {
        DrawText(TextFormat("Puntos: %d", *points), 10, 10, 20, WHITE);
    }

    void reset() noexcept {
        // Reset logic can be added here if needed
        // For example, if you want to reset the score to zero:
        // *points = 0;
    }
};

#endif