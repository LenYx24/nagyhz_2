cmake_minimum_required(VERSION 3.16)
project(lolmacrogame LANGUAGES CXX)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.14.0
)
FetchContent_MakeAvailable(googletest)
add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
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
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:lolmacrogame>
        VERBATIM)
else()
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${PROJECT_NAME}
        PRIVATE
         -fsanitize=address
         -fno-omit-frame-pointer
    )
    endif()
endif()
target_compile_options(${PROJECT_NAME}
        PRIVATE
        -fsanitize=address -Wall -Wextra -Wpedantic -Wconversion -Werror
)
target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address)


add_executable(${PROJECT_NAME}_test)
target_sources(${PROJECT_NAME}_test
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
        ./src/menu.cpp
        
        ./test/main_test.cpp
)
target_include_directories(${PROJECT_NAME}
        PRIVATE
        src
)
target_compile_options(${PROJECT_NAME}_test
        PRIVATE
        -Werror -Wall -Wextra -Wpedantic -Wconversion -fsanitize=address
)
target_compile_features(${PROJECT_NAME}_test PRIVATE cxx_std_17)
target_link_options(${PROJECT_NAME}_test PRIVATE -fsanitize=address)

target_link_libraries(${PROJECT_NAME}_test
        PRIVATE
        GTest::gtest_main
        sfml-graphics
)

install(TARGETS ${PROJECT_NAME})
