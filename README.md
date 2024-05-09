# ConsoleGameEngine

## About
A simple game engine for the Linux / MacOS terminal. Using the NCurses api's this simple game engine allows for building simple arcade style games. 

## Games

### Asteroids 
  Asteroids is a space-themed multidirectional shooter arcade video game. The game uses the ← ↑ → ↓ buttons on the keyboard to control the ship movement. The spacebar fires bullets toward asteroids. Your current score and high score are both displayed on the top left corner. 

  https://github.com/crweaver225/ConsoleGameEngine/assets/18660043/2cddb531-3843-43ae-a2ff-b68033590118

### Snake
  Snake is a classic arcade game where a user navigates a virtual snake an attempt to each as much fruit as possible. The game uses the ← ↑ → ↓ buttons on the keyboard to control the snakes movement. The snake body grows with each piece of fruit that it eats. A user loses by navigating off the screen or eatting its own body. 

  #### AI Mode
  This game support training an AI snake agent to play that game. This requires the libtorch library. When initializing the Snake game, pass true for the AI constructor parameter. The model will automatically save to the local project as it trains. Pass false as the second constructor parameter if you are happy with the model and wish for it to no longer train. 
```
Snake snake = Snake(/*AI=*/true, /*train=*/ false);
```

https://github.com/crweaver225/ConsoleGameEngine/assets/18660043/6f1c19b2-552f-4285-a43f-c6b35ee40d19


Watch full video of Asteroid:
https://www.youtube.com/watch?v=dHbLpaEQ0fc

### Requirements
- C++ 17 or higher
- CMake
- LibTorch v. 2.4.0
- libomp

### How to Use
1) Download the repo
2) Create a build directory
3) Navigate to the build directory in the terminal and enter cmake ..
4) Enter make
5) Run exeutable ./cge
