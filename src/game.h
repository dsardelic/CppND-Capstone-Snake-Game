#ifndef GAME_H
#define GAME_H

#include <random>  // std::mt19937

#include "controller.h"  // Controller
#include "location.h"    // Location
#include "renderer.h"    // Renderer
#include "snake.h"       // Snake

class Game {
 public:
  Game();
  virtual void Run(const Controller&, Renderer&, unsigned short) = 0;
  unsigned short GetScore() const;

 protected:
  Location food_location_;
  unsigned short score_{0};
  std::mt19937 engine_;
  std::uniform_int_distribution<unsigned short> random_w_;
  std::uniform_int_distribution<unsigned short> random_h_;

  virtual void PlaceFood() = 0;
  virtual void Update() = 0;
  virtual void CheckForCollisions() = 0;
};

class SimpleGame : public Game {
 public:
  SimpleGame();
  void Run(const Controller&, Renderer&, unsigned short) override;

 private:
  void PlaceFood() override;
  void Update() override;
  void CheckForCollisions() override;

  Snake uc_snake_;  // user-controlled Snake
};

class AdvancedGame : public Game {
 public:
  AdvancedGame();
  void Run(const Controller&, Renderer&, unsigned short) override;

 private:
  Snake uc_snake_;  // user-controlled Snake
  Snake ac_snake_;  // autonomous Snake

  void PlaceFood() override;
  void Update() override;
  void CheckForCollisions() override;
  void UpdateAutonomousSnakeHeading();
};

struct Node {
  Node(const Location&, unsigned short, const Location&);
  bool operator>(const Node&) const;

  Location location;
  unsigned short g_score, f_score;
};

#endif
