#ifndef SNAKE_H
#define SNAKE_H

#include <vector>  // std::vector

#include "SDL.h"        // SDL_Point
#include "constants.h"  // Constants

class Snake {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  Snake(int head_x, int head_y, Direction direction)
      : grid_width(Constants::kGridWidth),
        grid_height(Constants::kGridHeight),
        head_x(head_x),
        head_y(head_y),
        direction(direction) {}

  void Update();

  void GrowBody();
  bool SnakeCell(int x, int y);

  // Direction direction = Direction::kUp;
  Direction direction;

  float speed{0.1f};
  int size{1};
  bool alive{true};
  float head_x;
  float head_y;
  std::vector<SDL_Point> body;

 private:
  void UpdateHead();
  void UpdateBody(SDL_Point& current_cell, SDL_Point& prev_cell);

  bool growing{false};
  int grid_width;
  int grid_height;
};

#endif
