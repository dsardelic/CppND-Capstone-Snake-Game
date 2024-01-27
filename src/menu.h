#ifndef MENU_H
#define MENU_H

#include "game.h"

struct Menu {
  int option{0};

  void RunGameLoop();
  int PlayNewGame();
  void UpdateHighScores(int);
  void ViewHighScores();
};

#endif
