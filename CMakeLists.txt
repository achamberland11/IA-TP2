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
        src/main.cpp
        src/Core/Object.h
        src/Game/Entities/Entity.cpp
        src/Game/Entities/Entity.h
        src/Game/Components/Component.cpp
        src/Game/Components/Component.h
        src/Macros/ObjectMacros.h
        src/Game/Components/TransformComponent.cpp
        src/Game/Components/TransformComponent.h
        src/Game/Game.cpp
        src/Game/Game.h
        src/Game/World.cpp
        src/Game/World.h
        src/Game/Map/Map.cpp
        src/Game/Map/Map.h
        src/Game/Map/Tile.cpp
        src/Game/Map/Tile.h
        src/Game/Controllers/Controller.cpp
        src/Game/Controllers/Controller.h
        src/Game/Entities/Characters/Character.cpp
        src/Game/Entities/Characters/Character.h
        src/Game/Entities/Characters/PlayerCharacter.cpp
        src/Game/Entities/Characters/PlayerCharacter.h
        src/Game/Components/RendererComponent.cpp
        src/Game/Components/RendererComponent.h
        src/Game/Controllers/PlayerController.cpp
        src/Game/Controllers/PlayerController.h)

target_compile_features(IA-TP2 PRIVATE cxx_std_17)
target_include_directories(IA-TP2 PRIVATE
        src
        src/AI
)
target_link_libraries(IA-TP2 PRIVATE SFML::Graphics)

# Copy Assets folder to output directory
file(COPY ${CMAKE_SOURCE_DIR}/Assets DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY ${CMAKE_SOURCE_DIR}/Assets DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)
