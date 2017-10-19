cmake_minimum_required(VERSION 3.8)
project(COMP345)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        include/cards.h
        include/dice.h
        include/player.h
        include/map.h
        include/reader.h
        src/cards.cpp
        src/dice.cpp
        src/player.cpp
        src/map.cpp
        src/reader.cpp)

add_executable(COMP345 ${SOURCE_FILES})