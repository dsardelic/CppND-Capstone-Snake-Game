#ifndef RENDERER_H
#define RENDERER_H

#include "SDL.h"       // SDL_Window, SDL_Renderer
#include "location.h"  // Location
#include "snake.h"     // Snake

class Renderer {
 public:
  Renderer();
  ~Renderer();
  void Render(const Snake&, const Location&);
  void Render(const Snake&, const Snake&, const Location&);
  void UpdateWindowTitle(unsigned short, unsigned short);

 private:
  SDL_Window* sdl_window_;
  SDL_Renderer* sdl_renderer_;
};

#endif
