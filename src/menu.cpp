#include "menu.h"

#include <iostream>  // std::cout
#include <random>    // std::random_device
#include <string>    // std::string, std::getline

#include "constants.h"  // Constants
#include "game.h"       // Game
#include "highscore.h"  // HighScores

void Menu::RunGameLoop() {
  while (1) {
    bool is_valid_input{false};
    do {
      std::cout << "Options: " << std::endl;
      std::cout << "1 - Start new simple game" << std::endl;
      std::cout << "2 - Start new advanced game" << std::endl;
      std::cout << "3 - View high scores" << std::endl;
      std::cout << "4 - Exit" << std::endl;
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
    } while (!(is_valid_input && option >= 1 && option <= 4));

    switch (option) {
      case 1:
        UpdateHighScores(PlayNewSimpleGame());
        break;
      case 2:
        UpdateHighScores(PlayNewAdvancedGame());
        break;
      case 3:
        ViewHighScores();
        break;
      case 4:
        return;
    }
  }
}

int Menu::PlayNewSimpleGame() {
  int center_w(Constants::kGridWidth / 2);
  int center_h(Constants::kGridHeight / 2);
  Snake uc_snake{center_w, center_h, Snake::Direction::kUp};
  Renderer renderer;
  Controller controller;
  Game* game = new SimpleGame{uc_snake};
  game->Run(controller, renderer, Constants::kMsPerFrame);
  return game->GetScore();
}

int Menu::PlayNewAdvancedGame() {
  std::random_device rd;
  std::mt19937 gen{rd()};
  std::uniform_int_distribution<> distrib_w{
      0, static_cast<int>(Constants::kGridWidth - 1)
  };
  std::uniform_int_distribution<> distrib_h{
      0, static_cast<int>(Constants::kGridHeight - 1)
  };
  int uc_snake_w{distrib_w(gen)};
  int uc_snake_h{distrib_h(gen)};
  int ac_snake_w(
      (uc_snake_w + Constants::kGridWidth / 2) % Constants::kGridWidth
  );
  int ac_snake_h(
      (uc_snake_h + Constants::kGridHeight / 2) % Constants::kGridHeight
  );
  Snake uc_snake{uc_snake_w, uc_snake_h, Snake::Direction::kUp};
  Snake ac_snake{ac_snake_w, ac_snake_h, Snake::Direction::kUp};
  Renderer renderer;
  Controller controller;
  Game* game = new AdvancedGame{uc_snake, ac_snake};
  game->Run(controller, renderer, Constants::kMsPerFrame);
  return game->GetScore();
}

void Menu::UpdateHighScores(int score) {
  std::cout << "GAME OVER! Your score: " << score << std::endl;
  HighScores high_scores(Constants::kHighScoresFileUri);
  if (high_scores.IsNewHighScore(score)) {
    std::cout << "Well done, you've achieved a top 10 result!" << std::endl;
    std::cout << "Enter your name (max 10 characters): ";
    std::string name;
    std::getline(std::cin, name);
    std::cout << std::endl;
    high_scores.Save(name, score);
    ViewHighScores();
  } else {
    std::cout << std::endl;
  }
}

void Menu::ViewHighScores() {
  HighScores(Constants::kHighScoresFileUri).Print();
  std::cout << std::endl;
}
