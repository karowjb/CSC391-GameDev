#include "graphics.h"
#include "player.h"
#include "world.h"
#include "camera.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <chrono>

int main() {
    Graphics graphics{"game", 1280, 720};
    
    // objects
    Player player{{10,4},{1,1}};
    
    World world{31,11};
    //boundary walls 
    world.add_platform(0,0,30,1);
    world.add_platform(0,0,1,10);
    world.add_platform(30,0,1,10);
    world.add_platform(0,10,30,1);

    //Platforms
    world.add_platform(3,7,4,1);
    world.add_platform(13,4,6,1);

    //camera 
    int tilesize{64};
    Camera camera{graphics, tilesize};
    camera.move_to({10,5});
    

    bool running{true};
    bool grid_on{false};
    auto previous = std::chrono::high_resolution_clock::now();
    double lag{0.0};
    while (running) {
        auto current = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = current - previous;
        previous = current;
        lag += elapsed.count();

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
            player.handle_input(event);
        }
        // Update

        constexpr double dt = 1.0/60.0;
        while (lag >= dt){
            player.update(world, dt);
            camera.update(dt);
            Vec<double> position = player.get_sprite().first;
            // Update camera based on player y position being greater than half the world's height
            if (position.y < world.tilemap.height/2){
                position.y = world.tilemap.height/2;
            }
            camera.move_to(position);
            lag -= dt;
        }
        
        
        // draw the player and platforms
        graphics.clear();
        camera.render(world.tilemap,grid_on);
        auto [position, color] = player.get_sprite();
        camera.render(position,color);
        graphics.update();
    }
}

