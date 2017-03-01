#include "server.hh"

namespace seaperf {
namespace server {

future<> Server::listen(ipv4_addr addr) {
  listen_options lo;
  lo.reuse_address = true;
  m_listener = engine().listen(make_ipv4_address(addr), lo);
  do_accepts();
  return make_ready_future<>();
}

future<> Server::do_accepts() {
  return m_listener.accept()
      .then_wrapped(
          [this](future<connected_socket, socket_address> f_cs_sa) mutable {
            auto cs_sa = f_cs_sa.get();
            auto conn = new Conn{std::get<0>(std::move(cs_sa)),
                                 std::get<1>(std::move(cs_sa))};
            conn->process().then_wrapped([this, conn](auto&& f) {
              delete conn;
              try {
                f.get();
              } catch (std::exception& ex) {
                std::cerr << "request error: " << ex.what() << std::endl;
              }
            });
            do_accepts();
          })
      .then_wrapped([](auto f) {
        try {
          f.get();
        } catch (std::exception& ex) {
          std::cerr << "accept failed: " << ex.what() << std::endl;
        }
      });
}

future<> Server::stop() {
  // TODO
  return make_ready_future<>();
}

future<> Conn::process() {
  return repeat([this] {
           return m_in.read().then([this](auto buf) {
             if (buf) {
               m_byte_cnt += buf.size();
               return make_ready_future<stop_iteration>(stop_iteration::no);
             } else {
               return make_ready_future<stop_iteration>(stop_iteration::yes);
             }
           });
         })
      .then([this] {
        print("bytes received: %d\n", m_byte_cnt);
        return m_out.close();
      });
}
}  // namespace server
}  // namespace seaperf
