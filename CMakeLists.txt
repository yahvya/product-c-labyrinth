cmake_minimum_required(VERSION 3.30.1)

project(c_labyrinth
    DESCRIPTION "Mini jeux 2D de labyrinthe"
    VERSION "24.07.24"
)

add_compile_options(-UNDEBUG)

add_subdirectory("libs/libyaml-0.2.5")
add_subdirectory("libs/custom-color")
add_subdirectory("libs/raylib-5.0")

add_executable(game 
        src/game.c
        src/assets-manager/assets/assets.h
        src/assets-manager/assets/assets.c
        src/assets-manager/config-manager/config-manager.c
        src/assets-manager/config-manager/config-manager.h
        src/assets-manager/config-manager/tills-manager.c
        src/assets-manager/config-manager/tills-manager.h
        src/assets-manager/config-manager/image-config-manager.c
        src/assets-manager/config-manager/image-config-manager.h
        src/assets-manager/config-manager/items-manager.c
        src/assets-manager/config-manager/items-manager.h
        src/assets-manager/config-manager/heroes-manager.c
        src/assets-manager/config-manager/heroes-manager.h
        src/assets-manager/config-manager/enemies-manager.c
        src/assets-manager/config-manager/enemies-manager.h
        src/assets-manager/config-manager/game-map-manager.c
        src/assets-manager/config-manager/game-map-manager.h
        src/utils/generic-list/generic-list.c
        src/utils/generic-list/generic-list.h
        src/game/game.c
        src/game/game.h
        src/game/window-manager.c
        src/game/window-manager.h
        src/rendering/home-rendering.h
        src/rendering/home-rendering.c
)

target_link_libraries(game 
    yaml
    custom_color
    raylib
)

add_custom_command(
    TARGET game PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/resources"
    "${CMAKE_BINARY_DIR}/resources"
)
