#include "Movable.hpp"
#include <SFML/System/Vector2.hpp>
#include <memory>

namespace game
{
class Gamer : public Movable
{
    sf::ConvexShape _sprite;

  public:
    Gamer(float x, float y);
    Gamer();

    void show(sf::RenderWindow &win) const override;
    std::vector<sf::FloatRect> getHitboxes() const override;
    bool collide(const Entity &op) const override;
    
    void rotate(float x, float y);
    void move(float delta) override;
    void stop(float delta, sf::Vector2f def) override;
    void setPos(float x, float y) override;
    std::unique_ptr<Entity> copy() const override;
};
}
