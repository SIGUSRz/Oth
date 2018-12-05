cmake_minimum_required(VERSION 2.8.4)
project(Oth)

set(CMAKE_CXX_STANDARD 17)
set(BOOST_ROOT /usr/local/boost)
set(Boost_INCLUDE_DIR ${BOOST_ROOT}/include)
set(Boost_LIBRARY_DIR ${BOOST_ROOT}/lib)
set(LINK_PROGRAM_OPTIONS "-lboost_program_options")
set(BOOST_NO_SYSTEM_PATHS ON)

find_package(Boost COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -v")

set(BIN "bin")
set(INC "include")
set(SRC "src")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN})

include_directories(${INC})
add_library(board ${SRC}/board.cpp ${INC}/board.hpp)
add_library(game ${SRC}/game.cpp ${INC}/game.hpp)
target_link_libraries(game board)
add_executable(othello ${SRC}/othello.cpp)
target_link_libraries(othello game)
target_link_libraries(othello ${LINK_PROGRAM_OPTIONS})
#add_custom_target(Oth COMMAND make -C ${mytest_SOURCE_DIR}
#        CLION_EXE_DIR=${PROJECT_BINARY_DIR})

install(TARGETS othello DESTINATION ${BIN})