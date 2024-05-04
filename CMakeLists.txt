include(FetchContent)
cmake_minimum_required(VERSION 3.22)
project(pars_book)

set(CMAKE_CXX_STANDARD 17)

FetchContent_Declare(
  libpqxx
  GIT_REPOSITORY https://github.com/jtv/libpqxx.git
  GIT_TAG a6b1d60e74c1427c8ac2324b85cd4a0dc2068332
)
set(PQXX_LIBRARIES pqxx_static)

FetchContent_MakeAvailable(libpqxx)

add_executable(pars_book main.cpp contents.cpp search.cpp)
target_link_libraries(pars_book "${PQXX_LIBRARIES}")
