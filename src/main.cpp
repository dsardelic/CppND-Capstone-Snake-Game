#include <stdlib.h>  // system

#include <iostream>  // std::cout

#include "menu.h"  // Menu

int main() {
  system("clear");
  std::cout << "WELCOME TO SNAKE GAME!" << std::endl;
  std::cout << std::endl;
  Menu::Show();
  return 0;
}
