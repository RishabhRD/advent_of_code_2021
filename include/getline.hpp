#pragma once
#include <ranges>
#include <tl/to.hpp>
#include <fstream>
#include <vector>

namespace rd {

auto getline(char const *file) {
  std::ifstream stream{ file };
  std::vector<std::string> res;
  for(std::string line; getline(stream, line);){
    res.emplace_back(std::move(line));
  }
  return res;
}

}// namespace rd
