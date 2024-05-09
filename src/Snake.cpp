#include "Snake.h"

Snake::Snake(bool AI) {
    if (AI) {
        _ai = true;
    } else {
        _ai = false;
    }
    setFrameRate(150);
    ConstructConsole();
    Start();
}


bool Snake::OnUserCreate() {

    // initialize fuit
    resetFruit();
    // initialize user snake
    resetSnake();

    refresh();

    return true;
}

void Snake::SetAIDirection(std::vector<int> move) {
    // Move Left
    if (move[0] == 1) {
        switch (_direction) {
            case Up:
                _direction = Left;
                break;
            case Left:
                _direction = Down;
                break;
            case Down:
                _direction = Right;
                break;
            case Right:
                _direction = Up;
                break;
            case Still:
                _direction = Left;
                break;
        }
    // Move Straight
    } else if (move[1] == 1) {
         if (_direction == Still) {
            _direction = Up;
         } 
        
    // Move Right
    } else if (move[2] == 1) {
          switch (_direction) {
            case Up:
                _direction = Right;
                break;
            case Left:
                _direction = Up;
                break;
            case Down:
                _direction = Left;
                break;
            case Right:
                _direction = Down;
                break;
            case Still:
                _direction = Right;
                break;
        }
    } 
}

void Snake::setHumanDirection() {
    if (sKeyState[KEY_UP] == 1) {
        if (_direction == Down) { return; }
        _direction = Up;
    } else if (sKeyState[KEY_RIGHT] == 1) {
        if (_direction == Left) { return; }
        _direction = Right;
    } else if (sKeyState[KEY_LEFT] == 1) {
        if (_direction == Right) { return; }
        _direction = Left;
    } else if (sKeyState[KEY_DOWN] == 1) {
        if (_direction == Up) { return; }
        _direction = Down;
	}
}

bool Snake::OnUserUpdate(float elapsedTime) {

    clear();

    DrawWireFrameModel();

    std::vector<int> move;

    if (_ai ) {
 
        currentState = ReturnGameState();

        move = _snakeAgent.getAction(currentState);
  
        SetAIDirection(move);

    } else {

       setHumanDirection();
    }
    // Move snake body parts next spot forward
    for (int i = _snake.size() - 1; i > 0; i--) {
        _snake.bodyX[i] = _snake.bodyX[i - 1];
        _snake.bodyY[i] = _snake.bodyY[i - 1];
    }
    // Edge case: move first body part to the snake head
    if (_snake.size() > 0) {
        _snake.bodyX[0] = _snake._headX;
        _snake.bodyY[0] = _snake._headY;
    }

    // Set snake direction
    switch (_direction) {
        case Up:
            _snake._headY --;
            break;
        case Right:
            _snake._headX ++;
            break;
        case Down:
            _snake._headY ++;
            break;
        case Left:
            _snake._headX --;
            break;
        default:
            break;
    }
      
    // Did the snake go off screen
    if (isOffScreen(_snake._headX, _snake._headY)) {
        resetSnake();
        resetFruit();
        _gameEnded = true;
    }
    // Did the snake eat the fruit
    if (_snake._headX == _fruit._x && _snake._headY == _fruit._y) {
        _points += 1;
        resetFruit();
        _snake.bodyX.push_back(0);
        _snake.bodyY.push_back(0);
        _scored = true;

    }
    // Did the snake eat its body
    for (int i = 0; i < _snake.size(); i++) {
        if (_snake._headX == _snake.bodyX[i] && _snake._headY == _snake.bodyY[i]) {
            resetSnake();
            resetFruit();
            _gameEnded = true;
        }
    }
        
    if (_ai) {

        _iterations++;
            
        float reward = 0.0f;
        if (_iterations > 250 * (_snake.bodyX.size() + 1)) {
            resetSnake();
            resetFruit();
            _gameEnded = true;
            reward -= 10.0;
            _iterations = 0;
        } else if (_gameEnded) {
            reward -= 10.0;
            _iterations = 0;
        } else if (_scored) {
            reward += 10.0;
            _scored = false;
        }

        GameState newState{ReturnGameState()};
     
        GameStateSnapShot gameStateSnapShot{currentState, newState, move, reward ,_gameEnded};
 
        _snakeAgent.trainShortMemory(gameStateSnapShot);

        if (_gameEnded) {
            _snakeAgent.trainLongMemory();
            
            _gameEnded = false;
            _numberOfGames ++;
                
        }
    }
    
    RenderText(2,0,"Score: " + std::to_string(_points) + ", High Score: " + std::to_string(_highScore));
    RenderText(2,1,"Number of Games: " + std::to_string(_numberOfGames));

    refresh();

    return true;
}

