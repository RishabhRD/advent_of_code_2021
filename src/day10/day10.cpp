#include <functional>
#include <tl/fold.hpp>
#include <map>
#include <stack>
#include <iostream>
#include <fstream>
#include <ranges>
#include <getline.hpp>

using namespace std;

namespace rng = std::ranges;

auto parse_input(char const *file) {
  auto lines = rd::getline(file);
  return lines;
}

enum class events { opening_incomplete, wrong_match, extra_closing };

constexpr bool is_opening(char const c) {
  return c == '(' or c == '{' or c == '[' or c == '<';
}

constexpr bool is_matching(char const opening, char const closing) {
  return (opening == '(' and closing == ')')
         or (opening == '{' and closing == '}')
         or (opening == '[' and closing == ']')
         or (opening == '<' and closing == '>');
}

constexpr char closing_bracket(char const opening) {
  if (opening == '(')
    return ')';
  else if (opening == '{')
    return '}';
  else if (opening == '[')
    return ']';
  else
    return '>';
}

void scan(string_view input,
  auto &&wrong_match_handler,
  auto &&extra_closing_handler,
  auto &&opening_incomplete_handler) {
  stack<char> st;
  for (auto const c : input) {
    if (is_opening(c))
      st.push(c);
    else {
      if (size(st) == 0) {
        extra_closing_handler(c);
        return;
      } else {
        if (!is_matching(st.top(), c)) {
          wrong_match_handler(st.top(), c);
          return;
        }
        st.pop();
      }
    }
  }
  while (not empty(st)) {
    opening_incomplete_handler(st.top());
    st.pop();
  }
}

void part1(char const *file) {
  auto lines = parse_input(file);
  auto wrong_score = [](char c) {
    if (c == ')')
      return 3;
    else if (c == ']')
      return 57;
    else if (c == '}')
      return 1197;
    else
      return 25137;
  };
  map<char, int> wrong_cnts;
  auto wrong_handler = [&wrong_cnts](char, char c) { wrong_cnts[c]++; };
  for (string_view line : lines) {
    scan(
      line, wrong_handler, [](auto) {}, [](auto) {});
  }
  cout << tl::fold(wrong_cnts, 0, [&wrong_score](auto init, auto ele) {
    return init + (wrong_score(ele.first) * ele.second);
  }) << endl;
}

void part2(char const *file) {
  auto opening_score = [](char c) {
    if (c == '(')
      return 1;
    else if (c == '[')
      return 2;
    else if (c == '{')
      return 3;
    else
      return 4;
  };

  auto lines = parse_input(file);
  vector<unsigned long long> scores;
  for (string_view line : lines) {
    unsigned long long cur_score = 0;
    scan(
      line,
      [](auto, auto) {},
      [](auto) {},
      [&cur_score, &opening_score](
        char c) { cur_score = cur_score * 5 + opening_score(c); });
    if (cur_score != 0) scores.push_back(cur_score);
  }
  auto middle_itr = begin(scores) + (size(scores) / 2);
  *rng::nth_element(scores, middle_itr);
  cout << *middle_itr << endl;
}

int main() {
  constexpr static char const *test_file =
    "/home/rishabh/personal/advent_of_code_2021/src/day10/day10.test";
  constexpr static char const *input_file =
    "/home/rishabh/personal/advent_of_code_2021/src/day10/day10.input";
  part1(input_file);
  part2(input_file);
}
