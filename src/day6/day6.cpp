#include <numeric>
#include <iostream>
#include <tl/to.hpp>
#include <ranges>
#include <fstream>
#include <array>

namespace rng = std::ranges;
namespace vw = std::ranges::views;

using namespace std;


using ull = unsigned long long;
using fish_counter = array<ull, 9>;

template<typename T> auto count(const auto &cont) {
  return accumulate(cbegin(cont), cend(cont), T{});
}

auto parse_input(char const *file) {
  ifstream input(file);
  string line;
  getline(input, line);
  string_view input_str(line);
  auto input_vec = input_str | vw::split(',') | vw::transform([](auto str) {
    size_t num = 0;
    for (auto c : str) { num = num * 10 + (c - '0'); }
    return num;
  }) | tl::to<std::vector>();
  fish_counter cnt{};
  for (auto n : input_vec) { cnt[n]++; }
  return cnt;
}

fish_counter update_fish_timer(fish_counter f_cnt) {
  rng::rotate(f_cnt, next(begin(f_cnt)));
  f_cnt[6] += f_cnt[8];
  return f_cnt;
}

fish_counter after_n_mates(int n, fish_counter init) {
  for (int i = 0; i < n; i++) { init = update_fish_timer(std::move(init)); }
  return init;
}

void part1(const char *file) {
  cout << count<ull>(after_n_mates(80, parse_input(file))) << endl;
}
void part2(const char *file) {
  cout << count<ull>(after_n_mates(256, parse_input(file))) << endl;
}

int main() {
  constexpr static char const *const test_file =
    "/home/rishabh/personal/advent_of_code_2021/src/day6/day6.test";

  constexpr static char const *const input_file =
    "/home/rishabh/personal/advent_of_code_2021/src/day6/day6.input";

  part1(input_file);
  part2(input_file);
}
