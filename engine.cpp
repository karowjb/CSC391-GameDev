#include <engine.h>
#include "settings.h"
#include "player.h"
#include <chrono>
#include "level.h"
#include "world.h"
// #include "camera.h"

Engine::Engine(const Settings& settings)
    :graphics{settings.title, settings.screen_width, settings.screen_height},
      camera{graphics, settings.tilesize} {
    
    load_level(settings.starting_level);
}

void Engine::load_level(const std::string& level_filename) {
    Level level{level_filename, graphics, audio};
    audio.play_sound("background", true);
    world = std::make_shared<World>(level);

    // load player
    player = std::make_shared<Player>(*this, level.player_start_location, Vec<int>{1, 1});

    // move camera to start position
    camera.move_to(player->physics.position);
}

void Engine::input(){
    // Input
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            // handle windows and systems events first
            if (event.type == SDL_QUIT) {
                running = false;
                break;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_g){
                grid_on = !grid_on;
            }
            // pass the rest of the events to the player who will
            // react to keypresses by moving
            auto command = player->handle_input(*this,event);
            if (command){
                command->execute(*player, *this);
            }
        }
}
void Engine::update(double dt){
    player->update(*this, dt);
    camera.update(dt);
}

void Engine::render() {
    // draw the new scene
    graphics.clear();
    camera.render(world.backgrounds);
}

void Engine::run(){
    running = true;
    auto previous = std::chrono::high_resolution_clock::now();
    double lag{0.0};
    while (running) {
        auto current = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = current - previous;
        previous = current;
        lag += elapsed.count();


        constexpr double dt = 1.0/60.0;
        input();
        while (lag >= dt){
            update(dt);
            camera.move_to(player->get_sprite().first);
            lag -= dt;
        }
        render();
    
    }
}