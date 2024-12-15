#pragma once
#include "SFML/Audio.hpp"

namespace game
{
struct SoundManager
{
    sf::SoundBuffer laserBuf, shotBuf, fireballBuf, explosionBuf, ultBuf,
        itemPickBuf;
    SoundManager();
};
} // namespace game
