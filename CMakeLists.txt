cmake_minimum_required(VERSION 4.0)
project(IA-TP2 LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(IA-TP2
        src/main.cpp)

target_compile_features(IA-TP2 PRIVATE cxx_std_17)
target_link_libraries(IA-TP2 PRIVATE SFML::Graphics)

# Copy Assets folder to output directory
#file(COPY ${CMAKE_SOURCE_DIR}/Assets DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)