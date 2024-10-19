#pragma once
#include <SFML/System/Vector2.hpp>

namespace game
{
extern float RADTODEG;
extern float PI;
int sgn(int i);
float vabs(sf::Vector2f vec);
float vdotprod(sf::Vector2f a, sf::Vector2f b);
float vcos(sf::Vector2f a, sf::Vector2f b);
float vsin(sf::Vector2f a, sf::Vector2f b);
// Угол между векторами в диапазоне [0, 360], в градусах
float vdeg(sf::Vector2f a, sf::Vector2f b);
} // namespace game
