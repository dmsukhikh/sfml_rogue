#pragma once
#include <SFML/System/Vector2.hpp>

namespace game
{
extern float RADTODEG;
extern float PI;
float vabs(sf::Vector2f vec);
float vdotprod(sf::Vector2f a, sf::Vector2f b);
float vcos(sf::Vector2f a, sf::Vector2f b);
float vsin(sf::Vector2f a, sf::Vector2f b);
// Угол между векторами в диапазоне [0, 360], в градусах
float vdeg(sf::Vector2f a, sf::Vector2f b);
template<typename T>
int sgn(T x)
{
    return x < 0 ? -1 : (x > 0 ? 1 : 0);
}
} // namespace game
