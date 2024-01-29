#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"  // Snake

class Controller {
 public:
  void HandleInput(bool&, Snake&) const;
};

#endif
