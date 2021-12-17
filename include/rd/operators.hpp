namespace rd {
struct plus {
  template<typename T> auto operator()(T &&a, T &&b) { return a + b; }
};

struct multiply {
  template<typename T> auto operator()(T &&a, T &&b) { return a * b; }
};
}// namespace rd
