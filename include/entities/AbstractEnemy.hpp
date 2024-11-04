#pragma once
#include "Movable.hpp"
#include <SFML/System/Vector2.hpp>
#include <vector>

namespace game
{
struct Map;

class AbstractEnemy : public Movable
{
  protected:
    static std::vector<std::vector<int>> graph;
    static std::vector<sf::Vector2f> coordsData;
    std::pair<bool, sf::Vector2f> stopState = {false, {0, 0}};
    size_t idx = 0;

  public:
    std::vector<int> path;
    AbstractEnemy(float x, float y);
    AbstractEnemy();

    void collideHandling(Entity &op) override;
    void collideHandling(Movable &op) override;

    void changeIdx();
    void findPathToPlayer(float dest_x, float dest_y);
    static void setGraph(const Map &map);
};

} // namespace game
