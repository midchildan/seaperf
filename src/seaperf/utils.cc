#include <algorithm>
#include <iterator>
#include <random>
#include <sstream>
#include <string>
#include "utils.hh"

namespace seaperf {

std::string random_string(std::size_t len) {
  auto randchar = []() -> char {
    const char charset[] = "abcdefghijklmnopqrstuvwxyz";
    std::random_device rnd;
    return charset[rnd() % sizeof(charset)];
  };

  std::ostringstream rand_ss;
  std::ostream_iterator<char> rand_os_it {rand_ss};
  std::generate_n(rand_os_it, len, randchar);
  return rand_ss.str();
}
}
