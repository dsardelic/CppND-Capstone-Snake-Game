#ifndef MENU_H
#define MENU_H

#include <future>  // std::promise

namespace Menu {
void Show();

template <class GameType>
void PlayNewGame();

void UpdateHighScores(unsigned short);
void ViewHighScores();
};  // namespace Menu

#endif
