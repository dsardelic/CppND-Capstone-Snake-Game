#include "game.h"

#include <cmath>       // std::abs
#include <functional>  // std::greater
#include <iostream>
#include <map>     // std::map
#include <queue>   // std::priority_queue
#include <random>  // std::random_device
#include <set>     // std::set

#include "SDL.h"         // SDL_Point
#include "constants.h"   // Constants
#include "controller.h"  // Controller
#include "renderer.h"    // Renderer
#include "snake.h"       // Snake

Game::Game(Snake& snake) : uc_snake_{snake} {}

unsigned Game::GetScore() const { return score_; }

SimpleGame::SimpleGame(Snake& snake) : Game{snake} { PlaceFood(); };

void SimpleGame::Run(
    const Controller& controller, Renderer& renderer,
    std::size_t target_frame_duration
) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, uc_snake_);
    Update();
    renderer.Render(uc_snake_, food_);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score_, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void SimpleGame::PlaceFood() {
  std::random_device dev;
  std::mt19937 engine(dev());
  std::uniform_int_distribution<int> random_w(
      0, static_cast<int>(Constants::kGridWidth - 1)
  );
  std::uniform_int_distribution<int> random_h(
      0, static_cast<int>(Constants::kGridHeight - 1)
  );
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!uc_snake_.SnakeCell(x, y)) {
      food_.x = x;
      food_.y = y;
      return;
    }
  }
}

void SimpleGame::Update() {
  if (!uc_snake_.alive) return;
  uc_snake_.Update();
  CheckForCollisions();
  int new_x1 = static_cast<int>(uc_snake_.head_x);
  int new_y1 = static_cast<int>(uc_snake_.head_y);
  // Check if there's food over here
  if (food_.x == new_x1 && food_.y == new_y1) {
    ++score_;
    PlaceFood();
    // Grow snake and increase speed.
    uc_snake_.GrowBody();
    uc_snake_.speed += 0.02;
  }
}

void SimpleGame::CheckForCollisions() {
  SDL_Point uc_snake__head{
      static_cast<int>(uc_snake_.head_x), static_cast<int>(uc_snake_.head_y)
  };

  // check for uc_snake_ self-collision
  for (auto const& body_item : uc_snake_.body) {
    if (uc_snake__head.x == body_item.x && uc_snake__head.y == body_item.y) {
      uc_snake_.alive = false;
      break;
    }
  }
}

AdvancedGame::AdvancedGame(Snake& uc_snake_, Snake& ac_snake_)
    : Game{uc_snake_}, ac_snake_{ac_snake_} {
  PlaceFood();
}

void AdvancedGame::Run(
    const Controller& controller, Renderer& renderer,
    std::size_t target_frame_duration
) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, uc_snake_);
    Update();
    renderer.Render(uc_snake_, ac_snake_, food_);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score_, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void AdvancedGame::PlaceFood() {
  std::random_device dev;
  std::mt19937 engine(dev());
  std::uniform_int_distribution<int> random_w(
      0, static_cast<int>(Constants::kGridWidth - 1)
  );
  std::uniform_int_distribution<int> random_h(
      0, static_cast<int>(Constants::kGridHeight - 1)
  );
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!uc_snake_.SnakeCell(x, y) && !ac_snake_.SnakeCell(x, y)) {
      food_.x = x;
      food_.y = y;
      return;
    }
  }
}

void AdvancedGame::Update() {
  if (!(uc_snake_.alive && ac_snake_.alive)) return;

  uc_snake_.Update();
  UpdateAutonomousSnakeHeading();
  ac_snake_.Update();

  CheckForCollisions();

  int new_x1 = static_cast<int>(uc_snake_.head_x);
  int new_y1 = static_cast<int>(uc_snake_.head_y);
  // Check if there's food over here
  if (food_.x == new_x1 && food_.y == new_y1) {
    ++score_;
    PlaceFood();
    // Grow snake and increase speed.
    uc_snake_.GrowBody();
    uc_snake_.speed += 0.02;
  }

  int new_x2 = static_cast<int>(ac_snake_.head_x);
  int new_y2 = static_cast<int>(ac_snake_.head_y);
  // Check if there's food over here
  if (food_.x == new_x2 && food_.y == new_y2) {
    // score++;
    PlaceFood();
    // Grow snake and increase speed.
    ac_snake_.GrowBody();
    // uc_snake_.speed += 0.02;
  }
}

