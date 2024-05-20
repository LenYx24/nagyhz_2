cmake_minimum_required(VERSION 3.16)
project(lolmacrogametest LANGUAGES CXX)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(lolmacrogametest)

target_sources(lolmacrogametest
    PRIVATE
    # SRC FILES
    ../src/simulation.cpp
    ../src/draft.cpp
    ../src/game.cpp
    ../src/gamemoves.cpp
    ../src/gameobjects.cpp
    ../src/map.cpp
    ../src/ioparser.cpp
    ../src/resources.cpp
    ../src/statemanagement.cpp
    ../src/UIcomponents.cpp
    ../src/menu.cpp

    ./main.cpp
)
target_link_libraries(lolmacrogametest PRIVATE sfml-graphics)
target_compile_features(lolmacrogametest PRIVATE cxx_std_17)
target_compile_options(lolmacrogametest PRIVATE -Wall -g -O0)

install(TARGETS lolmacrogametest)
