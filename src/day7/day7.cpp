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

auto cost(const auto &positions, auto &&element_cost_function) {
  auto transformed_vec =
    positions | vw::transform(element_cost_function) | tl::to<std::vector>();
  return accumulate(cbegin(transformed_vec), cend(transformed_vec), 0);
}

void part1(char const *file) {
  auto positions = parse_input(file);
  rng::sort(positions);
  int const median = get_median(positions);
  cout << cost(positions, [=](auto num) { return abs(median - num); }) << endl;
}

void part2(char const *file) {
  auto positions = parse_input(file);
  rng::sort(positions);
  auto max_ele = positions.back();
  auto costs = vw::iota(0, max_ele + 1) | vw::transform([&](auto pos) {
    return cost(positions, [=](auto ele) {
      int n = abs(pos - ele);
      return n * (n + 1) / 2;
    });
  });
  cout << *rng::min_element(costs) << endl;
}

int main() {
  constexpr static char const *const test_file =
    "/home/rishabh/personal/advent_of_code_2021/src/day7/day7.test";

  constexpr static char const *const input_file =
    "/home/rishabh/personal/advent_of_code_2021/src/day7/day7.input";
  part1(input_file);
  part2(input_file);
}
