#include "menu.h"

#include <iostream>  // std::cout
#include <string>    // std::string, std::getline

#include "constants.h"  // Constants
#include "game.h"       // Game
#include "highscore.h"  // HighScores

void Menu::RunGameLoop() {
  while (1) {
    bool is_valid_input{false};
    do {
      std::cout << "Options: " << std::endl;
      std::cout << "1 - Start new game" << std::endl;
      std::cout << "2 - View high scores" << std::endl;
      std::cout << "3 - Exit" << std::endl;
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
    } while (!(is_valid_input && option >= 1 && option <= 3));

    switch (option) {
      case 1:
        UpdateHighScores(PlayNewGame());
        break;
      case 2:
        ViewHighScores();
        break;
      case 3:
        return;
    }
  }
}

int Menu::PlayNewGame() {
  Renderer renderer(
      Constants::kScreenWidth, Constants::kScreenHeight, Constants::kGridWidth,
      Constants::kGridHeight
  );
  Controller controller;
  Game game(Constants::kGridWidth, Constants::kGridHeight);
  game.Run(controller, renderer, Constants::kMsPerFrame);
  return game.GetScore();
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
