#pragma once

#include <chrono>

#include <core/distributed.hh>
#include <core/reactor.hh>

namespace seaperf {
namespace server {

class Conn {
 public:
  Conn(connected_socket&& sock, socket_address) : m_sock{std::move(sock)} {
    m_in = m_sock.input();
    m_out = m_sock.output();
  }

  future<> process();
  void set_bench_duration(timer<>::duration t);

 private:
  connected_socket m_sock;
  input_stream<char> m_in;
  output_stream<char> m_out;

  timer<>::duration m_bench_duration;
  bool m_is_time_up = false;
  int64_t m_byte_cnt = 0;
  timer<> m_bench_timer;
};

class Server {
 public:
  Server() {}

  future<> listen(ipv4_addr addr);
  future<> do_accepts();
  future<> stop();
  future<> stopped();

 private:
  server_socket m_listener;
  promise<> m_all_connections_stopped;
  future<> m_stopped = m_all_connections_stopped.get_future();
};

class Control {
 public:
  Control() : m_server{new distributed<Server>} {}

  future<> start() { return m_server->start(); }

  future<> stop() { return m_server->stop(); }

  future<> stopped() { return m_server->invoke_on_all(&Server::stopped); }

  future<> listen(ipv4_addr addr) {
    return m_server->invoke_on_all(&Server::listen, addr);
  }

 private:
  distributed<Server>* m_server;
};
}
}
