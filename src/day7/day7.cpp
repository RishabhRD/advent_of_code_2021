#include <numeric>
#include <tl/to.hpp>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

namespace rng = std::ranges;
namespace vw = std::ranges::views;

auto parse_input(char const *file) {
  ifstream input(file);
  string line;
  getline(input, line);
  string_view input_str(line);
  return input_str | vw::split(',') | vw::transform([](auto str) {
    int num = 0;
    for (auto c : str) { num = num * 10 + (c - '0'); }
    return num;
  }) | tl::to<std::vector>();
}

// precondition: vec is sorted
int get_median(const vector<int> &vec) {
  size_t mid_pos = size(vec) / 2;
  if (size(vec) % 2 == 0) {
    return (vec[mid_pos] + vec[mid_pos - 1]) / 2;
  } else {
    return vec[mid_pos];
  }
}

int get_min_fuel_cost(char const *file, auto &&ele_pair_cost_func) {
  auto positions = parse_input(file);
  auto max_ele = *rng::max_element(positions);
  auto costs = vw::iota(0, max_ele + 1) | vw::transform([&](auto to_pos) {
    auto to_pos_costs = positions | vw::transform([=](auto from_pos) {
      return ele_pair_cost_func(from_pos, to_pos);
    }) | tl::to<std::vector>();
    return accumulate(cbegin(to_pos_costs), cend(to_pos_costs), 0);
  });
  return *rng::min_element(costs);
}

void part1(char const *file) {
  cout << get_min_fuel_cost(file, [](auto from, auto to) {
    return abs(to - from);
  }) << endl;
}

void part2(char const *file) {
  cout << get_min_fuel_cost(file, [](auto from, auto to) {
    int n = abs(to - from);
    return n * (n + 1) / 2;
  }) << endl;
}

int main() {
  constexpr static char const *const test_file =
    "/home/rishabh/personal/advent_of_code_2021/src/day7/day7.test";

  constexpr static char const *const input_file =
    "/home/rishabh/personal/advent_of_code_2021/src/day7/day7.input";
  part1(input_file);
  part2(input_file);
}
