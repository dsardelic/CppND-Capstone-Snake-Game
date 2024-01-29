#include "snake.h"

#include <cmath>  // std::fmod

#include "constants.h"
#include "location.h"

Snake::Snake(const Location& head_location, Direction direction)
    : head_x(head_location.x), head_y(head_location.y), direction(direction) {}

void Snake::Update() {
  // We first capture the head's cell before updating.
  Location prev_head{HeadLocation()};
  UpdateHead();
  // Capture the head's cell after updating.
  Location current_head{HeadLocation()};
  // Update all of the body vector items
  // if the snake head has moved to a new cell.
  if (prev_head != current_head) UpdateBody(prev_head);
}

void Snake::UpdateHead() {
  switch (direction) {
    case Direction::kUp:
      head_y -= speed;
      break;
    case Direction::kDown:
      head_y += speed;
      break;
    case Direction::kLeft:
      head_x -= speed;
      break;
    case Direction::kRight:
      head_x += speed;
  }

  // Wrap the Snake around to the beginning if going off of the screen.
  head_x = std::fmod(head_x + Constants::kGridWidth, Constants::kGridWidth);
  head_y = std::fmod(head_y + Constants::kGridHeight, Constants::kGridHeight);
}

void Snake::UpdateBody(const Location& prev_head) {
  // add previous head location to vector
  body.push_back(prev_head);
  if (!growing) {
    // remove the tail from the vector
    body.erase(body.begin());
  } else {
    growing = false;
  }
}

void Snake::GrowBody() { growing = true; }

bool Snake::Occupies(const Location& location) const {
  if (location == HeadLocation()) return true;
  for (auto const& body_item_location : body) {
    if (location == body_item_location) return true;
  }
  return false;
}

Location Snake::HeadLocation() const {
  return Location{
      static_cast<unsigned short>(head_x), static_cast<unsigned short>(head_y)
  };
}

unsigned short Snake::Size() { return body.size() + 1; }
