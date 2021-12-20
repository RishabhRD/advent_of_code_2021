#include <ctre.hpp>
#include <iostream>
#include <fstream>
#include <ranges>
#include <getline.hpp>

using namespace std;

struct range {
  int min;
  int max;
};

auto to_int(auto &&rng) { return stoi(rng | tl::to<string>()); }

auto parse_input(char const *file) {
  auto line = rd::getline(file);
  auto m = ctre::match<"target area: x=(.+)\\.\\.(.+), y=(.+)\\.\\.(.+)">(line);
  range x_range{ to_int(m.get<1>()), to_int(m.get<2>()) };
  range y_range{ to_int(m.get<3>()), to_int(m.get<4>()) };
  return pair{ x_range, y_range };
}

bool is_in_range(range r, int num) { return num >= r.min and num <= r.max; }

bool is_in_range(range x_range, range y_range, int cur_x, int cur_y) {
  return is_in_range(x_range, cur_x) && is_in_range(y_range, cur_y);
}

int sign(int num) {
  if (num > 0)
    return 1;
  else if (num == 0)
    return 0;
  else
    return -1;
}

void part1(char const *file) {
  auto [x_range, y_range] = parse_input(file);
  auto max_y = [=](int vx, int vy) {
    constexpr static int num_steps = 1000;
    int x = 0;
    int y = 0;
    int max_height = 0;
    for (int i = 0; i < num_steps; i++) {
      x += vx;
      y += vy;
      max_height = max(max_height, y);
      vx -= sign(vx);
      vy -= 1;
      if (is_in_range(x_range, y_range, x, y)) { return max_height; }
    }
    return -1;
  };
  int max_height = 0;
  for (int vx = -1000; vx <= 1000; vx++) {
    for (int vy = -1000; vy <= 1000; vy++) {
      max_height = max(max_height, max_y(vx, vy));
    }
  }
  cout << max_height << endl;
}

void part2(char const *file) {
  auto [x_range, y_range] = parse_input(file);
  auto max_y = [=](int vx, int vy) {
    constexpr static int num_steps = 1000;
    int x = 0;
    int y = 0;
    int max_height = 0;
    for (int i = 0; i < num_steps; i++) {
      x += vx;
      y += vy;
      max_height = max(max_height, y);
      vx -= sign(vx);
      vy -= 1;
      if (is_in_range(x_range, y_range, x, y)) { return max_height; }
    }
    return -1;
  };
  int sum = 0;
  for (int vx = -1000; vx <= 1000; vx++) {
    for (int vy = -1000; vy <= 1000; vy++) {
      if(max_y(vx, vy) != -1) sum++;
    }
  }
  cout << sum << endl;
}

int main() {
  constexpr static char const *test_file =
    "/home/rishabh/personal/advent_of_code_2021/src/day17/day17.test";
  constexpr static char const *input_file =
    "/home/rishabh/personal/advent_of_code_2021/src/day17/day17.input";
  part1(input_file);
  part2(input_file);
}
