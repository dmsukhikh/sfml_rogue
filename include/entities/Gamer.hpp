#include "Entity.hpp"
#include <SFML/System/Vector2.hpp>
#include <cstdint>
#include <memory>

namespace game
{
class Gamer : public Entity
{
    const float _SIZE = 50;
    float _MAXSPEEDABS = 200;
    float _ACCABS = 1500;
    float _SLOWDOWNABS = 300;


    uint16_t _hp = 5;
    float _prevrotation = 0;
    sf::Vector2f _speed = {0, 0};
    sf::Vector2f _view;
    sf::ConvexShape _sprite;

  public:
    int _xmovement = 0, _ymovement = 0;

    Gamer(float x, float y);
    Gamer();

    void show(sf::RenderWindow &win) const override;
    std::vector<sf::FloatRect> getHitboxes() const override;
    bool collide(const Entity &op) const override;
    
    void rotate(float x, float y);
    void move(float delta);
    void stopFrom(const Entity &i, float delta);
    void setPos(float x, float y);
    std::unique_ptr<Entity> copy() const override;
};
}
