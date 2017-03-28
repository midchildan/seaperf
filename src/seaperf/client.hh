#pragma once

#include <core/distributed.hh>
#include <core/reactor.hh>

namespace seaperf {
namespace client {

class Client {
 public:
  Client();

  future<> run(ipv4_addr addr);
  void set_bench_duration(std::chrono::seconds t);

 private:
  future<> benchmark();

  connected_socket m_sock;
  input_stream<char> m_in;
  output_stream<char> m_out;

  std::string m_sendbuf;
  std::chrono::seconds m_bench_duration;
  bool m_is_time_up = false;
  timer<> m_bench_timer;
  std::chrono::seconds m_margin{5};

  static constexpr auto k_bufsize = 64000;
};
}
}
