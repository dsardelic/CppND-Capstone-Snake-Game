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
  free(sdl_renderer_);
}

void ClearScreen(SDL_Renderer* sdl_renderer) {
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);
}

void RenderFood(
    SDL_Renderer* sdl_renderer, SDL_Rect* block, const Location food
) {
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
  block->x = food.x * block->w;
  block->y = food.y * block->h;
  SDL_RenderFillRect(sdl_renderer, block);
}

void RenderSnakeBody(
    SDL_Renderer* sdl_renderer, SDL_Rect* block, const Snake& snake
) {
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (Location const& point : snake.body) {
    block->x = point.x * block->w;
    block->y = point.y * block->h;
    SDL_RenderFillRect(sdl_renderer, block);
  }
}

void RenderSnakeHead(
    SDL_Renderer* sdl_renderer, SDL_Rect* block, const Snake& snake, int r,
    int g, int b, int a
) {
  block->x = static_cast<int>(snake.head_x) * block->w;
  block->y = static_cast<int>(snake.head_y) * block->h;
  if (snake.alive) {
    SDL_SetRenderDrawColor(sdl_renderer, r, g, b, a);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer, block);
}

void Renderer::Render(const Snake& uc_snake, const Location& food) {
  SDL_Rect block = {
      0, 0, Constants::kScreenWidth / Constants::kGridWidth,
      Constants::kScreenHeight / Constants::kGridHeight
  };
  ClearScreen(sdl_renderer_);
  RenderFood(sdl_renderer_, &block, food);
  RenderSnakeBody(sdl_renderer_, &block, uc_snake);
  RenderSnakeHead(sdl_renderer_, &block, uc_snake, 0x00, 0x7A, 0xCC, 0xFF);
  SDL_RenderPresent(sdl_renderer_);
}

void Renderer::Render(
    const Snake& uc_snake, const Snake& ac_snake, const Location& food
) {
  SDL_Rect block = {
      0, 0, Constants::kScreenWidth / Constants::kGridWidth,
      Constants::kScreenHeight / Constants::kGridHeight
  };
  ClearScreen(sdl_renderer_);
  RenderFood(sdl_renderer_, &block, food);
  RenderSnakeBody(sdl_renderer_, &block, ac_snake);
  RenderSnakeBody(sdl_renderer_, &block, uc_snake);
  RenderSnakeHead(sdl_renderer_, &block, ac_snake, 0xC7, 0x15, 0x85, 0xFF);
  RenderSnakeHead(sdl_renderer_, &block, uc_snake, 0x00, 0x7A, 0xCC, 0xFF);
  SDL_RenderPresent(sdl_renderer_);
}

void Renderer::UpdateWindowTitle(unsigned short score, unsigned short fps) {
  std::string title{
      "Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)
  };
  SDL_SetWindowTitle(sdl_window_, title.c_str());
}
