#pragma once

#include <ios>
#include <iostream>
#include <istream>
#include <limits>

inline void menu_general(std::ranges::range auto entries) {
  for (const auto &entry : entries) {
    std::cout << entry << std::endl;
  }
}

inline int menu_select(int lower_bound, int upper_bound) {
  int selection;
  do {
    std::cin >> selection;
  } while (selection < lower_bound || selection > upper_bound);
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  return selection;
}