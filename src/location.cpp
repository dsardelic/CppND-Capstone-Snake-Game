#include "location.h"

#include <cmath>  // std::abs

Location::Location() : x{}, y{} {}

Location::Location(unsigned short x, unsigned short y) : x{x}, y{y} {}

bool Location::operator==(const Location& rhs) const {
  return x == rhs.x && y == rhs.y;
}

bool Location::operator!=(const Location& rhs) const { return !(*this == rhs); }

bool Location::operator<(const Location& rhs) const {
  if (x < rhs.x) return true;
  if (x == rhs.x && y < rhs.y) return true;
  return false;
}

unsigned short Location::ManhattanDistance(const Location& rhs) const {
  return std::abs(x - rhs.x) + std::abs(y - rhs.y);
}
