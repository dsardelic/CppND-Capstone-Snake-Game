#include "menu.h"

#include <iostream>  // std::cout

#include "constants.h"  // Constants
#include "game.h"       // Game

void Menu::RunGameLoop() {
  while (1) {
    auto is_valid_input{false};
    do {
      std::cout << "Options: " << std::endl;
      std::cout << "1 - Start new game" << std::endl;
      std::cout << "2 - Exit" << std::endl;
      std::cout << "Enter option number: ";
      std::cin >> option;
      if (std::cin.fail()) {
        is_valid_input = false;
      } else {
        is_valid_input = true;
      }
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << std::endl;
    } while (!(is_valid_input && option >= 1 && option <= 2));

    switch (option) {
      case 1:
        PlayNewGame();
        break;
      case 2:
        return;
    }
  }
}

void Menu::PlayNewGame() {
  Renderer renderer(
      Constants::kScreenWidth, Constants::kScreenHeight, Constants::kGridWidth,
      Constants::kGridHeight
  );
  Controller controller;
  Game game(Constants::kGridWidth, Constants::kGridHeight);
  game.Run(controller, renderer, Constants::kMsPerFrame);
  std::cout << "GAME OVER! Your score: " << game.GetScore() << std::endl;
  std::cout << std::endl;
}
