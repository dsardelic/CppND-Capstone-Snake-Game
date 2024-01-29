#include "menu.h"

#include <future>    // std::promise, std::future
#include <iostream>  // std::cout
#include <random>    // std::random_device
#include <string>    // std::string, std::getline
#include <thread>    // std::thread

#include "constants.h"  // Constants
#include "game.h"       // Game
#include "highscore.h"  // HighScores

void Menu::RunGameLoop() {
  unsigned short option;
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
      case 1: {
        std::promise<unsigned short> score_promise;
        std::future<unsigned short> score_future{score_promise.get_future()};
        std::thread game_thread{
            &Menu::PlayNewSimpleGame, this, std::move(score_promise)
        };
        UpdateHighScores(score_future.get());
        game_thread.join();
        break;
      }
      case 2: {
        std::promise<unsigned short> score_promise;
        std::future<unsigned short> score_future{score_promise.get_future()};
        std::thread game_thread{
            &Menu::PlayNewAdvancedGame, this, std::move(score_promise)
        };
        UpdateHighScores(score_future.get());
        game_thread.join();
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

void Menu::PlayNewSimpleGame(std::promise<unsigned short>&& score_promise) {
  Snake uc_snake{
      Location{Constants::kGridWidth / 2, Constants::kGridHeight / 2},
      Snake::Direction::kUp
  };
  Renderer renderer;
  Controller controller;
  Game* game = new SimpleGame{uc_snake};
  game->Run(controller, renderer, Constants::kMsPerFrame);
  score_promise.set_value(game->GetScore());
}

void Menu::PlayNewAdvancedGame(std::promise<unsigned short>&& score_promise) {
  std::random_device rd;
  std::mt19937 gen{rd()};
  std::uniform_int_distribution<unsigned short> distrib_w{
      0, Constants::kGridWidth - 1
  };
  std::uniform_int_distribution<unsigned short> distrib_h{
      0, Constants::kGridHeight - 1
  };
  unsigned short uc_snake_w{distrib_w(gen)};
  unsigned short uc_snake_h{distrib_h(gen)};
  unsigned short ac_snake_w(
      (uc_snake_w + Constants::kGridWidth / 2) % Constants::kGridWidth
  );
  unsigned short ac_snake_h(
      (uc_snake_h + Constants::kGridHeight / 2) % Constants::kGridHeight
  );
  Snake uc_snake{Location{uc_snake_w, uc_snake_h}, Snake::Direction::kUp};
  Snake ac_snake{Location{ac_snake_w, ac_snake_h}, Snake::Direction::kUp};
  Renderer renderer;
  Controller controller;
  Game* game = new AdvancedGame{uc_snake, ac_snake};
  game->Run(controller, renderer, Constants::kMsPerFrame);
  score_promise.set_value(game->GetScore());
}

void Menu::UpdateHighScores(unsigned short score) {
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
