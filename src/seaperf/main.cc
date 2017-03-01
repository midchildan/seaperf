#include <iostream>
#include <stdexcept>

#include <core/app-template.hh>
#include <core/future.hh>
#include <core/reactor.hh>
#include <net/api.hh>
#include <util/log.hh>

future<> handle_connection(connected_socket&, input_stream<char>& in,
                           output_stream<char>& out, int64_t& byte_cnt) {
  return repeat([&in, &out, &byte_cnt] {
           return in.read().then([&out, &byte_cnt](auto buf) {
             if (buf) {
               byte_cnt += buf.size();
               return make_ready_future<stop_iteration>(stop_iteration::no);
             } else {
               return make_ready_future<stop_iteration>(stop_iteration::yes);
             }
           });
         })
      .then([&out, &byte_cnt] {
        print("bytes received: %d\n", byte_cnt);
        return out.close();
      });
}

future<> app_main(app_template& app) {
  auto& args = app.configuration();

  auto port = args["port"].as<uint16_t>();
  listen_options lo;
  lo.reuse_address = true;
  return do_with(listen(make_ipv4_address({port}), lo), [](auto& listener) {
    return keep_doing([&listener] {
      return listener.accept().then(
          [](connected_socket s, socket_address) mutable {
            int64_t byte_cnt = 0;
            auto in = s.input();
            auto out = s.output();
            do_with(std::move(s), std::move(in), std::move(out), byte_cnt,
                    handle_connection);
          });
    });
  });
}

int main(int argc, char* argv[]) {
  namespace bpo = boost::program_options;

  app_template app;
  app.add_options()("port", bpo::value<uint16_t>()->default_value(12865),
                    "seaperf server port");

  try {
    app.run(argc, argv, [&app] { return app_main(app); });
  } catch (...) {
    std::cerr << std::current_exception() << '\n';
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
