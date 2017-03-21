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
    return m_out.write(m_sendbuf).then_wrapped([this](auto&& f) {
      try {
        f.get();
      } catch (std::exception& ex) {
        std::cerr << "write error: " << ex.what() << std::endl;
        return make_ready_future<stop_iteration>(stop_iteration::yes);
      }
      return make_ready_future<stop_iteration>(stop_iteration::no);
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
    m_in = m_sock.input();
    m_out = m_sock.output();
    return this->benchmark().then([this]() mutable {
      return when_all(m_in.close(), m_out.close()).discard_result();
    });
  });
}

void Client::set_bench_duration(timer<>::duration) {
  // TODO
}
}
}