void Snake::resetFruit() {
    std::random_device seed;
    std::mt19937 gen{seed()}; 

    std::uniform_int_distribution<> width_dist{2, ScreenWidth() - 4}; // set min and max
    _fruit._x = width_dist(gen);

    std::uniform_int_distribution<> height_dist{3, ScreenHeight() - 6}; // set min and max
    _fruit._y = height_dist(gen);

    refresh();
}

void Snake::resetSnake() {

    _snake.reset();
    _snake._headX = rand() % m_nScreenWidth;
    _snake._headY = rand() % m_nScreenHeight;

    _direction = Still;

    if (_points > _highScore) {
        _highScore = std::max (_highScore, _points);
        if (_ai) {  _snakeAgent.SaveModel(); }
    }
    
    _points = 0;
}

void Snake::DrawWireFrameModel() {
    // draw fruit
    DrawChar(_fruit._x, _fruit._y, ACS_DIAMOND);

    // Draw snake head
    DrawChar(_snake._headX, _snake._headY, 'O');

    // Draw snake body
    for (int i = 0; i < _snake.size(); i++) {
        DrawChar(_snake.bodyX[i], _snake.bodyY[i], 'X');
    }
}

GameState Snake::ReturnGameState() {

    std::vector<int> gameState(11, 0);

    auto isSnakeAt = [&](int x, int y) -> bool {
        
        for (size_t i = 0; i < _snake.bodyX.size(); i++) {
            if (_snake.bodyX[i] == x && _snake.bodyY[i] == y) {
                return true;
            }
        }
        
        return false;
    };

    int nextMoveStraightX = _snake._headX;
    int nextMoveStraightY = _snake._headY;

    int nextMoveRightX = _snake._headX;
    int nextMoveRightY = _snake._headY;

    int nextMoveLeftX = _snake._headX;
    int nextMoveLeftY = _snake._headY;

    switch (_direction) {
        case Up:
            nextMoveStraightY -= 1;
            nextMoveRightX += 1;
            nextMoveLeftX -= 1;
            gameState[5] = 1;
            break;
        case Right:
            nextMoveStraightX += 1;
            nextMoveRightY += 1;
            nextMoveLeftY -= 1;
            gameState[4] = 1;
            break;
        case Down:
            nextMoveStraightY += 1;
            nextMoveRightX -= 1;
            nextMoveLeftX += 1;
            gameState[6] = 1;
            break;
        case Left:
            nextMoveStraightX -= 1;
            nextMoveRightY -= 1;
            nextMoveLeftY += 1;
            gameState[3] = 1;
            break;
        default:
            break;
        }

    // Danger Straight
    if (isOffScreen(nextMoveStraightX, nextMoveStraightY) || isSnakeAt(nextMoveStraightX, nextMoveStraightY)) {
        gameState[0] = 1;
    }
    // Danger Right
    if (isOffScreen(nextMoveRightX, nextMoveRightY) || isSnakeAt(nextMoveRightX, nextMoveRightY)) {
        gameState[1] = 1;
    }
    // Danger Left
    if (isOffScreen(nextMoveLeftX, nextMoveLeftY) || isSnakeAt(nextMoveLeftX, nextMoveLeftY)) {
        gameState[2] = 1;
    }
    // Is fruit left
    if (_fruit._x < _snake._headX) {
        gameState[7] = 1;
    }
    // is fruit right
    if (_fruit._x > _snake._headX) {
        gameState[8] = 1;
    }
    // is fruit up
    if (_fruit._y < _snake._headY) {
        gameState[9] = 1;
    }
    // if fruit down
    if (_fruit._y > _snake._headY) {
        gameState[10] = 1;
    }

    GameState g{gameState};

    return g;
}

