#include <engine.h>
#include "settings.h"
#include "player.h"
#include <chrono>
#include <iostream>
#include "level.h"
#include "world.h"

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
        for (auto enemy : world->enemies){
            auto command = enemy->next_action(*this);
            if (command){
                command->execute(*enemy, *this);
            }
        }
}
void Engine::update(double dt){
    player->update(*this, dt);
    //check for enemy death

    world->enemies.erase(std::remove_if(world->enemies.begin(),world->enemies.end(), [](std::shared_ptr<Enemy>enemy){return !enemy->combat.is_alive;}), world->enemies.end());
    
    for (auto enemy : world->enemies){
        auto command = enemy->update(*this, dt);
        if (command){
            command->execute(*enemy, *this);
        }
    }
    camera.update(dt);
}

void Engine::render() {
    // draw the new scene
    graphics.clear();
    camera.render(world->backgrounds);
    camera.render(world->tilemap);
    camera.render(*player);
    for (auto enemy : world->enemies){
        camera.render(*enemy);
    }
    graphics.update();
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

void Engine::stop(){
    running = false;
}