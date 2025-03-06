#include "../include/SoundManager.hpp"

game::SoundManager::SoundManager()
{
    laserBuf.loadFromFile("resources/sounds/laser.wav");
    shotBuf.loadFromFile("resources/sounds/shot.wav");
    fireballBuf.loadFromFile("resources/sounds/fireball_moving.wav");
    explosionBuf.loadFromFile("resources/sounds/explosion.wav");
    ultBuf.loadFromFile("resources/sounds/ult.wav");
    itemPickBuf.loadFromFile("resources/sounds/itempick.wav");
}

