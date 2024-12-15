#include "Striker.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

namespace game
{
class Wizard : public Striker
{
    sf::RectangleShape _hat;

  public:
    Wizard(float x, float y);
    Wizard();

    void show(sf::RenderWindow &win) const override;
    std::unique_ptr<Entity> copy() const override;

    void move(float delta) override;
    void stop(float delta, sf::Vector2f def) override;
    void rotate(float x, float y) override;
    void setPos(float x, float y) override;

    std::optional<std::unique_ptr<game::Movable>> shot(float delta) override;
};
} // namespace game
