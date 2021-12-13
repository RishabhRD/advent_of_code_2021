#include <set>
#include <tl/fold.hpp>
#include <ctre.hpp>
#include <unordered_set>
#include <tl/to.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <ranges>
#include <getline.hpp>

using namespace std;

namespace rng = std::ranges;
namespace vw = std::ranges::views;

struct point {
  int x;
  int y;
  friend auto operator<=>(const point &, const point &) = default;
};

struct point_hash {
  std::size_t operator()(point const &pnt) const noexcept {
    auto h1 = std::hash<int>{}(pnt.x);
    auto h2 = std::hash<int>{}(pnt.y);
    return h1 ^ (h2 << 1);
  }
};

enum class rotation_axis { x, y };

struct fold_instruction {
  rotation_axis axis;
  int val;
};

auto to_number(auto &&rng) {
  int num = 0;
  for (auto c : rng) { num = num * 10 + (c - '0'); }
  return num;
}

auto front(auto &&rng) { return *rng::begin(rng); }

auto to_point(auto &&rng) {
  auto cur_itr = rng::begin(rng);
  auto x = *cur_itr;
  advance(cur_itr, 1);
  auto y = *cur_itr;
  return point{ x, y };
}

auto to_axis(auto &&rng) {
  auto c = *rng::begin(rng);
  if (c == 'x')
    return rotation_axis::x;
  else
    return rotation_axis::y;
}

auto parse_input(char const *file) {
  auto input = rd::getline(file);
  auto empty_line = rng::find(input, "");
  auto points =
    rng::subrange(rng::begin(input), empty_line)
    | vw::transform([](string_view str) {
        return to_point(str | vw::split(',') | vw::transform([](auto &&rng) {
          return to_number(rng);
        }));
      })
    | tl::to<unordered_set<point, point_hash>>();
  auto fold_instructions = rng::subrange(next(empty_line), rng::end(input))
                           | vw::transform([](string_view str) {
                               auto m =
                                 ctre::match<"fold along (.+)=(\\d+)">(str);
                               auto axis = to_axis(m.get<1>());
                               auto val = to_number(m.get<2>());
                               return fold_instruction{ axis, val };
                             })
                           | tl::to<vector>();
  return pair{ std::move(points), std::move(fold_instructions) };
}

auto fold_x(int x, auto &&points) {
  return points | vw::transform([=](auto pnt) {
    if (pnt.x > x)
      return point{ 2 * x - pnt.x, pnt.y };
    else
      return pnt;
  }) | tl::to<unordered_set<point, point_hash>>();
}

auto fold_y(int y, auto &&points) {
  return points | vw::transform([=](auto pnt) {
    if (pnt.y > y)
      return point{ pnt.x, 2 * y - pnt.y };
    else
      return pnt;
  }) | tl::to<unordered_set<point, point_hash>>();
}

auto fold(auto &&points, fold_instruction ins) {
  if (ins.axis == rotation_axis::x) {
    return fold_x(ins.val, forward<decltype(points)>(points));
  } else {
    return fold_y(ins.val, forward<decltype(points)>(points));
  }
}

void part1(char const *file) {
  auto [points, fold_instructions] = parse_input(file);
  cout << size(fold(std::move(points), fold_instructions[0])) << endl;
}

void part2(char const *file) {
  auto [points, fold_instructions] = parse_input(file);
  points = tl::fold(fold_instructions, std::move(points), [](auto &&points, auto ins) {
    return fold(forward<decltype(points)>(points), ins);
  });
  for(int i = 0; i < 50; i++){
    for(int j = 0; j < 50; j++){
      if(points.contains(point{j, i})){
	cout << '*';
      }else{
	cout << ' ';
      }
    }
    cout << endl;
  }
}

int main() {
  constexpr static char const *test_file =
    "/home/rishabh/personal/advent_of_code_2021/src/day13/day13.test";
  constexpr static char const *input_file =
    "/home/rishabh/personal/advent_of_code_2021/src/day13/day13.input";
  part1(input_file);
  part2(input_file);
}
