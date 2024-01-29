#ifndef MENU_H
#define MENU_H

struct Menu {
  int option{};

  void RunGameLoop();
  int PlayNewSimpleGame();
  int PlayNewAdvancedGame();
  void UpdateHighScores(int);
  void ViewHighScores();
};

#endif
