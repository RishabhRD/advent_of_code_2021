#include <ranges>
#include <vector>
#include <iostream>
#include <fstream>
#include <ctre.hpp>

using namespace std;
namespace vw = std::ranges::views;

template<typename T> int sgn(T val) { return (T(0) < val) - (val < T(0)); }

constexpr static char const *const test_file =
  "/home/rishabh/personal/advent_of_code_2021/src/day5/day5.test";

constexpr static char const *const input_file =
  "/home/rishabh/personal/advent_of_code_2021/src/day5/day5.input";

struct point {
  int x;
  int y;

  friend auto operator<=>(const point &, const point &) = default;
};

struct line {
  point start;
  point end;
};

struct point_matrix {
  vector<vector<int>> matrix;
  point_matrix(int max_x, int max_y)
    : matrix(max_x + 1, vector<int>(max_y + 1)) {}

  int count_if(auto &&f) {
    int cnt = 0;
    for (const auto &vec : matrix) {
      for (auto n : vec) {
        if (f(n)) cnt++;
      }
    }
    return cnt;
  }

  void add(point p) { matrix[p.x][p.y]++; }

  void print() {
    for (const auto &vec : matrix) {
      for (auto n : vec) {
        if (n == 0)
          cout << ". ";
        else
          cout << n << " ";
      }
      cout << endl;
    }
  }
};

auto to_int(auto m) { return stoi(string(begin(m), end(m))); }

line to_line(string str) {
  auto m = ctre::match<"(\\d+),(\\d+) -> (\\d+),(\\d+)">(str);
  return line{ point{ to_int(m.get<1>()), to_int(m.get<2>()) },
    point{ to_int(m.get<3>()), to_int(m.get<4>()) } };
}

auto parse_input(char const *file) {
  ifstream input{ file };
  vector<line> res;
  for (string line; getline(input, line);) { res.push_back(to_line(line)); }
  return res;
}

point get_extreme_point(const vector<line> &lines) {
  int max_x = 0;
  int max_y = 0;
  for (auto [s, e] : lines) {
    max_x = max({ max_x, s.x, e.x });
    max_y = max({ max_y, s.y, e.y });
  }
  return { max_x, max_y };
}

bool is_horizontal_line(line l) { return l.start.y == l.end.y; }

bool is_vertical_line(line l) { return l.start.x == l.end.x; }

bool is_positive_diagonal_line(line l) {
  auto [x1, y1] = l.start;
  auto [x2, y2] = l.end;
  return (abs(x2 - x1) == abs(y2 - y1)) && (sgn(x2 - x1) == sgn(y2 - y1));
}

bool is_negative_diagonal_line(line l) {
  auto [x1, y1] = l.start;
  auto [x2, y2] = l.end;
  return (abs(x2 - x1) == abs(y2 - y1)) && (sgn(x2 - x1) != sgn(y2 - y1));
}

bool is_diagonal_line(line l) {
  return is_positive_diagonal_line(l) or is_negative_diagonal_line(l);
}

void for_each_horizontal_point(line l, auto &&f) {
  auto [x1, y1] = l.start;
  auto [x2, y2] = l.end;
  for (auto x : vw::iota(min(x1, x2), max(x1, x2) + 1)) { f(point{ x, y1 }); }
}

void for_each_vertical_point(line l, auto &&f) {
  auto [x1, y1] = l.start;
  auto [x2, y2] = l.end;
  for (auto y : vw::iota(min(y1, y2), max(y1, y2) + 1)) { f(point{ x1, y }); }
}

void traverse_diagonal_with(line l, auto &&f, auto &&next) {
  auto [x1, y1] = l.start;
  auto [x2, y2] = l.end;
  if (max(x1, x2) - min(x1, x2) != max(y1, y2) - min(y1, y2)) return;
  auto min_point = min(l.start, l.end);
  auto max_point = max(l.start, l.end);
  for (auto cur = min_point; cur <= max_point; cur = next(cur)) { f(cur); }
}

void for_each_diagonal_point(line l, auto &&f) {
  auto positive_next = [](point p) { return point{ p.x + 1, p.y + 1 }; };
  auto negative_next = [](point p) { return point{ p.x + 1, p.y - 1 }; };
  if (is_positive_diagonal_line(l)) {
    traverse_diagonal_with(l, f, positive_next);
  } else {
    traverse_diagonal_with(l, f, negative_next);
  }
}

void part1(char const *file) {
  auto lines = parse_input(file);
  auto [max_x, max_y] = get_extreme_point(lines);
  point_matrix matrix{ max_x, max_y };
  auto add_to_matrix = [&](point p) { matrix.add(p); };
  for (line l : lines) {
    if (is_horizontal_line(l))
      for_each_horizontal_point(l, add_to_matrix);
    else if (is_vertical_line(l))
      for_each_vertical_point(l, add_to_matrix);
  }
  cout << matrix.count_if([](auto num) { return num > 1; }) << endl;
}

void part2(char const *file) {
  auto lines = parse_input(file);
  auto [max_x, max_y] = get_extreme_point(lines);
  point_matrix matrix{ max_x, max_y };
  auto add_to_matrix = [&](point p) { matrix.add(p); };
  for (line l : lines) {
    if (is_horizontal_line(l))
      for_each_horizontal_point(l, add_to_matrix);
    else if (is_vertical_line(l))
      for_each_vertical_point(l, add_to_matrix);
    else if (is_diagonal_line(l))
      for_each_diagonal_point(l, add_to_matrix);
  }
  cout << matrix.count_if([](auto num) { return num > 1; }) << endl;
}

int main() {
  part1(input_file);
  part2(input_file);
}
