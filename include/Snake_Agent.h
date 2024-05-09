#ifdef timeout
#undef timeout
#endif
#include <queue>
#include <torch/torch.h>
#include <random>


struct GameState {
    std::vector<int> _state;
};

struct GameStateSnapShot {
    GameState _oldState;
    GameState _newState;
    std::vector<int> _action;
    float reward;
    bool gameEnded;
};

class Snake_Agent {

    int _numberOfGames;
    float _gamma = 0.9;
    
    struct Dense_Net : torch::nn::Module {
        Dense_Net() {
            fc1 = register_module("fc1", torch::nn::Linear(11, 256));
            fc2 = register_module("fc2", torch::nn::Linear(256, 3));
        }

        torch::Tensor forward(torch::Tensor x) {
            x = torch::relu(fc1->forward(x.reshape({x.size(0), 11})));
            x = fc2->forward(x);
            return x;
        }

        torch::nn::Linear fc1{nullptr}, fc2{nullptr};
    };
    std::shared_ptr<Dense_Net> _net;
    std::unique_ptr<torch::optim::Adam> _optimizer;
    std::queue<GameStateSnapShot> _memory;

    public:
        Snake_Agent();
        std::vector<int> getAction(GameState gameState);
        void trainLongMemory();
        void trainShortMemory(GameStateSnapShot gameStateSnapShot);
        void SaveModel();
};