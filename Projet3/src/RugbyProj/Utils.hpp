#pragma once

#include <SFML/System/Vector2.hpp>
#include <chrono>

namespace Utils
{
	sf::Vector2f Vector2Normalize(const sf::Vector2f& vector);

    struct BoundingRect {
        float x;
        float y;
        float width;
        float height;


        BoundingRect(float x, float y, float width, float height) : x(x), y(y), width(width), height(height) {}

        bool exceedsTopLimit(float pointY) const {
            return (pointY <= y);
        }
        bool exceedsBottomLimit(float pointY) const {
            return (pointY >= (y + height));
        }
    };

    class Timer {
    public:
        bool isInit = false;

        void Start() {
            isInit = true;
            startTime = std::chrono::steady_clock::now();
        }

        bool CheckEndTimer(double seconds) {
            currentTime = std::chrono::steady_clock::now();
            std::chrono::duration<double> elapsed = currentTime - startTime;
            return elapsed.count() >= seconds;
        }

    private:
        std::chrono::steady_clock::time_point startTime;
        std::chrono::steady_clock::time_point currentTime;
    };
}
