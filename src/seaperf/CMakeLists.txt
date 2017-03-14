add_library(seaperf STATIC
  server.cc
  client.cc
  utils.cc
)

add_executable(seaclient
  client_app.cc
)

add_executable(seaserver
  server_app.cc
)

target_link_libraries(seaperf
  ${SEASTAR_LDFLAGS}
)

target_link_libraries(seaclient
  seaperf
  ${SEASTAR_LDFLAGS}
)

target_link_libraries(seaserver
  seaperf
  ${SEASTAR_LDFLAGS}
)

set_property(TARGET seaperf PROPERTY CXX_STANDARD 14)
set_property(TARGET seaperf PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET seaclient PROPERTY CXX_STANDARD 14)
set_property(TARGET seaclient PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET seaserver PROPERTY CXX_STANDARD 14)
set_property(TARGET seaserver PROPERTY CXX_STANDARD_REQUIRED ON)
