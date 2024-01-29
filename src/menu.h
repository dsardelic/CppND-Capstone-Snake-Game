#ifndef MENU_H
#define MENU_H

#include <future>  // std::promise

struct Menu {
  void RunGameLoop();
  void PlayNewSimpleGame(std::promise<unsigned short>&&);
  void PlayNewAdvancedGame(std::promise<unsigned short>&&);
  void UpdateHighScores(unsigned short);
  void ViewHighScores();
};

#endif
