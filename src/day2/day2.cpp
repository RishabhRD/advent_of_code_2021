#include <algorithm>
#include <iostream>
#include <fstream>
#include <ranges>

namespace rng = std::ranges;


using namespace std;

constexpr static char const *test_file =
  "/home/rishabh/personal/advent_of_code_2021/src/day2/day2";

enum class cmd_type { forward, down, up };

struct command {
  cmd_type type;
  int val;
};

cmd_type to_cmd_type(string_view str) {
  if (str == "forward") {
    return cmd_type::forward;
  } else if (str == "up") {
    return cmd_type::up;
  } else {
    return cmd_type::down;
  }
}

command to_command(string_view str) {
  auto const space_itr = rng::find(str, ' ');
  return { to_cmd_type(string_view{ begin(str), space_itr }),
    stoi(string{ next(space_itr), end(str) }) };
}

struct submarine {
  int x{};
  int y{};
  int aim{};

  int result() const noexcept { return x * y; }
};

void process_part1(submarine &sub, command cmd) {
  switch (cmd.type) {
  case cmd_type::down:
    sub.y += cmd.val;
    break;
  case cmd_type::up:
    sub.y -= cmd.val;
    break;
  case cmd_type::forward:
    sub.x += cmd.val;
    break;
  };
}

void process_part2(submarine &sub, command cmd) {
  switch (cmd.type) {
  case cmd_type::down:
    sub.aim += cmd.val;
    break;
  case cmd_type::up:
    sub.aim -= cmd.val;
    break;
  case cmd_type::forward:
    sub.x += cmd.val;
    sub.y += (sub.aim * cmd.val);
    break;
  };
}

void for_each_in_file(const char *file, auto &&f) {
  std::ifstream input{ file };
  for (std::string line; std::getline(input, line);) { f(line); }
}

void part1() {
  submarine s;
  for_each_in_file(
    test_file, [&](string_view str) { process_part1(s, to_command(str)); });
  cout << s.result() << endl;
}

void part2() {
  submarine s;
  for_each_in_file(
    test_file, [&](string_view str) { process_part2(s, to_command(str)); });
  cout << s.result() << endl;
}

int main() {
  part1();
  part2();
}
