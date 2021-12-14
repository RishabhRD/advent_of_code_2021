#include <tl/fold.hpp>
#include <tl/adjacent.hpp>
#include <unordered_map>
#include <ctre.hpp>
#include <string>
#include <tl/to.hpp>
#include <iostream>
#include <fstream>
#include <ranges>
#include <getline.hpp>

using namespace std;

using size_type = unsigned long long;

auto parse_polymer_template(string_view str) {
  const int n = size(str) - 1;
  unordered_map<string, size_type> cnts;
  for (int i = 0; i < n; i++) { cnts[string({ str[i], str[i + 1] })]++; }
  cnts[string({ str[n], ' ' })] = 1;
  return cnts;
}

auto parse_input(char const *file) {
  auto input = rd::getline(file);
  auto polymer_template = parse_polymer_template(input[0]);
  auto rules = input | views::drop(2) | views::transform([](string_view str) {
    auto m = ctre::match<"(.+) -> (.)">(str);
    auto pattern = m.get<1>() | tl::to<string>();
    auto addition_view = m.get<2>();
    auto addition = *ranges::begin(addition_view);
    return pair{ std::move(pattern), std::move(addition) };
  }) | tl::to<unordered_map>();
  return pair{ std::move(polymer_template), std::move(rules) };
}

auto add_polymer(auto &&polymer, auto const &mapping) {
  vector<pair<string, size_type>> to_add;
  for (auto [str, val] : mapping) {
    if (polymer.contains(str)) {
      auto cnt = polymer[str];
      polymer.erase(str);
      string first({ str[0], val });
      string second({ val, str[1] });
      to_add.emplace_back(std::move(first), cnt);
      to_add.emplace_back(std::move(second), cnt);
    }
  }
  for(auto const& [str, val] : to_add){
    polymer[str] += val;
  }
  return polymer;
}

auto add_polymer_n(auto polymer, auto const &mapping, int n) {
  return tl::fold(
    views::iota(0, n), std::move(polymer), [&mapping](auto &&prev, auto) {
      return add_polymer(forward<decltype(prev)>(prev), mapping);
    });
}

constexpr inline auto polymer_count_view = views::transform([](auto const &pr) {
  return pair{ pr.first[0], pr.second };
});

auto get_poly_char_counts(auto const &polymer) {
  array<size_type, 26> cnts{ 0 };
  for (auto const &[c, val] : polymer | polymer_count_view) {
    cnts[c - 'A'] += val;
  }
  return cnts | views::filter([](auto ele) { return ele != 0; })
         | tl::to<vector>();
}

void part1(char const *file) {
  auto [polymer, mapping] = parse_input(file);
  polymer = add_polymer_n(std::move(polymer), mapping, 10);
  auto char_cnts = get_poly_char_counts(polymer);
  cout << *ranges::max_element(char_cnts) - *ranges::min_element(char_cnts)
       << endl;
}

void part2(char const *file) {
  auto [polymer, mapping] = parse_input(file);
  polymer = add_polymer_n(std::move(polymer), mapping, 40);
  auto char_cnts = get_poly_char_counts(polymer);
  cout << *ranges::max_element(char_cnts) - *ranges::min_element(char_cnts)
       << endl;
}

int main() {
  constexpr static char const *test_file =
    "/home/rishabh/personal/advent_of_code_2021/src/day14/day14.test";
  constexpr static char const *input_file =
    "/home/rishabh/personal/advent_of_code_2021/src/day14/day14.input";
  part1(input_file);
  part2(input_file);
}
