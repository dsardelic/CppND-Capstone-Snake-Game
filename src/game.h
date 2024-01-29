#ifndef GAME_H
#define GAME_H

#include "SDL.h"         // SDL_Point
#include "controller.h"  // Controller
#include "renderer.h"    // Renderer
#include "snake.h"       // Snake

class Game {
 public:
  Game(Snake& user_snake);
  virtual void Run(
      const Controller& controller, Renderer& renderer,
      std::size_t target_frame_duration
  ) = 0;
  unsigned GetScore() const;

 protected:
  Snake uc_snake_;  // user-controlled Snake
  SDL_Point food_;
  unsigned score_{0};

  virtual void PlaceFood() = 0;
  virtual void Update() = 0;
  virtual void CheckForCollisions() = 0;
};

class SimpleGame : public Game {
 public:
  SimpleGame(Snake&);
  void Run(
      const Controller& controller, Renderer& renderer,
      std::size_t target_frame_duration
  ) override;

 private:
  void PlaceFood() override;
  void Update() override;
  void CheckForCollisions() override;
};

class AdvancedGame : public Game {
 public:
  AdvancedGame(Snake&, Snake&);
  void Run(
      const Controller& controller, Renderer& renderer,
      std::size_t target_frame_duration
  ) override;

 private:
  Snake ac_snake_;  // autonomous Snake

  void PlaceFood() override;
  void Update() override;
  void CheckForCollisions() override;
  void UpdateAutonomousSnakeHeading();
};

struct Location {
  Location();
  Location(int, int);
  bool operator==(const Location&) const;
  bool operator!=(const Location&) const;
  bool operator<(const Location&) const;
  int ManhattanDistance(const Location&) const;

  int x, y;
};

struct Node {
  Node(const Location&, int, const Location&);
  bool operator>(const Node&) const;

  Location location;
  int g_score, f_score;
};

#endif
