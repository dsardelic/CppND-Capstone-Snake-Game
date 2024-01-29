#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>  // std::string

namespace Constants {

constexpr unsigned short kFramesPerSecond{60};
constexpr unsigned short kMsPerFrame{1000 / kFramesPerSecond};
constexpr unsigned short kScreenWidth{640};
constexpr unsigned short kScreenHeight{640};
constexpr unsigned short kGridWidth{kScreenWidth / 20};
constexpr unsigned short kGridHeight{kScreenHeight / 20};
constexpr unsigned short kMaxHighScoresCount{10};
const std::string kHighScoresFileUri{"highscores.dat"};
constexpr float kSnakeSpeedIncreaseStep{0.02f};
}  // namespace Constants

#endif
