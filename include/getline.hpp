#pragma once
#include <string>
#include <ranges>
#include <tl/to.hpp>
#include <fstream>
#include <vector>
#include <rd/generator.hpp>

namespace rd {

rd::generator<std::string> getlines(char const *file) {
  std::ifstream stream{ file };
  for (std::string line; getline(stream, line);) { co_yield line; }
}

std::string getline(char const* file){
  std::ifstream stream{ file };
  std::string line;
  getline(stream, line);
  return line;
}

}// namespace rd
