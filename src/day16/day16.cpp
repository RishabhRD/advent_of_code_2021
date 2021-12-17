#include <numeric>
#include <tl/fold.hpp>
#include <rd/generator.hpp>
#include <bitset>
#include <variant>
#include <rd/generator.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <ranges>
#include <getline.hpp>
#include <rd/lval_accumulate.hpp>
#include <rd/operators.hpp>

using namespace std;

using size_type = decltype(declval<bitset<3>>().to_ullong());

class packet_base {
  size_type version_;
  size_type type_id_;

public:
  packet_base(size_type version, size_type type_id)
    : version_(version), type_id_(type_id) {}
  auto version() const { return version_; }
  auto type_id() const { return type_id_; }
  decltype(auto) version() { return version_; }
  decltype(auto) type_id() { return type_id_; }
};

class literal_packet : public packet_base {
  size_type literal_value_;

public:
  literal_packet(packet_base base, size_type literal_value)
    : packet_base(std::move(base)), literal_value_(literal_value) {}
  auto literal_value() const { return literal_value_; }
  decltype(auto) literal_value() { return literal_value_; }
};

class operator_packet;

using packet = variant<literal_packet, operator_packet>;

class operator_packet : public packet_base {
  size_type len_type_id_;
  vector<packet> subpackets_;

public:
  operator_packet(packet_base base,
    size_type len_type_id,
    auto &&subpackets_view)
    : packet_base(base), len_type_id_(len_type_id),
      subpackets_(subpackets_view | tl::to<vector>()) {}

  auto len_type_id() const { return len_type_id_; }
  decltype(auto) len_type_id() { return len_type_id_; }
  const vector<packet> &packets() const { return subpackets_; }
  vector<packet> &packets() { return subpackets_; }
};

class bit_reader {
  string str;
  std::size_t pos = 0;

public:
  explicit bit_reader(string_view str) : str(str) {}

  template<std::size_t N> constexpr auto read() {
    auto bits = std::bitset<N>(str, pos);
    pos += N;
    return bits;
  }

  constexpr auto bits_read() const { return pos; }
};

packet parse_packet(bit_reader &reader);

size_type parse_version(bit_reader &reader) {
  return reader.read<3>().to_ullong();
}

size_type parse_type_id(bit_reader &reader) {
  return reader.read<3>().to_ullong();
}

packet_base parse_packet_base(bit_reader &reader) {
  return packet_base{ parse_version(reader), parse_type_id(reader) };
}

size_type parse_literal_value(bit_reader &reader) {
  bool to_continue = true;
  size_type value{};
  while (to_continue) {
    auto bits = reader.read<5>();
    to_continue = bits[4];
    bits[4] = 0;
    value = (value << 4) + bits.to_ullong();
  }
  return value;
}

literal_packet parse_literal_packet(packet_base base, bit_reader &reader) {
  return literal_packet{ std::move(base), parse_literal_value(reader) };
}

auto parse_len_type_id(bit_reader &reader) {
  return reader.read<1>().to_ullong();
}

rd::generator<packet> parse_n_packets(bit_reader &reader, size_type n) {
  // TODO: how efficient is it?
  for (size_type i = 0; i < n; i++) { co_yield parse_packet(reader); }
}

rd::generator<packet> parse_packets_with_max_bits(bit_reader &reader,
  size_type const max_bits_to_read) {
  auto bits_read = [&, init_read_bits = reader.bits_read()]() {
    return reader.bits_read() - init_read_bits;
  };
  while (bits_read() != max_bits_to_read) { co_yield parse_packet(reader); }
}

auto parse_operator_num_bits(bit_reader &reader) {
  return reader.read<15>().to_ullong();
}

auto parse_operator_num_packets(bit_reader &reader) {
  return reader.read<11>().to_ullong();
}

operator_packet parse_operator_packet(packet_base base, bit_reader &reader) {
  auto len_type_id = parse_len_type_id(reader);
  if (len_type_id == 0) {
    return operator_packet(std::move(base),
      len_type_id,
      parse_packets_with_max_bits(reader, parse_operator_num_bits(reader)));
  } else {
    return operator_packet(std::move(base),
      len_type_id,
      parse_n_packets(reader, parse_operator_num_packets(reader)));
  }
}

packet parse_packet(bit_reader &reader) {
  packet_base base = parse_packet_base(reader);
  if (base.type_id() == 4) {
    return parse_literal_packet(std::move(base), reader);
  } else {
    return parse_operator_packet(std::move(base), reader);
  }
}

constexpr inline auto hex_to_binary = [](auto str) {
  if (str == '0')
    return "0000"sv;
  else if (str == '1')
    return "0001"sv;
  else if (str == '2')
    return "0010"sv;
  else if (str == '3')
    return "0011"sv;
  else if (str == '4')
    return "0100"sv;
  else if (str == '5')
    return "0101"sv;
  else if (str == '6')
    return "0110"sv;
  else if (str == '7')
    return "0111"sv;
  else if (str == '8')
    return "1000"sv;
  else if (str == '9')
    return "1001"sv;
  else if (str == 'A')
    return "1010"sv;
  else if (str == 'B')
    return "1011"sv;
  else if (str == 'C')
    return "1100"sv;
  else if (str == 'D')
    return "1101"sv;
  else if (str == 'E')
    return "1110"sv;
  else
    return "1111"sv;
};

auto parse_input(char const *file) {
  auto const str = rd::getline(file);
  return str | views::transform(hex_to_binary) | views::join | tl::to<string>();
}

size_type version_sum(packet const &pkt) {
  if (pkt.index() == 0) {
    return std::get<literal_packet>(pkt).version();
  } else {
    auto const &op_pkt = std::get<operator_packet>(pkt);
    return rd::lvalue_accumulate(op_pkt.packets(),
      op_pkt.version(),
      rd::plus{},
      [](const auto &sub_packet) { return version_sum(sub_packet); });
  }
}

size_type value(packet const &pkt) {
  if (pkt.index() == 0) {
    return std::get<literal_packet>(pkt).literal_value();
  } else {
    auto const &opt_pkt = std::get<operator_packet>(pkt);
    auto projection = [](auto const &ele) { return value(ele); };
    switch (opt_pkt.type_id()) {
    case 0:
      return rd::lvalue_accumulate(
        opt_pkt.packets(), 0ull, rd::plus{}, projection);
    case 1:
      return rd::lvalue_accumulate(
        opt_pkt.packets(), 1ull, rd::multiply{}, projection);
    case 2:
      return value(
        *ranges::min_element(opt_pkt.packets(), ranges::less{}, projection));
    case 3:
      return value(
        *ranges::max_element(opt_pkt.packets(), ranges::less{}, projection));
    case 5:
      return value(opt_pkt.packets().at(0)) > value(opt_pkt.packets().at(1));
    case 6:
      return value(opt_pkt.packets().at(0)) < value(opt_pkt.packets().at(1));
    case 7:
      return value(opt_pkt.packets().at(0)) == value(opt_pkt.packets().at(1));
    };
    throw "Should not reach here";
  }
}

void part1(char const *file) {
  bit_reader reader{ parse_input(file) };
  cout << version_sum(parse_packet(reader)) << endl;
}

void part2(char const *file) {
  bit_reader reader{ parse_input(file) };
  cout << value(parse_packet(reader)) << endl;
}

int main() {
  constexpr static char const *test_file =
    "/home/rishabh/personal/advent_of_code_2021/src/day16/day16.test";
  constexpr static char const *input_file =
    "/home/rishabh/personal/advent_of_code_2021/src/day16/day16.input";
  part1(input_file);
  part2(input_file);
}
