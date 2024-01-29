#ifndef RENDERER_H
#define RENDERER_H

#include "SDL.h"    // SDL_Window, SDL_Renderer
#include "snake.h"  // Snake

class Renderer {
 public:
  Renderer();
  ~Renderer();

  void Render(const Snake& snake1, SDL_Point const& food);
  void Render(const Snake& snake1, const Snake& snake2, SDL_Point const& food);
  void UpdateWindowTitle(int score, int fps);

 private:
  SDL_Window* sdl_window;
  SDL_Renderer* sdl_renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
};

#endif
