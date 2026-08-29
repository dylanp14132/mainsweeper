cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(mainsweeper LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.5.1
)

add_executable(mainsweeper main.cpp)
target_link_libraries(mainsweeper PRIVATE sfml-system sfml-window sfml-graphics sfml-audio)