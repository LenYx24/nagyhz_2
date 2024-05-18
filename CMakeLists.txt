cmake_minimum_required(VERSION 3.16)
project(lolmacrogame LANGUAGES CXX)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)
add_executable(lolmacrogame)

target_sources(lolmacrogame
    PRIVATE

    ./src/simulation.cpp
    ./src/draft.cpp
    ./src/game.cpp
    ./src/gamemoves.cpp
    ./src/gameobjects.cpp
    ./src/map.cpp
    ./src/ioparser.cpp
    ./src/gameobjects.cpp
    ./src/resources.cpp
    ./src/statemanagement.cpp
    ./src/UIcomponents.cpp  
    ./src/main.cpp 
    ./src/menu.cpp
)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
target_link_libraries(lolmacrogame PRIVATE sfml-graphics)
target_compile_features(lolmacrogame PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET lolmacrogame
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:lolmacrogame>
        VERBATIM)
else()
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(lolmacrogame
        PRIVATE
         -fsanitize=address
         -fno-omit-frame-pointer)
    endif()
endif()
target_compile_options(lolmacrogame PRIVATE -fsanitize=address -Wall -Wextra -Wpedantic -Wconversion -Werror) #  -g -O0
target_link_options(lolmacrogame PRIVATE -fsanitize=address)

install(TARGETS lolmacrogame)
