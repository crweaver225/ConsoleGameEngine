#include "Snake_Agent.h"

Snake_Agent::Snake_Agent()  {
   _net = std::make_shared<Dense_Net>();  // Properly initialize the network
   
    try {
        torch::load(_net, "snake_ai2.pt");
        std::cout << "Model loaded successfully." << std::endl;
        _numberOfGames = 200;
    } catch (const torch::Error& e) {
        std::cerr << "Error loading the model: " << e.what() << std::endl;
    }
    
    _optimizer = std::make_unique<torch::optim::Adam>(_net->parameters(), 0.001);  
}

void Snake_Agent::SaveModel() {
    torch::save(_net, "snake_ai.pt");
}

void Snake_Agent::trainLongMemory() {
    
    int review_size = _memory.size();

    torch::Tensor state = torch::zeros({review_size, 11}, torch::kFloat32);
    torch::Tensor newState = torch::zeros({review_size, 11}, torch::kFloat32);
    torch::Tensor action = torch::zeros({review_size, 3}, torch::kInt64);
    torch::Tensor reward = torch::zeros({review_size, 1}, torch::kFloat32);
    torch::Tensor gameEnded = torch::zeros({review_size, 1}, torch::kInt32); 
    
    for (int i = 0; i < review_size; i++) {
        GameStateSnapShot gameStateSnapShot = _memory.front();
        _memory.pop();
        
        state[i] = torch::tensor(gameStateSnapShot._oldState._state, torch::kFloat32);
        newState[i] = torch::tensor(gameStateSnapShot._newState._state, torch::kFloat32);
        action[i] = torch::tensor(gameStateSnapShot._action, torch::kInt64);
        reward[i] = torch::tensor(gameStateSnapShot.reward, torch::kFloat32);
        gameEnded[i] = torch::tensor(static_cast<int>(gameStateSnapShot.gameEnded), torch::kInt32);
        
    }
    
    torch::Tensor output = _net->forward(state);
    torch::Tensor target = output.clone();

    for (int review = 0; review < review_size; review++) {
        int action_idx = torch::argmax(action[review]).item<int>();
        
        torch::Tensor Q_new = reward[review];
        if (!gameEnded[review].item<bool>()) {
            torch::Tensor newStateTensor = newState[review].view({1, 11});
            Q_new = reward[review] + _gamma * torch::max(_net->forward(newStateTensor));
        }
        target[review][action_idx] = Q_new.item<float>();
    }
    
    _optimizer->zero_grad();
    torch::Tensor loss = torch::mse_loss(output, target);
    loss.backward();
    _optimizer->step();

    _numberOfGames++;
}

void Snake_Agent::trainShortMemory(GameStateSnapShot gameStateSnapShot) {

    _memory.push(gameStateSnapShot);
    
    torch::Tensor state = torch::tensor(gameStateSnapShot._oldState._state).toType(torch::kFloat32);
    state = state.view({1, 11});

    torch::Tensor newState = torch::tensor(gameStateSnapShot._newState._state).toType(torch::kFloat32);
    newState = newState.view({1, 11});

    torch::Tensor action = torch::tensor(gameStateSnapShot._action).toType(torch::kFloat32);
    torch::Tensor reward = torch::tensor(gameStateSnapShot.reward).toType(torch::kFloat32);

    torch::Tensor output = _net->forward(state);
    torch::Tensor target = output.clone();

    torch::Tensor Q_new = reward;
    if (!gameStateSnapShot.gameEnded) {
        Q_new = reward + _gamma * torch::max(_net->forward(newState));
    }

    int action_idx = torch::argmax(action).item<int>();
    target[0][action_idx] = Q_new.item<float>();

    _optimizer->zero_grad();
    torch::Tensor loss = torch::mse_loss(output, target);
    loss.backward();
    _optimizer->step();
    
}


std::vector<int> Snake_Agent::getAction(GameState gameState) {
    
    int epsilon = std::max(20, 200 - _numberOfGames); 
    std::vector<int> finalMove{0, 0, 0};
    
    static std::random_device rd; 
    static std::mt19937 gen(rd()); 

    std::uniform_int_distribution<> distr(0, 200); 
    int random_number = distr(gen);
    
    if (random_number < epsilon) {
        std::uniform_int_distribution<> distr2(0, 2); 
        int move = distr2(gen);
        finalMove[move] = 1;
    } else {
        
        // Ensure tensor is correctly typed and sized
        torch::Tensor state = torch::tensor(gameState._state, torch::dtype(torch::kFloat32)).view({1, 11});
        torch::Tensor output = _net->forward(state);
        int moveIndex = output.argmax(1).item<int>();
        finalMove[moveIndex] = 1;
    }
    return finalMove;
}