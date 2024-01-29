#include "renderer.h"

#include <iostream>  // std::cerr
#include <string>    // std::string

#include "SDL.h"        // SDL_Window, SDL_Renderer
#include "constants.h"  // Constants
#include "snake.h"      // Snake

Renderer::Renderer() {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window_ = SDL_CreateWindow(
      "Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      Constants::kScreenWidth, Constants::kScreenHeight, SDL_WINDOW_SHOWN
  );

  if (nullptr == sdl_window_) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer_ = SDL_CreateRenderer(sdl_window_, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer_) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window_);
  SDL_Quit();
}

void Renderer::Render(const Snake& uc_snake, const Location& food) {
  SDL_Rect block;
  block.w = Constants::kScreenWidth / Constants::kGridWidth;
  block.h = Constants::kScreenHeight / Constants::kGridHeight;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer_, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer_);

  // Render food
  SDL_SetRenderDrawColor(sdl_renderer_, 0xFF, 0xCC, 0x00, 0xFF);
  block.x = food.x * block.w;
  block.y = food.y * block.h;
  SDL_RenderFillRect(sdl_renderer_, &block);

  // Render uc_snake's body
  SDL_SetRenderDrawColor(sdl_renderer_, 0xFF, 0xFF, 0xFF, 0xFF);
  for (Location const& point : uc_snake.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer_, &block);
  }

  // Render uc_snake's head
  block.x = static_cast<int>(uc_snake.head_x) * block.w;
  block.y = static_cast<int>(uc_snake.head_y) * block.h;
  if (uc_snake.alive) {
    SDL_SetRenderDrawColor(sdl_renderer_, 0x00, 0x7A, 0xCC, 0xFF);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer_, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer_, &block);

  // Update Screen
  SDL_RenderPresent(sdl_renderer_);
}

void Renderer::Render(
    const Snake& uc_snake, const Snake& ac_snake, const Location& food
) {
  SDL_Rect block;
  block.w = Constants::kScreenWidth / Constants::kGridWidth;
  block.h = Constants::kScreenHeight / Constants::kGridHeight;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer_, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer_);

  // Render food
  SDL_SetRenderDrawColor(sdl_renderer_, 0xFF, 0xCC, 0x00, 0xFF);
  block.x = food.x * block.w;
  block.y = food.y * block.h;
  SDL_RenderFillRect(sdl_renderer_, &block);

  // Render snake1's body
  SDL_SetRenderDrawColor(sdl_renderer_, 0xFF, 0xFF, 0xFF, 0xFF);
  for (Location const& point : uc_snake.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer_, &block);
  }

  // Render snake1's head
  block.x = static_cast<int>(uc_snake.head_x) * block.w;
  block.y = static_cast<int>(uc_snake.head_y) * block.h;
  if (uc_snake.alive) {
    SDL_SetRenderDrawColor(sdl_renderer_, 0x00, 0x7A, 0xCC, 0xFF);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer_, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer_, &block);

  // Render snake2's body
  SDL_SetRenderDrawColor(sdl_renderer_, 0xFF, 0xFF, 0xFF, 0xFF);
  for (Location const& point : ac_snake.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer_, &block);
  }

  // Render snake2's head
  block.x = static_cast<int>(ac_snake.head_x) * block.w;
  block.y = static_cast<int>(ac_snake.head_y) * block.h;
  if (ac_snake.alive) {
    SDL_SetRenderDrawColor(sdl_renderer_, 0xC7, 0x15, 0x85, 0xFF);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer_, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer_, &block);

  // Update Screen
  SDL_RenderPresent(sdl_renderer_);
}

void Renderer::UpdateWindowTitle(unsigned short score, unsigned short fps) {
  std::string title{
      "Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)
  };
  SDL_SetWindowTitle(sdl_window_, title.c_str());
}
