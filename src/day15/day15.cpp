#include <queue>
#include <tl/zip.hpp>
#include <tl/generator.hpp>
#include <iostream>
#include <fstream>
#include <ranges>
#include <getline.hpp>

using namespace std;

using size_type = unsigned long long;

auto parse_input(char const *file) {
  return rd::getline_view(file) | views::transform([](string_view str) {
    return str | views::transform([](auto c) {
      return static_cast<size_type>(c - '0');
    }) | tl::to<vector>();
  }) | tl::to<vector>();
}

tl::generator<pair<int, int>> neighbors(int n, int x, int y) {
  constexpr static int dx_[] = { -1, 1, 0, 0 };
  constexpr static int dy_[] = { 0, 0, -1, 1 };
  for (auto [dx, dy] : tl::views::zip(dx_, dy_)) {
    int new_x = x + dx;
    int new_y = y + dy;
    if (new_x >= 0 and new_y >= 0 and new_x < n and new_y < n) {
      pair res{ new_x, new_y };
      co_yield res;
    }
  }
}

auto get_min_path(auto const &risk_level) {
  int const n = size(risk_level);
  constexpr static size_type inf = INT32_MAX;
  vector total_risk(n, vector(n, inf));
  total_risk[0][0] = 0;
  using pq_entry = tuple<size_type, int, int>;
  priority_queue<pq_entry, vector<pq_entry>, greater<pq_entry>> pq;
  pq.emplace(total_risk[0][0], 0, 0);
  while (not empty(pq)) {
    auto [cur_weight, cur_x, cur_y] = pq.top();
    pq.pop();
    for (auto [new_x, new_y] : neighbors(n, cur_x, cur_y)) {
      auto new_weight = risk_level[new_x][new_y];
      if (total_risk[new_x][new_y]
          > total_risk[cur_x][cur_y] + risk_level[new_x][new_y]) {
        total_risk[new_x][new_y] =
          total_risk[cur_x][cur_y] + risk_level[new_x][new_y];
        pq.emplace(total_risk[new_x][new_y], new_x, new_y);
      }
    }
  }
  return total_risk[n - 1][n - 1];
}

constexpr auto new_element = [](auto old_ele) {
  return old_ele + 1 < 10 ? old_ele + 1 : 1;
};

auto expand_column(auto map) {
  int const n = size(map[0]);
  int const num_to_add = 4;
  for (auto i : views::iota(0, num_to_add)) {
    for (auto &row : map) {
      for (auto num : row | views::reverse | views::take(n) | views::reverse
                        | views::transform(new_element) | tl::to<vector>()) {
        row.push_back(num);
      }
    }
  }
  return map;
}

auto expand_row(auto map) {
  int const original_num_rows = size(map);
  int const n = size(map[0]);
  int const num_to_add = 4;
  for (int i : views::iota(0, num_to_add)) {
    int const m = size(map);
    for (auto &&rows : map | views::reverse | views::take(original_num_rows)
                         | views::reverse | views::transform([](auto &&row) {
                             return row | views::transform(new_element)
                                    | tl::to<vector>();
                           })
                         | tl::to<vector>()) {
      map.push_back(std::forward<decltype(rows)>(rows));
    }
  }
  return map;
}

auto expand_map(auto map) { return expand_row(expand_column(move(map))); }

void part1(char const *file) {
  cout << get_min_path(parse_input(file)) << endl;
}

void part2(char const *file) {
  cout << get_min_path(expand_map(parse_input(file))) << endl;
}

int main() {
  constexpr static char const *test_file =
    "/home/rishabh/personal/advent_of_code_2021/src/day15/day15.test";
  constexpr static char const *input_file =
    "/home/rishabh/personal/advent_of_code_2021/src/day15/day15.input";
  part1(input_file);
  part2(input_file);
}
