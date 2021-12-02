#include <iostream>
#include <fstream>
#include <ranges>
#include <tl/adjacent.hpp>
#include <tl/to.hpp>


namespace views = std::views;

using namespace std;

void part1() {
  std::ifstream input{
    "/home/rishabh/personal/advent_of_code_2021/src/day1/day1"
  };
  auto input_vec =
    std::ranges::istream_view<int>(input) | tl::to<std::vector>();

  int cnt = 0;

  for (size_t i = 1; i < size(input_vec); i++) {
    if (input_vec[i] > input_vec[i - 1]) cnt++;
  }

  std::cout << cnt << endl;
}

void part2(){
  std::ifstream input{
    "/home/rishabh/personal/advent_of_code_2021/src/day1/day1"
  };
  auto input_vec =
    std::ranges::istream_view<int>(input) | tl::to<std::vector>();
  int cnt = 0;
  size_t sliding_len = 3;
  int prev_sum = input_vec[0] + input_vec[1] + input_vec[2];
  size_t last_idx = size(input_vec) - sliding_len + 1;
  for(size_t i = 1; i < last_idx; i++){
    int cur_sum = input_vec[i] + input_vec[i + 1] + input_vec[i + 2];
    if(cur_sum > prev_sum) cnt++;
    prev_sum = cur_sum;
  }
  cout << cnt << endl;
}

int main() {
  part2();
}
