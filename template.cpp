#include <iostream>
#include <fstream>
#include <ranges>
#include <getline.hpp>

using namespace std;

namespace rng = std::ranges;
namespace vw = std::ranges::views;


auto parse_input(char const* file){
  auto input = rd::getline(file);
}

void part1(char const* file){
}

void part2(char const* file){
}

int main() {
  constexpr static char const *test_file =
    "/home/rishabh/personal/advent_of_code_2021/src/day9/day9.test";
  constexpr static char const *input_file =
    "/home/rishabh/personal/advent_of_code_2021/src/day9/day9.input";
  part1(input_file);
  part2(input_file);
}
