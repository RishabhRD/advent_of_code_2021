#include <numeric>
#include <iostream>
#include <tl/to.hpp>
#include <ranges>
#include <fstream>

namespace rng = std::ranges;
namespace vw = std::ranges::views;

using namespace std;

auto parse_input(char const *file) {
  ifstream input(file);
  string line;
  getline(input, line);
  string_view input_str(line);
  return input_str | vw::split(',') | vw::transform([](auto str) {
    size_t num = 0;
    for (auto c : str) { num = num * 10 + (c - '0'); }
    return num;
  }) | tl::to<std::vector>();
}

struct fish_counter {
  vector<unsigned long long> counter;

  fish_counter(const vector<size_t> &input) : counter(9, 0) {
    for (auto n : input) counter[n]++;
  }

  decltype(auto) operator[](size_t const i) { return counter[i]; }

  auto operator[](size_t const i) const { return counter[i]; }

  auto count() const {
    return accumulate(cbegin(counter), cend(counter), 0ull);
  }
};

fish_counter update_fish_timer(fish_counter f_cnt) {
  rng::rotate(f_cnt.counter, next(begin(f_cnt.counter)));
  f_cnt[6] += f_cnt[8];
  return f_cnt;
}

fish_counter after_n_mates(int n, fish_counter init) {
  for (int i = 0; i < n; i++) { init = update_fish_timer(std::move(init)); }
  return init;
}

void part1(const char *file) {
  cout << after_n_mates(80, fish_counter{ parse_input(file) }).count() << endl;
}
void part2(const char *file) {
  cout << after_n_mates(256, fish_counter{ parse_input(file) }).count() << endl;
}

int main() {
constexpr static char const *const test_file =
  "/home/rishabh/personal/advent_of_code_2021/src/day6/day6.test";

constexpr static char const *const input_file =
  "/home/rishabh/personal/advent_of_code_2021/src/day6/day6.input";

  part1(input_file);
  part2(input_file);
}
