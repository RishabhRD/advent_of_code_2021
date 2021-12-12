#include <tl/fold.hpp>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <ranges>
#include <getline.hpp>

using namespace std;

namespace rng = std::ranges;
namespace vw = std::ranges::views;

bool is_big(string_view str) { return isupper(str[0]); }

bool is_small(string_view str) { return islower(str[0]); }

auto parse_input(char const *file) {
  auto lines = rd::getline(file);
  unordered_map<string, unordered_set<string>> graph;
  for (string_view line : lines) {
    auto nodes = line | vw::split('-') | vw::transform([](auto str) {
      string res;
      for (auto c : str) res += c;
      return res;
    }) | tl::to<vector>();
    graph[nodes[0]].insert(nodes[1]);
    graph[nodes[1]].insert(nodes[0]);
  }
  return graph;
}

auto opposite(auto &&f) {
  return [&f](auto ele) { return !f(ele); };
}

int num_path(const string &cur,
  unordered_map<string, unordered_set<string>> &graph,
  unordered_map<string, bool> &visited) {
  if (cur == "end") return 1;
  visited[cur] = true;
  auto not_visited = [&visited](
                       const string &str) { return visited[str] == false; };
  auto is_not_start = [](string_view str) { return str != "start"; };
  int paths = 0;
  for (const auto &str : graph[cur] | vw::filter(is_big)) {
    paths += num_path(str, graph, visited);
  }
  for (const auto &str : graph[cur] | vw::filter(is_small)
                           | vw::filter(is_not_start)
                           | vw::filter(not_visited)) {
    paths += num_path(str, graph, visited);
  }
  visited[cur] = false;
  return paths;
}

int num_path_special(const string &cur,
  unordered_map<string, unordered_set<string>> &graph,
  unordered_set<string> small_visited_nodes,
  bool special_visited_twice) {
  if (cur == "end") return 1;
  if (small_visited_nodes.contains(cur)) {
    if (special_visited_twice) return 0;
    special_visited_twice = true;
  }
  if (is_small(cur)) small_visited_nodes.insert(cur);
  auto is_not_start = [](string_view str) { return str != "start"; };
  auto filtered_range = graph[cur] | vw::filter(is_not_start);
  return tl::fold(filtered_range, 0, [&](auto prev, auto &&str) {
    return prev
           + num_path_special(
             str, graph, small_visited_nodes, special_visited_twice);
  });
}

void part1(char const *file) {
  auto graph = parse_input(file);
  unordered_map<string, bool> visited;
  cout << num_path("start", graph, visited) << endl;
}

void part2(char const *file) {
  auto graph = parse_input(file);
  unordered_set<string> visited;
  cout << num_path_special("start", graph, visited, false) << endl;
}

int main() {
  constexpr static char const *test_file =
    "/home/rishabh/personal/advent_of_code_2021/src/day12/day12.test";
  constexpr static char const *input_file =
    "/home/rishabh/personal/advent_of_code_2021/src/day12/day12.input";
  part1(input_file);
  part2(input_file);
}
