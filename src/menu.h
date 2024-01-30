#ifndef MENU_H
#define MENU_H

#include <future>  // std::promise

namespace Menu {
void Show();
void PlayNewSimpleGame(std::promise<unsigned short>&&);
void PlayNewAdvancedGame(std::promise<unsigned short>&&);
void UpdateHighScores(unsigned short);
void ViewHighScores();
};  // namespace Menu

#endif
