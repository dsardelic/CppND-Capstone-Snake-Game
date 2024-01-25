#include <iostream>  // std::cout

#include "menu.h"  // Menu

int main() {
  std::cout << "WELCOME TO SNAKE GAME!" << std::endl;
  std::cout << std::endl;
  Menu menu;
  menu.RunGameLoop();
  return 0;
}
