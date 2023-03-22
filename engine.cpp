#include <engine.h>
#include "settings.h"
#include "player.h"
#include <chrono>

Engine::Engine(const Settings& settings)
    :graphics{settings.title, settings.screen_width,settings.screen_height}, 
    camera{graphics,settings.tilesize}, 
    world{31,11}{
        load_level();
    }

void Engine::load_level(){
    world.add_platform(0,0,30,1);
    world.add_platform(0,0,1,10);
    world.add_platform(30,0,1,10);
    world.add_platform(0,10,31,1);
    //Platforms
    world.add_platform(3,7,4,1);
    world.add_platform(13,4,6,1);
    // add player 
    player = std::make_shared<Player>(Vec<double>{10,4},Vec<int>{1,1});
    // move camera
    // Camera camera{graphics, tilesize};
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
            auto command = player->handle_input(event);
            if (command){
                command->execute(*player, *this);
            }
        }
}
void Engine::update(double dt){
    player->update(*this, dt);
    camera.update(dt);
}

void Engine::render(){
    graphics.clear();
        camera.render(world.tilemap,grid_on);
        auto [position, color] = player->get_sprite();
        camera.render(position,color);
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