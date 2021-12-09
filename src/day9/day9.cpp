#include <queue>
#include <functional>
#include <tl/generator.hpp>
#include <vector>
#include <ranges>
#include <iostream>
#include <tl/to.hpp>
#include <getline.hpp>
#include <tl/fold.hpp>

using namespace std;

namespace rng = std::ranges;
namespace vw = std::ranges::views;

// TODO: how to yield r-value in generator

tl::generator<pair<int, int>> neighbors(int m, int n, int i, int j) {
  if (i > 0) {
    pair res{ i - 1, j };
    co_yield res;
  }
  if (j > 0) {
    pair res{ i, j - 1 };
    co_yield res;
  }
  if (i < m - 1) {
    pair res{ i + 1, j };
    co_yield res;
  }
  if (j < n - 1) {
    pair res{ i, j + 1 };
    co_yield res;
  }
}

tl::generator<pair<int, int>> low_point_heights(auto &matrix) {
  auto const m = size(matrix);
  auto const n = size(matrix[0]);
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      auto neigh = neighbors(m, n, i, j);
      if (rng::all_of(neigh, [&matrix, i, j](auto node) {
            return matrix[node.first][node.second] > matrix[i][j];
          })) {
        pair<int, int> res{ i, j };
        co_yield res;
      }
    }
  }
}

auto parse_input(char const *file) {
  auto input = rd::getline(file);
  return input | vw::transform([](string_view str) {
    return str | vw::transform([](auto c) { return c - '0'; })
           | tl::to<vector>();
  }) | tl::to<vector>();
}

void part1(char const *file) {
  auto matrix = parse_input(file);
  auto risks = low_point_heights(matrix) | vw::transform([&matrix](auto p) {
    return matrix[p.first][p.second] + 1;
  });
  cout << tl::fold(risks, 0, std::plus<>()) << endl;
}

tl::generator<pair<int, int>> to_basin(auto &matrix,
  pair<int, int> init_point) {
  const auto m = size(matrix);
  const auto n = size(matrix[0]);
  vector visited(m, vector(n, false));
  queue<pair<int, int>> q;
  q.push(init_point);
  auto is_not_9 = [&](pair<int, int> p) {
    return matrix[p.first][p.second] != 9;
  };
  auto is_not_visited = [&](pair<int, int> p) {
    return visited[p.first][p.second] == false;
  };
  while (not empty(q)) {
    auto [x, y] = q.front();
    pair y_res{ x, y };
    co_yield y_res;
    q.pop();
    auto is_increasing = [&matrix, x, y](pair<int, int> p) {
      return matrix[p.first][p.second] > matrix[x][y];
    };
    for (auto [new_x, new_y] : neighbors(m, n, x, y) | vw::filter(is_not_9)
                                 | vw::filter(is_not_visited)
                                 | vw::filter(is_increasing)) {
      visited[new_x][new_y] = true;
      q.emplace(new_x, new_y);
    }
  }
}

void part2(char const *file) {
  auto matrix = parse_input(file);
  auto basin_sizes =
    low_point_heights(matrix) | vw::transform([&matrix](auto init_point) {
      return to_basin(matrix, init_point);
    })
    | vw::transform([](auto basin) { return rng::distance(basin); })
    | tl::to<vector>();
  rng::sort(basin_sizes, greater<int>{});
  cout << basin_sizes[0] * basin_sizes[1] * basin_sizes[2] << endl;
}

int main() {
  constexpr static char const *test_file =
    "/home/rishabh/personal/advent_of_code_2021/src/day9/day9.test";
  constexpr static char const *input_file =
    "/home/rishabh/personal/advent_of_code_2021/src/day9/day9.input";
  part1(input_file);
  part2(input_file);
}
