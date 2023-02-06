cmake_minimum_required(VERSION 3.24.1)
project(GAMEDEV)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-std=c++17 -g -Wall -Wextra)

# set(SDL2_INCLUDES "/opt/homebrew/Cellar/sdl2/2.24.1/include/SDL2/SDL.h")
# set(SDL2_INCLUDES "/opt/homebrew/Cellar/sdl2/2.24.1/include/SDL2")
set(SDL2_INCLUDES "/opt/homebrew/include")
set(SDL2_LIBRARIES "/opt/homebrew/lib/libSDL2.dylib")

# add_executable(main physics.cpp)
# add_executable(main homework.cpp)
add_library(gamelib
  graphics.cpp
  player.cpp
  world.cpp
)
add_executable(main main.cpp)
target_include_directories(gamelib PUBLIC ${SDL2_INCLUDES})

target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${SDL2_INCLUDES})
target_link_libraries(main PUBLIC gamelib ${SDL2_LIBRARIES})
