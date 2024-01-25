#ifndef MENU_H
#define MENU_H

#include "game.h"

struct Menu {
  int option{0};

  void RunGameLoop();
  void PlayNewGame();
};

#endif
