#pragma once

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

 private:
  connected_socket m_sock;
  input_stream<char> m_in;
  output_stream<char> m_out;
  int64_t m_byte_cnt = 0;
};

class Server {
 public:
  Server() {}

  future<> listen(ipv4_addr addr);
  future<> do_accepts();
  future<> stop();

 private:
  server_socket m_listener;
};

class Control {
 public:
  Control() : m_server{new distributed<Server>} {}

  future<> start() { return m_server->start(); }

  future<> stop() { return m_server->stop(); }

  future<> listen(ipv4_addr addr) {
    return m_server->invoke_on_all(&Server::listen, addr);
  }

 private:
  distributed<Server>* m_server;
};
}
}
