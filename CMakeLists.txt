cmake_minimum_required(VERSION 3.20)
project(sdl)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-std=c++17 -g -Wall -Wextra)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)

add_library(gamelib
  graphics.cpp
  player.cpp
  world.cpp
  tilemap.cpp
  sprite.cpp
  camera.cpp
  physics.cpp
  fsm.cpp
  command.cpp
  engine.cpp
  settings.cpp
  sprite.cpp
  randomness.cpp
  audio.cpp
  animatedsprite.cpp
  level.cpp
)

target_include_directories(gamelib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
target_link_libraries(gamelib PUBLIC SDL2::SDL2 SDL2_image::SDL2_image SDL2_mixer::SDL2_mixer)

add_executable(main main.cpp)
target_link_libraries(main PUBLIC gamelib)
