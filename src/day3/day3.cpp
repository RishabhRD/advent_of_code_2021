#include <tl/to.hpp>
#include <ranges>
#include <numeric>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstddef>

namespace rng = std::ranges;
namespace vw = std::ranges::views;

enum class bit { zero, one };

constexpr static char const *test_file =
  "/home/rishabh/personal/advent_of_code_2021/src/day3/day3.test";
constexpr static char const *input_file =
  "/home/rishabh/personal/advent_of_code_2021/src/day3/day3.input";

bit to_bit(char c) {
  if (c == '0') return bit::zero;
  return bit::one;
}

int to_num(bit c) {
  if (c == bit::zero) return 0;
  return 1;
}

struct bit_count {
  size_t num_zero{};
  size_t num_one{};
};

bit most_frequent_bit(bit_count cnt) {
  if (cnt.num_zero > cnt.num_one)
    return bit::zero;
  else
    return bit::one;
}

bit most_frequent_bit_with_equality_value(bit_count cnt, bit equality_value) {
  if (cnt.num_zero > cnt.num_one)
    return bit::zero;
  else if (cnt.num_one > cnt.num_zero)
    return bit::one;
  else
    return equality_value;
}

bit least_frequent_bit_with_equality_value(bit_count cnt, bit equality_value) {
  if (cnt.num_zero < cnt.num_one)
    return bit::zero;
  else if (cnt.num_one < cnt.num_zero)
    return bit::one;
  else
    return equality_value;
}

bit invert_bit(bit b) {
  if (b == bit::zero) return bit::one;
  return bit::zero;
}

bit_count increment_bit_count(bit_count prev, bit cur_bit) {
  if (cur_bit == bit::zero) { return { prev.num_zero + 1, prev.num_one }; }
  return { prev.num_zero, prev.num_one + 1 };
}

std::vector<bit_count> accumulate_bit_count(
  const std::vector<std::string> &lines) {
  const size_t n = size(lines.front());
  return std::accumulate(cbegin(lines),
    cend(lines),
    std::vector(n, bit_count{ 0, 0 }),
    [=](std::vector<bit_count> pre, std::string_view line) {
      // TODO: make this functional programming style
      for (std::size_t i = 0; i < n; i++) {
        pre[i] = increment_bit_count(pre[i], to_bit(line[i]));
      }
      return pre;
    });
}

auto to_bits(const std::vector<bit_count> &b_counts) {
  return vw::transform(b_counts, most_frequent_bit);
}

auto invert_bits(auto bits) { return vw::transform(bits, invert_bit); }

int to_decimal(const auto bits) {
  return std::accumulate(cbegin(bits), cend(bits), 0, [](int prev, bit cur) {
    return prev * 2 + to_num(cur);
  });
}

auto get_lines(const char *file) {
  std::ifstream input{ file };
  return std::ranges::istream_view<std::string>(input) | tl::to<std::vector>();
}

void part1(char const *file) {
  const auto lines = get_lines(file);
  const auto b_counts = accumulate_bit_count(lines);
  auto gamma_bits = to_bits(b_counts);
  auto epsilon_bits = invert_bits(gamma_bits);
  std::cout << to_decimal(gamma_bits) * to_decimal(epsilon_bits) << std::endl;
}

int calculate_score(auto lines, auto criteria) {
  using size_type = std::remove_cvref_t<decltype(size(lines))>;
  size_type position = 0;
  while (size(lines) > 1) {
    auto filtered =
      lines | vw::filter([&](auto line) { return line[position] == '1'; });
    long unsigned int cnt = static_cast<long unsigned int>(rng::distance(filtered));
    auto most_common = criteria(cnt >= (size(lines) - cnt));
    std::erase_if(lines, [&](auto line) { return line[position] != most_common; });
    ++position;
  }
  return std::accumulate(begin(lines[0]), end(lines[0]), 0, [](int prev, char c) {
    return prev * 2 + (c - '0');
  });
}

void part2(char const *file) {
  const auto lines = get_lines(file);
  int ox_score = calculate_score(lines,
    [](bool one_is_most_common) { return one_is_most_common ? '1' : '0'; });
  int co_score = calculate_score(lines,
    [](bool one_is_most_common) { return one_is_most_common ? '0' : '1'; });
  std::cout << ox_score * co_score << std::endl;
}

int main() {
  part1(input_file);
  part2(input_file);
}
