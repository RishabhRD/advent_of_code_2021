#pragma once
#include <string>
#include <ranges>
#include <tl/to.hpp>
#include <fstream>
#include <vector>
#include <tl/generator.hpp>

namespace rd {

auto getline(char const *file) {
  std::ifstream stream{ file };
  std::vector<std::string> res;
  for (std::string line; getline(stream, line);) { res.emplace_back(line); }
  return res;
}

tl::generator<std::string> getline_view(char const *file) {
  std::ifstream stream{ file };
  for (std::string line; getline(stream, line);) { co_yield line; }
}

}// namespace rd
