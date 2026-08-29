cmake_minimum_required(VERSION 3.7.2)
project(mainsweeper LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.2
        GIT_SHALLOW TRUE
)
set(SFML_STATIC_LIBRARIES TRUE CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS FALSE CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(SFML)

add_executable(mainsweeper main.cpp)
target_link_libraries(mainsweeper PRIVATE sfml-system sfml-window sfml-graphics)

