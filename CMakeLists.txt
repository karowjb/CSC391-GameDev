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
  object.cpp
  enemy.cpp
  enemyType.cpp
  combat.cpp
  quadtree.cpp
  animate_quadtree.cpp
  perf_quadtree.cpp
)

target_include_directories(gamelib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
target_link_libraries(gamelib PUBLIC SDL2::SDL2 SDL2_image::SDL2_image SDL2_mixer::SDL2_mixer)

add_executable(main main.cpp)
target_link_libraries(main PUBLIC gamelib)

add_executable(test_quadtree test_quadtree.cpp)
target_link_libraries(test_quadtree PUBLIC gamelib)

add_executable(animate_quadtree animate_quadtree.cpp)
target_link_libraries(animate_quadtree PUBLIC gamelib)

add_executable(perf_quadtree perf_quadtree.cpp)
target_link_libraries(perf_quadtree PUBLIC gamelib)