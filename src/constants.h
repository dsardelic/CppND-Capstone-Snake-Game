#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdlib.h>  // std::size_t

#include <string>  // std::string

namespace Constants {

constexpr std::size_t kFramesPerSecond{60};
constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
constexpr std::size_t kScreenWidth{640};
constexpr std::size_t kScreenHeight{640};
constexpr std::size_t kGridWidth{kScreenWidth / 20};
constexpr std::size_t kGridHeight{kScreenHeight / 20};

const std::string kHighScoresFileUri{"highscores.dat"};
constexpr std::size_t kMaxHighScoresCount{10};
}  // namespace Constants

#endif