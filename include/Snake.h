#include "ConsoleGameEngine.h"
#include <random>
#include "Snake_Agent.h"

class Snake: public ConsoleGameEngine {
    public:
        explicit Snake(bool AI, bool train = false);
        virtual bool OnUserCreate() override;
        virtual bool OnUserUpdate(float elapsedTime) override;

    private:
        enum Direction {
            Up, Right, Down, Left, Still
        };
        struct Fruit {
            int _x;
            int _y;
        };
        struct User_Snake {
            int _headX;
            int _headY;
            std::vector<int> bodyX;
            std::vector<int> bodyY;

            void reset() {
                bodyX.clear();
                bodyY.clear();
            }
            int size() const {
                return bodyX.size();
            }
        };

        GameState ReturnGameState();

        User_Snake _snake;
        Fruit _fruit;
        Direction _direction;

        Snake_Agent _snakeAgent;
        bool _ai;
        bool _train;
        void runAgent();

        float _elapsedFrames;
        int _points = 0;
        bool _gameEnded = false;
        bool _scored = false;

        void resetFruit();
        void resetSnake();

        void SetAIDirection(std::vector<int> move);
        void setHumanDirection();

        size_t _iterations = 1;
        int _numberOfGames = 1;
        int _highScore = 0;

        GameState currentState;

        void DrawWireFrameModel();

};