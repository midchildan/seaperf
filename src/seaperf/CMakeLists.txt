add_executable(seaperf
  main.cc
  server.cc
)
target_link_libraries(seaperf
  ${SEASTAR_LDFLAGS}
)
set_property(TARGET seaperf PROPERTY CXX_STANDARD 14)
set_property(TARGET seaperf PROPERTY CXX_STANDARD_REQUIRED ON)
