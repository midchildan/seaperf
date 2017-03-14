#include <algorithm>
#include <random>
#include <string>
#include "utils.hh"

namespace seaperf {

std::string random_string(std::size_t len) {
  auto randchar = []() -> char {
    const char charset[] = "abcdefghijklmnopqrstuvwxyz";
    std::random_device rnd;
    return charset[rnd() % sizeof(charset)];
  };

  std::string res;
  std::generate_n(res.begin(), len, randchar);
  return res;
}
}
