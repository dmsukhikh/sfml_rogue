#pragma once


namespace game
{
    class Game;
    class Gamer;

    class Wall;
    class Port;
    class LevelPort;

    class EntitiesVisitor
    {
    public:
        virtual void visitWall(Wall &wall) = 0;
        virtual void visitPort(Port &port) = 0;
        virtual void visitLevelPort() = 0;
    };
    
    class DefaultVisitor : public EntitiesVisitor
    {
        // Не будет висячей, потому что время жизни visitor'a связано с временем жизни игры
        Game* _game;
        Gamer* _gamer;
        float delta;

    public:
        DefaultVisitor(game::Game* game);
    
        void visitWall(Wall &wall) override;
        void visitPort(Port &port) override;
        void visitLevelPort() override;
    };
}
