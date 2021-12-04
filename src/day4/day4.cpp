#include <tl/enumerate.hpp>
#include <unordered_set>
#include <ranges>
#include <tl/to.hpp>
#include <iostream>
#include <fstream>

namespace vw = std::ranges::views;

using namespace std;

constexpr static char const *test_file =
  "/home/rishabh/personal/advent_of_code_2021/src/day4/day4.test";
constexpr static char const *input_file =
  "/home/rishabh/personal/advent_of_code_2021/src/day4/day4.input";

struct cell {
  bool marked{};
  int value{};
};

using Board = vector<vector<cell>>;

void mark(Board &board, int val) {
  for (size_t i = 0; i < 5; i++) {
    for (size_t j = 0; j < 5; j++) {
      if (board[i][j].value == val) board[i][j].marked = true;
    }
  }
}

bool is_winning(Board const &board) {
  for (size_t i = 0; i < 5; i++) {
    bool marked = true;
    for (size_t j = 0; j < 5; j++) {
      if (board[i][j].marked == false) {
        marked = false;
        break;
      }
    }
    if (marked) return true;
  }
  for (size_t i = 0; i < 5; i++) {
    bool marked = true;
    for (size_t j = 0; j < 5; j++) {
      if (board[j][i].marked == false) {
        marked = false;
        break;
      }
    }
    if (marked) return true;
  }
  return false;
}

int winning_score(Board const &board) {
  int score = 0;
  for (size_t i = 0; i < 5; i++) {
    for (size_t j = 0; j < 5; j++) {
      if (board[i][j].marked == false) score += board[i][j].value;
    }
  }
  return score;
}

auto stoi(auto str) {
  int cnt = 0;
  for (auto c : str) { cnt = cnt * 10 + (c - '0'); }
  return cnt;
}

auto get_first_line(std::ifstream &input) {
  string line;
  getline(input, line);
  return line;
}

auto parse_rest_file(std::ifstream &input) {
  vector<Board> res;
  // TODO: find-out how to split based on ' '(1 space) or '  '(2 spaces)
  // for (string line; getline(input, line);) {
  // if(line == "") continue;
  // cur_board.emplace_back(line | vw::split(' ') | vw::transform([](auto str) {
  //   return cell{ false, stoi(str) };
  // }) | tl::to<std::vector>());
  // if (size(cur_board) == 5) {
  //   res.emplace_back(std::exchange(cur_board, {}));
  // }
  // }
  while (true) {
    Board cur_board{};
    for (int i = 0; i < 5; i++) {
      vector<cell> cur_row;
      for (int j = 0; j < 5; j++) {
        int temp{};
        if (!(input >> temp)) { return res; }
        cur_row.push_back({ false, temp });
      }
      cur_board.push_back(std::move(cur_row));
    }
    res.push_back(std::move(cur_board));
  }
  return res;
}

auto parse_first_line(string_view line) {
  return line | vw::split(',')
         | vw::transform([](auto str) { return stoi(str); })
         | tl::to<std::vector>();
}

auto parse_input(const char *file) {
  std::ifstream input{ file };
  return pair{ parse_first_line(get_first_line(input)),
    parse_rest_file(input) };
}

void part1(char const *file) {
  auto [num_stream, bingo_list] = parse_input(file);
  for (auto num : num_stream) {
    for (auto &&board : bingo_list) {
      mark(board, num);
      if (is_winning(board)) {
        cout << winning_score(board) * num << endl;
        return;
      }
    }
  }
}

void part2(char const *file) {
  unordered_set<size_t> winning_boards;
  auto [num_stream, bingo_list] = parse_input(file);
  const auto num_boards = size(bingo_list);
  size_t cur_won = 0;
  for (auto num : num_stream) {
    for (size_t idx = 0; idx < size(bingo_list); idx++) {
      auto &board = bingo_list[idx];
      if (winning_boards.find(idx) != end(winning_boards)) continue;
      mark(board, num);
      if (is_winning(board)) {
        cur_won++;
        winning_boards.insert(idx);
        if (cur_won == num_boards) {
          cout << winning_score(board) * num << endl;
          return;
        }
      }
    }
  }
}

int main() {
  part1(input_file);
  part2(input_file);
}
