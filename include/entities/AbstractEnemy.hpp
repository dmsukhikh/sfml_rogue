#pragma once
#include "Movable.hpp"
#include <SFML/System/Vector2.hpp>
#include <optional>
#include <memory>
#include <vector>

namespace game
{
struct Map;

class AbstractEnemy : public Movable
{
  protected:
    std::pair<bool, sf::Vector2f> stopState = {false, {0, 0}};
    static std::vector<std::vector<int>> graph;
    static sf::Vector2f screenSize;
    sf::Vector2f panicPoint;
    float panicCD = 5, shotCD = 0;
    void findPathToPoint(float dest_x, float dest_y);
    size_t idx = 0;

  public:
    static std::vector<sf::Vector2f> coordsData;
    static void setScreenSize(sf::Vector2f screenSize);
    std::vector<int> path;
    AbstractEnemy(float x, float y);
    AbstractEnemy();

    void collideHandling(Entity &op) override;
    void collideHandling(Movable &op) override;

    void changeIdx();
    static void setGraph(const Map &map);
    void setPanicPoint(const std::vector<std::unique_ptr<game::Entity>> &bob,
                       sf::Vector2f gamerPos, float delta);

    virtual void processMoving(sf::Vector2f gamerPos) = 0;
    virtual std::optional<std::unique_ptr<game::Movable>> shot(float delta) = 0;
};

} // namespace game