void AdvancedGame::CheckForCollisions() {
  SDL_Point uc_snake__head{
      static_cast<int>(uc_snake_.head_x), static_cast<int>(uc_snake_.head_y)
  };
  SDL_Point ac_snake__head{
      static_cast<int>(ac_snake_.head_x), static_cast<int>(ac_snake_.head_y)
  };

  // check for uc_snake_-ac_snake_ head-on collision
  if (uc_snake__head.x == ac_snake__head.x &&
      uc_snake__head.y == ac_snake__head.y) {
    uc_snake_.alive = false;
    ac_snake_.alive = false;
  }

  // check if uc_snake_ collided with ac_snake_'s body
  for (auto const& body_item : ac_snake_.body) {
    if (uc_snake__head.x == body_item.x && uc_snake__head.y == body_item.y) {
      uc_snake_.alive = false;
      break;
    }
  }

  // check if ac_snake_ collided with uc_snake_'s body
  // check for uc_snake_ self-collision
  for (auto const& body_item : uc_snake_.body) {
    if (ac_snake__head.x == body_item.x && ac_snake__head.y == body_item.y) {
      ac_snake_.alive = false;
      break;
    }
    if (uc_snake__head.x == body_item.x && uc_snake__head.y == body_item.y) {
      uc_snake_.alive = false;
      break;
    }
  }
}

// Using A* algorithm
Location NextLocation(
    const Location& origin, const Location& destination,
    const std::vector<SDL_Point>& obstacles
) {
  std::map<Location, Location> location_to_predecessor;
  std::set<Location> processed;
  std::priority_queue<Node, std::vector<Node>, std::greater<Node>> unprocessed;
  unprocessed.push(Node{origin, 0, destination});
  while (1) {
    auto node = unprocessed.top();
    unprocessed.pop();
    if (node.location == destination) break;
    static const int neighbor_offsets[4][2] = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    };
    for (const auto& offset : neighbor_offsets) {
      Location neighbor(
          (node.location.x + offset[0] + Constants::kGridWidth) %
              Constants::kGridWidth,
          (node.location.y + offset[1] + Constants::kGridHeight) %
              Constants::kGridHeight
      );
      if (processed.find(neighbor) == processed.end()) {
        bool is_obstacle{false};
        for (const auto& obstacle : obstacles) {
          if (neighbor == Location{obstacle.x, obstacle.y}) {
            is_obstacle = true;
            break;
          }
        }
        if (!is_obstacle) {
          unprocessed.push(Node{neighbor, node.g_score + 1, destination});
          location_to_predecessor[neighbor] = node.location;
        }
      }
    }
    processed.insert(node.location);
  }

  Location location{destination};
  Location predecessor{location_to_predecessor[location]};
  while (predecessor != origin) {
    location = predecessor;
    predecessor = location_to_predecessor[location];
  }
  return location;
}

void AdvancedGame::UpdateAutonomousSnakeHeading() {
  Location location{
      static_cast<int>(ac_snake_.head_x), static_cast<int>(ac_snake_.head_y)
  };
  Location destination{food_.x, food_.y};
  Location next_location = NextLocation(location, destination, ac_snake_.body);
  switch (next_location.x - location.x) {
    case -1:
      ac_snake_.direction = Snake::Direction::kLeft;
      break;
    case 1:
      ac_snake_.direction = Snake::Direction::kRight;
      break;
    case 0:
      if (next_location.y - location.y == -1) {
        ac_snake_.direction = Snake::Direction::kUp;
      } else {
        ac_snake_.direction = Snake::Direction::kDown;
      }
  }
}

Location::Location() : x{}, y{} {}

Location::Location(int x, int y) : x{x}, y{y} {}

bool Location::operator==(const Location& rhs) const {
  return x == rhs.x && y == rhs.y;
}

bool Location::operator!=(const Location& rhs) const { return !(*this == rhs); }

bool Location::operator<(const Location& rhs) const {
  if (x < rhs.x) return true;
  if (x == rhs.x && y < rhs.y) return true;
  return false;
}

int Location::ManhattanDistance(const Location& rhs) const {
  return std::abs(x - rhs.x) + std::abs(y - rhs.y);
}

Node::Node(const Location& location, int g_score, const Location& destination)
    : location{location}, g_score{g_score} {
  f_score = g_score + location.ManhattanDistance(destination);
}

bool Node::operator>(const Node& rhs) const { return f_score > rhs.f_score; }
