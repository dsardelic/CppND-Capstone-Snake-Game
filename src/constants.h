#include <stdlib.h>  // std::size_t

namespace Constants {

constexpr std::size_t kFramesPerSecond{60};
constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
constexpr std::size_t kScreenWidth{640};
constexpr std::size_t kScreenHeight{640};
constexpr std::size_t kGridWidth{kScreenWidth / 20};
constexpr std::size_t kGridHeight{kScreenHeight / 20};
}  // namespace Constants
