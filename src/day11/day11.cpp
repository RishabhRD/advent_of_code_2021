#include <set>
#include <tl/cartesian_product.hpp>
#include <functional>
#include <tl/fold.hpp>
#include <tl/zip.hpp>
#include <tl/generator.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <ranges>
#include <getline.hpp>

using namespace std;

namespace rng = std::ranges;
namespace vw = std::ranges::views;


auto parse_input(char const *file) {
  return rd::getlines(file) | vw::transform([](auto line) {
    return line | vw::transform([](auto c) { return c - '0'; })
           | tl::to<vector>();
  }) | tl::to<vector>();
}

tl::generator<pair<int, int>> neighbors(int m, int n, int cur_x, int cur_y) {
  constexpr int dx[] = { -1, 1, 0, 0, -1, -1, 1, 1 };
  constexpr int dy[] = { 0, 0, -1, 1, -1, 1, -1, 1 };
  for (auto [x, y] : tl::views::zip(dx, dy)) {
    int new_x = x + cur_x;
    int new_y = y + cur_y;
    if (new_x >= 0 and new_y >= 0 and new_x < m and new_y < n) {
      pair res{ new_x, new_y };
      co_yield res;
    }
  }
}

tl::generator<pair<int, int>> matrix_indecies(int const m, int const n) {
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      pair res{ i, j };
      co_yield res;
    }
  }
}

auto opposite(auto &&func) {
  return [&func](auto ele) { return !func(ele); };
}

int flow_lights(auto &&matrix) {
  int const m = size(matrix);
  int const n = size(matrix[0]);
  auto is_9_or_less = [&matrix](pair<int, int> p) {
    return matrix[p.first][p.second] <= 9;
  };
  auto is_greater_than_9 = opposite(is_9_or_less);
  set<pair<int, int>> to_glow;
  set<pair<int, int>> next_to_glow;
  for (auto [x, y] : matrix_indecies(m, n) | vw::filter(is_greater_than_9)) {
    to_glow.emplace(x, y);
  }
  while (not empty(to_glow)) {
    for (auto [x, y] : to_glow) {
      for (auto [new_x, new_y] :
        neighbors(m, n, x, y) | vw::filter(is_9_or_less)) {
        matrix[new_x][new_y]++;
        if (matrix[new_x][new_y] > 9) next_to_glow.emplace(new_x, new_y);
      }
    }
    to_glow = exchange(next_to_glow, {});
  }
  int score = 0;
  for (auto [x, y] : matrix_indecies(m, n) | vw::filter(is_greater_than_9)) {
    matrix[x][y] = 0;
    score++;
  }
  return score;
}

void increment_matrix(vector<vector<int>> &matrix) {
  int const m = size(matrix);
  int const n = size(matrix[0]);
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) { matrix[i][j]++; }
  }
}

auto next_step(auto &matrix) {
  increment_matrix(matrix);
  return flow_lights(matrix);
}

void part1(char const *file) {
  auto matrix = parse_input(file);
  auto score = 0;
  for (int i = 0; i < 100; i++) { score += next_step(matrix); }
  cout << score << endl;
}

void part2(char const *file) {
  auto matrix = parse_input(file);
  int const m = size(matrix);
  int const n = size(matrix[0]);
  int step = 1;
  for (;; step++) {
    auto score = next_step(matrix);
    if (score == m * n) { break; }
  }
  cout << step << endl;
}

int main() {
  constexpr static char const *test_file =
    "/home/rishabh/personal/advent_of_code_2021/src/day11/day11.test";
  constexpr static char const *input_file =
    "/home/rishabh/personal/advent_of_code_2021/src/day11/day11.input";
  part1(input_file);
  part2(input_file);
}
