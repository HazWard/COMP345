cmake_minimum_required(VERSION 3.8)
project(COMP345)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        include/cards.h
        include/dice.h
        include/player.h
        include/map.h
        include/reader.h
        include/exception.h
        include/game.h
        src/cards.cpp
        src/dice.cpp
        src/player.cpp
        src/map.cpp
        src/reader.cpp
        src/game.cpp
        src/exception.cpp src/Observer.cpp include/Observer.h)

add_executable(COMP345 ${SOURCE_FILES})