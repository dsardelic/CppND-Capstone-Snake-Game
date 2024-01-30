#include "menu.h"

#include <stdlib.h>  // system

#include <future>    // std::promise, std::future
#include <iostream>  // std::cout
#include <memory>    // std::unique_ptr, std::make_unique
#include <random>    // std::random_device
#include <string>    // std::string, std::getline
#include <thread>    // std::thread

#include "constants.h"  // Constants
#include "game.h"       // Game
#include "highscore.h"  // HighScores

template <class GameType>
void RunGame(std::promise<unsigned short>&& score_promise) {
  Renderer renderer;
  Controller controller;
  std::unique_ptr<Game> game{std::make_unique<GameType>()};
  game->Run(controller, renderer, Constants::kMsPerFrame);
  score_promise.set_value(game->GetScore());
}

template <class GameType>
void Menu::PlayNewGame() {
  std::promise<unsigned short> score_promise;
  std::future<unsigned short> score_future{score_promise.get_future()};
  std::thread game_thread{RunGame<GameType>, std::move(score_promise)};
  Menu::UpdateHighScores(score_future.get());
  game_thread.join();
}

void Menu::Show() {
  unsigned short option;
  while (1) {
    bool is_valid_input{false};
    do {
      std::cout << "Options: " << std::endl;
      std::cout << "1 - Start new simple game" << std::endl;
      std::cout << "2 - Start new advanced game" << std::endl;
      std::cout << "3 - View high scores" << std::endl;
      std::cout << "4 - Exit" << std::endl;
      std::cout << std::endl;
      std::cout << "Enter option number: ";
      std::cin >> option;
      system("clear");
      if (std::cin.fail()) {
        is_valid_input = false;
      } else {
        is_valid_input = true;
      }
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } while (!(is_valid_input && option >= 1 && option <= 4));

    switch (option) {
      case 1:
        PlayNewGame<SimpleGame>();
        break;
      case 2: {
        PlayNewGame<AdvancedGame>();
        break;
      }
      case 3:
        ViewHighScores();
        break;
      case 4:
        return;
    }
  }
}

void Menu::UpdateHighScores(unsigned short score) {
  std::cout << "GAME OVER! Your score: " << score << std::endl;
  std::cout << std::endl;
  HighScores high_scores(Constants::kHighScoresFileUri);
  if (high_scores.IsNewHighScore(score)) {
    std::cout << "Well done, you've achieved a top "
              << Constants::kMaxHighScoresCount << " result!" << std::endl;
    std::cout << "Enter your name (max 10 characters): ";
    std::string name;
    std::getline(std::cin, name);
    std::cout << std::endl;
    high_scores.Save(name, score);
    ViewHighScores();
  }
}

void Menu::ViewHighScores() {
  HighScores(Constants::kHighScoresFileUri).Print();
  std::cout << std::endl;
}
