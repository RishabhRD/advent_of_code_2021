#include <algorithm>
#include <iostream>
#include <fstream>
#include <ranges>

namespace rng = std::ranges;


using namespace std;

void part1() {
  std::ifstream input{
    "/home/rishabh/personal/advent_of_code_2021/src/day2/part1"
  };

  auto x = 0;
  auto y = 0;

  for (std::string line; std::getline(input, line);) {
    auto space_itr = rng::find(line, ' ');
    auto first = string_view{ begin(line), space_itr };
    auto amt = stoi(string{ next(space_itr), end(line) });
    if (first == "forward") {
      x += amt;
    } else if (first == "up") {
      y -= amt;
    } else {
      y += amt;
    }
  }
  cout << x * y << endl;
}

void part2() {
  std::ifstream input{
    "/home/rishabh/personal/advent_of_code_2021/src/day2/part2"
  };

  auto aim = 0;
  auto x = 0;
  auto y = 0;

  for (std::string line; std::getline(input, line);) {
    auto space_itr = rng::find(line, ' ');
    auto first = string_view{ begin(line), space_itr };
    auto amt = stoi(string{ next(space_itr), end(line) });
    if(first == "down"){
      aim += amt;
    }else if(first == "up"){
      aim -= amt;
    }else{
      x += amt;
      y += (aim * amt);
    }
  }
  cout << x * y << endl;
}

int main() { part2(); }
