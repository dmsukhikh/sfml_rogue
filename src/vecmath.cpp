#include "../include/vecmath.hpp"
#include <cmath>

float game::PI = 3.141593;
float game::RADTODEG = 180/PI;

float game::vabs(sf::Vector2f vec)
{
    return std::sqrt(vec.x*vec.x + vec.y*vec.y);
}

float game::vdotprod(sf::Vector2f a, sf::Vector2f b)
{
    return a.x*b.x + a.y*b.y;
}

float game::vcos(sf::Vector2f a, sf::Vector2f b)
{
    if (vabs(a) == 0. || vabs(b) == 0.)
    {
        return 1.;
    }
    else
    {
        return vdotprod(a, b) / (vabs(a) * vabs(b));
    }
}

float game::vsin(sf::Vector2f a, sf::Vector2f b)
{
    return (a.x*b.y - a.y*b.x)/ (vabs(a) * vabs(b));
}

float game::vdeg(sf::Vector2f a, sf::Vector2f b)
{
    float c = vcos(a, b), s = vsin(a, b);
    float out = game::RADTODEG * std::acos(vcos(a, b));
    if (s >= 0) 
    {
        return out;
    }
    else
    {
        return 2*game::PI - out;
    }
}

int game::sgn(int i)
{
    return i == 0 ? 0 : 1 - 2*(i < 0); 
}
