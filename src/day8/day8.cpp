#include <bitset>
#include <ranges>
#include <numeric>
#include <tl/to.hpp>
#include <iostream>
#include <getline.hpp>

using namespace std;

namespace rng = std::ranges;
namespace vw = std::ranges::views;

using digit = std::bitset<7>;

struct display {
  vector<digit> representations;
  vector<digit> output;
};

display parse_display(string_view line) {
  auto to_digit = [](auto rng) {
    digit d;
    for (auto c : rng) { d[c - 'a'] = true; }
    return d;
  };
  auto representations = line | vw::split(' ') | vw::take(10)
                         | vw::transform(to_digit) | tl::to<vector>();
  auto outupt = line | vw::split(' ') | vw::drop(11) | vw::transform(to_digit)
                | tl::to<vector>();
  return { std::move(representations), std::move(outupt) };
}

auto opposite(auto &&predicate) {
  return [predicate = std::forward<decltype(predicate)>(predicate)](
           auto i) { return !predicate(i); };
}

auto front(auto input) { return *rng::begin(input); }

auto contains(digit outer, digit inner) { return (inner & outer) == inner; }

auto decode_representation(const display &display) {
  array<digit, 10> mappings;
  const auto &rep = display.representations;
  for (auto dig : rep) {
    if (dig.count() == 2)
      mappings[1] = dig;
    else if (dig.count() == 3)
      mappings[7] = dig;
    else if (dig.count() == 4)
      mappings[4] = dig;
    else if (dig.count() == 7)
      mappings[8] = dig;
  }

  auto contains_n = [&mappings](int num){
    return [num, &mappings](digit d){
      return contains(d, mappings[num]);
    };
  };
  auto contained_by_n = [&mappings](int num){
    return [num, &mappings](digit d){
      return contains(mappings[num], d);
    };
  };
  auto len_of = [](int len){
    return [len](digit d){
      return d.count() == len;
    };
  };

  auto six_len_ele = rep | vw::filter(len_of(6));
  mappings[9] = front(six_len_ele | vw::filter(contains_n(4)));
  auto zero_or_six = six_len_ele | vw::filter(opposite(contains_n(4)));
  mappings[0] = front(zero_or_six | vw::filter(contains_n(1)));
  mappings[6] = front(zero_or_six | vw::filter(opposite(contains_n(1))));

  auto five_len_ele = rep | vw::filter(len_of(5));
  mappings[3] = front(five_len_ele | vw::filter(contains_n(1)));
  auto five_or_two =  five_len_ele | vw::filter(opposite(contains_n(1)));
  mappings[5] = front(five_or_two | vw::filter(contained_by_n(6)));
  mappings[2] = front(five_or_two | vw::filter(opposite(contained_by_n(6))));

  return mappings;
}

auto parse_displays(char const *file) {
  return rd::getlines(file) | vw::transform(parse_display) | tl::to<vector>();
}

void part1(char const *file) {
  auto displays = parse_displays(file);
  cout << accumulate(
    cbegin(displays), cend(displays), 0, [](auto cur, auto &&display) {
      return cur + rng::count_if(display.output, [](auto bits) {
        return bits.count() == 2 || bits.count() == 3 || bits.count() == 4
               || bits.count() == 7;
      });
    }) << endl;
}

auto to_decimal(const display &display) {
  auto const mappings = decode_representation(display);
  auto nums = display.output | vw::transform([&mappings](auto dig) {
    auto dist = rng::find(mappings, dig) - rng::cbegin(mappings);
    return dist;
  }) | tl::to<std::vector>();
  auto num = accumulate(cbegin(nums), cend(nums), 0, [](auto init, auto ele) {
    return init * 10 + ele;
  });
  return num;
}

void part2(char const *file) {
  auto displays = parse_displays(file);
  auto nums = displays | vw::transform(to_decimal) | tl::to<std::vector>();
  cout << accumulate(cbegin(nums), cend(nums), 0) << endl;
}

int main() {
  constexpr static char const *test_file =
    "/home/rishabh/personal/advent_of_code_2021/src/day8/day8.test";
  constexpr static char const *input_file =
    "/home/rishabh/personal/advent_of_code_2021/src/day8/day8.input";
  part1(input_file);
  part2(input_file);
}
