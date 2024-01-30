#include "game.h"

#include <chrono>      // std::chrono
#include <functional>  // std::greater
#include <map>         // std::map
#include <queue>       // std::priority_queue
#include <set>         // std::set
#include <thread>      // std::this_thread

#include "constants.h"   // Constants
#include "controller.h"  // Controller
#include "location.h"    // Location
#include "renderer.h"    // Renderer
#include "snake.h"       // Snake

Game::Game() {
  std::random_device dev;
  engine_ = std::mt19937(dev());
  random_w_ = std::uniform_int_distribution<unsigned short>(
      0, Constants::kGridWidth - 1
  );
  random_h_ = std::uniform_int_distribution<unsigned short>(
      0, Constants::kGridHeight - 1
  );
}

unsigned short Game::GetScore() const { return score_; }

SimpleGame::SimpleGame()
    : Game{},
      uc_snake_{
          Location{Constants::kGridWidth / 2, Constants::kGridHeight / 2},
          Snake::Direction::kUp
      } {
  PlaceFood();
};

void SimpleGame::Run(
    const Controller& controller, Renderer& renderer,
    unsigned short target_frame_duration
) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  unsigned short frame_count{0};
  bool running{true};

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, uc_snake_);
    Update(running);
    renderer.Render(uc_snake_, food_location_);

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
  while (1) {
    Location location{random_w_(engine_), random_h_(engine_)};
    if (!uc_snake_.Occupies(location)) {
      food_location_ = location;
      return;
    }
  }
}

void SimpleGame::Update(bool& running) {
  if (!uc_snake_.alive) {
    std::this_thread::sleep_for(
        std::chrono::milliseconds(Constants::kMilisBeforeWindowClose)
    );
    running = false;
    return;
  }
  uc_snake_.Update();
  CheckForCollisions();
  if (uc_snake_.HeadLocation() == food_location_) {
    ++score_;
    PlaceFood();
    uc_snake_.GrowBody();
    uc_snake_.speed += Constants::kSnakeSpeedIncreaseStep;
  }
}

void SimpleGame::CheckForCollisions() {
  auto head{uc_snake_.HeadLocation()};
  // check for uc_snake_ self-collision
  for (const auto& body_item : uc_snake_.body) {
    if (head == body_item) {
      uc_snake_.alive = false;
      break;
    }
  }
}

AdvancedGame::AdvancedGame()
    : Game{},
      uc_snake_{
          Location{random_w_(engine_), random_h_(engine_)},
          Snake::Direction::kUp
      },
      ac_snake_{
          Location{random_w_(engine_), random_h_(engine_)},
          Snake::Direction::kUp
      } {
  if (ac_snake_.HeadLocation().ManhattanDistance(uc_snake_.HeadLocation()) <
      Constants::kMinDistanceBetweenSnakes) {
    ac_snake_.head_x = random_w_(engine_);
    ac_snake_.head_y = random_h_(engine_);
  }
  PlaceFood();
}

void AdvancedGame::Run(
    const Controller& controller, Renderer& renderer,
    unsigned short target_frame_duration
) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  unsigned short frame_count{0};
  bool running{true};

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, uc_snake_);
    Update(running);
    renderer.Render(uc_snake_, ac_snake_, food_location_);

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
  while (1) {
    Location location{random_w_(engine_), random_h_(engine_)};
    if (!(uc_snake_.Occupies(location) || ac_snake_.Occupies(location))) {
      food_location_ = location;
      return;
    }
  }
}

void AdvancedGame::Update(bool& running) {
  if (!(uc_snake_.alive && ac_snake_.alive)) {
    std::this_thread::sleep_for(
        std::chrono::milliseconds(Constants::kMilisBeforeWindowClose)
    );
    running = false;
    return;
  }
  uc_snake_.Update();
  UpdateSnakeDirection(ac_snake_, food_location_);
  ac_snake_.Update();
  CheckForCollisions();
  if (uc_snake_.HeadLocation() == food_location_) {
    ++score_;
    PlaceFood();
    uc_snake_.GrowBody();
    uc_snake_.speed += Constants::kSnakeSpeedIncreaseStep;
  }
  if (ac_snake_.HeadLocation() == food_location_) {
    PlaceFood();
    ac_snake_.GrowBody();
  }
}

void AdvancedGame::CheckForCollisions() {
  auto uc_snake_head{uc_snake_.HeadLocation()};
  auto ac_snake_head{ac_snake_.HeadLocation()};

  // check for head-on collision between snakes
  if (uc_snake_head == ac_snake_head) {
    uc_snake_.alive = false;
    ac_snake_.alive = false;
  }

  // check if uc_snake collided with ac_snake's body
  for (const auto& body_item : ac_snake_.body) {
    if (uc_snake_head == body_item) {
      uc_snake_.alive = false;
      break;
    }
  }

  // check if ac_snake collided with uc_snake's body
  // check for uc_snake self-collision
  for (const auto& body_item : uc_snake_.body) {
    if (ac_snake_head == body_item) {
      ac_snake_.alive = false;
      break;
    }
    if (uc_snake_head == body_item) {
      uc_snake_.alive = false;
      break;
    }
  }
}

// Using A* algorithm
Location NextLocation(
    const Location& origin, const Location& destination,
    const std::vector<Location>& obstacles
) {
  std::map<Location, Location> location_to_predecessor;
  std::set<Location> processed;
  std::priority_queue<Node, std::vector<Node>, std::greater<Node>> unprocessed;
  unprocessed.push(Node{origin, 0, destination});
  while (1) {
    auto node = unprocessed.top();
    unprocessed.pop();
    if (node.location == destination) break;
    static const short neighbor_offsets[4][2] = {
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
          if (neighbor == obstacle) {
            is_obstacle = true;
            break;
          }
        }
        if (!is_obstacle) {
          unprocessed.push(Node(neighbor, node.g_score + 1, destination));
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

void AdvancedGame::UpdateSnakeDirection(
    Snake& snake, const Location& destination
) {
  Location head_location{snake.HeadLocation()};
  Location next_head_location{
      NextLocation(head_location, destination, snake.body)
  };
  switch (next_head_location.x - head_location.x) {
    case -1:
      snake.direction = Snake::Direction::kLeft;
      break;
    case 1:
      snake.direction = Snake::Direction::kRight;
      break;
    case 0:
      if (next_head_location.y - head_location.y == -1) {
        snake.direction = Snake::Direction::kUp;
      } else {
        snake.direction = Snake::Direction::kDown;
      }
  }
}

Node::Node(
    const Location& location, unsigned short g_score,
    const Location& destination
)
    : location{location}, g_score{g_score} {
  f_score = g_score + location.ManhattanDistance(destination);
}

bool Node::operator>(const Node& rhs) const { return f_score > rhs.f_score; }
