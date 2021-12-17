namespace rd {
auto lvalue_accumulate(
  auto &&range,
  auto init,
  auto &&operation,
  auto &&projection = [](auto const &ele) { return ele; }) {
  for (auto const &ele : range) {
    init = operation(std::move(init), projection(ele));
  }
  return init;
}
}// namespace rd
