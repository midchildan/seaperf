find_package(PkgConfig REQUIRED)

if(CMAKE_BUILD_TYPE MATCHES Debug)
  set (ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:${CMAKE_CURRENT_SOURCE_DIR}/seastar/build/debug")
else()
  set (ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:${CMAKE_CURRENT_SOURCE_DIR}/seastar/build/release")
endif()
pkg_search_module(SEASTAR REQUIRED seastar)
include_directories(SYSTEM ${SEASTAR_INCLUDE_DIRS})

add_subdirectory(seaperf)
