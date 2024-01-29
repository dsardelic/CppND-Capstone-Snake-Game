#ifndef MENU_H
#define MENU_H

struct Menu {
  void RunGameLoop();
  unsigned short PlayNewSimpleGame();
  unsigned short PlayNewAdvancedGame();
  void UpdateHighScores(unsigned short);
  void ViewHighScores();
};

#endif
