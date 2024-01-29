#ifndef SNAKE_H
#define SNAKE_H

#include <vector>  // std::vector

#include "location.h"  // Location

class Snake {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  Snake(const Location&, Direction);
  void Update();
  void GrowBody();
  bool Occupies(const Location&) const;
  Location HeadLocation() const;
  unsigned short Size();

  float head_x;
  float head_y;
  std::vector<Location> body;
  float speed{0.1f};
  Direction direction;
  bool alive{true};

 private:
  void UpdateHead();
  void UpdateBody(const Location&);

  bool growing{false};
};

#endif
