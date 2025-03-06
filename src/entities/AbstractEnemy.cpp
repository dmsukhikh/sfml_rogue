#include "../../include/entities/AbstractEnemy.hpp"
#include "../../include/MapManager.hpp"
#include <stdexcept>
#include <random>
#include <queue>
#include "../../include/vecmath.hpp"

std::vector<std::vector<int>> game::AbstractEnemy::graph;
std::vector<sf::Vector2f> game::AbstractEnemy::coordsData;
sf::Vector2f game::AbstractEnemy::screenSize{0, 0};

game::AbstractEnemy::AbstractEnemy(float x, float y) : Movable(x, y) 
{
    type = game::EntityType::Enemy;
}

game::AbstractEnemy::AbstractEnemy() : game::AbstractEnemy(0, 0) {}

void game::AbstractEnemy::setGraph(const Map &map)
{
    graph = map._adjList;
    coordsData = map._mapCoords;
}

void game::AbstractEnemy::findPathToPoint(float dest_x, float dest_y)
{
    // Весьма сложная в плане вычислений функция. Возможно, ее нужно будет
    // оптимизировать в будущем

    int dest_idx = 0, src_idx = 0;
    for (int i = 0; i < coordsData.size(); ++i)
    {
        sf::Vector2f newsrc = coordsData[src_idx] - getPos();
        sf::Vector2f ustal = coordsData[i] - getPos();

        sf::Vector2f curmin =
                         coordsData[dest_idx] - sf::Vector2f{dest_x, dest_y},
                     curi = coordsData[i] - sf::Vector2f{dest_x, dest_y};
        if (vabs(curmin) > vabs(curi))
        {
            dest_idx = i;
        }

        if (vabs(ustal) < vabs(newsrc))
        {
            src_idx = i;
        }
    }
    idx = src_idx;

    if (dest_idx == -1) throw std::range_error("trouble in findPathToPlayer");
    std::vector<std::pair<int, int>> pathData(coordsData.size(), {-1, true});

    {
        std::queue<size_t> q;
        q.push(idx);   
        pathData[idx].second = false;

        while (!q.empty())
        {
            size_t i = q.front();
            q.pop();
            for (auto j : graph[i])
            {
                if (pathData[j].second)
                {
                    pathData[j].first = i;
                    pathData[j].second = false;
                    q.push(j);
                }
            }
            pathData[i].second = false;
        }
    }

    path.clear();
    for (int i = dest_idx; i != idx && pathData[i].first != -1;
         i = pathData[i].first)
    {
        path.push_back(i);
    }
}

void game::AbstractEnemy::collideHandling(Entity &op)
{
    if (collide(op))
    {
        switch (op.getType())
        {
            case game::EntityType::Wall:
                stopState = {true, op.getPos() - getPos()}; 
            case game::EntityType::Port:
                stopState = {true, op.getPos() - getPos()}; 
            default:
                break;
        }
    }
}

void game::AbstractEnemy::collideHandling(Movable &op) 
{
    switch (op.getType())
    {
        case game::EntityType::Shot:
            op.collideHandling(*this);
        default:
            break;
    }
}

void game::AbstractEnemy::setScreenSize(sf::Vector2f newSize)
{
    screenSize = newSize;
}

void game::AbstractEnemy::setPanicPoint(
    const std::vector<std::unique_ptr<game::Entity>> &bob,
    sf::Vector2f gamerPos, float delta)
{
    if ((fabs(gamerPos.x - getPos().x) < screenSize.x * 0.35 &&
         fabs(gamerPos.y - getPos().y) < screenSize.y * 0.35) &&
        panicCD > 5)
    {
        panicCD = 0;

        std::random_device rd; 
        std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> rand_bob(0, bob.size()-1);
        int tries = 0;
        sf::Vector2f newPanic;

        while (tries < 10)
        {
            size_t i = rand_bob(gen);
            if (bob[i]->getType() != EntityType::None) continue;
            else
            {
                newPanic = bob[i]->getPos();
                auto diff = gamerPos - bob[i]->getPos();
                if (fabs(diff.x) < screenSize.x * 0.35 && 
                    fabs(diff.y) < screenSize.y * 0.35)
                {
                    ++tries;
                    continue;
                }
                else break;
            }
        }
        panicPoint = newPanic;
    }
    panicCD += delta;
}

