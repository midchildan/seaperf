#include <iostream>
#include <stdexcept>
#include <string>

#include <core/app-template.hh>
#include <core/future.hh>
#include <core/reactor.hh>
#include <net/api.hh>
#include <util/log.hh>

#include "client.hh"

future<> app_main(app_template& app) {
  auto& args = app.configuration();
  auto host = args["host"].as<std::string>();
  auto port = args["port"].as<uint16_t>();
  auto server_addr = ipv4_addr(host, port);
  auto client = seaperf::client::Client{};
  return client.run(server_addr);
}

int main(int argc, char* argv[]) {
  namespace bpo = boost::program_options;

  app_template app;
  app.add_options()("host", bpo::value<std::string>()->default_value("127.0.0.1"),
                    "seaperf server host");
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