#include <algorithm>
#include <string>

#include "client.hh"
#include "utils.hh"

namespace seaperf {
namespace client {

Client::Client() {}

future<> Client::benchmark() {
  return repeat([this] {
    if (m_is_time_up) {
      return make_ready_future<stop_iteration>(stop_iteration::yes);
    }
    m_sock.output().write(m_sendbuf)
        .then_wrapped([this](auto&& f) {
          try {
            f.get();
            return make_ready_future<stop_iteration>(stop_iteration::no);
          } catch (std::exception& ex) {
            std::cerr << "write error: " << ex.what() << std::endl;
            return make_ready_future<stop_iteration>(stop_iteration::yes);
          }
        });
  });
}

future<> Client::run(ipv4_addr addr) {
  m_sendbuf = random_string(k_bufsize);

  m_is_time_up = false;
  m_bench_timer.set_callback([this] { m_is_time_up = true; });
  m_bench_timer.arm(m_bench_duration + m_margin);

  return engine().connect(addr).then([this](auto sock) mutable {
    m_sock = std::move(sock);
    return this->benchmark();
  });
}
}
}
