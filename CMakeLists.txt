cmake_minimum_required(VERSION 3.8)
project(COMP345)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        include/cards.h
        include/country.h
        include/dice.h
        include/graph.h
        include/node.h
        include/player.h
        src/cards.cpp
        src/country.cpp
        src/dice.cpp
        src/driver.cpp
        src/graph.cpp
        src/node.cpp
        src/player.cpp)

add_executable(COMP345 ${SOURCE_FILES})